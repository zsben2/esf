cd ..\c
echo "���ڽ��ж�̬����"
gcc -o "./output/gmp_Si_1_dynamic" "./src/gmp����Si_1.c" -lgmp
echo "���ڽ��о�̬����"
gcc -o "./output/gmp_Si_1_static" "./src/gmp����Si_1.c" -L./lib -lgmp
cd output
mkdir S1_1
gmp_Si_1_static.exe
cd ..\..\python
python packing.py
pause