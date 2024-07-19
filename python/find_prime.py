from pprint import pformat
from tools.number_theory import primes
from math import ceil, floor, e, exp, log    # 上下取整
from tools.ZCBtools import getLogger, AbsoluteTime
from typing import Iterable
import pickle
import os

logger = getLogger('output/log.txt')
N = 50216
K = floor(e * log(N) + e) # == 32


# 获取索引 i 两边最近的素数列表，n 为最大列表的最大索引
def generate_primes_on_both_sides_of_index(n:int):
    max_prime_less_than_index = [] # 小于等于索引 i 的最大素数
    min_prime_more_than_index = [] # 大于等于索引 i 的最小素数
    primes_less_than_n_iter: iter = primes(max=n+100) # 多取一个比 n 大的素数
    curr_prime = 0
    next_prime = next(primes_less_than_n_iter)
    for i in range(n+1):
        min_prime_more_than_index.append(next_prime)
        if i == next_prime:
            curr_prime = next_prime
            next_prime = next(primes_less_than_n_iter) # 不会报错，因为有一个比 n 大的素数
        max_prime_less_than_index.append(curr_prime)
    return max_prime_less_than_index, min_prime_more_than_index

max_prime_less_than_index, min_prime_more_than_index = generate_primes_on_both_sides_of_index(N // 2)
# index_between_prime_pair = list(zip(max_prime_less_than_index, min_prime_more_than_index))


# 找满足下列条件的素数 p
# n/(k+3) < p <= n/(k+1)
# p > max((k+2)*(k+3)/2, 3*k+8)
# 其中 1 <= k <= min(n-1, K)
# 2 <= n <= N
@AbsoluteTime(logger)
def find_prime() -> dict:
    result = {}
    for k in range(1, K+1): # 1 <= k <= K
        result[k] = {
            'not_find': [], # 不能找到素数的那些n
            'find': [] # 能找到素数的那些n
        }
        for n in range(ceil(exp(k/e-1)), N+1):
            # 求出满足 n/(k+3) < p <= n/(k+1) 的所有素数 p ∈ P = {p1, ..., p2} (p1 <= p2)

            # 若 n/(k+1) 是整数且是素数，则
            # p2 = max_prime_less_than_index[n//(k+1)]
            # 若 n/(k+1) 是整数但不是素数，则
            # p2 = max_prime_less_than_index[n//(k+1)]
            # 若 n/(k+1) 是小数，则 n//(k+1) 是整数
            # p2 = max_prime_less_than_index[n//(k+1)]
            # 综上
            p2 = max_prime_less_than_index[n//(k+1)]

            # 若 n/(k+3) 是整数且是素数，由于 p1 不能取 n//(k+3)，因此
            # p1 = min_prime_more_than_index[n//(k+3) + 1]
            # 若 n/(k+3) 是整数但不是素数，则
            # p1 = min_prime_more_than_index[n//(k+3) + 1]
            # 若 n/(k+3) 是小数，则
            # p1 = min_prime_more_than_index[n//(k+3) + 1]
            # 综上
            p1 = min_prime_more_than_index[n//(k+3) + 1]

            if not p1 <= p2:
                result[k]['not_find'].append(n)
                continue

            # 求闭区间 [p1, p2] 中的所有素数
            P: list = intersection_primes(p1, p2)

            # 如果存在素数 p > max((k+2)*(k+3)/2, 3*k+8)
            if max(P) > max((k+2) * (k+3) / 2, 3*k + 8):
                result[k]['find'].append(n)
            else:
                result[k]['not_find'].append(n)

    return result


# 求闭区间 [a, b] 中的所有素数
def intersection_primes(a: float, b: float) -> list:
    a: int = ceil(a)
    b: int = floor(b)
    if a > b:
        return []
    primes_sup = set(min_prime_more_than_index[a:b+1])
    primes_inf = set(max_prime_less_than_index[a:b+1])
    return list(primes_sup & primes_inf)


# 转置
@AbsoluteTime(logger)
def transposition(find_prime_result:dict) -> dict:
    transposition_result = { n: {'find': [], 'not_find': []} for n in range(1, N+1) }
    for k in range(1, K+1):
        for n in find_prime_result[k]['find']:
            transposition_result[n]['find'].append(k) # 能找到素数的那些k
        for n in find_prime_result[k]['not_find']:
            transposition_result[n]['not_find'].append(k) # 不能找到素数的那些k

    reduce_transposition_result = reduce_by_value(transposition_result)
    for d in reduce_transposition_result.values():
        d['find'] = reduce_interval(d['find'])
        d['not_find'] = reduce_interval(d['not_find'])

    return reduce_transposition_result


# 将字典d的key按照相同的value进行合并，key以元组(a,b)表示闭区间[a,b]
@AbsoluteTime(logger)
def reduce_by_value(d: dict) -> dict:
    reduce_result = {}
    items = list(d.items())
    items.sort()

    if len(items) == 0:
        return reduce_result

    last_left_key, last_value = items[0]
    if len(items) == 1:
        reduce_result[CloseInterval(last_left_key, last_left_key)] = last_value
        return reduce_result

    last_right_key = last_left_key
    for curr_key, curr_value in items[1:]:
        if last_value != curr_value:
            reduce_result[CloseInterval(last_left_key, last_right_key)] = last_value
            last_left_key = curr_key
            last_value = curr_value
        last_right_key = curr_key
    reduce_result[CloseInterval(last_left_key, last_right_key)] = last_value

    return reduce_result


# 在按k分组的字典中，将关于n的离散单点集约化为区间
@AbsoluteTime(logger)
def reduce_point_into_interval(find_prime_result:dict) -> dict:
    reduce_result = {}
    for k, d in find_prime_result.items():
        reduce_result[k] = {
            'not_find': reduce_interval(d['not_find']), # 不能找到素数的那些n
            'find': reduce_interval(d['find']) # 能找到素数的那些n
        }
    return reduce_result


# 离散单点集约化为闭区间
def reduce_interval(points: Iterable):
    result = []
    points = list(points)
    if len(points) == 0:
        return result
    points.sort()
    interval_left = points[0]
    last_point = interval_left
    for i in range(1, len(points)):
        curr_point = points[i]
        if last_point + 1 < curr_point: # 非连续区间
            result.append(CloseInterval(interval_left, last_point))
            interval_left = curr_point
        last_point = curr_point
    result.append(CloseInterval(interval_left, last_point))
    return result


class CloseInterval:
    def __init__(self, left, right):
        self.left = left
        self.right = right

    def __hash__(self):
        return hash((self.left, self.right))

    def __eq__(self, interval):
        return self.left == interval.left and self.right == interval.right

    def __le__(self, interval):
        return self.left < interval.left or self.left == interval.left and self.right < interval.right

    def __repr__(self):
        return f'[{self.left}, {self.right}]'

    def cap(self, interval):
        left = max(self.left, interval.left)
        right = min(self.right, interval.right)
        if left > right:
            return None
        return CloseInterval(left, right)


# 将变量保存到二进制文件中
@AbsoluteTime(logger)
def save_as(data, file_name:str):
    with open(file_name, 'wb') as f:
        pickle.dump(data, f)


# 从二进制文件中读取数据
@AbsoluteTime(logger)
def read_from(file_name:str):
    with open(file_name, 'rb') as f:
        data = pickle.load(f)
    return data


# 索引值i两端的相邻素数对，p0 <= i <= p1
def get_prime_pair_on_both_sides_of_index(i:float) -> tuple:
    a = max_prime_less_than_index[floor(i)]
    b = min_prime_more_than_index[ceil(i)]
    return a, b


def solveN(cache_path:str) -> dict:
    if os.path.exists(cache_path):
        result = read_from(cache_path)
    else:
        result = find_prime() # 需时约 1 min 9.003677 s
        save_as(result, cache_path)

    k_result_dict = reduce_point_into_interval(result) # 需时约 192.926645 ms
    # save_as(k_result_dict, 'output/k_result_dict.bin')
    logger.info(pformat(k_result_dict))

    n_result_dict = transposition(result) # 需时约 674.047709 ms
    # save_as(n_result_dict, 'output/n_result_dict.bin')
    logger.info(pformat(n_result_dict))
    return n_result_dict


if __name__ == '__main__':
    solveN('output/result.bin')
