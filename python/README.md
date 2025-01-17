# 编译运行注意

本工程是 C 程序前后工作的一些脚本。

## find_prime.py

`find_prime.py` 的运行将会生成日志文件 `output/log.txt` 和缓存文件 `output/result.bin` 。

设

1. $N = 50216$;
2. $K = \lfloor e \ln N + e\rfloor = 32$;
3. $2 \le n \le N$;
4. $1 \le k\le \min\\{n-1, K\\}$.

本程序用于查找满足下列条件的素数 $p$ ：

5. $\frac{n}{k+3} < p \le \frac{n}{k+1}$;
6. $p > \max\\{(k+2)(k+3)/2, 3k+8\\}$.

存在找不到满足上述条件素数的 $n$ 就需要用 C 程序去做进一步验证。

由于 `find_prime` 函数运行时间在 1 分钟左右，因此我将其返回结果用 `output/result.bin` 缓存起来，以备直接读取复用。 

`transposition` 函数的日志结果输出如下：

```log
{[1, 1]: {'find': [], 'not_find': [[1, 2]]},
 [2, 2]: {'find': [], 'not_find': [[1, 4]]},
 [3, 3]: {'find': [], 'not_find': [[1, 5]]},
 [4, 4]: {'find': [], 'not_find': [[1, 6]]},
 [5, 6]: {'find': [], 'not_find': [[1, 7]]},
 [7, 10]: {'find': [], 'not_find': [[1, 8]]},
 [11, 14]: {'find': [], 'not_find': [[1, 9]]},
 [15, 21]: {'find': [], 'not_find': [[1, 10]]},
 [22, 25]: {'find': [], 'not_find': [[1, 11]]},
 [26, 30]: {'find': [[1, 1]], 'not_find': [[2, 11]]},
 [31, 43]: {'find': [[1, 1]], 'not_find': [[2, 12]]},
 [44, 50]: {'find': [[1, 1]], 'not_find': [[2, 13]]},
 [51, 63]: {'find': [[1, 2]], 'not_find': [[3, 13]]},
 [64, 75]: {'find': [[1, 2]], 'not_find': [[3, 14]]},
 [76, 91]: {'find': [[1, 3]], 'not_find': [[4, 14]]},
 [92, 114]: {'find': [[1, 3]], 'not_find': [[4, 15]]},
 [115, 132]: {'find': [[1, 4]], 'not_find': [[5, 15]]},
 [133, 173]: {'find': [[1, 4]], 'not_find': [[5, 16]]},
 [174, 191]: {'find': [[1, 5]], 'not_find': [[6, 16]]},
 [192, 258]: {'find': [[1, 5]], 'not_find': [[6, 17]]},
 [259, 276]: {'find': [[1, 6]], 'not_find': [[7, 17]]},
 [277, 375]: {'find': [[1, 6]], 'not_find': [[7, 18]]},
 [376, 399]: {'find': [[1, 7]], 'not_find': [[8, 18]]},
 [400, 530]: {'find': [[1, 7]], 'not_find': [[8, 19]]},
 [531, 576]: {'find': [[1, 8]], 'not_find': [[9, 19]]},
 [577, 669]: {'find': [[1, 8]], 'not_find': [[9, 20]]},
 [670, 833]: {'find': [[1, 9]], 'not_find': [[10, 20]]},
 [834, 868]: {'find': [[1, 9]], 'not_find': [[10, 21]]},
 [869, 1163]: {'find': [[1, 10]], 'not_find': [[11, 21]]},
 [1164, 1203]: {'find': [[1, 11]], 'not_find': [[12, 21]]},
 [1204, 1390]: {'find': [[1, 11]], 'not_find': [[12, 22]]},
 [1391, 1739]: {'find': [[1, 12]], 'not_find': [[13, 22]]},
 [1740, 1777]: {'find': [[1, 12]], 'not_find': [[13, 23]]},
 [1778, 2054]: {'find': [[1, 13]], 'not_find': [[14, 23]]},
 [2055, 2511]: {'find': [[1, 14]], 'not_find': [[15, 23]]},
 [2512, 2512]: {'find': [[1, 15]], 'not_find': [[16, 23]]},
 [2513, 2940]: {'find': [[1, 15]], 'not_find': [[16, 24]]},
 [2941, 3437]: {'find': [[1, 16]], 'not_find': [[17, 24]]},
 [3438, 3629]: {'find': [[1, 17]], 'not_find': [[18, 24]]},
 [3630, 4008]: {'find': [[1, 17]], 'not_find': [[18, 25]]},
 [4009, 4659]: {'find': [[1, 18]], 'not_find': [[19, 25]]},
 [4660, 5244]: {'find': [[1, 19]], 'not_find': [[20, 25]]},
 [5245, 5396]: {'find': [[1, 19]], 'not_find': [[20, 26]]},
 [5397, 6093]: {'find': [[1, 20]], 'not_find': [[21, 26]]},
 [6094, 7060]: {'find': [[1, 21]], 'not_find': [[22, 26]]},
 [7061, 7576]: {'find': [[1, 22]], 'not_find': [[23, 26]]},
 [7577, 7943]: {'find': [[1, 22]], 'not_find': [[23, 27]]},
 [7944, 8824]: {'find': [[1, 23]], 'not_find': [[24, 27]]},
 [8825, 9853]: {'find': [[1, 24]], 'not_find': [[25, 27]]},
 [9854, 10944]: {'find': [[1, 25]], 'not_find': [[26, 27]]},
 [10945, 11042]: {'find': [[1, 25]], 'not_find': [[26, 28]]},
 [11043, 12291]: {'find': [[1, 26]], 'not_find': [[27, 28]]},
 [12292, 13542]: {'find': [[1, 27]], 'not_find': [[28, 28]]},
 [13543, 15811]: {'find': [[1, 28]], 'not_find': []},
 [15812, 22842]: {'find': [[1, 29]], 'not_find': []},
 [22843, 32999]: {'find': [[1, 30]], 'not_find': []},
 [33000, 47674]: {'find': [[1, 31]], 'not_find': []},
 [47675, 50216]: {'find': [[1, 32]], 'not_find': []}}
```

