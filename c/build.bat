echo "���ڽ��ж�̬����"
gcc -o "./output/gmp����Si����̬��" "./src/gmp����Si.c" -lgmp
echo "���ڽ��о�̬����"
gcc -o "./output/gmp����Si����̬��" "./src/gmp����Si.c" -L./lib -lgmp
del integer.txt
pause