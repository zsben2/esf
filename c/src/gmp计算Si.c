#include "tools.h"

void check_solution_S_save(mpq_t**, char*);

int main() {
    clock_t tk[4] = {clock()};
    mpq_t ***S;
    logging("当前算法一次性开辟 N * N * K / 2 = 75 GB 空间然后再滚动释放\n");
    logging("也是开不了的\n");

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

    int n, k, i, err;

    // 存放倒数, reciprocal[n] = 1 / n 是 2 <= n <= N 有效
    mpq_t *reciprocal = get_reciprocal(N + 1);
    // S[n][i][k] 是 2 <= n <= N, 1 <= i <= n, 1 <= k <= min(n-1, e ln n + e) 有效
    mpq_t ***S = mpq_malloc_init_triangle_block(N + 1, K + 1); // S[1][1][1] = 0

    FILE *file = open_file(file_name, "wb");

    logging("\nstep 1: initial finished\n"); // 不能走到这一步

    // k = 1
    for (n = 2; n <= N; n++) {
        // 1 <= i < n
        for (i = 1; i < n; i++) {
            // S[n][i][1] = S[n-1][i][1] + 1/n
            mpq_add(S[n][i][1], S[n-1][i][1], reciprocal[n]);
            print_if_integer(S[n][i][1], n, i, 1, -1);
        }

        // i = n
        // S[n][n][1] = T[n-1][1];
        mpq_set(S[n][n][1], T[n-1][1]); // 已约分
        print_if_integer(S[n][n][1], n, n, 1, -1);
    }

    // save & clear S[n] for 0 <= n <= 1
    for (n = 0; n <= 1; n++) {
        for (i = 0; i <= n; i++) {
            for (k = 0; k <= K; k++) {
                save_to(file, S[n][i][k], n, i, k, -1);
                mpq_clear(S[n][i][k]);
            }
            free(S[n][i]);
        }
        free(S[n]);
    }

    logging("\nstep 2: k = 1 finished");
    print_time(&tk[1], tk[0], "step 1, 2");
    logging("\n");

    mpq_t **Sn, *Sni;
    int max_k;

    // 2 <= k <= min(n-1, floor(e ln n + e))
    for (n = 2; n <= N; n++) {
        Sn = S[n];
        max_k = sup_K(n); // <= K

        // save & clear S[n][0] for 2 <= n <= N
        for (k = 0; k <= K; k++) {
            save_to(file, Sn[0][k], n, 0, k, -1);
            mpq_clear(Sn[0][k]);
        }
        free(S[n][0]);

        for (i = 1; i <= n; i++) {
            Sni = Sn[i];

            // save & clear S[n][i][0] for 1 <= i <= n
            save_to(file, Sni[0], n, i, 0, -1);
            mpq_clear(Sni[0]);

            for (k = 2; k <= max_k; k++) {
                // S[n][i][k] = T[n][k] - 1/i * S[n][i][k-1]
                mpq_mul(Sni[k], reciprocal[i], Sni[k-1]);
                mpq_sub(Sni[k], T[n][k], Sni[k]);
                // integer check
                print_if_integer(Sni[k], n, i, k, -1);

                // save & clear S[n][i][k] for 1 <= k <= max_k - 1
                save_to(file, Sni[k-1], n, i, k-1, -1);
                mpq_clear(Sni[k-1]);
            }

            // save & clear S[n][i][k] for max_k <= k <= K
            for (k = max_k; k <= K; k++) {
                save_to(file, Sni[k], n, i, k, -1);
                mpq_clear(Sni[k]);
            }

            // clear S[n][i] for 1 <= i <= n
            free(Sni);
        }
        logging("n = %d, ", n);
        print_time(&tk[n], tk[n-1], "");

        // clear S[n] for 2 <= n <= N
        free(Sn);
    }
    free_mpq_list(reciprocal, N + 1);
    free(S);
    fclose(file);

    logging("\nstep 3: 2 <= k <= 2 <= k <= min(n-1, floor(e ln n + e)) finished");
    print_time(&tk[N+1], tk[0], __func__);
    logging("\n");
}
