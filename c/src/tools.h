// code = GBK
#ifndef HEAD_H_INCLUDED
#define HEAD_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gmp.h>
#include <time.h>
#include <stdarg.h>

#define N 13542
#define K 28
// 缓存的 S1_1 文件目录
#define S1_1_DIR "S1_1"

mpq_t **solution_T(void);
int min(const int, const int);
int sup_K(const int);
void print_if_integer(const mpq_t, ...); // 以-1结尾表示可变参数终止
void save_to(FILE*, const mpq_t, ...); // 以-1结尾表示可变参数终止
void print_time(clock_t*, const clock_t, const char*);
void mpz_read(mpz_t, FILE*, char*);
void mpq_read(mpq_t, FILE*);
mpq_t** read_T(const char*);
FILE *open_file(const char*, const char*);
mpq_t *mpq_malloc_init_list(const int);
mpq_t **mpq_ptr_malloc_list(const int);
mpq_t **mpq_malloc_init_array(const int, const int);
mpq_t ***mpq_malloc_init_triangle_block(const int, const int);
void free_mpq_list(mpq_t*, const int);
void free_mpq_array(mpq_t**, const int, const int);
mpq_t *get_reciprocal(const int);
void logging(char*, ...);
void logging_without_time(char*, ...);


int min(const int a, const int b) {
    return a < b ? a : b;
}

int sup_K(const int n) {
    return min(n - 1, (int) (M_E * log(n) + M_E)); // <= K
}

void print_if_integer(const mpq_t s, ...) {
    // 如果非（分母为1，且分数非零）
    // if (!(mpz_divisible_p(mpq_numref(S[k][n]), mpq_denref(S[k][n])) &&
    //       mpq_sgn(S[k][n]))) {
    // if (!(!mpz_cmp_ui(mpq_denref(s), 1) && mpq_sgn(s))) {
    if (mpz_cmp_ui(mpq_denref(s), 1) || !mpq_sgn(s)) {
        return;
    }

    FILE *file = open_file("integer.txt", "a"); // 追加模式
    logging("S");
    fprintf(file, "S");

    va_list p_args;
    // 初始化 p_args 指针，使其指向第一个可变参数。 s 是变参列表的前一个参数
    va_start(p_args, s);
    int index = va_arg(p_args, int); // 下一个参数

    while (index != -1) {
        logging_without_time("[%d]", index);
        fprintf(file, "[%d]", index);
        index = va_arg(p_args, int); // 下一个参数
    }

    logging_without_time(" = %Zd\n", mpq_numref(s));
    gmp_fprintf(file, " = %Zd\n", mpq_numref(s));

    fclose(file);
    va_end(p_args);
}

void save_to(FILE *file, const mpq_t s, ...) {
    va_list p_args;
    // 初始化 p_args 指针，使其指向第一个可变参数。 s 是变参列表的前一个参数
    va_start(p_args, s);
    int err = 0;

    // 写入分子
    if (!mpz_out_raw(file, mpq_numref(s)) || err) {
        err = 1;
    }
    // 写入分母
    if (!mpz_out_raw(file, mpq_denref(s)) || err) {
        err = 2;
    }

    // 出错处理
    switch (err) {
        case 0: return;
        case 1: logging("分子写入失败\n"); break;
        case 2: logging("分母写入失败\n"); break;
    }

    logging("S");
    int index = va_arg(p_args, int); // 下一个参数
    while (index != -1) {
        logging_without_time("[%d]", index);
        index = va_arg(p_args, int); // 下一个参数
    }
    logging_without_time(" = %Qd\n", s);

    va_end(p_args);
    fclose(file);
    exit(-1);
}

void print_time(clock_t* p_end_time,
                const clock_t start_time,
                const char *note) {
    // 记录结束时间
    *p_end_time = clock();
    // 计算CPU使用时间(s)
    double cpu_seconds_used =
        ((double) (*p_end_time - start_time)) / CLOCKS_PER_SEC;
    if (*note) {
        logging("%s process use time: ", note);
    } else {
        logging("process use time: ");
    }
    // 打印CPU使用时间
    if (cpu_seconds_used - 60.0l < 1e-8l) {
        logging_without_time("%f s\n", (float) cpu_seconds_used);
    } else {
        long cpu_minute_used = cpu_seconds_used / 60l;
        cpu_seconds_used -= cpu_minute_used * 60l;
        if (cpu_minute_used < 60l) {
            logging_without_time("%d min %f s\n",
                (int) cpu_minute_used, (float) cpu_seconds_used);
        } else {
            int cpu_hour_used = cpu_minute_used / 60;
            cpu_minute_used -= cpu_hour_used * 60;
            logging_without_time("%d h %d min %f s\n",
                cpu_hour_used, (int) cpu_minute_used, (float) cpu_seconds_used);
        }
    }
}

void mpz_read(mpz_t z, FILE *file, char *note) {
    if (!mpz_inp_raw(z, file)) {
        logging("%s读入失败\n", note);
        fclose(file);
        exit(-1);
    }
}

void mpq_read(mpq_t s, FILE *file) {
    mpz_t numerator, denominator;
    mpz_inits(numerator, denominator, NULL);
    mpz_read(numerator, file, "分子");
    mpz_read(denominator, file, "分母");
    // 令 s = numerator / denominator
    mpz_set(mpq_numref(s), numerator);
    mpz_set(mpq_denref(s), denominator);
    // mpq_canonicalize(s); // 毋须约分
}

mpq_t** read_T(const char* file_name) {
    // T[n][k] 是 1 <= k <= K, k <= n <= N 有效;
    mpq_t **T = mpq_malloc_init_array(N + 1, K + 1);
    FILE *file = open_file(file_name, "rb");
    int k, n;

    for (k = 1; k <= K; k++) {
        for (n = k; n <= N; n++) {
            mpq_read(T[n][k], file);
        }
    }

    fclose(file);
    return T;
}

