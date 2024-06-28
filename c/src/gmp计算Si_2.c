#include "tools.h"
// 可用于并行计算的电脑（或进程）数目（可按需要修改）
#define M 1000

void check_solution_S_using_S1_1(mpq_t**, const int);
void read_S1_1(mpq_t*, const int);
int is_job_n(const int n, const int m);

int main() {
    int m = 0; // m 表示机器号, 0 <= m < M
    clock_t tk[4] = {clock()};

    logging("当前算法利用各级 S1_1 仅对给定的部分 n 进行计算, 约消耗 5.6 ~ 5.8 GB 内存\n\n");
    logging("2 台电脑运行时，单台电脑单次循环消耗时间服从 t = -0.000562106 n + 1.40402e-06 n^2\n");
    logging("2 台电脑运行时，单台电脑累计消耗时间服从 sumT = 0.0833342 n - 0.000203596 n^2 + 2.4687e-07 n^3\n");
    logging("t(13524) = 250 s = 4 min 10 s, sumT(13524) = 576872 s = 6 day 16 h 16 min 32 s\n\n");
    logging("3 台电脑运行时，单台电脑单次循环消耗时间服从 t = -0.000859396 n + 1.59795e-06 n^2 \n");
    logging("3 台电脑运行时，单台电脑累计消耗时间服从 sumT = 0.0914364 n - 0.000186365 n^2 + 1.82565e-07 n^3\n");
    logging("t(13524) = 281.4 s = 4 min 41.3 s, sumT(13524) = 420445.3 s = 4 day 20 h 47 min 25.3 s\n\n");

    mpq_t **T = read_T("T.bin");
    print_time(&tk[1], tk[0], "read T");

    check_solution_S_using_S1_1(T, m);
    print_time(&tk[2], tk[1], "check solve S");

    free_mpq_array(T, N + 1, K + 1);
    print_time(&tk[3], tk[2], "free T");

    print_time(&tk[3], tk[0], __func__);
    return 0;
}

// 将所有任务按照 n 做模 M 同余划分, 仅执行与 m 同余的 n
void check_solution_S_using_S1_1(mpq_t **T, const int m) {
    // 记录开始时间
    clock_t tk[N/M + 2] = {clock()};
    mpq_t *S1_1 = mpq_malloc_init_list(N + 1);
    mpq_t *reciprocal = get_reciprocal(N + 1);

    logging("\nstep 1: initial finished\n");
    print_time(&tk[1], tk[0], "");
    logging("\n");

    int n, k, i, max_k, tk_index = 2;
    mpq_t Snik;
    mpq_init(Snik);

    // 在迭代状态中, S1_1[i], Snik 分别表示 S[n-1][i][1], S[n][i][k]
    // 2 <= n <= N, 1 <= i <= n, 1 <= k <= min(n-1, e ln n + e) 有效
    for (n = 2; n <= N; n++) {
        // 仅执行 n % M == m 的那部分 n
        if (!is_job_n(n, m)) {
            continue;
        }

        // S1_1 文件的最大文件名到 N, 名为 n 的文件最大索引为 n, 有效索引为 1 <= i <= n
        // 在并行文件调用计算的时候, 因为需要调用 S[n-1][i][1] 的数据, 所以需要调用名为 n - 1 的文件
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

        logging("n = %d, ", n);
        print_time(&tk[tk_index], tk[tk_index-1], "");
        tk_index++;
    }

    logging("\nstep 2: iteration finished");
    print_time(&tk[tk_index], tk[1], "step 2");
    logging("\n");

    // clear
    free_mpq_list(reciprocal, N + 1);
    free_mpq_list(S1_1, N + 1);
    mpq_clear(Snik);

    logging("\nstep 3: clear finished");
    print_time(&tk[tk_index+1], tk[tk_index], __func__);
    logging("\n");
}

// 文件名为 n 的 S1_1 的最大索引到 n, 有效索引为 1 <= i <= n
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

int is_job_n(const int n, const int m) {
    // n % M == m % M 说明当前的 n 须保留
    return n % M == m % M;
}
