#include "tools.h"

void check_solution_S_save(mpq_t**, char*);

int main() {
    clock_t tk[4] = {clock()};
    mpq_t ***S;
    logging("当前算法仅开放两行关于 n, i 的空间滚动使用\n");

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
    // 在初始状态中, S1_1, S2i_ 分别表示 S[1][*][1], S[2][1][*]
    mpq_t *S1_1 = mpq_malloc_init_list(N + 1); // S[1][1][1] = 0
    mpq_t *S2i_ = mpq_malloc_init_list(K + 1);
    mpq_t *reciprocal = get_reciprocal(N + 1);

    logging("\nstep 1: initial finished\n");
    print_time(&tk[1], tk[0], "");
    logging("\n");

    int n, i, k, max_k;
    FILE *file = open_file(file_name, "wb");

    // 在迭代状态中, S1_1[i], S2i_[k] 分别表示 S[n-1][i][1], S[n][i][k]
    for (n = 2; n <= N; n++) {
        max_k = sup_K(n);

        // 1 <= i < n
        for (i = 1; i < n; i++) {
            // k = 1, S[n][i][1] = S[n-1][i][1] + 1/n
            mpq_add(S2i_[1], S1_1[i], reciprocal[n]);
            // check & save S[n][i][1]
            print_if_integer(S2i_[1], n, i, 1, -1);
            save_to(file, S2i_[1], n, i, 1, -1);
            // 新 S[n-1][i][1] = 现在的 S[n][i][1]
            mpq_set(S1_1[i], S2i_[1]);

            // 2 <= k <= max_k
            for (k = 2; k <= max_k; k++) {
                // S[n][i][k] = T[n][k] - 1/i * S[n][i][k-1]
                mpq_mul(S2i_[k], reciprocal[i], S2i_[k-1]);
                mpq_sub(S2i_[k], T[n][k], S2i_[k]);
                // check & save S[n][i][k] for 2 <= k <= max_k
                print_if_integer(S2i_[k], n, i, k, -1);
                save_to(file, S2i_[k], n, i, k, -1);
            }
        }

        // i = n, 1 <= k <= max_k
        for (k = 1; k <= max_k; k++) {
            // S[n][n][k] = T[n-1][k] for 1 <= k <= max_k
            mpq_set(S2i_[k], T[n-1][k]); // 已约分
            // check & save S[n][n][k]
            print_if_integer(S2i_[k], n, n, k, -1);
            save_to(file, S2i_[k], n, n, k, -1);
        }
        // 新 S[n-1][n][1] = 现在的 S[n][n][1]
        mpq_set(S1_1[n], S2i_[1]);

        logging("n = %d, ", n);
        print_time(&tk[n], tk[n-1], "");
    }

    logging("\nstep 2: iteration finished");
    print_time(&tk[N], tk[1], "step 2");
    logging("\n");


    // clear
    free_mpq_list(S1_1, N + 1);
    free_mpq_list(S2i_, K + 1);
    free_mpq_list(reciprocal, N + 1);
    fclose(file);

    logging("\nstep 3: clear finished");
    print_time(&tk[N+1], tk[N], __func__);
    logging("\n");
}
