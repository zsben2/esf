echo "���ڽ��ж�̬����"
gcc -o "./output/gmp_Si_dynamic" "./src/gmp����Si.c" -lgmp
gcc -o "./output/gmp_T_dynamic" "./src/gmp����T.c" -lgmp
echo "���ڽ��о�̬����"
gcc -o "./output/gmp_Si_static" "./src/gmp����Si.c" -L./lib -lgmp
gcc -o "./output/gmp_T_static" "./src/gmp����T.c" -L./lib -lgmp
del ".\output\integer.txt"
pause