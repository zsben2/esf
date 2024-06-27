#include "tools.h"

int save_S(mpq_t***, char*);
mpq_t ***read_S(char*);
mpq_t ***solution_S(mpq_t **);
void check_integer(mpq_t***);
void free_S(mpq_t***);

int main() {
    clock_t tk[5] = {clock()};
    int re_run = 1;
    mpq_t ***S;
    logging("每个 mpq_t 变量至少消耗 sizeof(mpq_t) = %d B 的空间\n", sizeof(mpq_t));
    logging(
        "当前算法计算的 S 将会消耗 (N * N * K / 2 + N) * %d B = %lf GB 的空间\n",
        sizeof(mpq_t),
        ((double) N * N * K / 2 + N) * sizeof(mpq_t)
    );
    logging("但会保存所有原始数据以便再用\n");
    logging("实际上就连开空间的那一步都不能成功执行\n");

    if (re_run) {
        mpq_t **T = read_T("T.bin");
        print_time(&tk[1], tk[0], "read T");

        S = solution_S(T);
        print_time(&tk[2], tk[1], "solve S");

        save_S(S, "S.bin");
        free_mpq_array(T, N + 1, K + 1);
        print_time(&tk[3], tk[2], "save S & free T");

    } else {
        S = read_S("S.bin");
        print_time(&tk[3], tk[0], "read S");
    }

    check_integer(S);
    free_S(S);
    print_time(&tk[4], tk[3], "check & free S");

    return 0;
}

mpq_t ***solution_S(mpq_t **T) {
    // 记录开始时间
    clock_t tk[N+2] = {clock()};

    int n, i;
    // 存放倒数, reciprocal[n] = 1 / n 是 2 <= n <= N 有效
    mpq_t *reciprocal = get_reciprocal(N + 1);
    // S[n][i][k] 是 2 <= n <= N, 1 <= i <= n, 1 <= k <= min(n-1, e ln n + e) 有效
    mpq_t ***S = mpq_malloc_init_triangle_block(N + 1, K + 1); // S[1][1][1] = 0

    logging("\nstep 1: initial finished\n"); // 前面开辟空间不能走到这一步

    // k = 1
    for (n = 2; n <= N; n++) {
        // 1 <= i < n
        for (i = 1; i < n; i++) {
            // S[n][i][1] = S[n-1][i][1] + 1/n
            mpq_add(S[n][i][1], S[n-1][i][1], reciprocal[n]);
        }

        // i = n
        // S[n][n][1] = T[n-1][1];
        mpq_set(S[n][n][1], T[n-1][1]); // 已约分
    }

    logging("\nstep 2: k = 1 finished\n");
    print_time(&tk[1], tk[0], "step 1, 2");
    logging("\n");

    mpq_t **Sn, *Sni;
    int k, max_k;
    // 2 <= k <= min(n-1, floor(e ln n + e))
    for (n = 2; n <= N; n++) {
        Sn = S[n];
        max_k = sup_K(n);

        for (i = 1; i <= n; i++) {
            Sni = Sn[i];

            for (k = 2; k <= max_k; k++) {
                // S[n][i][k] = T[n][k] - 1/i * S[n][i][k-1]
                mpq_mul(Sni[k], reciprocal[i], Sni[k-1]);
                mpq_sub(Sni[k], T[n][k], Sni[k]);
            }
        }
        logging("n = %d, ", n);
        print_time(&tk[n], tk[n-1], ""); // 打印时间
    }

    free_mpq_list(reciprocal, N + 1);
    logging("\nstep 3: 2 <= k <= 2 <= k <= min(n-1, floor(e ln n + e)) finished\n");
    print_time(&tk[N+1], tk[0], __func__);
    return S;
}

void check_integer(mpq_t*** S) {
    int k, n, i, max_k;
    for (n = 2; n <= N; n++) {
        max_k = sup_K(n);
        for (i = 1; i <= n; i++) {
            for (k = 1; k <= max_k; k++) {
                print_if_integer(S[n][i][k], n, i, k, -1);
            }
        }
    }
}

void free_S(mpq_t*** S) {
    int k, n, i;
    for (n = 0; n <= N; n++) {
        for (i = 0; i <= n; i++) {
            for (k = 0; k <= K; k++) {
                mpq_clear(S[n][i][k]);
            }
            free(S[n][i]);
        }
        free(S[n]);
    }
    free(S);
}

int save_S(mpq_t*** S, char* file_name) {
    FILE *file = open_file(file_name, "wb");

    int k, n, i, err = 0;
    for (n = 0; n <= N; n++) {
        for (i = 0; i <= n; i++) {
            for (k = 0; k <= K; k++) {
                save_to(file, S[n][i][k], n, i, k, -1);
            }
        }
    }

    fclose(file);
    return 0;
}

mpq_t*** read_S(char* file_name) {
    // S[n][i][k] 是 2 <= n <= N, 1 <= i <= n, 1 <= k <= min(n-1, e ln n + e) 有效
    mpq_t ***S = mpq_malloc_init_triangle_block(N + 1, K + 1);
    FILE *file = open_file(file_name, "rb"); // 打开名为"data.bin"的二进制文件（只读模式）
    int k, n, i;

    for (n = 0; n <= N; n++) {
        for (i = 0; i <= n; i++) {
            for (k = 0; k <= K; k++) {
                mpq_read(S[n][i][k], file);
            }
        }
    }

    fclose(file);
    return S;
}