echo "���ڽ��ж�̬����"
gcc -o "./output/gmp����Si����̬��" "./src/gmp����Si.c" -lgmp
gcc -o "./output/gmp����T����̬��" "./src/gmp����T.c" -lgmp
echo "���ڽ��о�̬����"
gcc -o "./output/gmp����Si����̬��" "./src/gmp����Si.c" -L./lib -lgmp
gcc -o "./output/gmp����T����̬��" "./src/gmp����T.c" -L./lib -lgmp
del "./output/integer.txt"
pause