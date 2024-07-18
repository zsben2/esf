#include "tools.h"
// �����ڲ��м���ĵ��ԣ�����̣���Ŀ
// M = 1 ʱռ�� 2862 MB �ڴ�
// M = 5 ʱռ�� 1181 MB �ڴ�
// M = 7 ʱռ�� 835 MB �ڴ�
// M = 10 ʱռ�� 590 MB �ڴ�
// M = 100 ʱռ�� 64 MB �ڴ�
#define M 1000


void check_solution_S_using_S1_1(mpq_t**, const int, const char*);
void read_S1_1(mpq_t*, const int);
void free_mpq_list(mpq_t*, const int);
void free_part_T(mpq_t **, const int);
mpq_t **solve_part_T(const int);
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
    logging("��ǰ�㷨���ø��� S1_1 ���Ը����Ĳ��� n ���м��㣬��������Ҫ�õ��� T ����̬�ͷ��ڴ�\n");
    logging("��ǰ�㷨���Ỻ��������Ҫִ�е� n, �Ա���崻�ʱ�������Ӻ��ʵ� n ��ʼ����\n");
    logging("��ǰ�㷨�� M = 1 ʱ���ķ�ֵ�ڴ�Ϊ 2992 MB\n");

    mpq_t **T = solve_part_T(m); // �������õ�������

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
    logging("\n");
    logging("------------------------------------------------------------\n");
    logging("���ڼ��� S[n][i][k]\n");
    logging("------------------------------------------------------------\n");
    logging("\n");

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

mpq_t **solve_part_T(const int m) {
    // ��¼��ʼʱ��
    clock_t tk[N+2] = {clock()};
    logging("\n");
    logging("------------------------------------------------------------\n");
    logging("���ڼ��� T[n][k]\n");
    logging("------------------------------------------------------------\n");
    logging("Ԥ�ƺ�ʱ 4 min 20 s, ���� 2862 MB ���ڴ�\n");
    logging("\n");

    int k, n;
    mpq_t *reciprocal = get_reciprocal(N + 1);
    // T[n][k] �� 1 <= k <= K, k <= n <= N ��Ч;
    mpq_t **T = mpq_ptr_malloc_list(N + 1);
    // �ڵ�����, T1 ���ڱ�ʾ T[n-1], T2 ���ڱ�ʾ T[n]
    mpq_t *T1 = mpq_malloc_init_list(K + 1);
    mpq_t *T2 = mpq_malloc_init_list(K + 1);
    mpq_t *T3 = NULL; // ��ʱ����
    char n_value_log[20];

    /*
    ��Ϊ����ѧ��ʽ����
    T[1][1] = 1
    T[n][1] = 1 / n + T[n-1][1] for 2 <= n <= N
    ����Ϊ�ڳ�ʼ������ʱ��
    T[0][1] = 0
    ����
    (eq1) T[n][1] = 1 / n + T[n-1][1] for 1 <= n <= N

    ��Ϊ����ѧ��ʽ����
    T[k][k] = 1 / k * T[k-1][k-1] for 2 <= k <= K
    T[n][k] = 1 / n * T[n-1][k-1] + T[n-1][k] for 2 <= k <= K, k < n <= N
    ����Ϊ�ڳ�ʼ������ʱ��
    T[k-1][k] = 0 for 2 <= k <= K, i.e. T[1][2] = T[2][3] = ... = T[K-1][K] = 0
    ����
    (eq2) T[n][k] = 1 / n * T[n-1][k-1] + T[n-1][k] for 2 <= k <= K, k <= n <= N
    */
    for (n = 1; n <= N; n++) {
        // k = 1
eq1:    mpq_add(T2[1], reciprocal[n], T1[1]);
        // k >= 2
        for (k = 2; k <= min(n, K); k++) {
eq2:        mpq_mul(T2[k], reciprocal[n], T1[k-1]);
            mpq_add(T2[k], T2[k], T1[k]);
        }

        // �ڴ����
        // ���α��� n ʱ, T3 = NULL, ��� T1 �ռ仺������. ���� n = 1 ���������
        // �ٴα��� n ʱ, T3 ���ֻ���
        // ���β����� n ʱ, T3 �ǻ���Ŀ��ÿռ�. ���� n = 1 ���������
        // �ٴβ����� n ʱ, T3 = NULL
        T3 = T3 ? T3 : T1;
        T1 = T2;
        if (is_save_n(n, m)) {
            T[n] = T2;
            T2 = mpq_malloc_init_list(K + 1);
        } else {
            T2 = T3;
            T3 = NULL;
        }

        // sprintf(n_value_log, "n = %d,", n);
        // print_time(&tk[n], tk[n-1], n_value_log); // ��ӡʱ��
    }

    free_mpq_list(T2, K + 1);
    free_mpq_list(T3 ? T3 : T1, K + 1);
    free_mpq_list(reciprocal, N + 1);
    print_time(&tk[N+1], tk[0], __func__);
    return T;
}
