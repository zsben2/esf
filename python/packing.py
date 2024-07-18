import os
import re

M = 100
N = 13542
pattern_M = r'#define M \d+'
replacement_M = '#define M {}'
pattern_m = r'#define m \d+'
replacement_m = '#define m {}'
base_dir = '..\\c'
src_dir = f'{base_dir}\\src'
lib_dir = f'{base_dir}\\lib'
output_dir = f'{base_dir}\\output'
packing_dir = '.\\packing'
is_mv = False


# M, m 字符串替换
def Mm_replace(m):
    with open(f'{src_dir}\\gmp计算Si_2.c', 'r') as Si_12_2:
        text = Si_12_2.read()

    text = re.sub(pattern_M, replacement_M.format(M), text)
    text = re.sub(pattern_m, replacement_m.format(m), text)

    with open(f'{src_dir}\\gmp计算Si_2.c', 'w') as Si_12_2:
        Si_12_2.write(text)


def system(command):
    print(command)
    os.system(command)


def packing():
    # 开一个作业组文件夹
    system(f'rmdir /s /q {packing_dir}\\')
    system(f'mkdir {packing_dir}\\')

    for m in range(M):
        # M, m 字符串替换
        Mm_replace(m)

        m_dir = f'{packing_dir}\\{m}'
        # 开一个作业文件夹
        system(f'mkdir {m_dir}')
        # 编译出 dynamic.exe 文件
        system('echo 正在进行动态编译')
        # system(f'gcc -o {m_dir}\\gmp_Si_1_dynamic {src_dir}\\gmp计算Si_1.c -lgmp')
        system(f'gcc -o {m_dir}\\gmp_Si_2_dynamic {src_dir}\\gmp计算Si_2.c -lgmp')

        # 编译出 static.exe 文件
        system('echo 正在进行静态编译')
        # system(f'gcc -o {m_dir}\\gmp_Si_1_static {src_dir}\\gmp计算Si_1.c -L{lib_dir} -lgmp')
        system(f'gcc -o {m_dir}\\gmp_Si_2_static {src_dir}\\gmp计算Si_2.c -L{lib_dir} -lgmp')

        # 编制运行文件
        with open(f'{m_dir}\\first-run.bat', 'w') as run_file:
            run_file.write('del integer.txt\n')
            run_file.write('del n.txt\n')
            run_file.write('gmp_Si_2_static.exe\n')
            run_file.write('pause\n')
        # 复制需要运行的动态库
        system(f'copy {output_dir}\\libgmp-10.dll {m_dir}\\libgmp-10.dll')

        # 开一个文件夹存数据
        system(f'mkdir {m_dir}\\S1_1')
        for n in range(2, N + 1):
            if n % M == m:
                n_1_file_name = '%05d' % (n - 1)
                system('{0} {1}\\S1_1\\{2}.bin {3}\\S1_1\\{2}.bin'.format(
                    'mv' if is_mv else 'copy',
                    output_dir,
                    n_1_file_name,
                    m_dir
                ))

        # 打包
        system(f'cd {packing_dir} && zip -r -o {m}.zip {m}')
        # 删除文件夹
        system(f'rmdir /s /q {m_dir}')


if __name__ == '__main__':
    # 打包
    packing()
