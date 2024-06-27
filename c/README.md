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

```log
Thu Jun 27 22:45:50 2024 - 当前算法一次性开辟 N * N * K / 2 = 75 GB 空间然后再滚动释放
Thu Jun 27 22:45:50 2024 - 也是开不了的
Thu Jun 27 22:47:00 2024 - read T process use time: 1 min 9.558000 s
```

继而电脑死机
