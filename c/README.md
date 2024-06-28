# 编译运行注意

使用静态库编译时，请将 `D:\Program\mingw64\x86_64-w64-mingw32\include\gmp.h` 文件的 `__GMP_LIBGMP_DLL` 值改为 `0` 。

参考自 https://github.com/m-ab-s/media-autobuild_suite/issues/2352#issuecomment-1368171107

## 文件介绍

### 编译脚本

文件 `build.bat` 用于在 Windows 下编译目录 `src/` 中的 `.c` 文件，并将可执行文件 `.exe` 输出至 `output/` 目录中。

### 运行脚本

文件 `run.bat` 用于在 Windows 下运行目录 `output/` 中的 `.exe` 文件，并将日志文件 `log.txt` 、整数校验文件 `integer.txt` 、数组 T 的二进制缓存文件 `T.bin` 输出至 `output/` 目录中。

### 整数校验文件

文件 `output/integer.txt` 中会输出经校验为整数的 `T[n][k]` 和 `S[n][i][k]` 。但因为头文件里写死了 `S` 开头，所以在文件 `integer.txt` 中实际打印为  `S[n][k]` 和 `S[n][i][k]` 。

### 数组 T 的元素缓存文件

文件 `output/T.bin` 中保存了数组 `T[n][k]` 的所有元素。数组索引的有效范围可参考：

```c
for (k = 1; k <= K; k++) {
    for (n = k; n <= N; n++) {
        save_to(file, T[n][k], n, k, -1);
    }
} 
```

### 数组 S 的元素缓存文件

文件 `output/S.bin` 中保存了数组 `S[n][i][k]` 的所有元素。数组索引的有效范围可参考：

```c
for (n = 2; n <= N; n++) {
    max_K = min(n - 1, (int) (E * log(n) + E))
    // 1 <= i < n
    for (i = 1; i < n; i++) {
        // k = 1, S[n][i][1] = S[n-1][i][1] + 1/n
        save_to(file, S2i_[1], n, i, 1, -1);
        // S[n][i][k] = T[n][k] - 1/i * S[n][i][k-1] for 2 <= k <= max_k
        for (k = 2; k <= max_k; k++)
            save_to(file, S2i_[k], n, i, k, -1);
    }
    // S[n][n][k] = T[n-1][k] for 1 <= k <= max_k
    for (k = 1; k <= max_k; k++)
        save_to(file, S2i_[k], n, n, k, -1);
}
```

### 日志文件

文件 `output/log.txt` 中会打印出程序执行期间的一些日志。

当前是第 1 版串行代码，需要消耗极大量的内存空间，以至于根本不可能跑下来。下面是一次试跑的日志，可参考其程序运行时间和执行的步骤。

**执行 `gmp_T_static.exe` 的日志**

```log
Thu Jun 27 16:43:47 2024 - step 1, k = 1
Thu Jun 27 16:43:47 2024 - k = 1, process use time: 0.103000 s
Thu Jun 27 16:43:50 2024 - k = 2, process use time: 2.356000 s
Thu Jun 27 16:43:54 2024 - k = 3, process use time: 3.834000 s
Thu Jun 27 16:43:59 2024 - k = 4, process use time: 5.658000 s
Thu Jun 27 16:44:05 2024 - k = 5, process use time: 5.884000 s
Thu Jun 27 16:44:12 2024 - k = 6, process use time: 7.083000 s
Thu Jun 27 16:44:20 2024 - k = 7, process use time: 7.795000 s
Thu Jun 27 16:44:28 2024 - k = 8, process use time: 7.590000 s
Thu Jun 27 16:44:36 2024 - k = 9, process use time: 7.854000 s
Thu Jun 27 16:44:44 2024 - k = 10, process use time: 8.485000 s
Thu Jun 27 16:44:53 2024 - k = 11, process use time: 8.692000 s
Thu Jun 27 16:45:02 2024 - k = 12, process use time: 9.186000 s
Thu Jun 27 16:45:11 2024 - k = 13, process use time: 9.392000 s
Thu Jun 27 16:45:21 2024 - k = 14, process use time: 9.712000 s
Thu Jun 27 16:45:32 2024 - k = 15, process use time: 10.819000 s
Thu Jun 27 16:45:42 2024 - k = 16, process use time: 10.617000 s
Thu Jun 27 16:45:53 2024 - k = 17, process use time: 10.639000 s
Thu Jun 27 16:46:04 2024 - k = 18, process use time: 10.858000 s
Thu Jun 27 16:46:16 2024 - k = 19, process use time: 11.817000 s
Thu Jun 27 16:46:27 2024 - k = 20, process use time: 11.397000 s
Thu Jun 27 16:46:39 2024 - k = 21, process use time: 11.612000 s
Thu Jun 27 16:46:51 2024 - k = 22, process use time: 11.808000 s
Thu Jun 27 16:47:03 2024 - k = 23, process use time: 12.095000 s
Thu Jun 27 16:47:15 2024 - k = 24, process use time: 12.462000 s
Thu Jun 27 16:47:28 2024 - k = 25, process use time: 12.808000 s
Thu Jun 27 16:47:40 2024 - k = 26, process use time: 12.563000 s
Thu Jun 27 16:47:53 2024 - k = 27, process use time: 12.803000 s
Thu Jun 27 16:48:06 2024 - k = 28, process use time: 12.985000 s
Thu Jun 27 16:48:06 2024 - 
Thu Jun 27 16:48:06 2024 - step 2
Thu Jun 27 16:48:06 2024 - 
Thu Jun 27 16:48:06 2024 - solution_T process use time: 4 min 19.002001 s
Thu Jun 27 16:48:06 2024 - solve T process use time: 4 min 19.006001 s
Thu Jun 27 16:48:52 2024 - save T process use time: 45.862000 s
Thu Jun 27 16:48:52 2024 - S[1][1] = 1
Thu Jun 27 16:48:53 2024 - S[3][2] = 1
Thu Jun 27 16:48:53 2024 - check & free T process use time: 1.028000 s
Thu Jun 27 16:48:53 2024 - main process use time: 5 min 5.902000 s
```

**执行 `gmp_Si_static.exe` 的日志**

仅试跑了一点点数据，没有跑完。

