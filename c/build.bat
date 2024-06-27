echo "正在进行动态编译"
gcc -o "./output/gmp计算Si（动态）" "./src/gmp计算Si.c" -lgmp
gcc -o "./output/gmp计算T（动态）" "./src/gmp计算T.c" -lgmp
echo "正在进行静态编译"
gcc -o "./output/gmp计算Si（静态）" "./src/gmp计算Si.c" -L./lib -lgmp
gcc -o "./output/gmp计算T（静态）" "./src/gmp计算T.c" -L./lib -lgmp
del "./output/integer.txt"
pause