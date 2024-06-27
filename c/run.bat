echo "正在进行动态编译"
gcc -o "./output/gmp计算Si（动态）" "./src/gmp计算Si.c" -lgmp
echo "正在进行静态编译"
gcc -o "./output/gmp计算Si（静态）" "./src/gmp计算Si.c" -L./lib -lgmp
del integer.txt
pause