```log
Fri Jun 28 11:22:41 2024 - 当前算法仅开放一行 i 的空间滚动使用，并放弃使用一个闲置空间
Fri Jun 28 11:22:41 2024 - 去除保存文件的语句
Fri Jun 28 11:22:41 2024 - 预计需要运行 14 day 6 h 25 min 39 s = 1232739 s
Fri Jun 28 11:22:41 2024 - 经初步运行的数据分析，单个 n 运行时间服从 0 - 0.000740566 * n + 1.57395e-06 * n^2
Fri Jun 28 11:22:41 2024 - 总计运行时间服从 0 + 0.197321 * n - 0.000455747 * n^2 + 5.28968e-07 * n^3
Fri Jun 28 11:23:17 2024 - read T process use time: 35.432999 s
Fri Jun 28 11:23:17 2024 - 
step 1: initial finished
Fri Jun 28 11:23:17 2024 - process use time: 0.006000 s
Fri Jun 28 11:23:17 2024 - 
Fri Jun 28 11:23:17 2024 - S[2][2][1] = 1
Fri Jun 28 11:23:17 2024 - n = 2, Fri Jun 28 11:23:17 2024 - process use time: 0.222000 s
Fri Jun 28 11:23:17 2024 - n = 3, Fri Jun 28 11:23:17 2024 - process use time: 0.008000 s
Fri Jun 28 11:23:17 2024 - S[4][4][2] = 1
Fri Jun 28 11:23:17 2024 - n = 4, Fri Jun 28 11:23:17 2024 - process use time: 0.095000 s
Fri Jun 28 11:23:17 2024 - n = 5, Fri Jun 28 11:23:17 2024 - process use time: 0.007000 s
Fri Jun 28 11:23:17 2024 - n = 6, Fri Jun 28 11:23:17 2024 - process use time: 0.007000 s
Fri Jun 28 11:23:17 2024 - n = 7, Fri Jun 28 11:23:17 2024 - process use time: 0.006000 s
Fri Jun 28 11:23:17 2024 - n = 8, Fri Jun 28 11:23:17 2024 - process use time: 0.006000 s
Fri Jun 28 11:23:17 2024 - n = 9, Fri Jun 28 11:23:17 2024 - process use time: 0.007000 s
Fri Jun 28 11:23:17 2024 - n = 10, Fri Jun 28 11:23:17 2024 - process use time: 0.007000 s
Fri Jun 28 11:23:17 2024 - n = 11, Fri Jun 28 11:23:17 2024 - process use time: 0.007000 s
Fri Jun 28 11:23:17 2024 - n = 12, Fri Jun 28 11:23:17 2024 - process use time: 0.007000 s
Fri Jun 28 11:23:17 2024 - n = 13, Fri Jun 28 11:23:17 2024 - process use time: 0.007000 s
Fri Jun 28 11:23:17 2024 - n = 14, Fri Jun 28 11:23:17 2024 - process use time: 0.006000 s
Fri Jun 28 11:23:17 2024 - n = 15, Fri Jun 28 11:23:17 2024 - process use time: 0.006000 s
Fri Jun 28 11:23:17 2024 - n = 16, Fri Jun 28 11:23:17 2024 - process use time: 0.007000 s
Fri Jun 28 11:23:17 2024 - n = 17, Fri Jun 28 11:23:17 2024 - process use time: 0.007000 s
Fri Jun 28 11:23:17 2024 - n = 18, Fri Jun 28 11:23:17 2024 - process use time: 0.007000 s
Fri Jun 28 11:23:17 2024 - n = 19, Fri Jun 28 11:23:17 2024 - process use time: 0.005000 s
Fri Jun 28 11:23:17 2024 - n = 20, Fri Jun 28 11:23:17 2024 - process use time: 0.009000 s
Fri Jun 28 11:23:17 2024 - n = 21, Fri Jun 28 11:23:17 2024 - process use time: 0.007000 s
Fri Jun 28 11:23:17 2024 - n = 22, Fri Jun 28 11:23:17 2024 - process use time: 0.007000 s
Fri Jun 28 11:23:17 2024 - n = 23, Fri Jun 28 11:23:17 2024 - process use time: 0.005000 s
Fri Jun 28 11:23:17 2024 - n = 24, Fri Jun 28 11:23:17 2024 - process use time: 0.006000 s
Fri Jun 28 11:23:17 2024 - n = 25, Fri Jun 28 11:23:17 2024 - process use time: 0.009000 s
Fri Jun 28 11:23:17 2024 - n = 26, Fri Jun 28 11:23:17 2024 - process use time: 0.005000 s
Fri Jun 28 11:23:17 2024 - n = 27, Fri Jun 28 11:23:17 2024 - process use time: 0.006000 s
Fri Jun 28 11:23:17 2024 - n = 28, Fri Jun 28 11:23:17 2024 - process use time: 0.005000 s
Fri Jun 28 11:23:17 2024 - n = 29, Fri Jun 28 11:23:17 2024 - process use time: 0.006000 s
Fri Jun 28 11:23:17 2024 - n = 30, Fri Jun 28 11:23:17 2024 - process use time: 0.006000 s
Fri Jun 28 11:23:17 2024 - n = 31, Fri Jun 28 11:23:17 2024 - process use time: 0.006000 s
Fri Jun 28 11:23:18 2024 - n = 32, Fri Jun 28 11:23:18 2024 - process use time: 0.006000 s
Fri Jun 28 11:23:18 2024 - n = 33, Fri Jun 28 11:23:18 2024 - process use time: 0.005000 s
Fri Jun 28 11:23:18 2024 - n = 34, Fri Jun 28 11:23:18 2024 - process use time: 0.004000 s
Fri Jun 28 11:23:18 2024 - n = 35, Fri Jun 28 11:23:18 2024 - process use time: 0.007000 s
Fri Jun 28 11:23:18 2024 - n = 36, Fri Jun 28 11:23:18 2024 - process use time: 0.006000 s
Fri Jun 28 11:23:18 2024 - n = 37, Fri Jun 28 11:23:18 2024 - process use time: 0.005000 s
Fri Jun 28 11:23:18 2024 - n = 38, Fri Jun 28 11:23:18 2024 - process use time: 0.006000 s
Fri Jun 28 11:23:18 2024 - n = 39, Fri Jun 28 11:23:18 2024 - process use time: 0.005000 s
Fri Jun 28 11:23:18 2024 - n = 40, Fri Jun 28 11:23:18 2024 - process use time: 0.006000 s
Fri Jun 28 11:23:18 2024 - n = 41, Fri Jun 28 11:23:18 2024 - process use time: 0.006000 s
Fri Jun 28 11:23:18 2024 - n = 42, Fri Jun 28 11:23:18 2024 - process use time: 0.006000 s
Fri Jun 28 11:23:18 2024 - n = 43, Fri Jun 28 11:23:18 2024 - process use time: 0.005000 s
Fri Jun 28 11:23:18 2024 - n = 44, Fri Jun 28 11:23:18 2024 - process use time: 0.006000 s
Fri Jun 28 11:23:18 2024 - n = 45, Fri Jun 28 11:23:18 2024 - process use time: 0.005000 s
Fri Jun 28 11:23:18 2024 - n = 46, Fri Jun 28 11:23:18 2024 - process use time: 0.005000 s
Fri Jun 28 11:23:18 2024 - n = 47, Fri Jun 28 11:23:18 2024 - process use time: 0.006000 s
Fri Jun 28 11:23:18 2024 - n = 48, Fri Jun 28 11:23:18 2024 - process use time: 0.007000 s
Fri Jun 28 11:23:18 2024 - n = 49, Fri Jun 28 11:23:18 2024 - process use time: 0.005000 s
Fri Jun 28 11:23:18 2024 - n = 50, Fri Jun 28 11:23:18 2024 - process use time: 0.005000 s
Fri Jun 28 11:23:18 2024 - n = 51, Fri Jun 28 11:23:18 2024 - process use time: 0.006000 s
Fri Jun 28 11:23:18 2024 - n = 52, Fri Jun 28 11:23:18 2024 - process use time: 0.005000 s
Fri Jun 28 11:23:18 2024 - n = 53, Fri Jun 28 11:23:18 2024 - process use time: 0.005000 s
Fri Jun 28 11:23:18 2024 - n = 54, Fri Jun 28 11:23:18 2024 - process use time: 0.008000 s
Fri Jun 28 11:23:18 2024 - n = 55, Fri Jun 28 11:23:18 2024 - process use time: 0.005000 s
Fri Jun 28 11:23:18 2024 - n = 56, Fri Jun 28 11:23:18 2024 - process use time: 0.006000 s
Fri Jun 28 11:23:18 2024 - n = 57, Fri Jun 28 11:23:18 2024 - process use time: 0.006000 s
Fri Jun 28 11:23:18 2024 - n = 58, Fri Jun 28 11:23:18 2024 - process use time: 0.005000 s
Fri Jun 28 11:23:18 2024 - n = 59, Fri Jun 28 11:23:18 2024 - process use time: 0.009000 s
Fri Jun 28 11:23:18 2024 - n = 60, Fri Jun 28 11:23:18 2024 - process use time: 0.007000 s
Fri Jun 28 11:23:18 2024 - n = 61, Fri Jun 28 11:23:18 2024 - process use time: 0.009000 s
Fri Jun 28 11:23:18 2024 - n = 62, Fri Jun 28 11:23:18 2024 - process use time: 0.006000 s
Fri Jun 28 11:23:18 2024 - n = 63, Fri Jun 28 11:23:18 2024 - process use time: 0.008000 s
Fri Jun 28 11:23:18 2024 - n = 64, Fri Jun 28 11:23:18 2024 - process use time: 0.007000 s
Fri Jun 28 11:23:18 2024 - n = 65, Fri Jun 28 11:23:18 2024 - process use time: 0.007000 s
Fri Jun 28 11:23:18 2024 - n = 66, Fri Jun 28 11:23:18 2024 - process use time: 0.006000 s
Fri Jun 28 11:23:18 2024 - n = 67, Fri Jun 28 11:23:18 2024 - process use time: 0.005000 s
Fri Jun 28 11:23:18 2024 - n = 68, Fri Jun 28 11:23:18 2024 - process use time: 0.006000 s
Fri Jun 28 11:23:18 2024 - n = 69, Fri Jun 28 11:23:18 2024 - process use time: 0.008000 s
Fri Jun 28 11:23:18 2024 - n = 70, Fri Jun 28 11:23:18 2024 - process use time: 0.006000 s
Fri Jun 28 11:23:18 2024 - n = 71, Fri Jun 28 11:23:18 2024 - process use time: 0.006000 s
Fri Jun 28 11:23:18 2024 - n = 72, Fri Jun 28 11:23:18 2024 - process use time: 0.009000 s
Fri Jun 28 11:23:18 2024 - n = 73, Fri Jun 28 11:23:18 2024 - process use time: 0.011000 s
Fri Jun 28 11:23:18 2024 - n = 74, Fri Jun 28 11:23:18 2024 - process use time: 0.007000 s
Fri Jun 28 11:23:18 2024 - n = 75, Fri Jun 28 11:23:18 2024 - process use time: 0.008000 s
Fri Jun 28 11:23:18 2024 - n = 76, Fri Jun 28 11:23:18 2024 - process use time: 0.006000 s
Fri Jun 28 11:23:18 2024 - n = 77, Fri Jun 28 11:23:18 2024 - process use time: 0.008000 s
Fri Jun 28 11:23:18 2024 - n = 78, Fri Jun 28 11:23:18 2024 - process use time: 0.008000 s
Fri Jun 28 11:23:18 2024 - n = 79, Fri Jun 28 11:23:18 2024 - process use time: 0.007000 s
Fri Jun 28 11:23:18 2024 - n = 80, Fri Jun 28 11:23:18 2024 - process use time: 0.007000 s
Fri Jun 28 11:23:18 2024 - n = 81, Fri Jun 28 11:23:18 2024 - process use time: 0.007000 s
Fri Jun 28 11:23:18 2024 - n = 82, Fri Jun 28 11:23:18 2024 - process use time: 0.009000 s
Fri Jun 28 11:23:18 2024 - n = 83, Fri Jun 28 11:23:18 2024 - process use time: 0.007000 s
Fri Jun 28 11:23:18 2024 - n = 84, Fri Jun 28 11:23:18 2024 - process use time: 0.007000 s
Fri Jun 28 11:23:18 2024 - n = 85, Fri Jun 28 11:23:18 2024 - process use time: 0.008000 s
Fri Jun 28 11:23:18 2024 - n = 86, Fri Jun 28 11:23:18 2024 - process use time: 0.008000 s
Fri Jun 28 11:23:18 2024 - n = 87, Fri Jun 28 11:23:18 2024 - process use time: 0.009000 s
Fri Jun 28 11:23:18 2024 - n = 88, Fri Jun 28 11:23:18 2024 - process use time: 0.007000 s
Fri Jun 28 11:23:18 2024 - n = 89, Fri Jun 28 11:23:18 2024 - process use time: 0.008000 s
Fri Jun 28 11:23:18 2024 - n = 90, Fri Jun 28 11:23:18 2024 - process use time: 0.009000 s
Fri Jun 28 11:23:18 2024 - n = 91, Fri Jun 28 11:23:18 2024 - process use time: 0.050000 s
Fri Jun 28 11:23:18 2024 - n = 92, Fri Jun 28 11:23:18 2024 - process use time: 0.030000 s
Fri Jun 28 11:23:18 2024 - n = 93, Fri Jun 28 11:23:18 2024 - process use time: 0.008000 s
Fri Jun 28 11:23:18 2024 - n = 94, Fri Jun 28 11:23:18 2024 - process use time: 0.010000 s
Fri Jun 28 11:23:18 2024 - n = 95, Fri Jun 28 11:23:18 2024 - process use time: 0.008000 s
Fri Jun 28 11:23:18 2024 - n = 96, Fri Jun 28 11:23:18 2024 - process use time: 0.010000 s
Fri Jun 28 11:23:18 2024 - n = 97, Fri Jun 28 11:23:18 2024 - process use time: 0.010000 s
Fri Jun 28 11:23:18 2024 - n = 98, Fri Jun 28 11:23:18 2024 - process use time: 0.010000 s
Fri Jun 28 11:23:18 2024 - n = 99, Fri Jun 28 11:23:18 2024 - process use time: 0.011000 s
Fri Jun 28 11:23:18 2024 - n = 100, Fri Jun 28 11:23:18 2024 - process use time: 0.010000 s
Fri Jun 28 11:23:18 2024 - n = 101, Fri Jun 28 11:23:18 2024 - process use time: 0.009000 s
Fri Jun 28 11:23:18 2024 - n = 102, Fri Jun 28 11:23:18 2024 - process use time: 0.034000 s
Fri Jun 28 11:23:18 2024 - n = 103, Fri Jun 28 11:23:18 2024 - process use time: 0.012000 s
Fri Jun 28 11:23:18 2024 - n = 104, Fri Jun 28 11:23:18 2024 - process use time: 0.010000 s
Fri Jun 28 11:23:18 2024 - n = 105, Fri Jun 28 11:23:18 2024 - process use time: 0.009000 s
Fri Jun 28 11:23:18 2024 - n = 106, Fri Jun 28 11:23:18 2024 - process use time: 0.009000 s
Fri Jun 28 11:23:18 2024 - n = 107, Fri Jun 28 11:23:18 2024 - process use time: 0.011000 s
Fri Jun 28 11:23:18 2024 - n = 108, Fri Jun 28 11:23:18 2024 - process use time: 0.010000 s
Fri Jun 28 11:23:18 2024 - n = 109, Fri Jun 28 11:23:18 2024 - process use time: 0.009000 s
Fri Jun 28 11:23:18 2024 - n = 110, Fri Jun 28 11:23:18 2024 - process use time: 0.011000 s
Fri Jun 28 11:23:18 2024 - n = 111, Fri Jun 28 11:23:18 2024 - process use time: 0.011000 s
Fri Jun 28 11:23:18 2024 - n = 112, Fri Jun 28 11:23:18 2024 - process use time: 0.011000 s
Fri Jun 28 11:23:18 2024 - n = 113, Fri Jun 28 11:23:18 2024 - process use time: 0.010000 s
Fri Jun 28 11:23:18 2024 - n = 114, Fri Jun 28 11:23:18 2024 - process use time: 0.012000 s
Fri Jun 28 11:23:18 2024 - n = 115, Fri Jun 28 11:23:18 2024 - process use time: 0.011000 s
Fri Jun 28 11:23:18 2024 - n = 116, Fri Jun 28 11:23:18 2024 - process use time: 0.010000 s
Fri Jun 28 11:23:18 2024 - n = 117, Fri Jun 28 11:23:18 2024 - process use time: 0.012000 s
Fri Jun 28 11:23:18 2024 - n = 118, Fri Jun 28 11:23:18 2024 - process use time: 0.011000 s
Fri Jun 28 11:23:18 2024 - n = 119, Fri Jun 28 11:23:18 2024 - process use time: 0.013000 s
Fri Jun 28 11:23:18 2024 - n = 120, Fri Jun 28 11:23:18 2024 - process use time: 0.012000 s
Fri Jun 28 11:23:18 2024 - n = 121, Fri Jun 28 11:23:18 2024 - process use time: 0.011000 s
Fri Jun 28 11:23:18 2024 - n = 122, Fri Jun 28 11:23:18 2024 - process use time: 0.012000 s
Fri Jun 28 11:23:18 2024 - n = 123, Fri Jun 28 11:23:18 2024 - process use time: 0.013000 s
Fri Jun 28 11:23:18 2024 - n = 124, Fri Jun 28 11:23:18 2024 - process use time: 0.011000 s
Fri Jun 28 11:23:18 2024 - n = 125, Fri Jun 28 11:23:18 2024 - process use time: 0.013000 s
Fri Jun 28 11:23:18 2024 - n = 126, Fri Jun 28 11:23:18 2024 - process use time: 0.012000 s
Fri Jun 28 11:23:18 2024 - n = 127, Fri Jun 28 11:23:18 2024 - process use time: 0.012000 s
Fri Jun 28 11:23:18 2024 - n = 128, Fri Jun 28 11:23:18 2024 - process use time: 0.012000 s
Fri Jun 28 11:23:18 2024 - n = 129, Fri Jun 28 11:23:18 2024 - process use time: 0.014000 s
Fri Jun 28 11:23:18 2024 - n = 130, Fri Jun 28 11:23:18 2024 - process use time: 0.014000 s
Fri Jun 28 11:23:18 2024 - n = 131, Fri Jun 28 11:23:18 2024 - process use time: 0.014000 s
Fri Jun 28 11:23:18 2024 - n = 132, Fri Jun 28 11:23:18 2024 - process use time: 0.013000 s
Fri Jun 28 11:23:18 2024 - n = 133, Fri Jun 28 11:23:18 2024 - process use time: 0.015000 s
Fri Jun 28 11:23:18 2024 - n = 134, Fri Jun 28 11:23:18 2024 - process use time: 0.015000 s
Fri Jun 28 11:23:18 2024 - n = 135, Fri Jun 28 11:23:18 2024 - process use time: 0.018000 s
Fri Jun 28 11:23:18 2024 - n = 136, Fri Jun 28 11:23:19 2024 - process use time: 0.035000 s
Fri Jun 28 11:23:19 2024 - n = 137, Fri Jun 28 11:23:19 2024 - process use time: 0.019000 s
Fri Jun 28 11:23:19 2024 - n = 138, Fri Jun 28 11:23:19 2024 - process use time: 0.017000 s
Fri Jun 28 11:23:19 2024 - n = 139, Fri Jun 28 11:23:19 2024 - process use time: 0.015000 s
Fri Jun 28 11:23:19 2024 - n = 140, Fri Jun 28 11:23:19 2024 - process use time: 0.017000 s
Fri Jun 28 11:23:19 2024 - n = 141, Fri Jun 28 11:23:19 2024 - process use time: 0.016000 s
Fri Jun 28 11:23:19 2024 - n = 142, Fri Jun 28 11:23:19 2024 - process use time: 0.020000 s
Fri Jun 28 11:23:19 2024 - n = 143, Fri Jun 28 11:23:19 2024 - process use time: 0.015000 s
Fri Jun 28 11:23:19 2024 - n = 144, Fri Jun 28 11:23:19 2024 - process use time: 0.018000 s
Fri Jun 28 11:23:19 2024 - n = 145, Fri Jun 28 11:23:19 2024 - process use time: 0.017000 s
Fri Jun 28 11:23:19 2024 - n = 146, Fri Jun 28 11:23:19 2024 - process use time: 0.017000 s
Fri Jun 28 11:23:19 2024 - n = 147, Fri Jun 28 11:23:19 2024 - process use time: 0.015000 s
Fri Jun 28 11:23:19 2024 - n = 148, Fri Jun 28 11:23:19 2024 - process use time: 0.018000 s
Fri Jun 28 11:23:19 2024 - n = 149, Fri Jun 28 11:23:19 2024 - process use time: 0.015000 s
Fri Jun 28 11:23:19 2024 - n = 150, Fri Jun 28 11:23:19 2024 - process use time: 0.018000 s
Fri Jun 28 11:23:19 2024 - n = 151, Fri Jun 28 11:23:19 2024 - process use time: 0.016000 s
Fri Jun 28 11:23:19 2024 - n = 152, Fri Jun 28 11:23:19 2024 - process use time: 0.017000 s
Fri Jun 28 11:23:19 2024 - n = 153, Fri Jun 28 11:23:19 2024 - process use time: 0.017000 s
Fri Jun 28 11:23:19 2024 - n = 154, Fri Jun 28 11:23:19 2024 - process use time: 0.018000 s
Fri Jun 28 11:23:19 2024 - n = 155, Fri Jun 28 11:23:19 2024 - process use time: 0.018000 s
Fri Jun 28 11:23:19 2024 - n = 156, Fri Jun 28 11:23:19 2024 - process use time: 0.017000 s
Fri Jun 28 11:23:19 2024 - n = 157, Fri Jun 28 11:23:19 2024 - process use time: 0.018000 s
Fri Jun 28 11:23:19 2024 - n = 158, Fri Jun 28 11:23:19 2024 - process use time: 0.018000 s
Fri Jun 28 11:23:19 2024 - n = 159, Fri Jun 28 11:23:19 2024 - process use time: 0.018000 s
Fri Jun 28 11:23:19 2024 - n = 160, Fri Jun 28 11:23:19 2024 - process use time: 0.018000 s
Fri Jun 28 11:23:19 2024 - n = 161, Fri Jun 28 11:23:19 2024 - process use time: 0.020000 s
Fri Jun 28 11:23:19 2024 - n = 162, Fri Jun 28 11:23:19 2024 - process use time: 0.019000 s
Fri Jun 28 11:23:19 2024 - n = 163, Fri Jun 28 11:23:19 2024 - process use time: 0.020000 s
Fri Jun 28 11:23:19 2024 - n = 164, Fri Jun 28 11:23:19 2024 - process use time: 0.019000 s
Fri Jun 28 11:23:19 2024 - n = 165, Fri Jun 28 11:23:19 2024 - process use time: 0.018000 s
Fri Jun 28 11:23:19 2024 - n = 166, Fri Jun 28 11:23:19 2024 - process use time: 0.020000 s
Fri Jun 28 11:23:19 2024 - n = 167, Fri Jun 28 11:23:19 2024 - process use time: 0.019000 s
Fri Jun 28 11:23:19 2024 - n = 168, Fri Jun 28 11:23:19 2024 - process use time: 0.021000 s
Fri Jun 28 11:23:19 2024 - n = 169, Fri Jun 28 11:23:19 2024 - process use time: 0.021000 s
Fri Jun 28 11:23:19 2024 - n = 170, Fri Jun 28 11:23:19 2024 - process use time: 0.021000 s
Fri Jun 28 11:23:19 2024 - n = 171, Fri Jun 28 11:23:19 2024 - process use time: 0.020000 s
Fri Jun 28 11:23:19 2024 - n = 172, Fri Jun 28 11:23:19 2024 - process use time: 0.022000 s
Fri Jun 28 11:23:19 2024 - n = 173, Fri Jun 28 11:23:19 2024 - process use time: 0.021000 s
Fri Jun 28 11:23:19 2024 - n = 174, Fri Jun 28 11:23:19 2024 - process use time: 0.021000 s
Fri Jun 28 11:23:19 2024 - n = 175, Fri Jun 28 11:23:19 2024 - process use time: 0.021000 s
Fri Jun 28 11:23:19 2024 - n = 176, Fri Jun 28 11:23:19 2024 - process use time: 0.022000 s
Fri Jun 28 11:23:19 2024 - n = 177, Fri Jun 28 11:23:19 2024 - process use time: 0.023000 s
Fri Jun 28 11:23:19 2024 - n = 178, Fri Jun 28 11:23:19 2024 - process use time: 0.021000 s
Fri Jun 28 11:23:19 2024 - n = 179, Fri Jun 28 11:23:19 2024 - process use time: 0.024000 s
Fri Jun 28 11:23:19 2024 - n = 180, Fri Jun 28 11:23:19 2024 - process use time: 0.022000 s
Fri Jun 28 11:23:19 2024 - n = 181, Fri Jun 28 11:23:19 2024 - process use time: 0.021000 s
Fri Jun 28 11:23:19 2024 - n = 182, Fri Jun 28 11:23:19 2024 - process use time: 0.026000 s
Fri Jun 28 11:23:19 2024 - n = 183, Fri Jun 28 11:23:19 2024 - process use time: 0.023000 s
Fri Jun 28 11:23:19 2024 - n = 184, Fri Jun 28 11:23:19 2024 - process use time: 0.023000 s
Fri Jun 28 11:23:19 2024 - n = 185, Fri Jun 28 11:23:19 2024 - process use time: 0.024000 s
Fri Jun 28 11:23:19 2024 - n = 186, Fri Jun 28 11:23:19 2024 - process use time: 0.024000 s
Fri Jun 28 11:23:20 2024 - n = 187, Fri Jun 28 11:23:20 2024 - process use time: 0.024000 s
Fri Jun 28 11:23:20 2024 - n = 188, Fri Jun 28 11:23:20 2024 - process use time: 0.024000 s
Fri Jun 28 11:23:20 2024 - n = 189, Fri Jun 28 11:23:20 2024 - process use time: 0.025000 s
Fri Jun 28 11:23:20 2024 - n = 190, Fri Jun 28 11:23:20 2024 - process use time: 0.024000 s
Fri Jun 28 11:23:20 2024 - n = 191, Fri Jun 28 11:23:20 2024 - process use time: 0.025000 s
Fri Jun 28 11:23:20 2024 - n = 192, Fri Jun 28 11:23:20 2024 - process use time: 0.026000 s
Fri Jun 28 11:23:20 2024 - n = 193, Fri Jun 28 11:23:20 2024 - process use time: 0.028000 s
Fri Jun 28 11:23:20 2024 - n = 194, Fri Jun 28 11:23:20 2024 - process use time: 0.027000 s
Fri Jun 28 11:23:20 2024 - n = 195, Fri Jun 28 11:23:20 2024 - process use time: 0.029000 s
Fri Jun 28 11:23:20 2024 - n = 196, Fri Jun 28 11:23:20 2024 - process use time: 0.027000 s
Fri Jun 28 11:23:20 2024 - n = 197, Fri Jun 28 11:23:20 2024 - process use time: 0.029000 s
Fri Jun 28 11:23:20 2024 - n = 198, Fri Jun 28 11:23:20 2024 - process use time: 0.028000 s
Fri Jun 28 11:23:20 2024 - n = 199, Fri Jun 28 11:23:20 2024 - process use time: 0.029000 s
Fri Jun 28 11:23:20 2024 - n = 200, Fri Jun 28 11:23:20 2024 - process use time: 0.030000 s
Fri Jun 28 11:23:20 2024 - n = 201, Fri Jun 28 11:23:20 2024 - process use time: 0.030000 s
Fri Jun 28 11:23:20 2024 - n = 202, Fri Jun 28 11:23:20 2024 - process use time: 0.030000 s
Fri Jun 28 11:23:20 2024 - n = 203, Fri Jun 28 11:23:20 2024 - process use time: 0.033000 s
Fri Jun 28 11:23:20 2024 - n = 204, Fri Jun 28 11:23:20 2024 - process use time: 0.030000 s
Fri Jun 28 11:23:20 2024 - n = 205, Fri Jun 28 11:23:20 2024 - process use time: 0.029000 s
Fri Jun 28 11:23:20 2024 - n = 206, Fri Jun 28 11:23:20 2024 - process use time: 0.031000 s
Fri Jun 28 11:23:20 2024 - n = 207, Fri Jun 28 11:23:20 2024 - process use time: 0.031000 s
Fri Jun 28 11:23:20 2024 - n = 208, Fri Jun 28 11:23:20 2024 - process use time: 0.032000 s
Fri Jun 28 11:23:20 2024 - n = 209, Fri Jun 28 11:23:20 2024 - process use time: 0.032000 s
Fri Jun 28 11:23:20 2024 - n = 210, Fri Jun 28 11:23:20 2024 - process use time: 0.033000 s
Fri Jun 28 11:23:20 2024 - n = 211, Fri Jun 28 11:23:20 2024 - process use time: 0.031000 s
Fri Jun 28 11:23:20 2024 - n = 212, Fri Jun 28 11:23:20 2024 - process use time: 0.033000 s
Fri Jun 28 11:23:20 2024 - n = 213, Fri Jun 28 11:23:20 2024 - process use time: 0.034000 s
Fri Jun 28 11:23:20 2024 - n = 214, Fri Jun 28 11:23:20 2024 - process use time: 0.033000 s
Fri Jun 28 11:23:20 2024 - n = 215, Fri Jun 28 11:23:20 2024 - process use time: 0.037000 s
Fri Jun 28 11:23:20 2024 - n = 216, Fri Jun 28 11:23:20 2024 - process use time: 0.033000 s
Fri Jun 28 11:23:20 2024 - n = 217, Fri Jun 28 11:23:20 2024 - process use time: 0.036000 s
Fri Jun 28 11:23:20 2024 - n = 218, Fri Jun 28 11:23:20 2024 - process use time: 0.034000 s
Fri Jun 28 11:23:20 2024 - n = 219, Fri Jun 28 11:23:20 2024 - process use time: 0.034000 s
Fri Jun 28 11:23:21 2024 - n = 220, Fri Jun 28 11:23:21 2024 - process use time: 0.035000 s
Fri Jun 28 11:23:21 2024 - n = 221, Fri Jun 28 11:23:21 2024 - process use time: 0.033000 s
Fri Jun 28 11:23:21 2024 - n = 222, Fri Jun 28 11:23:21 2024 - process use time: 0.034000 s
Fri Jun 28 11:23:21 2024 - n = 223, Fri Jun 28 11:23:21 2024 - process use time: 0.035000 s
Fri Jun 28 11:23:21 2024 - n = 224, Fri Jun 28 11:23:21 2024 - process use time: 0.034000 s
Fri Jun 28 11:23:21 2024 - n = 225, Fri Jun 28 11:23:21 2024 - process use time: 0.036000 s
Fri Jun 28 11:23:21 2024 - n = 226, Fri Jun 28 11:23:21 2024 - process use time: 0.036000 s
Fri Jun 28 11:23:21 2024 - n = 227, Fri Jun 28 11:23:21 2024 - process use time: 0.035000 s
Fri Jun 28 11:23:21 2024 - n = 228, Fri Jun 28 11:23:21 2024 - process use time: 0.037000 s
Fri Jun 28 11:23:21 2024 - n = 229, Fri Jun 28 11:23:21 2024 - process use time: 0.037000 s
Fri Jun 28 11:23:21 2024 - n = 230, Fri Jun 28 11:23:21 2024 - process use time: 0.036000 s
Fri Jun 28 11:23:21 2024 - n = 231, Fri Jun 28 11:23:21 2024 - process use time: 0.039000 s
Fri Jun 28 11:23:21 2024 - n = 232, Fri Jun 28 11:23:21 2024 - process use time: 0.041000 s
Fri Jun 28 11:23:21 2024 - n = 233, Fri Jun 28 11:23:21 2024 - process use time: 0.039000 s
Fri Jun 28 11:23:21 2024 - n = 234, Fri Jun 28 11:23:21 2024 - process use time: 0.036000 s
Fri Jun 28 11:23:21 2024 - n = 235, Fri Jun 28 11:23:21 2024 - process use time: 0.037000 s
Fri Jun 28 11:23:21 2024 - n = 236, Fri Jun 28 11:23:21 2024 - process use time: 0.038000 s
Fri Jun 28 11:23:21 2024 - n = 237, Fri Jun 28 11:23:21 2024 - process use time: 0.040000 s
Fri Jun 28 11:23:21 2024 - n = 238, Fri Jun 28 11:23:21 2024 - process use time: 0.039000 s
Fri Jun 28 11:23:21 2024 - n = 239, Fri Jun 28 11:23:21 2024 - process use time: 0.039000 s
Fri Jun 28 11:23:21 2024 - n = 240, Fri Jun 28 11:23:21 2024 - process use time: 0.042000 s
Fri Jun 28 11:23:21 2024 - n = 241, Fri Jun 28 11:23:21 2024 - process use time: 0.039000 s
Fri Jun 28 11:23:21 2024 - n = 242, Fri Jun 28 11:23:21 2024 - process use time: 0.042000 s
Fri Jun 28 11:23:21 2024 - n = 243, Fri Jun 28 11:23:21 2024 - process use time: 0.041000 s
Fri Jun 28 11:23:21 2024 - n = 244, Fri Jun 28 11:23:21 2024 - process use time: 0.045000 s
Fri Jun 28 11:23:21 2024 - n = 245, Fri Jun 28 11:23:21 2024 - process use time: 0.040000 s
Fri Jun 28 11:23:22 2024 - n = 246, Fri Jun 28 11:23:22 2024 - process use time: 0.044000 s
Fri Jun 28 11:23:22 2024 - n = 247, Fri Jun 28 11:23:22 2024 - process use time: 0.042000 s
Fri Jun 28 11:23:22 2024 - n = 248, Fri Jun 28 11:23:22 2024 - process use time: 0.041000 s
Fri Jun 28 11:23:22 2024 - n = 249, Fri Jun 28 11:23:22 2024 - process use time: 0.043000 s
Fri Jun 28 11:23:22 2024 - n = 250, Fri Jun 28 11:23:22 2024 - process use time: 0.044000 s
Fri Jun 28 11:23:22 2024 - n = 251, Fri Jun 28 11:23:22 2024 - process use time: 0.044000 s
Fri Jun 28 11:23:22 2024 - n = 252, Fri Jun 28 11:23:22 2024 - process use time: 0.044000 s
Fri Jun 28 11:23:22 2024 - n = 253, Fri Jun 28 11:23:22 2024 - process use time: 0.045000 s
Fri Jun 28 11:23:22 2024 - n = 254, Fri Jun 28 11:23:22 2024 - process use time: 0.045000 s
Fri Jun 28 11:23:22 2024 - n = 255, Fri Jun 28 11:23:22 2024 - process use time: 0.044000 s
Fri Jun 28 11:23:22 2024 - n = 256, Fri Jun 28 11:23:22 2024 - process use time: 0.047000 s
Fri Jun 28 11:23:22 2024 - n = 257, Fri Jun 28 11:23:22 2024 - process use time: 0.047000 s
Fri Jun 28 11:23:22 2024 - n = 258, Fri Jun 28 11:23:22 2024 - process use time: 0.049000 s
Fri Jun 28 11:23:22 2024 - n = 259, Fri Jun 28 11:23:22 2024 - process use time: 0.046000 s
Fri Jun 28 11:23:22 2024 - n = 260, Fri Jun 28 11:23:22 2024 - process use time: 0.047000 s
Fri Jun 28 11:23:22 2024 - n = 261, Fri Jun 28 11:23:22 2024 - process use time: 0.049000 s
Fri Jun 28 11:23:22 2024 - n = 262, Fri Jun 28 11:23:22 2024 - process use time: 0.046000 s
Fri Jun 28 11:23:22 2024 - n = 263, Fri Jun 28 11:23:22 2024 - process use time: 0.048000 s
Fri Jun 28 11:23:22 2024 - n = 264, Fri Jun 28 11:23:22 2024 - process use time: 0.048000 s
Fri Jun 28 11:23:22 2024 - n = 265, Fri Jun 28 11:23:22 2024 - process use time: 0.048000 s
Fri Jun 28 11:23:22 2024 - n = 266, Fri Jun 28 11:23:22 2024 - process use time: 0.046000 s
Fri Jun 28 11:23:22 2024 - n = 267, Fri Jun 28 11:23:22 2024 - process use time: 0.050000 s
Fri Jun 28 11:23:23 2024 - n = 268, Fri Jun 28 11:23:23 2024 - process use time: 0.049000 s
Fri Jun 28 11:23:23 2024 - n = 269, Fri Jun 28 11:23:23 2024 - process use time: 0.050000 s
Fri Jun 28 11:23:23 2024 - n = 270, Fri Jun 28 11:23:23 2024 - process use time: 0.050000 s
Fri Jun 28 11:23:23 2024 - n = 271, Fri Jun 28 11:23:23 2024 - process use time: 0.052000 s
Fri Jun 28 11:23:23 2024 - n = 272, Fri Jun 28 11:23:23 2024 - process use time: 0.051000 s
Fri Jun 28 11:23:23 2024 - n = 273, Fri Jun 28 11:23:23 2024 - process use time: 0.060000 s
Fri Jun 28 11:23:23 2024 - n = 274, Fri Jun 28 11:23:23 2024 - process use time: 0.052000 s
Fri Jun 28 11:23:23 2024 - n = 275, Fri Jun 28 11:23:23 2024 - process use time: 0.052000 s
Fri Jun 28 11:23:23 2024 - n = 276, Fri Jun 28 11:23:23 2024 - process use time: 0.055000 s
Fri Jun 28 11:23:23 2024 - n = 277, Fri Jun 28 11:23:23 2024 - process use time: 0.117000 s
Fri Jun 28 11:23:23 2024 - n = 278, Fri Jun 28 11:23:23 2024 - process use time: 0.060000 s
Fri Jun 28 11:23:23 2024 - n = 279, Fri Jun 28 11:23:23 2024 - process use time: 0.058000 s
Fri Jun 28 11:23:23 2024 - n = 280, Fri Jun 28 11:23:23 2024 - process use time: 0.056000 s
Fri Jun 28 11:23:23 2024 - n = 281, Fri Jun 28 11:23:23 2024 - process use time: 0.058000 s
Fri Jun 28 11:23:23 2024 - n = 282, Fri Jun 28 11:23:23 2024 - process use time: 0.057000 s
Fri Jun 28 11:23:23 2024 - n = 283, Fri Jun 28 11:23:23 2024 - process use time: 0.061000 s
Fri Jun 28 11:23:23 2024 - n = 284, Fri Jun 28 11:23:23 2024 - process use time: 0.057000 s
Fri Jun 28 11:23:24 2024 - n = 285, Fri Jun 28 11:23:24 2024 - process use time: 0.061000 s
Fri Jun 28 11:23:24 2024 - n = 286, Fri Jun 28 11:23:24 2024 - process use time: 0.061000 s
Fri Jun 28 11:23:24 2024 - n = 287, Fri Jun 28 11:23:24 2024 - process use time: 0.058000 s
Fri Jun 28 11:23:24 2024 - n = 288, Fri Jun 28 11:23:24 2024 - process use time: 0.061000 s
Fri Jun 28 11:23:24 2024 - n = 289, Fri Jun 28 11:23:24 2024 - process use time: 0.061000 s
Fri Jun 28 11:23:24 2024 - n = 290, Fri Jun 28 11:23:24 2024 - process use time: 0.061000 s
Fri Jun 28 11:23:24 2024 - n = 291, Fri Jun 28 11:23:24 2024 - process use time: 0.062000 s
Fri Jun 28 11:23:24 2024 - n = 292, Fri Jun 28 11:23:24 2024 - process use time: 0.065000 s
Fri Jun 28 11:23:24 2024 - n = 293, Fri Jun 28 11:23:24 2024 - process use time: 0.064000 s
Fri Jun 28 11:23:24 2024 - n = 294, Fri Jun 28 11:23:24 2024 - process use time: 0.063000 s
Fri Jun 28 11:23:24 2024 - n = 295, Fri Jun 28 11:23:24 2024 - process use time: 0.064000 s
Fri Jun 28 11:23:24 2024 - n = 296, Fri Jun 28 11:23:24 2024 - process use time: 0.066000 s
Fri Jun 28 11:23:24 2024 - n = 297, Fri Jun 28 11:23:24 2024 - process use time: 0.066000 s
Fri Jun 28 11:23:24 2024 - n = 298, Fri Jun 28 11:23:24 2024 - process use time: 0.066000 s
Fri Jun 28 11:23:24 2024 - n = 299, Fri Jun 28 11:23:24 2024 - process use time: 0.067000 s
Fri Jun 28 11:23:24 2024 - n = 300, Fri Jun 28 11:23:24 2024 - process use time: 0.069000 s
Fri Jun 28 11:23:25 2024 - n = 301, Fri Jun 28 11:23:25 2024 - process use time: 0.067000 s
Fri Jun 28 11:23:25 2024 - n = 302, Fri Jun 28 11:23:25 2024 - process use time: 0.082000 s
Fri Jun 28 11:23:25 2024 - n = 303, Fri Jun 28 11:23:25 2024 - process use time: 0.068000 s
Fri Jun 28 11:23:25 2024 - n = 304, Fri Jun 28 11:23:25 2024 - process use time: 0.068000 s
Fri Jun 28 11:23:25 2024 - n = 305, Fri Jun 28 11:23:25 2024 - process use time: 0.072000 s
Fri Jun 28 11:23:25 2024 - n = 306, Fri Jun 28 11:23:25 2024 - process use time: 0.074000 s
Fri Jun 28 11:23:25 2024 - n = 307, Fri Jun 28 11:23:25 2024 - process use time: 0.074000 s
Fri Jun 28 11:23:25 2024 - n = 308, Fri Jun 28 11:23:25 2024 - process use time: 0.073000 s
Fri Jun 28 11:23:25 2024 - n = 309, Fri Jun 28 11:23:25 2024 - process use time: 0.068000 s
Fri Jun 28 11:23:25 2024 - n = 310, Fri Jun 28 11:23:25 2024 - process use time: 0.073000 s
Fri Jun 28 11:23:25 2024 - n = 311, Fri Jun 28 11:23:25 2024 - process use time: 0.072000 s
Fri Jun 28 11:23:25 2024 - n = 312, Fri Jun 28 11:23:25 2024 - process use time: 0.071000 s
Fri Jun 28 11:23:25 2024 - n = 313, Fri Jun 28 11:23:25 2024 - process use time: 0.074000 s
Fri Jun 28 11:23:25 2024 - n = 314, Fri Jun 28 11:23:25 2024 - process use time: 0.073000 s
Fri Jun 28 11:23:26 2024 - n = 315, Fri Jun 28 11:23:26 2024 - process use time: 0.071000 s
Fri Jun 28 11:23:26 2024 - n = 316, Fri Jun 28 11:23:26 2024 - process use time: 0.076000 s
Fri Jun 28 11:23:26 2024 - n = 317, Fri Jun 28 11:23:26 2024 - process use time: 0.073000 s
Fri Jun 28 11:23:26 2024 - n = 318, Fri Jun 28 11:23:26 2024 - process use time: 0.073000 s
Fri Jun 28 11:23:26 2024 - n = 319, Fri Jun 28 11:23:26 2024 - process use time: 0.076000 s
Fri Jun 28 11:23:26 2024 - n = 320, Fri Jun 28 11:23:26 2024 - process use time: 0.081000 s
Fri Jun 28 11:23:26 2024 - n = 321, Fri Jun 28 11:23:26 2024 - process use time: 0.082000 s
Fri Jun 28 11:23:26 2024 - n = 322, Fri Jun 28 11:23:26 2024 - process use time: 0.074000 s
Fri Jun 28 11:23:26 2024 - n = 323, Fri Jun 28 11:23:26 2024 - process use time: 0.076000 s
Fri Jun 28 11:23:26 2024 - n = 324, Fri Jun 28 11:23:26 2024 - process use time: 0.077000 s
Fri Jun 28 11:23:26 2024 - n = 325, Fri Jun 28 11:23:26 2024 - process use time: 0.078000 s
Fri Jun 28 11:23:26 2024 - n = 326, Fri Jun 28 11:23:26 2024 - process use time: 0.080000 s
Fri Jun 28 11:23:26 2024 - n = 327, Fri Jun 28 11:23:26 2024 - process use time: 0.083000 s
Fri Jun 28 11:23:27 2024 - n = 328, Fri Jun 28 11:23:27 2024 - process use time: 0.078000 s
Fri Jun 28 11:23:27 2024 - n = 329, Fri Jun 28 11:23:27 2024 - process use time: 0.079000 s
Fri Jun 28 11:23:27 2024 - n = 330, Fri Jun 28 11:23:27 2024 - process use time: 0.077000 s
Fri Jun 28 11:23:27 2024 - n = 331, Fri Jun 28 11:23:27 2024 - process use time: 0.084000 s
Fri Jun 28 11:23:27 2024 - n = 332, Fri Jun 28 11:23:27 2024 - process use time: 0.079000 s
Fri Jun 28 11:23:27 2024 - n = 333, Fri Jun 28 11:23:27 2024 - process use time: 0.083000 s
Fri Jun 28 11:23:27 2024 - n = 334, Fri Jun 28 11:23:27 2024 - process use time: 0.088000 s
Fri Jun 28 11:23:27 2024 - n = 335, Fri Jun 28 11:23:27 2024 - process use time: 0.083000 s
Fri Jun 28 11:23:27 2024 - n = 336, Fri Jun 28 11:23:27 2024 - process use time: 0.082000 s
Fri Jun 28 11:23:27 2024 - n = 337, Fri Jun 28 11:23:27 2024 - process use time: 0.085000 s
Fri Jun 28 11:23:27 2024 - n = 338, Fri Jun 28 11:23:27 2024 - process use time: 0.081000 s
Fri Jun 28 11:23:27 2024 - n = 339, Fri Jun 28 11:23:27 2024 - process use time: 0.089000 s
Fri Jun 28 11:23:28 2024 - n = 340, Fri Jun 28 11:23:28 2024 - process use time: 0.082000 s
Fri Jun 28 11:23:28 2024 - n = 341, Fri Jun 28 11:23:28 2024 - process use time: 0.087000 s
Fri Jun 28 11:23:28 2024 - n = 342, Fri Jun 28 11:23:28 2024 - process use time: 0.084000 s
Fri Jun 28 11:23:28 2024 - n = 343, Fri Jun 28 11:23:28 2024 - process use time: 0.085000 s
Fri Jun 28 11:23:28 2024 - n = 344, Fri Jun 28 11:23:28 2024 - process use time: 0.086000 s
Fri Jun 28 11:23:28 2024 - n = 345, Fri Jun 28 11:23:28 2024 - process use time: 0.090000 s
Fri Jun 28 11:23:28 2024 - n = 346, Fri Jun 28 11:23:28 2024 - process use time: 0.090000 s
Fri Jun 28 11:23:28 2024 - n = 347, Fri Jun 28 11:23:28 2024 - process use time: 0.086000 s
Fri Jun 28 11:23:28 2024 - n = 348, Fri Jun 28 11:23:28 2024 - process use time: 0.088000 s
Fri Jun 28 11:23:28 2024 - n = 349, Fri Jun 28 11:23:28 2024 - process use time: 0.091000 s
Fri Jun 28 11:23:28 2024 - n = 350, Fri Jun 28 11:23:28 2024 - process use time: 0.089000 s
Fri Jun 28 11:23:29 2024 - n = 351, Fri Jun 28 11:23:29 2024 - process use time: 0.089000 s
Fri Jun 28 11:23:29 2024 - n = 352, Fri Jun 28 11:23:29 2024 - process use time: 0.094000 s
Fri Jun 28 11:23:29 2024 - n = 353, Fri Jun 28 11:23:29 2024 - process use time: 0.091000 s
Fri Jun 28 11:23:29 2024 - n = 354, Fri Jun 28 11:23:29 2024 - process use time: 0.088000 s
Fri Jun 28 11:23:29 2024 - n = 355, Fri Jun 28 11:23:29 2024 - process use time: 0.091000 s
Fri Jun 28 11:23:29 2024 - n = 356, Fri Jun 28 11:23:29 2024 - process use time: 0.095000 s
Fri Jun 28 11:23:29 2024 - n = 357, Fri Jun 28 11:23:29 2024 - process use time: 0.093000 s
Fri Jun 28 11:23:29 2024 - n = 358, Fri Jun 28 11:23:29 2024 - process use time: 0.092000 s
Fri Jun 28 11:23:29 2024 - n = 359, Fri Jun 28 11:23:29 2024 - process use time: 0.098000 s
Fri Jun 28 11:23:29 2024 - n = 360, Fri Jun 28 11:23:29 2024 - process use time: 0.097000 s
Fri Jun 28 11:23:29 2024 - n = 361, Fri Jun 28 11:23:29 2024 - process use time: 0.095000 s
Fri Jun 28 11:23:30 2024 - n = 362, Fri Jun 28 11:23:30 2024 - process use time: 0.098000 s
Fri Jun 28 11:23:30 2024 - n = 363, Fri Jun 28 11:23:30 2024 - process use time: 0.101000 s
Fri Jun 28 11:23:30 2024 - n = 364, Fri Jun 28 11:23:30 2024 - process use time: 0.096000 s
Fri Jun 28 11:23:30 2024 - n = 365, Fri Jun 28 11:23:30 2024 - process use time: 0.098000 s
Fri Jun 28 11:23:30 2024 - n = 366, Fri Jun 28 11:23:30 2024 - process use time: 0.099000 s
Fri Jun 28 11:23:30 2024 - n = 367, Fri Jun 28 11:23:30 2024 - process use time: 0.102000 s
Fri Jun 28 11:23:30 2024 - n = 368, Fri Jun 28 11:23:30 2024 - process use time: 0.102000 s
Fri Jun 28 11:23:30 2024 - n = 369, Fri Jun 28 11:23:30 2024 - process use time: 0.099000 s
Fri Jun 28 11:23:30 2024 - n = 370, Fri Jun 28 11:23:30 2024 - process use time: 0.103000 s
Fri Jun 28 11:23:30 2024 - n = 371, Fri Jun 28 11:23:30 2024 - process use time: 0.100000 s
Fri Jun 28 11:23:31 2024 - n = 372, Fri Jun 28 11:23:31 2024 - process use time: 0.105000 s
Fri Jun 28 11:23:31 2024 - n = 373, Fri Jun 28 11:23:31 2024 - process use time: 0.103000 s
Fri Jun 28 11:23:31 2024 - n = 374, Fri Jun 28 11:23:31 2024 - process use time: 0.104000 s
Fri Jun 28 11:23:31 2024 - n = 375, Fri Jun 28 11:23:31 2024 - process use time: 0.105000 s
Fri Jun 28 11:23:31 2024 - n = 376, Fri Jun 28 11:23:31 2024 - process use time: 0.105000 s
Fri Jun 28 11:23:31 2024 - n = 377, Fri Jun 28 11:23:31 2024 - process use time: 0.109000 s
Fri Jun 28 11:23:31 2024 - n = 378, Fri Jun 28 11:23:31 2024 - process use time: 0.103000 s
Fri Jun 28 11:23:31 2024 - n = 379, Fri Jun 28 11:23:31 2024 - process use time: 0.109000 s
Fri Jun 28 11:23:31 2024 - n = 380, Fri Jun 28 11:23:31 2024 - process use time: 0.104000 s
Fri Jun 28 11:23:32 2024 - n = 381, Fri Jun 28 11:23:32 2024 - process use time: 0.110000 s
Fri Jun 28 11:23:32 2024 - n = 382, Fri Jun 28 11:23:32 2024 - process use time: 0.107000 s
Fri Jun 28 11:23:32 2024 - n = 383, Fri Jun 28 11:23:32 2024 - process use time: 0.108000 s
Fri Jun 28 11:23:32 2024 - n = 384, Fri Jun 28 11:23:32 2024 - process use time: 0.110000 s
Fri Jun 28 11:23:32 2024 - n = 385, Fri Jun 28 11:23:32 2024 - process use time: 0.106000 s
Fri Jun 28 11:23:32 2024 - n = 386, Fri Jun 28 11:23:32 2024 - process use time: 0.111000 s
Fri Jun 28 11:23:32 2024 - n = 387, Fri Jun 28 11:23:32 2024 - process use time: 0.111000 s
Fri Jun 28 11:23:32 2024 - n = 388, Fri Jun 28 11:23:32 2024 - process use time: 0.111000 s
Fri Jun 28 11:23:32 2024 - n = 389, Fri Jun 28 11:23:32 2024 - process use time: 0.109000 s
Fri Jun 28 11:23:32 2024 - n = 390, Fri Jun 28 11:23:33 2024 - process use time: 0.117000 s
Fri Jun 28 11:23:33 2024 - n = 391, Fri Jun 28 11:23:33 2024 - process use time: 0.112000 s
Fri Jun 28 11:23:33 2024 - n = 392, Fri Jun 28 11:23:33 2024 - process use time: 0.113000 s
Fri Jun 28 11:23:33 2024 - n = 393, Fri Jun 28 11:23:33 2024 - process use time: 0.209000 s
Fri Jun 28 11:23:33 2024 - n = 394, Fri Jun 28 11:23:33 2024 - process use time: 0.121000 s
Fri Jun 28 11:23:33 2024 - n = 395, Fri Jun 28 11:23:33 2024 - process use time: 0.117000 s
Fri Jun 28 11:23:33 2024 - n = 396, Fri Jun 28 11:23:33 2024 - process use time: 0.116000 s
Fri Jun 28 11:23:33 2024 - n = 397, Fri Jun 28 11:23:33 2024 - process use time: 0.117000 s
Fri Jun 28 11:23:34 2024 - n = 398, Fri Jun 28 11:23:34 2024 - process use time: 0.117000 s
Fri Jun 28 11:23:34 2024 - n = 399, Fri Jun 28 11:23:34 2024 - process use time: 0.120000 s
Fri Jun 28 11:23:34 2024 - n = 400, Fri Jun 28 11:23:34 2024 - process use time: 0.126000 s
Fri Jun 28 11:23:34 2024 - n = 401, Fri Jun 28 11:23:34 2024 - process use time: 0.125000 s
Fri Jun 28 11:23:34 2024 - n = 402, Fri Jun 28 11:23:34 2024 - process use time: 0.128000 s
Fri Jun 28 11:23:34 2024 - n = 403, Fri Jun 28 11:23:34 2024 - process use time: 0.131000 s
Fri Jun 28 11:23:34 2024 - n = 404, Fri Jun 28 11:23:34 2024 - process use time: 0.127000 s
Fri Jun 28 11:23:34 2024 - n = 405, Fri Jun 28 11:23:34 2024 - process use time: 0.131000 s
Fri Jun 28 11:23:35 2024 - n = 406, Fri Jun 28 11:23:35 2024 - process use time: 0.131000 s
Fri Jun 28 11:23:35 2024 - n = 407, Fri Jun 28 11:23:35 2024 - process use time: 0.130000 s
Fri Jun 28 11:23:35 2024 - n = 408, Fri Jun 28 11:23:35 2024 - process use time: 0.131000 s
Fri Jun 28 11:23:35 2024 - n = 409, Fri Jun 28 11:23:35 2024 - process use time: 0.130000 s
Fri Jun 28 11:23:35 2024 - n = 410, Fri Jun 28 11:23:35 2024 - process use time: 0.132000 s
Fri Jun 28 11:23:35 2024 - n = 411, Fri Jun 28 11:23:35 2024 - process use time: 0.136000 s
Fri Jun 28 11:23:35 2024 - n = 412, Fri Jun 28 11:23:35 2024 - process use time: 0.132000 s
Fri Jun 28 11:23:35 2024 - n = 413, Fri Jun 28 11:23:35 2024 - process use time: 0.136000 s
Fri Jun 28 11:23:36 2024 - n = 414, Fri Jun 28 11:23:36 2024 - process use time: 0.138000 s
Fri Jun 28 11:23:36 2024 - n = 415, Fri Jun 28 11:23:36 2024 - process use time: 0.137000 s
Fri Jun 28 11:23:36 2024 - n = 416, Fri Jun 28 11:23:36 2024 - process use time: 0.139000 s
Fri Jun 28 11:23:36 2024 - n = 417, Fri Jun 28 11:23:36 2024 - process use time: 0.143000 s
Fri Jun 28 11:23:36 2024 - n = 418, Fri Jun 28 11:23:36 2024 - process use time: 0.138000 s
Fri Jun 28 11:23:36 2024 - n = 419, Fri Jun 28 11:23:36 2024 - process use time: 0.141000 s
Fri Jun 28 11:23:36 2024 - n = 420, Fri Jun 28 11:23:36 2024 - process use time: 0.140000 s
Fri Jun 28 11:23:37 2024 - n = 421, Fri Jun 28 11:23:37 2024 - process use time: 0.145000 s
Fri Jun 28 11:23:37 2024 - n = 422, Fri Jun 28 11:23:37 2024 - process use time: 0.141000 s
Fri Jun 28 11:23:37 2024 - n = 423, Fri Jun 28 11:23:37 2024 - process use time: 0.141000 s
Fri Jun 28 11:23:37 2024 - n = 424, Fri Jun 28 11:23:37 2024 - process use time: 0.141000 s
Fri Jun 28 11:23:37 2024 - n = 425, Fri Jun 28 11:23:37 2024 - process use time: 0.143000 s
Fri Jun 28 11:23:37 2024 - n = 426, Fri Jun 28 11:23:37 2024 - process use time: 0.143000 s
Fri Jun 28 11:23:37 2024 - n = 427, Fri Jun 28 11:23:37 2024 - process use time: 0.147000 s
Fri Jun 28 11:23:38 2024 - n = 428, Fri Jun 28 11:23:38 2024 - process use time: 0.146000 s
Fri Jun 28 11:23:38 2024 - n = 429, Fri Jun 28 11:23:38 2024 - process use time: 0.147000 s
Fri Jun 28 11:23:38 2024 - n = 430, Fri Jun 28 11:23:38 2024 - process use time: 0.145000 s
Fri Jun 28 11:23:38 2024 - n = 431, Fri Jun 28 11:23:38 2024 - process use time: 0.149000 s
Fri Jun 28 11:23:38 2024 - n = 432, Fri Jun 28 11:23:38 2024 - process use time: 0.150000 s
Fri Jun 28 11:23:38 2024 - n = 433, Fri Jun 28 11:23:38 2024 - process use time: 0.149000 s
Fri Jun 28 11:23:38 2024 - n = 434, Fri Jun 28 11:23:38 2024 - process use time: 0.152000 s
Fri Jun 28 11:23:39 2024 - n = 435, Fri Jun 28 11:23:39 2024 - process use time: 0.155000 s
Fri Jun 28 11:23:39 2024 - n = 436, Fri Jun 28 11:23:39 2024 - process use time: 0.153000 s
Fri Jun 28 11:23:39 2024 - n = 437, Fri Jun 28 11:23:39 2024 - process use time: 0.151000 s
Fri Jun 28 11:23:39 2024 - n = 438, Fri Jun 28 11:23:39 2024 - process use time: 0.155000 s
Fri Jun 28 11:23:39 2024 - n = 439, Fri Jun 28 11:23:39 2024 - process use time: 0.154000 s
Fri Jun 28 11:23:39 2024 - n = 440, Fri Jun 28 11:23:39 2024 - process use time: 0.163000 s
Fri Jun 28 11:23:40 2024 - n = 441, Fri Jun 28 11:23:40 2024 - process use time: 0.157000 s
Fri Jun 28 11:23:40 2024 - n = 442, Fri Jun 28 11:23:40 2024 - process use time: 0.154000 s
Fri Jun 28 11:23:40 2024 - n = 443, Fri Jun 28 11:23:40 2024 - process use time: 0.155000 s
Fri Jun 28 11:23:40 2024 - n = 444, Fri Jun 28 11:23:40 2024 - process use time: 0.159000 s
Fri Jun 28 11:23:40 2024 - n = 445, Fri Jun 28 11:23:40 2024 - process use time: 0.154000 s
Fri Jun 28 11:23:40 2024 - n = 446, Fri Jun 28 11:23:40 2024 - process use time: 0.158000 s
Fri Jun 28 11:23:41 2024 - n = 447, Fri Jun 28 11:23:41 2024 - process use time: 0.159000 s
Fri Jun 28 11:23:41 2024 - n = 448, Fri Jun 28 11:23:41 2024 - process use time: 0.163000 s
Fri Jun 28 11:23:41 2024 - n = 449, Fri Jun 28 11:23:41 2024 - process use time: 0.158000 s
Fri Jun 28 11:23:41 2024 - n = 450, Fri Jun 28 11:23:41 2024 - process use time: 0.159000 s
Fri Jun 28 11:23:41 2024 - n = 451, Fri Jun 28 11:23:41 2024 - process use time: 0.164000 s
Fri Jun 28 11:23:41 2024 - n = 452, Fri Jun 28 11:23:41 2024 - process use time: 0.165000 s
Fri Jun 28 11:23:41 2024 - n = 453, Fri Jun 28 11:23:41 2024 - process use time: 0.163000 s
Fri Jun 28 11:23:42 2024 - n = 454, Fri Jun 28 11:23:42 2024 - process use time: 0.168000 s
Fri Jun 28 11:23:42 2024 - n = 455, Fri Jun 28 11:23:42 2024 - process use time: 0.164000 s
Fri Jun 28 11:23:42 2024 - n = 456, Fri Jun 28 11:23:42 2024 - process use time: 0.170000 s
Fri Jun 28 11:23:42 2024 - n = 457, Fri Jun 28 11:23:42 2024 - process use time: 0.172000 s
Fri Jun 28 11:23:42 2024 - n = 458, Fri Jun 28 11:23:42 2024 - process use time: 0.168000 s
Fri Jun 28 11:23:42 2024 - n = 459, Fri Jun 28 11:23:42 2024 - process use time: 0.168000 s
Fri Jun 28 11:23:43 2024 - n = 460, Fri Jun 28 11:23:43 2024 - process use time: 0.171000 s
Fri Jun 28 11:23:43 2024 - n = 461, Fri Jun 28 11:23:43 2024 - process use time: 0.172000 s
Fri Jun 28 11:23:43 2024 - n = 462, Fri Jun 28 11:23:43 2024 - process use time: 0.171000 s
Fri Jun 28 11:23:43 2024 - n = 463, Fri Jun 28 11:23:43 2024 - process use time: 0.169000 s
Fri Jun 28 11:23:43 2024 - n = 464, Fri Jun 28 11:23:43 2024 - process use time: 0.175000 s
Fri Jun 28 11:23:44 2024 - n = 465, Fri Jun 28 11:23:44 2024 - process use time: 0.173000 s
Fri Jun 28 11:23:44 2024 - n = 466, Fri Jun 28 11:23:44 2024 - process use time: 0.172000 s
Fri Jun 28 11:23:44 2024 - n = 467, Fri Jun 28 11:23:44 2024 - process use time: 0.176000 s
Fri Jun 28 11:23:44 2024 - n = 468, Fri Jun 28 11:23:44 2024 - process use time: 0.175000 s
Fri Jun 28 11:23:44 2024 - n = 469, Fri Jun 28 11:23:44 2024 - process use time: 0.176000 s
Fri Jun 28 11:23:44 2024 - n = 470, Fri Jun 28 11:23:44 2024 - process use time: 0.181000 s
Fri Jun 28 11:23:45 2024 - n = 471, Fri Jun 28 11:23:45 2024 - process use time: 0.181000 s
Fri Jun 28 11:23:45 2024 - n = 472, Fri Jun 28 11:23:45 2024 - process use time: 0.175000 s
Fri Jun 28 11:23:45 2024 - n = 473, Fri Jun 28 11:23:45 2024 - process use time: 0.180000 s
Fri Jun 28 11:23:45 2024 - n = 474, Fri Jun 28 11:23:45 2024 - process use time: 0.186000 s
Fri Jun 28 11:23:45 2024 - n = 475, Fri Jun 28 11:23:45 2024 - process use time: 0.180000 s
Fri Jun 28 11:23:45 2024 - n = 476, Fri Jun 28 11:23:45 2024 - process use time: 0.181000 s
Fri Jun 28 11:23:46 2024 - n = 477, Fri Jun 28 11:23:46 2024 - process use time: 0.189000 s
Fri Jun 28 11:23:46 2024 - n = 478, Fri Jun 28 11:23:46 2024 - process use time: 0.180000 s
Fri Jun 28 11:23:46 2024 - n = 479, Fri Jun 28 11:23:46 2024 - process use time: 0.192000 s
Fri Jun 28 11:23:46 2024 - n = 480, Fri Jun 28 11:23:46 2024 - process use time: 0.188000 s
Fri Jun 28 11:23:46 2024 - n = 481, Fri Jun 28 11:23:46 2024 - process use time: 0.190000 s
Fri Jun 28 11:23:47 2024 - n = 482, Fri Jun 28 11:23:47 2024 - process use time: 0.190000 s
Fri Jun 28 11:23:47 2024 - n = 483, Fri Jun 28 11:23:47 2024 - process use time: 0.185000 s
Fri Jun 28 11:23:47 2024 - n = 484, Fri Jun 28 11:23:47 2024 - process use time: 0.186000 s
Fri Jun 28 11:23:47 2024 - n = 485, Fri Jun 28 11:23:47 2024 - process use time: 0.194000 s
Fri Jun 28 11:23:47 2024 - n = 486, Fri Jun 28 11:23:47 2024 - process use time: 0.189000 s
Fri Jun 28 11:23:48 2024 - n = 487, Fri Jun 28 11:23:48 2024 - process use time: 0.187000 s
Fri Jun 28 11:23:48 2024 - n = 488, Fri Jun 28 11:23:48 2024 - process use time: 0.260000 s
Fri Jun 28 11:23:48 2024 - n = 489, Fri Jun 28 11:23:48 2024 - process use time: 0.191000 s
Fri Jun 28 11:23:48 2024 - n = 490, Fri Jun 28 11:23:48 2024 - process use time: 0.198000 s
Fri Jun 28 11:23:48 2024 - n = 491, Fri Jun 28 11:23:48 2024 - process use time: 0.193000 s
Fri Jun 28 11:23:49 2024 - n = 492, Fri Jun 28 11:23:49 2024 - process use time: 0.190000 s
Fri Jun 28 11:23:49 2024 - n = 493, Fri Jun 28 11:23:49 2024 - process use time: 0.194000 s
Fri Jun 28 11:23:49 2024 - n = 494, Fri Jun 28 11:23:49 2024 - process use time: 0.196000 s
Fri Jun 28 11:23:49 2024 - n = 495, Fri Jun 28 11:23:49 2024 - process use time: 0.198000 s
Fri Jun 28 11:23:49 2024 - n = 496, Fri Jun 28 11:23:49 2024 - process use time: 0.200000 s
Fri Jun 28 11:23:50 2024 - n = 497, Fri Jun 28 11:23:50 2024 - process use time: 0.196000 s
Fri Jun 28 11:23:50 2024 - n = 498, Fri Jun 28 11:23:50 2024 - process use time: 0.199000 s
Fri Jun 28 11:23:50 2024 - n = 499, Fri Jun 28 11:23:50 2024 - process use time: 0.204000 s
Fri Jun 28 11:23:50 2024 - n = 500, Fri Jun 28 11:23:50 2024 - process use time: 0.206000 s
Fri Jun 28 11:23:50 2024 - n = 501, Fri Jun 28 11:23:50 2024 - process use time: 0.205000 s
```

