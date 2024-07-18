cd ..\c
echo "正在进行动态编译"
gcc -o "./output/gmp_Si_1_dynamic" "./src/gmp计算Si_1.c" -lgmp
echo "正在进行静态编译"
gcc -o "./output/gmp_Si_1_static" "./src/gmp计算Si_1.c" -L./lib -lgmp
cd output
mkdir S1_1
gmp_Si_1_static.exe
cd ..\..\python
python packing.py
pause