#include "tools.h"

void check_solution_S(mpq_t**);

int main() {
    clock_t tk[4] = {clock()};
    mpq_t ***S;
    logging("当前算法仅开放一行 i 的空间滚动使用，并放弃使用一个闲置空间\n");
    logging("去除保存文件的语句\n");
    logging("预计需要运行 14 day 6 h 25 min 39 s = 1232739 s\n");
    logging("经初步运行的数据分析，单个 n 运行时间服从 0 - 0.000740566 * n + 1.57395e-06 * n^2\n");
    logging("总计运行时间服从 0 + 0.197321 * n - 0.000455747 * n^2 + 5.28968e-07 * n^3\n");

    mpq_t **T = read_T("T.bin");
    print_time(&tk[1], tk[0], "read T");

    check_solution_S(T);
    print_time(&tk[2], tk[1], "check solve S");

    free_mpq_array(T, N + 1, K + 1);
    print_time(&tk[3], tk[2], "free T");

    print_time(&tk[3], tk[0], __func__);
    return 0;
}

void check_solution_S(mpq_t **T) {
    // 记录开始时间
    clock_t tk[N+2] = {clock()};

    // 2 <= n <= N, 1 <= i <= n, 1 <= k <= min(n-1, e ln n + e) 有效
    // 仅初始化两片空间
    // 在初始状态中, S1_1 表示 S[1][*][1]
    mpq_t *S1_1 = mpq_malloc_init_list(N + 1); // S[1][1][1] = 0
    // 存放倒数, reciprocal[n] = 1 / n 对 1 <= n <= N 有效
    mpq_t *reciprocal = get_reciprocal(N + 1);
    mpq_t Snik;
    mpq_init(Snik);

    logging("\nstep 1: initial finished\n");
    print_time(&tk[1], tk[0], "");
    logging("\n");

    int n, k, i, max_k;

    // 在迭代状态中, S1_1[i], Snik 分别表示 S[n-1][i][1], S[n][i][k]
    for (n = 2; n <= N; n++) {
        max_k = sup_K(n);

        // 1 <= i < n
        for (i = 1; i < n; i++) {
            // k = 1, S[n][i][1] = S[n-1][i][1] + 1/n
            mpq_add(Snik, S1_1[i], reciprocal[n]);
            // check S[n][i][1]
            print_if_integer(Snik, n, i, 1, -1);
            // 新 S[n-1][i][1] = 现在的 S[n][i][1] for 1 <= i < n
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
            mpq_set(Snik, T[n-1][k]); // 已约分
            // check S[n][n][k]
            print_if_integer(Snik, n, n, k, -1);
            // 新 S[n-1][n][1] = 现在的 S[n][n][1]
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

    logging("\nstep 3: clear finished");
    print_time(&tk[N+1], tk[N], __func__);
    logging("\n");
}