从本版本起，放弃将 S 缓存为二进制文件。想检验 `S[n][i][k]` 的话，需要将整个程序重跑一遍，无法从缓存文件中重读数据。如此可节约大概 20 PB 的磁盘空间，使本程序真实可能在单台 PC 上跑完全程。

当前算法仅开放一行 `i` 的空间滚动使用，并放弃使用一个闲置空间。由于去除保存文件的语句，因此减少了大量的文件读写时间，大幅提升程序执行效率。预计需要运行 14 day 6 h 25 min 39 s = 1232739 s。经初步运行的数据分析，单个 $n$ 运行时间服从 $1.57395\times 10^{-6} n^2 - 0.000740566 n$ 。总计运行时间服从 $5.28968\times 10^{-7} n^3 - 0.000455747 n^2 + 0.197321 n$ 。

### 历史版本 [v1.2](https://github.com/zsben2/esf/releases/tag/v1.2)  的时间估算

设 $n$ 为执行的趟次， $t$ 是该趟执行时间， $s$ 是截至该趟总共执行时间。显然， $n$ 和 $t$ 都能直接在上面的日志里取得， $s$ 需要将某行以前的所以 $t$ 加一下才能得到。使用 [Minitab](https://github.com/zsben2/esf/releases/download/v1.2/Minitab.mpx) 拟合计算，得到下图的拟合式

$$t = 1.60125\times 10^{-6} n ^ 2 - 0.000539289 n $$

![拟合线_ t 与 n](https://github.com/zsben2/esf/assets/63939755/44d662c8-86e0-4366-9997-9a11b246a9ca)

$$s = 4.92881\times 10^{-7} n ^ 3 - 0.000272266 n ^ 2 + 0.134986 n $$

![拟合线_ s 与 n](https://github.com/zsben2/esf/assets/63939755/f3a5dcc3-ee60-4b68-87ba-397f4a010121)

因此可以预测执行到第 $N=13542$ 趟时，需要消耗总时长为

$$\begin{aligned}
s
&= 4.92881\times 10^{-7} \times 13542 ^ 3 - 0.000272266 \times 13542 ^ 2 + 0.134986 \times 13542 \\
&= 1175923.8841302455 \text{ s } \\
&= 13 \text{ d } 14 \text{ h } 38 \text{ min } 44 \text{ s }
\end{aligned}$$
