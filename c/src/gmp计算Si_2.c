#include "tools.h"
// 可用于并行计算的电脑（或进程）数目
// M = 1 时占用5.6 - 5.8 GB 内存
// M = 5 时占用3.9 - 4.0 GB 内存
// M = 7 时占用3.6 - 3.7 GB 内存
// M = 10 时占用3.3 - 3.4 GB 内存
// M = 15 时占用3.1 - 3.2 GB 内存
// M = 100 时占用2.8 - 2.9 GB 内存
#define M 1000


void check_solution_S_using_S1_1(mpq_t**, const int, const char*);
void read_S1_1(mpq_t*, const int);
void free_mpq_list(mpq_t*, const int);
void free_part_T(mpq_t **, const int);
mpq_t **get_part_T(const char*, const int, const int);
int is_job_n(const int, const int);
int is_save_n(const int, const int);
void save_n(const int, const char*);
int read_n(const char*);

int main() {
    int m = 0; // m 表示机器号, 0 <= m < M
    clock_t tk[4] = {clock()};
    mpq_t ***S;
    logging("\n");
    logging("============================================================\n");
    logging("本次作业 m = %d, M = %d\n", m, M);
    logging("============================================================\n");
    logging("\n");
    logging("当前算法利用各级 S1_1 仅对给定的部分 n 进行计算，仅读取需要用到的 T 或计算全量 T 并动态释放内存\n");
    logging("当前算法将会缓存下来将要执行的 n, 以便在宕机时可立即从合适的 n 开始重跑\n");
    logging("计算 T 且 M = 1 时消耗峰值内存为 2992 MB\n");
    logging("读取 T 且 M = 1 时消耗峰值内存为 5.8 GB\n");

    mpq_t **T = get_part_T("T.bin", m, 0); // 仅保留用到的行列

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

// 将所有任务按照 n 做模 M 同余划分, 仅执行与 m 同余的 n
void check_solution_S_using_S1_1(mpq_t **T, const int m, const char *n_file_name) {
    // 记录开始时间
    clock_t tk[N/M + 2] = {clock()};
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
    logging("从 n = %d 开始运行\n", n);

    // 在迭代状态中, S1_1[i], Snik 分别表示 S[n-1][i][1], S[n][i][k]
    // 2 <= n <= N, 1 <= i <= n, 1 <= k <= min(n-1, e ln n + e) 有效
    for (; n <= N; n++) {
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

mpq_t **get_part_T(const char *file_name, const int m, const int is_read_T_file) {
    mpq_t **T;
    int n;
    if (is_read_T_file) {
        T = mpq_ptr_malloc_list(N + 1); // T[n][k] 是 1 <= k <= K, k < n <= N 有效;

        // T[0] 未开辟空间, 仅开辟用到的空间
        for (n = 1; n <= N; n++) {
            if (is_save_n(n, m)) {
                T[n] = mpq_malloc_init_list(K + 1);
            }
        }

        int k;
        mpq_t temp;
        mpq_init(temp);
        FILE *file = open_file(file_name, "rb");

        for (k = 1; k <= K; k++) {
            for (n = k; n <= N; n++) {
                mpq_read(temp, file);
                if (is_save_n(n, m)) {
                    mpq_set(T[n][k], temp);
                }
            }
        }

        mpq_clear(temp);
        fclose(file);
    } else {
        T = solution_T();
        for (n = 1; n <= N; n++) {
            if (!is_save_n(n, m)) {
                free_mpq_list(T[n], K + 1);
            }
        }
    }
    return T;
}

void free_part_T(mpq_t **T, const int n) {
    int k;
    free_mpq_list(T[n-1], K + 1);
    if (M >= 2) {
        free_mpq_list(T[n], K + 1);
    }
}

int is_job_n(const int n, const int m) {
    // n % M == m % M 说明当前的 n 须保留
    return n % M == m % M;
}

int is_save_n(const int n, const int m) {
    // 须保留 n % M == m % M 时的 n, n - 1 列
    // (n + 1) % M == m % M 说明当前的 n 是下一个 n' 的 n' - 1, 且有 n' % M == m % M, 那么这个 n 需要保留
    return is_job_n(n + 1, m) || is_job_n(n, m);
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