#include "tools.h"
#define M 1000 // �����ڲ��м���ĵ��ԣ�����̣���Ŀ
#define m 4 // m ��ʾ������, 0 <= m < M

void check_solution_S_using_S1_1(const char*);
void read_S1_1(mpq_t*, const int);
int is_job_n(const int);
void save_n(const int, const char*);
int read_n(const char*);

int main() {
    clock_t tk[2] = {clock()};
    logging("\n");
    logging("============================================================\n");
    logging("������ҵ m = %d, M = %d\n", m, M);
    logging("============================================================\n");
    logging("\n");
    logging("��ǰ�㷨���ø��� S1_1 ���Ը����Ĳ��� n ���м��㣬�� T �� S �ϲ������Լ����ڴ�����\n");
    logging("��ǰ�㷨���Ỻ��������Ҫִ�е� n, �Ա���崻�ʱ�������Ӻ��ʵ� n ��ʼ����\n");
    logging("��ǰ�㷨���ķ�ֵ�ڴ�Ϊ 140 MB\n");

    check_solution_S_using_S1_1("n.txt");
    print_time(&tk[1], tk[0], "check solve S");

    return 0;
}

// ������������ n ��ģ M ͬ�໮��, ��ִ���� m ͬ��� n
void check_solution_S_using_S1_1(const char *n_file_name) {
    // ��¼��ʼʱ��
    clock_t tk[N/M + 2] = {clock()};

    mpq_t *reciprocal = get_reciprocal(N + 1);
    mpq_t *S1_1 = mpq_malloc_init_list(N + 1);
    mpq_t *T1 = mpq_malloc_init_list(K + 1);
    mpq_t *T2 = mpq_malloc_init_list(K + 1);
    mpq_t *T3, Snik; // ��ʱ����
    mpq_init(Snik);

    char n_value_log[20];
    int n, k, i, max_k, tk_index = 2;
    int n0 = read_n(n_file_name);

    logging("�� n = %d ��ʼ����\n", n0);
    print_time(&tk[1], tk[0], "step 1: initial finished,");
    logging("\n");

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

    ������
    (eq3) S[n][i][1] = S[n-1][i][1] + 1 / n for 1 <= i < n, n >= 2
    (eq4) S[n][i][k] = T[n][k] - 1 / i * S[n][i][k-1] for 2 <= k <= max_k, 1 <= i < n, n >= 2
    (eq5) S[n][n][k] = T[n-1][k] for 1 <= k <= max_k, n >= 2

    �ڵ���״̬��, S1_1[i], Snik �ֱ��ʾ S[n-1][i][1], S[n][i][k],
    �� 2 <= n <= N, 1 <= i <= n, 1 <= k <= min(n-1, e ln n + e) ��Ч.
    �ڵ���״̬��, T1 ���ڱ�ʾ T[n-1], T2 ���ڱ�ʾ T[n], �� 1 <= n <= N ��Ч.
    */

    for (n = 1; n <= N; n++) {
        // k = 1
eq1:    mpq_add(T2[1], reciprocal[n], T1[1]);
        // k >= 2
        for (k = 2; k <= min(n, K); k++) {
eq2:        mpq_mul(T2[k], reciprocal[n], T1[k-1]);
            mpq_add(T2[k], T2[k], T1[k]);
        }

        // ������ n >= n0 �� n % M == m ���ǲ��� n
        if (n >= n0 && is_job_n(n)) {
            // S1_1 �ļ�������ļ����� N, ��Ϊ n ���ļ��������Ϊ n, ��Ч����Ϊ 1 <= i <= n
            // ��Ϊ��Ҫʹ�� S[n-1][i][1] ������, ������Ҫ������Ϊ n - 1 ���ļ�
            read_S1_1(S1_1, n - 1);
            max_k = sup_K(n);

            // 1 <= i < n
            for (i = 1; i < n; i++) {
                // k = 1
eq3:            mpq_add(Snik, S1_1[i], reciprocal[n]);
                print_if_integer(Snik, n, i, 1, -1);

                // 2 <= k <= max_k
                for (k = 2; k <= max_k; k++) {
eq4:                mpq_mul(Snik, reciprocal[i], Snik);
                    mpq_sub(Snik, T2[k], Snik);
                    print_if_integer(Snik, n, i, k, -1);
                }
            }

            // i = n, 1 <= k <= max_k
            for (k = 1; k <= max_k; k++) {
eq5:            mpq_set(Snik, T1[k]);
                print_if_integer(Snik, n, n, k, -1);
            }

            save_n(n, n_file_name);
            sprintf(n_value_log, "n = %d,", n);
            print_time(&tk[tk_index], tk[tk_index-1], n_value_log);
            tk_index++;
        }

        // ����ʹ�� T1, T2 �ռ�
        T3 = T1;
        T1 = T2;
        T2 = T3;
    }

    logging("\n");
    logging("step 2: iteration finished\n");
    print_time(&tk[tk_index], tk[1], "step 2");
    logging("\n");

    // clear
    free_mpq_list(reciprocal, N + 1);
    free_mpq_list(S1_1, N + 1);
    free_mpq_list(T1, K + 1);
    free_mpq_list(T2, K + 1);
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

int is_job_n(const int n) {
    // n % M == m % M ˵����ǰ�� n �뱣��
    return n % M == m % M;
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