FILE *open_file(const char *file_name, const char *mode) {
    FILE *file = fopen(file_name, mode);
    if (!file) {
        logging("无法以 %s 方式打开文件 %s\n", mode, file_name);
        exit(-1);
    }
    return file;
}

mpq_t *mpq_malloc_init_list(const int len) {
    mpq_t *R = (mpq_t *) malloc(sizeof(mpq_t) * len);

    if (!R) {
        logging("空间开辟失败\n");
        exit(-1);
    }

    int l;
    for (l = 0; l < len; l++) {
        mpq_init(R[l]);
    }

    return R;
}

mpq_t **mpq_ptr_malloc_list(const int width) {
    mpq_t **R = (mpq_t **) malloc(sizeof(mpq_t *) * width);

    if (!R) {
        logging("空间开辟失败\n");
        exit(-1);
    }

    return R;
}

// 将会消耗至少 N * K * 32 B = 11.556 MB 的空间
mpq_t **mpq_malloc_init_array(const int width, const int len) {
    mpq_t **R = mpq_ptr_malloc_list(width);

    int w;
    for (w = 0; w < width; w++) {
        R[w] = mpq_malloc_init_list(len);
    }

    return R;
}

// 将会消耗至少 N * N * K / 2 * 32 B = 76.5145 GB 的空间
mpq_t ***mpq_malloc_init_triangle_block(const int height, const int len) {
    mpq_t ***R = (mpq_t ***) malloc(sizeof(mpq_t **) * height);

    if (!R) {
        logging("空间开辟失败\n");
        exit(-1);
    }

    int h;
    for (h = 0; h < height; h++) {
        R[h] = mpq_malloc_init_array(h + 1, len);
    }

    return R;
}

void free_mpq_list(mpq_t *R, const int len) {
    int l;
    for (l = 0; l < len; l++) {
        mpq_clear(R[l]);
    }
    free(R);
}

void free_mpq_array(mpq_t **R, const int width, const int len) {
    int w;
    for (w = 0; w < width; w++) {
        free_mpq_list(R[w], len);
    }
    free(R);
}

mpq_t *get_reciprocal(const int len) {
    int n;
    mpq_t *reciprocal = mpq_malloc_init_list(len);
    for (n = 1; n < len; n++) {
        mpq_set_ui(reciprocal[n], 1, n);
    }
    return reciprocal;
}

void logging(char *str, ...) {
    time_t timer = time(NULL);
    char *time_format = strtok(ctime(&timer), "\n");

    va_list p_args;
    va_start(p_args, str);

    printf(time_format);
    printf(" - ");
    gmp_vprintf(str, p_args);

    FILE *file = fopen("log.txt", "a");
    if (!file) {
        printf("无法打开日志文件\n");
        exit(-1);
    }

    fprintf(file, time_format);
    fprintf(file, " - ");
    gmp_vfprintf(file, str, p_args);
    fclose(file);

    va_end(p_args);
}

void logging_without_time(char *str, ...) {
    va_list p_args;
    va_start(p_args, str);

    FILE *file = fopen("log.txt", "a");
    if (!file) {
        printf("无法打开日志文件\n");
        exit(-1);
    }

    gmp_vprintf(str, p_args);
    gmp_vfprintf(file, str, p_args);
    fclose(file);

    va_end(p_args);
}

mpq_t** solution_T() {
    // 记录开始时间
    clock_t tk[K+2] = {clock()};

    int k, n;
    // 倒数 reciprocal[n] = 1 / n 是 1 <= n <= N 有效
    mpq_t *reciprocal = get_reciprocal(N + 1);
    // T[n][k] 对 1 <= k <= K, k <= n <= N 有效
    mpq_t **T = mpq_malloc_init_array(N + 1, K + 1);

    /*
    因为在数学算式中有
    T[1][1] = 1
    T[n][1] = 1 / n + T[n-1][1] for 2 <= n <= N
    又因为在初始化变量时有
    T[0][1] = 0
    所以
    (eq1) T[n][1] = 1 / n + T[n-1][1] for 1 <= n <= N
    */
    for (n = 1; n <= N; n++) {
eq1:    mpq_add(T[n][1], reciprocal[n], T[n-1][1]);
    }
    logging("step 1, k = 1\n");
    print_time(&tk[1], tk[0], "k = 1,"); // 打印时间

    /*
    因为在数学算式中有
    T[k][k] = 1 / k * T[k-1][k-1] for 2 <= k <= K
    T[n][k] = 1 / n * T[n-1][k-1] + T[n-1][k] for 2 <= k <= K, k < n <= N
    又因为在初始化变量时有
    T[k-1][k] = 0 for 2 <= k <= K, i.e. T[1][2] = T[2][3] = ... = T[K-1][K] = 0
    所以
    (eq2) T[n][k] = 1 / n * T[n-1][k-1] + T[n-1][k] for 2 <= k <= K, k <= n <= N
    */
    char k_value_log[20];
    for (k = 2; k <= K; k++) {
        for (n = k; n <= N; n++) {
eq2:        mpq_mul(T[n][k], reciprocal[n], T[n-1][k-1]);
            mpq_add(T[n][k], T[n][k], T[n-1][k]);
        }
        sprintf(k_value_log, "k = %d,", k);
        print_time(&tk[k], tk[k-1], k_value_log); // 打印时间
    }

    free_mpq_list(reciprocal, N + 1);
    logging("\n");
    logging("step 2\n");
    logging("\n");
    print_time(&tk[K+1], tk[0], __func__);
    return T;
}

#endif // HEAD_H_INCLUDED
