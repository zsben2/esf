#include "tools.h"

void check_solution_S_save(mpq_t**, char*);

int main() {
    clock_t tk[4] = {clock()};
    mpq_t ***S;
    logging("��ǰ�㷨������һ�� i �� N ���ռ����ʹ��\n");

    mpq_t **T = read_T("T.bin");
    print_time(&tk[1], tk[0], "read T");

    check_solution_S_save(T, "S.bin");
    print_time(&tk[2], tk[1], "check solve S");

    free_mpq_array(T, N + 1, K + 1);
    print_time(&tk[3], tk[2], "free T");

    return 0;
}

void check_solution_S_save(mpq_t **T, char *file_name) {
    // ��¼��ʼʱ��
    clock_t tk[N+2] = {clock()};

    // 2 <= n <= N, 1 <= i <= n, 1 <= k <= min(n-1, e ln n + e) ��Ч
    // ����ʼ����Ƭ�ռ�
    // �ڳ�ʼ״̬��, S1_1 ��ʾ S[1][*][1]
    mpq_t *S1_1 = mpq_malloc_init_list(N + 1); // S[1][1][1] = 0
    // ��ŵ���, reciprocal[n] = 1 / n �� 2 <= n <= N ��Ч
    mpq_t *reciprocal = get_reciprocal(N + 1);

    logging("\nstep 1: initial finished\n");
    print_time(&tk[1], tk[0], "");
    logging("\n");

    int n, k, i, max_k;
    mpq_t Snik;
    mpq_init(Snik);
    // ���õ� 0 �ſռ䱻���ڴ洢 k-1 ʱ��ֵ
    mpq_t *pSnik_1 = &S1_1[0];
    FILE *file = open_file(file_name, "wb");

    // �ڵ���״̬��, S1_1[i], *pSnik_1, Snik �ֱ��ʾ S[n-1][i][1], S[n][i][k-1], S[n][i][k]
    for (n = 2; n <= N; n++) {
        max_k = sup_K(n);

        // 1 <= i < n
        for (i = 1; i < n; i++) {
            // k = 1, S[n][i][1] = S[n-1][i][1] + 1/n
            mpq_add(Snik, S1_1[i], reciprocal[n]);
            // check & save S[n][i][1]
            print_if_integer(Snik, n, i, 1, -1);
            save_to(file, Snik, n, i, 1, -1);
            // �� S[n-1][i][1] = ���ڵ� S[n][i][1]
            mpq_set(S1_1[i], Snik);
            // ��¼ S[n][i][1]
            mpq_set(*pSnik_1, Snik);

            // 2 <= k <= max_k
            for (k = 2; k <= max_k; k++) {
                // S[n][i][k] = T[n][k] - 1/i * S[n][i][k-1]
                mpq_mul(Snik, reciprocal[i], *pSnik_1);
                mpq_sub(Snik, T[n][k], Snik);
                // check & save S[n][i][k] for 2 <= k <= max_k
                print_if_integer(Snik, n, i, k, -1);
                save_to(file, Snik, n, i, k, -1);
                // �� S[n][i][k-1] = ���ڵ� S[n][i][k]
                mpq_set(*pSnik_1, Snik);
            }
        }

        // i = n, 1 <= k <= max_k
        for (k = 1; k <= max_k; k++) {
            // S[n][n][k] = T[n-1][k] for 1 <= k <= max_k
            mpq_set(Snik, T[n-1][k]); // ��Լ��
            // check & save S[n][n][k]
            print_if_integer(Snik, n, n, k, -1);
            save_to(file, Snik, n, n, k, -1);
            // �� S[n-1][n][1] = ���ڵ� S[n][n][1]
            if (k == 1) {
                mpq_set(S1_1[n], Snik);
            }
        }

        logging("n = %d, ", n);
        print_time(&tk[n], tk[n-1], "");
    }

    logging("\nstep 2: iteration finished");
    print_time(&tk[N], tk[1], "step 2");
    logging("\n");

    // clear
    free_mpq_list(S1_1, N + 1);
    free_mpq_list(reciprocal, N + 1);
    mpq_clear(Snik);
    fclose(file);

    logging("\nstep 3: clear finished");
    print_time(&tk[N+1], tk[N], __func__);
    logging("\n");
}
