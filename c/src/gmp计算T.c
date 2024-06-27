#include "tools.h"

int save_T(mpq_t**, char*);
void check_integer(mpq_t**);


int main() {
    clock_t tk[4] = {clock()};
    int re_run = 1;
    mpq_t **T;
    int n, k;

    if (re_run) {
        T = solution_T();
        print_time(&tk[1], tk[0], "solve T");

        save_T(T, "T.bin");
        print_time(&tk[2], tk[1], "save T");

    } else {
        T = read_T("T.bin");
        print_time(&tk[2], tk[0], "read T");
    }

    check_integer(T);
    free_mpq_array(T, N + 1, K + 1);
    print_time(&tk[3], tk[2], "check & free T");
    print_time(&tk[3], tk[0], __func__);

    return 0;
}

void check_integer(mpq_t** T) {
    int k, n;
    for (k = 1; k <= K; k++) {
        for (n = k; n <= N; n++) {
            print_if_integer(T[n][k], n, k, -1);
        }
    }
}

int save_T(mpq_t** T, char* file_name) {
    FILE *file = open_file(file_name, "wb");

    int k, n;
    for (k = 1; k <= K; k++) {
        for (n = k; n <= N; n++) {
            save_to(file, T[n][k], n, k, -1);
        }
    }

    fclose(file);
    return 0;
}