对于如下结构的日志：

```log
[a, b]: {'find': [[c, d]], 'not_find': [[e, f]]}
```

其意思表示为：对于 $a\le n\le b$ ，位于 $c \le k\le d$ 中的 $k$ 是能找到满足条件 5, 6 的素数的；位于 $e\le k\le f$ 中的 $k$ 是找不到满足条件 5, 6 的素数的。若不存在 `[c, d]` 或 `[e, f]` ，则表示没有这样的 $k$ 。

因此需要做进一步验证的 $n$ 是那些存在找不到满足条件 5, 6 的素数的 $n$ ，即 $1\le n\le 13542$ 。（其中 $n=1$ 舍去）



## packing.py

本程序用于 C 程序运行后的打包分发工作。

程序第 4 行的 `M` 表示可以同时执行程序的进程数（如果一部计算机只执行一个进程的话，亦可作计算机数）。本程序会在当前目录创建一个 `packing/` 文件夹里面存放 $M$ 个压缩文件 `[m].zip` ，其中 $0\le m < M$ 。这 $M$ 个压缩文件解压后即为一个完整第 $m$ 号任务所需要的 ***程序 2*** 及 **数据** 。

上述 ***程序 2*** 是指：

1. 编译好第 $m$ 号任务的 `gmp_Si_2_dynamic.exe` 和 `gmp_Si_2_static.exe` 。
2. `gmp_Si_2_dynamic.exe` 执行所需要依赖的 `libgmp-10.dll`
3. *首次* 运行第 $m$ 号任务的所有命令脚本 `first-run.bat` 。

注意脚本 `first-run.bat` 仅可用于 *首次* 运行该任务，因为该脚本会删除程序生成的 `integer.txt` 文件和 `n.txt` 文件。该脚本的内容如下：

```shell
del integer.txt
del n.txt
gmp_Si_2_static.exe
pause
```

如果程序被中断，需要继续执行的话，请直接执行 `gmp_Si_2_static.exe` 。

上述 **数据** 是指第 $m$ 号任务将要执行的 $n$ 值所用到的 `S1_1` 数据，被存放在文件夹 `S1_1/` 中。因为程序只会执行模 $M$ 同余 $m$ 的 $n$ ，而执行 $n$ 需要调用 `S1_1[n-1]` 的数据，因此文件夹 `S1_1/` 中存放的是 `[n-1].bin` 文件，其中 $n\equiv m \pmod M$ ， $2\le n\le 13542$ 。（显然， `13542.bin` 将不会被使用）
