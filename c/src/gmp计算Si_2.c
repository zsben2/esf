#include "tools.h"
// �����ڲ��м���ĵ��ԣ�����̣���Ŀ
// M = 1 ʱռ��5.6 - 5.8 GB �ڴ�
// M = 5 ʱռ��3.9 - 4.0 GB �ڴ�
// M = 7 ʱռ��3.6 - 3.7 GB �ڴ�
// M = 10 ʱռ��3.3 - 3.4 GB �ڴ�
// M = 15 ʱռ��3.1 - 3.2 GB �ڴ�
// M = 100 ʱռ��2.8 - 2.9 GB �ڴ�
#define M 1000


void check_solution_S_using_S1_1(mpq_t**, const int, const char*);
void read_S1_1(mpq_t*, const int);
void free_mpq_list(mpq_t*, const int);
void free_part_T(mpq_t **, const int);
mpq_t **get_part_T(const char*, const int, const int);
int is_job_n(const int, const int);
int is_save_n(const int, const int);
void save_n(const int, const char*);
int read_n(const char*);

int main() {
    int m = 0; // m ��ʾ������, 0 <= m < M
    clock_t tk[4] = {clock()};
    mpq_t ***S;
    logging("\n");
    logging("============================================================\n");
    logging("������ҵ m = %d, M = %d\n", m, M);
    logging("============================================================\n");
    logging("\n");
    logging("��ǰ�㷨���ø��� S1_1 ���Ը����Ĳ��� n ���м��㣬����ȡ��Ҫ�õ��� T �����ȫ�� T ����̬�ͷ��ڴ�\n");
    logging("��ǰ�㷨���Ỻ��������Ҫִ�е� n, �Ա���崻�ʱ�������Ӻ��ʵ� n ��ʼ����\n");
    logging("���� T �� M = 1 ʱ���ķ�ֵ�ڴ�Ϊ 2992 MB\n");
    logging("��ȡ T �� M = 1 ʱ���ķ�ֵ�ڴ�Ϊ 5.8 GB\n");

    mpq_t **T = get_part_T("T.bin", m, 0); // �������õ�������

    print_time(&tk[1], tk[0], "read T");

    check_solution_S_using_S1_1(T, m, "n.txt");
    print_time(&tk[2], tk[1], "check solve S");

    if (M == 1) {
        free_mpq_list(T[N], K + 1);
    }
    free(T);

    print_time(&tk[3], tk[2], "free T");
    print_time(&tk[3], tk[0], __func__);
    return 0;
}

// ������������ n ��ģ M ͬ�໮��, ��ִ���� m ͬ��� n
void check_solution_S_using_S1_1(mpq_t **T, const int m, const char *n_file_name) {
    // ��¼��ʼʱ��
    clock_t tk[N/M + 2] = {clock()};
    mpq_t *S1_1 = mpq_malloc_init_list(N + 1);
    mpq_t *reciprocal = get_reciprocal(N + 1);

    logging("\n");
    logging("step 1: initial finished\n");
    print_time(&tk[1], tk[0], "");
    logging("\n");

    mpq_t Snik;
    mpq_init(Snik);
    char n_value_log[20];
    int k, i, max_k, tk_index = 2;
    int n = read_n(n_file_name);
    logging("�� n = %d ��ʼ����\n", n);

    // �ڵ���״̬��, S1_1[i], Snik �ֱ��ʾ S[n-1][i][1], S[n][i][k]
    // 2 <= n <= N, 1 <= i <= n, 1 <= k <= min(n-1, e ln n + e) ��Ч
    for (; n <= N; n++) {
        // ��ִ�� n % M == m ���ǲ��� n
        if (!is_job_n(n, m)) {
            continue;
        }
        // S1_1 �ļ�������ļ����� N, ��Ϊ n ���ļ��������Ϊ n, ��Ч����Ϊ 1 <= i <= n
        // �ڲ����ļ����ü����ʱ��, ��Ϊ��Ҫ���� S[n-1][i][1] ������, ������Ҫ������Ϊ n - 1 ���ļ�
        read_S1_1(S1_1, n - 1);
        max_k = sup_K(n);

        // 1 <= i < n
        for (i = 1; i < n; i++) {
            // k = 1, S[n][i][1] = S[n-1][i][1] + 1/n
            mpq_add(Snik, S1_1[i], reciprocal[n]);
            // check S[n][i][1]
            print_if_integer(Snik, n, i, 1, -1);

            // 2 <= k <= max_k
            for (k = 2; k <= max_k; k++) {
                // S[n][i][k] = T[n][k] - 1/i * S[n][i][k-1]
                mpq_mul(Snik, reciprocal[i], Snik);
                mpq_sub(Snik, T[n][k], Snik);
                // check S[n][i][k] for 2 <= k <= max_k
                print_if_integer(Snik, n, i, k, -1);
            }
        }

        // i = n, 1 <= k <= max_k
        for (k = 1; k <= max_k; k++) {
            // S[n][n][k] = T[n-1][k] for 1 <= k <= max_k
            mpq_set(Snik, T[n-1][k]);
            // check S[n][n][k]
            print_if_integer(Snik, n, n, k, -1);
        }

        free_part_T(T, n);

        save_n(n, n_file_name);
        sprintf(n_value_log, "n = %d,", n);
        print_time(&tk[tk_index], tk[tk_index-1], n_value_log);
        tk_index++;
    }

    logging("\n");
    logging("step 2: iteration finished\n");
    print_time(&tk[tk_index], tk[1], "step 2");
    logging("\n");

    // clear
    free_mpq_list(reciprocal, N + 1);
    free_mpq_list(S1_1, N + 1);
    mpq_clear(Snik);

    logging("step 3: clear finished\n");
    print_time(&tk[tk_index+1], tk[tk_index], __func__);
    logging("\n");
}

