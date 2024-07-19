from functools import reduce
from typing import Iterable
import math

__all__ = ['gcd', 'lcm', 'factorization', 'factors', 'primes']

# 求两个数的最大公因数
__gcd = math.gcd
# def __gcd(a:int, b:int) -> int:
#     if b == 0:
#         return a
#     return __gcd(b, a % b)


# 求数组的最大公因数
def gcd(*l:Iterable):
    return reduce(__gcd, l)

# 求两个数的最小公倍数
def __lcm(x, y):
    return x * y // __gcd(x, y)

# 求数组的最小公倍数
def lcm(*l:Iterable):
    return reduce(__lcm, l)

# 素因子分解，本程序的时间复杂度比for i in primes()要好
def factorization(x:int) -> dict: # key是素数, value是阶
    if x <= 0:
        print('你输入的数字不合法')
        return []

    t = x
    result = {} #存放因式

    if x == 1:
        result[1] = 1
    else:
        i = 2

        while True:
            if t == i:
                result[i] = result.get(i, 0) + 1
                break

            if t % i == 0:
                result[i] = result.get(i, 0) + 1
                t = t / i
            else:
                i += 1

    return result


# 求所有因数的递归子函数
def __factors(fact_list:list) -> list:
    if len(fact_list) == 0:
        return [1]

    p, e = fact_list.pop()
    L = []

    for mul in __factors(fact_list):
        for i in range(e+1):
            multi = p ** i * mul
            L.append(multi)
    return L

# 求数x的所有因数
def factors(x:int) -> list:
    faction = factorization(x)
    result = __factors(list(faction.items()))
    result.sort()
    return result


# 素数生成器，生成max_以下的不超过num个素数。生成10000个素数仅耗时0.4s
def primes(max=float('inf'), num=float('inf')):
    PRIMES = [2]
    yield 2
    target = 3

    while True:
        sqrt_target = math.sqrt(target) # float
        for i in PRIMES: # 检验是否被已有素数整除
            if i - sqrt_target <= 1e-5: # i <= sqrt_target
                if target % i == 0: # 被整除就是合数了
                    break
            else: # 遍历至超过sqrt(target)的素数，都仍未被整除，就是新素数
                PRIMES.append(target)
                yield target
                break

        if target > max or len(PRIMES) > num:
            return

        target += 2



if __name__ == '__main__':
    # from itertools import combinations # 组合
    # min_lcm_7_12 = min(lcm(18,*B) for B in combinations(range(19,30), 5))
    # print(min_lcm_7_12)

    # ak = 30
    # q = lcm(180,ak)/ak
    # print(lcm(180,ak), q, ak*q/(q-1))

    # print(lcm(*range(1,11),12,15))
    # print(lcm(*range(7,11),12,15)-120*10)
    # print(lcm())
    # print(lcm(7,8,9,10,12,15,16,18,20))
    # print(factors(84))
    # print(factors(2))
    # print(factors(4))
    # print(factors(6))
    # print(factors(12))
    # print(factors(24))
    # print(factors(60))
    # print(factors(120))
    # print(factors(168))
    # print(factors(180))
    # print(factors(240))
    # print(factors(252))
    # print(factors(360))
    # print(factors(432))
    # print(factors(600))
    # print(factors(660))
    # print(factors(720))
    # print(factors(780))
    # print(factors(792))
    # print(factors(840))
    # print(factors(924))
    # print(factors(936))
    # print(factors(1008))
    # print(factors(1080))
    # print(factors(1260))
    # print(factors(1680))
    # print(factors(2520))
    # print(factors(5040))
    # print(factors(10080))
    # print(factors(15120))
    # print(factors(27720))
    # lcm40 = lcm(*range(1, 60))
    # print(lcm40)
    # print(factorization(lcm40))
    # print(factors(720))
    # print(factors(840))
    # print(factors(1260))
    # print(factors(1680))
    # print(factors(2520))
    # print(factors(5040))
    # print(factorization(5040))
    # print(lcm(*range(1,24)))
    # print(factors(lcm(*range(1,24))))
    # print(lcm([232792560, 20, 21, 22, 23]))
    p = primes(max=100000)
    # for i in range(100000):
        # next(p)
    for i in p:
        # print()
        pass
