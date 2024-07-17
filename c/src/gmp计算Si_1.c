#include "tools.h"

void solve_S1_1(void);

int main() {
    clock_t tk[3] = {clock()};

    logging("\n");
    logging("============================================================\n");
    logging("正在执行 Si_10_1\n");
    logging("============================================================\n");
    logging("\n");
    logging("当前算法首先计算全体 S[n][*][1] (1 <= n <= N), 保存在 ./%s/[n].bin 文件里\n", S1_1_DIR);
    logging("并将 T[n][1] 的计算合并入主循环中\n");
    logging("以便按照 n 划分任务进行多进程并行计算, 文件 [n].bin 里保存的就是 S[n][*][1] 的数据\n");
    logging("共运行 1 h 23 min 10.238000 s, 占用 278 GB 硬盘空间\n");

    print_time(&tk[1], tk[0], "read T_1");

    solve_S1_1();
    print_time(&tk[2], tk[0], __func__);

    return 0;
}

void solve_S1_1() {
    clock_t tk[N+2] = {clock()};

    // S1_1[i] 存储 S[n-1][i][1], T_1[n] 存储 T[n][1]
    mpq_t *S1_1 = mpq_malloc_init_list(N + 1); // S[1][1][1] = 0
    mpq_t *T_1 = mpq_malloc_init_list(N + 1); // T_1[0] = 0
    mpq_t *reciprocal = get_reciprocal(N + 1);
    print_time(&tk[1], tk[0], "initial finished");

    int n, i;
    FILE *file;
    char file_name[30], n_value_log[20];

    /*
    对 2 <= n <= N, 本函数的目标是要预先计算所有的 S[n][i][1] for 1 <= i <= n
    其中 1 <= i < n 时, S[n][i][1] = S[n-1][i][1] + 1 / n
    当 i = n 时, S[n][i][1] = S[n][n][1] = T[n-1][1]

    因为在数学算式中有
    S[1][1][1] = 0
    S[n][n][1] = T[n-1][1] for 2 <= n <= N
    又因为在初始化变量时有
    T[0][1] = 0
    所以有
    (eq1) S[n][n][1] = T[n-1][1] for 1 <= n <= N (i.e. i = n)

    对于
    (eq2) S[n][i][1] = S[n-1][i][1] + 1 / n for 2 <= n <= N, 1 <= i < n
    而言, 从编程变量的角度上来讲就是
    (eq3) S[n][i][1] += 1 / n for 2 <= n <= N, 1 <= i < n
    这是因为在 S[n][i][1] 迭代的过程中，总是存储着 n - 1 时的值
    特别强调: 第一次进入 i 循环时是初始状态 S1_1[1] = S[1][1][1] = 0

    另外有
    (eq4) T_1[n] = 1 / n + T_1[n-1] for 1 <= n <= N
    */
    for (n = 1; n <= N; n++) {
        // 在即时计算中 S1_1[i] 保存的就是 S[n][i][1] 的数据, 因而文件名直接用 n 来命名
        // 但是在并行文件调用计算的时候, 因为需要调用 S[n-1][i][1] 的数据, 所以需要调用名为 n - 1 的文件
        sprintf(file_name, "./%s/%05d.bin", S1_1_DIR, n);
        file = open_file(file_name, "wb");
        // S[n][0][1] 一直未被赋值和使用, 其值为 0, 导出是为了之后读取的对齐方便, 无实际计算意义
        save_to(file, S1_1[0], 0, -1);

        for (i = 1; i < n; i++) {
eq3:        mpq_add(S1_1[i], S1_1[i], reciprocal[n]);
            save_to(file, S1_1[i], i, -1);
        }

eq4:    mpq_add(T_1[n], reciprocal[n], T_1[n-1]);
eq1:    mpq_set(S1_1[n], T_1[n-1]);
        save_to(file, S1_1[n], n, -1);

        fclose(file);
        sprintf(n_value_log, "n = %d,", n);
        print_time(&tk[n], tk[n-1], n_value_log);
    }

    free_mpq_list(T_1, N + 1);
    free_mpq_list(S1_1, N + 1);
    free_mpq_list(reciprocal, N + 1);
    print_time(&tk[N+1], tk[0], __func__);
}
