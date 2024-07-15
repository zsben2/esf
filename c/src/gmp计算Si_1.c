#include "tools.h"

void solve_S1_1(void);

int main() {
    clock_t tk[3] = {clock()};

    logging("\n");
    logging("============================================================\n");
    logging("����ִ�� Si_10_1\n");
    logging("============================================================\n");
    logging("\n");
    logging("��ǰ�㷨���ȼ���ȫ�� S[n][*][1] (1 <= n <= N), ������ ./%s/[n].bin �ļ���\n", S1_1_DIR);
    logging("���� T[n][1] �ļ���ϲ�����ѭ����\n");
    logging("�Ա㰴�� n ����������ж���̲��м���, �ļ� [n].bin �ﱣ��ľ��� S[n][*][1] ������\n");
    logging("������ 1 h 23 min 10.238000 s, ռ�� 278 GB Ӳ�̿ռ�\n");

    print_time(&tk[1], tk[0], "read T_1");

    solve_S1_1();
    print_time(&tk[2], tk[0], __func__);

    return 0;
}

void solve_S1_1() {
    clock_t tk[N+2] = {clock()};

    // S1_1[i] �洢 S[n-1][i][1], T_1[n] �洢 T[n][1]
    mpq_t *S1_1 = mpq_malloc_init_list(N + 1); // S[1][1][1] = 0
    mpq_t *T_1 = mpq_malloc_init_list(N + 1); // T_1[0] = 0
    mpq_t *reciprocal = get_reciprocal(N + 1);
    print_time(&tk[1], tk[0], "initial finished");

    int n, i;
    FILE *file;
    char file_name[30], n_value_log[20];

    /*
    �� 2 <= n <= N, ��������Ŀ����ҪԤ�ȼ������е� S[n][i][1] for 1 <= i <= n
    ���� 1 <= i < n ʱ, S[n][i][1] = S[n-1][i][1] + 1 / n
    �� i = n ʱ, S[n][i][1] = S[n][n][1] = T[n-1][1]

    ��Ϊ����ѧ��ʽ����
    S[1][1][1] = 0
    S[n][n][1] = T[n-1][1] for 2 <= n <= N
    ����Ϊ�ڳ�ʼ������ʱ��
    T[0][1] = 0
    ������
    (eq1) S[n][n][1] = T[n-1][1] for 1 <= n <= N (i.e. i = n)

    ����
    (eq2) S[n][i][1] = S[n-1][i][1] + 1 / n for 2 <= n <= N, 1 <= i < n
    ����, �ӱ�̱����ĽǶ�����������
    (eq3) S[n][i][1] += 1 / n for 2 <= n <= N, 1 <= i < n
    ������Ϊ�� S[n][i][1] �����Ĺ����У����Ǵ洢�� n - 1 ʱ��ֵ
    �ر�ǿ��: ��һ�ν��� i ѭ��ʱ�ǳ�ʼ״̬ S1_1[1] = S[1][1][1] = 0

    ������
    (eq4) T_1[n] = 1 / n + T_1[n-1] for 1 <= n <= N
    */
    for (n = 1; n <= N; n++) {
        // �ڼ�ʱ������ S1_1[i] ����ľ��� S[n][i][1] ������, ����ļ���ֱ���� n ������
        // �����ڲ����ļ����ü����ʱ��, ��Ϊ��Ҫ���� S[n-1][i][1] ������, ������Ҫ������Ϊ n - 1 ���ļ�
        sprintf(file_name, "./%s/%05d.bin", S1_1_DIR, n);
        file = open_file(file_name, "wb");
        // S[n][0][1] һֱδ����ֵ��ʹ��, ��ֵΪ 0, ������Ϊ��֮���ȡ�Ķ��뷽��, ��ʵ�ʼ�������
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
