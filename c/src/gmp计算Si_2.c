#include "tools.h"
#define M 1000 // 可用于并行计算的电脑（或进程）数目
#define m 4 // m 表示机器号, 0 <= m < M

void check_solution_S_using_S1_1(const char*);
void read_S1_1(mpq_t*, const int);
int is_job_n(const int);
void save_n(const int, const char*);
int read_n(const char*);

int main() {
    clock_t tk[2] = {clock()};
    logging("\n");
    logging("============================================================\n");
    logging("本次作业 m = %d, M = %d\n", m, M);
    logging("============================================================\n");
    logging("\n");
    logging("当前算法利用各级 S1_1 仅对给定的部分 n 进行计算，将 T 与 S 合并计算以减少内存消耗\n");
    logging("当前算法将会缓存下来将要执行的 n, 以便在宕机时可立即从合适的 n 开始重跑\n");
    logging("当前算法消耗峰值内存为 140 MB\n");

    check_solution_S_using_S1_1("n.txt");
    print_time(&tk[1], tk[0], "check solve S");

    return 0;
}

// 将所有任务按照 n 做模 M 同余划分, 仅执行与 m 同余的 n
void check_solution_S_using_S1_1(const char *n_file_name) {
    // 记录开始时间
    clock_t tk[N/M + 2] = {clock()};

    mpq_t *reciprocal = get_reciprocal(N + 1);
    mpq_t *S1_1 = mpq_malloc_init_list(N + 1);
    mpq_t *T1 = mpq_malloc_init_list(K + 1);
    mpq_t *T2 = mpq_malloc_init_list(K + 1);
    mpq_t *T3, Snik; // 临时变量
    mpq_init(Snik);

    char n_value_log[20];
    int n, k, i, max_k, tk_index = 2;
    int n0 = read_n(n_file_name);

    logging("从 n = %d 开始运行\n", n0);
    print_time(&tk[1], tk[0], "step 1: initial finished,");
    logging("\n");

    /*
    因为在数学算式中有
    T[1][1] = 1
    T[n][1] = 1 / n + T[n-1][1] for 2 <= n <= N
    又因为在初始化变量时有
    T[0][1] = 0
    所以
    (eq1) T[n][1] = 1 / n + T[n-1][1] for 1 <= n <= N

    因为在数学算式中有
    T[k][k] = 1 / k * T[k-1][k-1] for 2 <= k <= K
    T[n][k] = 1 / n * T[n-1][k-1] + T[n-1][k] for 2 <= k <= K, k < n <= N
    又因为在初始化变量时有
    T[k-1][k] = 0 for 2 <= k <= K, i.e. T[1][2] = T[2][3] = ... = T[K-1][K] = 0
    所以
    (eq2) T[n][k] = 1 / n * T[n-1][k-1] + T[n-1][k] for 2 <= k <= K, k <= n <= N

    另外有
    (eq3) S[n][i][1] = S[n-1][i][1] + 1 / n for 1 <= i < n, n >= 2
    (eq4) S[n][i][k] = T[n][k] - 1 / i * S[n][i][k-1] for 2 <= k <= max_k, 1 <= i < n, n >= 2
    (eq5) S[n][n][k] = T[n-1][k] for 1 <= k <= max_k, n >= 2

    在迭代状态中, S1_1[i], Snik 分别表示 S[n-1][i][1], S[n][i][k],
    对 2 <= n <= N, 1 <= i <= n, 1 <= k <= min(n-1, e ln n + e) 有效.
    在迭代状态中, T1 用于表示 T[n-1], T2 用于表示 T[n], 对 1 <= n <= N 有效.
    */

    for (n = 1; n <= N; n++) {
        // k = 1
eq1:    mpq_add(T2[1], reciprocal[n], T1[1]);
        // k >= 2
        for (k = 2; k <= min(n, K); k++) {
eq2:        mpq_mul(T2[k], reciprocal[n], T1[k-1]);
            mpq_add(T2[k], T2[k], T1[k]);
        }

        // 仅计算 n >= n0 且 n % M == m 的那部分 n
        if (n >= n0 && is_job_n(n)) {
            // S1_1 文件的最大文件名到 N, 名为 n 的文件最大索引为 n, 有效索引为 1 <= i <= n
            // 因为需要使用 S[n-1][i][1] 的数据, 所以需要调用名为 n - 1 的文件
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

        // 滚动使用 T1, T2 空间
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

// S1_1 的最大索引到 N, 有效索引为 1 <= i <= n - 1
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
    // n % M == m % M 说明当前的 n 须保留
    return n % M == m % M;
}

void save_n(const int n, const char *file_name) {
    FILE *file = open_file(file_name, "w");
    fprintf(file, "%d", n + 1); // 注意下一轮循环将要运行的是 n + 1
    fclose(file);
}

int read_n(const char *file_name) {
    FILE *file = fopen(file_name, "r");
    // 找不到缓存文件，重头开始计算
    if (!file) {
        return 2;
    }

    int n;
    fscanf(file, "%d", &n);
    fclose(file);
    return n;
}
