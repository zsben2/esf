#include "tools.h"

void check_solution_S_with_cache(mpq_t**, const char*);
mpq_t *read_nth_S1_1(const char*, int*);
void save_nth_S1_1(const int, const mpq_t*, const char*);

int main() {
    clock_t tk[4] = {clock()};
    mpq_t ***S;
    logging("��ǰ�㷨������� S1_1, �Ա������ n ��ʼ�������У��������Ϊ������ S[n][i][k]\n");

    mpq_t **T = read_T("T.bin");
    print_time(&tk[1], tk[0], "read T");

    check_solution_S_with_cache(T, "n_S1_1_cache.bin");
    print_time(&tk[2], tk[1], "check solve S");

    free_mpq_array(T, N + 1, K + 1);
    print_time(&tk[3], tk[2], "free T");

    print_time(&tk[3], tk[0], __func__);
    return 0;
}

void check_solution_S_with_cache(mpq_t **T, const char *cache_file_name) {
    // ��¼��ʼʱ��
    clock_t tk[N+2] = {clock()};

    // 2 <= n <= N, 1 <= i <= n, 1 <= k <= min(n-1, e ln n + e) ��Ч
    // ����ʼ��һ�пռ�
    // �ڳ�ʼ״̬��, S1_1 ��ʾ S[1][*][1]
    int n;
    mpq_t *S1_1 = read_nth_S1_1(cache_file_name, &n); // S[1][1][1] = 0
    mpq_t *reciprocal = get_reciprocal(N + 1);
    mpq_t Snik;
    mpq_init(Snik);

    int t1 = n - 1;
    logging("\nstep 1: initial finished\n");
    print_time(&tk[t1], tk[0], "");
    logging("\n");

    int i, k, max_k;

    // �ڵ���״̬��, S1_1[i], Snik �ֱ��ʾ S[n-1][i][1], S[n][i][k]
    for (; n <= N; n++) {
        max_k = sup_K(n);

        // 1 <= i < n
        for (i = 1; i < n; i++) {
            // k = 1, S[n][i][1] = S[n-1][i][1] + 1/n
            mpq_add(Snik, S1_1[i], reciprocal[n]);
            // check S[n][i][1]
            print_if_integer(Snik, n, i, 1, -1);
            // �� S[n-1][i][1] = ���ڵ� S[n][i][1] for 1 <= i < n
            mpq_set(S1_1[i], Snik);

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
            mpq_set(Snik, T[n-1][k]); // ��Լ��
            // check S[n][n][k]
            print_if_integer(Snik, n, n, k, -1);
            // �� S[n-1][n][1] = ���ڵ� S[n][n][1]
            if (k == 1) {
                mpq_set(S1_1[n], Snik);
            }
        }

        logging("n = %d, ", n);
        save_nth_S1_1(n, S1_1, cache_file_name);
        print_time(&tk[n], tk[n-1], "");
    }

    logging("\nstep 2: iteration finished");
    print_time(&tk[N], tk[t1], "step 2");
    logging("\n");

    // clear
    free_mpq_list(S1_1, N + 1);
    free_mpq_list(reciprocal, N + 1);

    logging("\nstep 3: clear finished");
    print_time(&tk[N+1], tk[N], __func__);
    logging("\n");
}

mpq_t *read_nth_S1_1(const char *file_name, int *ptr_n) {
    mpq_t *S1_1 = mpq_malloc_init_list(N + 1); // S[1][1][1] = 0
    FILE *file = fopen(file_name, "rb");

    // �Ҳ��������ļ�����ͷ��ʼ����
    if (!file) {
        *ptr_n = 2;
        return S1_1;
    }

    mpz_t n;
    mpz_init(n);
    mpz_read(n, file, "n "); // �� n

    *ptr_n = (int) mpz_get_ui(n);

    // �� S1_1
    // ע�� S1_1 װ���� n - 1 ʱ�����ݣ���� i ��� *ptr_n - 1
    int i;
    for (i = 0; i < *ptr_n; i++) {
        mpq_read(S1_1[i], file);
    }

    fclose(file);
    return S1_1;
}

void save_nth_S1_1(const int n, const mpq_t *S1_1, const char *file_name) {
    clock_t tk[2] = {clock()};
    mpz_t n_mpz;
    mpz_init(n_mpz);
    // ע����һ��ѭ����Ҫ���е��� n + 1
    mpz_set_ui(n_mpz, (unsigned long int) n + 1);

    FILE *file = open_file(file_name, "wb");

    // д�� n + 1
    if (!mpz_out_raw(file, n_mpz)) {
        exit(-1);
    }

    // д�� S1_1
    int i;
    for (i = 0; i <= n; i++) {
        save_to(file, S1_1[i], i, -1);
    }

    fclose(file);
    print_time(&tk[1], tk[0], __func__);
}