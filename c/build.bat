echo "���ڽ��ж�̬����"
gcc -o "./output/gmp_Si_1_dynamic" "./src/gmp����Si_1.c" -lgmp
gcc -o "./output/gmp_Si_2_dynamic" "./src/gmp����Si_2.c" -lgmp
echo "���ڽ��о�̬����"
gcc -o "./output/gmp_Si_1_static" "./src/gmp����Si_1.c" -L./lib -lgmp
gcc -o "./output/gmp_Si_2_static" "./src/gmp����Si_2.c" -L./lib -lgmp
del ".\output\integer.txt"
pause