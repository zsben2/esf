echo "正在进行动态编译"
gcc -o "./output/gmp_Si_1_dynamic" "./src/gmp计算Si_1.c" -lgmp
gcc -o "./output/gmp_Si_2_dynamic" "./src/gmp计算Si.c" -lgmp
gcc -o "./output/gmp_T_dynamic" "./src/gmp计算T.c" -lgmp
echo "正在进行静态编译"
gcc -o "./output/gmp_Si_1_static" "./src/gmp计算Si_1.c" -L./lib -lgmp
gcc -o "./output/gmp_Si_2_static" "./src/gmp计算Si.c" -L./lib -lgmp
gcc -o "./output/gmp_T_static" "./src/gmp计算T.c" -L./lib -lgmp
del ".\output\integer.txt"
pause