// S1_1 ����������� N, ��Ч����Ϊ 1 <= i <= n - 1
void read_S1_1(mpq_t *S1_1, const int n) {
    char S1_1_file_name[30];
    sprintf(S1_1_file_name, "./%s/%05d.bin", S1_1_DIR, n);
    FILE *file = open_file(S1_1_file_name, "rb");

    int i;
    for (i = 0; i <= n; i++) {
        mpq_read(S1_1[i], file);
    }

    fclose(file);
}

mpq_t **get_part_T(const char *file_name, const int m, const int is_read_T_file) {
    mpq_t **T;
    int n;
    if (is_read_T_file) {
        T = mpq_ptr_malloc_list(N + 1); // T[n][k] �� 1 <= k <= K, k < n <= N ��Ч;

        // T[0] δ���ٿռ�, �������õ��Ŀռ�
        for (n = 1; n <= N; n++) {
            if (is_save_n(n, m)) {
                T[n] = mpq_malloc_init_list(K + 1);
            }
        }

        int k;
        mpq_t temp;
        mpq_init(temp);
        FILE *file = open_file(file_name, "rb");

        for (k = 1; k <= K; k++) {
            for (n = k; n <= N; n++) {
                mpq_read(temp, file);
                if (is_save_n(n, m)) {
                    mpq_set(T[n][k], temp);
                }
            }
        }

        mpq_clear(temp);
        fclose(file);
    } else {
        T = solution_T();
        for (n = 1; n <= N; n++) {
            if (!is_save_n(n, m)) {
                free_mpq_list(T[n], K + 1);
            }
        }
    }
    return T;
}

void free_part_T(mpq_t **T, const int n) {
    int k;
    free_mpq_list(T[n-1], K + 1);
    if (M >= 2) {
        free_mpq_list(T[n], K + 1);
    }
}

int is_job_n(const int n, const int m) {
    // n % M == m % M ˵����ǰ�� n �뱣��
    return n % M == m % M;
}

int is_save_n(const int n, const int m) {
    // �뱣�� n % M == m % M ʱ�� n, n - 1 ��
    // (n + 1) % M == m % M ˵����ǰ�� n ����һ�� n' �� n' - 1, ���� n' % M == m % M, ��ô��� n ��Ҫ����
    return is_job_n(n + 1, m) || is_job_n(n, m);
}

void save_n(const int n, const char *file_name) {
    FILE *file = open_file(file_name, "w");
    fprintf(file, "%d", n + 1); // ע����һ��ѭ����Ҫ���е��� n + 1
    fclose(file);
}

int read_n(const char *file_name) {
    FILE *file = fopen(file_name, "r");
    // �Ҳ��������ļ�����ͷ��ʼ����
    if (!file) {
        return 2;
    }

    int n;
    fscanf(file, "%d", &n);
    fclose(file);
    return n;
}