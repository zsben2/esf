#include "tools.h"

void check_solution_S_save(mpq_t**, char*);

int main() {
    clock_t tk[4] = {clock()};
    mpq_t ***S;
    logging("当前算法仅开放两片关于 n 的空间滚动使用\n");
    logging("至少消耗 2 * N * K * 32 B = 23.143 MB\n");

    mpq_t **T = read_T("T.bin");
    print_time(&tk[1], tk[0], "read T");

    check_solution_S_save(T, "S.bin");
    print_time(&tk[2], tk[1], "check solve S");

    free_mpq_array(T, N + 1, K + 1);
    print_time(&tk[3], tk[2], "free T");

    return 0;
}

void check_solution_S_save(mpq_t **T, char *file_name) {
    // 记录开始时间
    clock_t tk[N+2] = {clock()};

    // 2 <= n <= N, 1 <= i <= n, 1 <= k <= min(n-1, e ln n + e) 有效
    // 仅初始化两片空间
    // 在初始状态中, S1, S2 分别表示 S[1], S[2]
    mpq_t **S1 = mpq_malloc_init_array(N + 1, K + 1); // S[1][1][1] = 0
    mpq_t **S2 = mpq_malloc_init_array(N + 1, K + 1);
    // 存放倒数, reciprocal[n] = 1 / n 是 2 <= n <= N 有效
    mpq_t *reciprocal = get_reciprocal(N + 1);

    logging("\nstep 1: initial finished\n");
    print_time(&tk[1], tk[0], "");
    logging("\n");

    int n, k, i, max_k;
    mpq_t **S3, *Sni;
    FILE *file = open_file(file_name, "wb");

    // 在迭代状态中, S1, S2 分别表示 S[n-1], S[n]
    for (n = 2; n <= N; n++) {
        max_k = sup_K(n);
        // i = 0, skip
        for (k = 0; k <= K; k++) {
            save_to(file, S2[0][k], n, 0, k, -1);
        }
        // 1 <= i < n
        for (i = 1; i < n; i++) {
            Sni = S2[i];
            // k = 0, skip
            save_to(file, Sni[0], n, i, 0, -1);

            // k = 1, S[n][i][1] = S[n-1][i][1] + 1/n
            mpq_add(Sni[1], S1[i][1], reciprocal[n]);
            // check & save S[n][i][1]
            print_if_integer(Sni[1], n, i, 1, -1);
            save_to(file, Sni[1], n, i, 1, -1);

            // 2 <= k <= max_k
            for (k = 2; k <= max_k; k++) {
                // S[n][i][k] = T[n][k] - 1/i * S[n][i][k-1]
                mpq_mul(Sni[k], reciprocal[i], Sni[k-1]);
                mpq_sub(Sni[k], T[n][k], Sni[k]);
                // check & save S[n][i][k] for 2 <= k <= max_k
                print_if_integer(Sni[k], n, i, k, -1);
                save_to(file, Sni[k], n, i, k, -1);
            }

            // save S[n][i][k] for max_k + 1 <= k <= K, skip
            for (k = max_k + 1; k <= K; k++) {
                save_to(file, Sni[k], n, i, k, -1);
            }
        }

        // i = n, k = 0, skip
        save_to(file, S2[n][0], n, n, 0, -1);

        // i = n, 1 <= k <= max_k <= n - 1
        for (k = 1; k <= max_k; k++) {
            // S[n][n][k] = T[n-1][k] for 1 <= k <= max_k
            mpq_set(S2[n][k], T[n-1][k]); // 已约分
            // check & save S[n][n][k]
            print_if_integer(S2[n][k], n, n, k, -1);
            save_to(file, S2[n][k], n, n, k, -1);
        }

        // i = n, max_k + 1 <= k <= K, skip
        for (k = max_k + 1; k <= K; k++) {
            save_to(file, S2[n][k], n, n, k, -1);
        }

        // 交换 S1, S2, 滚动使用空间
        S3 = S2;
        S1 = S3;
        S2 = S3;

        logging("n = %d, ", n);
        print_time(&tk[n], tk[n-1], "");
    }

    logging("\nstep 2: iteration finished");
    print_time(&tk[N], tk[1], "step 2");
    logging("\n");

    // clear
    free_mpq_array(S1, N + 1, K + 1);
    free_mpq_array(S2, N + 1, K + 1);
    free_mpq_list(reciprocal, N + 1);
    fclose(file);

    logging("\nstep 3: clear finished");
    print_time(&tk[N+1], tk[N], __func__);
    logging("\n");
}
