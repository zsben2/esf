'''
赵崇斌工具库 ZCBtools.py
最后更新日期：2024.07.19
更新日志：
2024.07.19 加入函数getLogger
2019.06.22 重写reduceByKey函数，加入分块功能，原reduceByKey函数重命名为reduceByKey_
2019.06.21 重写flatten函数，新增非递归的循环版本_flatten2，原递归版本重命名为_flatten1，
           非递归版本平均效率提升5倍以上。将重写的list函数重命名为list_。新增blockReduceByKey函数
2019.06.16 showtqdm改为_showtqdm；加入函数_f；重写list函数，完整封装实现pmap函数
2019.06.15 加入函数pmap，但仍未实现；加入函数map_，showtqdm；加入装饰器absoluteTime
2019.06.14 加入类__NOW和_NOW，和动态常量NOW，返回当前时间
2019.06.13 加入函数reduceByKey和flatten
'''
from functools import reduce
from time import strftime, time
from types import FunctionType

from tqdm import tqdm
from typing import Union
from numpy import sqrt
from multiprocessing import Process, Pool, Queue, Manager
from collections.abc import Iterator
import inspect
import logging
import os


__all__ = ['NOW', 'reduceByKey', 'reduceByKey_', 'flatten', 'pmap', 
    'absoluteTime', 'list_', 'map_', 'getLogger', 'get_sup_model', 'AbsoluteTime']
__author__ = '赵崇斌'
__version__ = '2019.06.22'

class __NOW(type):
    def __repr__(self):
        return strftime('%Y-%m-%d %H:%M:%S')
class _NOW(object, metaclass=__NOW):
    pass

NOW = str(_NOW)
'''
解释：动态常量，返回当前时间
调用格式：ZCBtools.NOW
'''

# _build_list = list
def list_(obj):
    '''
    重写的list函数，是为了兼容ApplyResult和MapResult类型
    查看原list函数帮助请使用
    help(ZCBtools._build_list)
    '''
    from multiprocessing.pool import MapResult, ApplyResult
    if isinstance(obj, (MapResult, ApplyResult)):
        return obj.get()
    return list(obj)


def _absoluteTime(myprint, func, *args, **kwargs):
    t1 = time()
    ret = func(*args, **kwargs)
    t2 = time()
    deltaT = t2 - t1
    if deltaT <= 10 ** -3:
        myprint('%s函数运行时间：%.6f μs' % (func.__name__, deltaT*10**6))
    elif deltaT <= 1:
        myprint('%s函数运行时间：%.6f ms' % (func.__name__, deltaT*10**3))
    elif deltaT <= 60:
        myprint('%s函数运行时间：%.6f s' % (func.__name__, deltaT))
    elif deltaT <= 3600:
        myprint('%s函数运行时间：%d min %.6f s' % (func.__name__, deltaT//60, deltaT%60))
    else:
        myprint('%s函数运行时间：%d h %d min %.6f s' % (func.__name__, deltaT//3600, deltaT%3600//60, deltaT%60))
    return ret


absoluteTime = lambda func: lambda *args, **kwargs: _absoluteTime(print, func, *args, **kwargs)
absoluteTime.__doc__ = '''
参数：func——被装饰的函数
返回：运行函数func所使用的绝对时间长度
解释：装饰器函数。可参照Mathematica的AbsoluteTime函数
'''


class AbsoluteTime:
    '''
    装饰器类，可直接装饰函数，也可携带一个Logger再装饰函数。例如：
    @AbsoluteTime
    def func1():
        pass
    或
    log = logging.getLogger('test')
    @AbsoluteTime(log)
    def func2():
        pass
    前者效果等同于直接使用装饰器函数
    @absoluteTime
    def func1():
        pass
    '''
    def __init__(self, log_or_func:Union[logging.Logger, FunctionType]):
        self.log_or_func = log_or_func

    def __call__(self, *args, **kwargs):
        if isinstance(self.log_or_func, logging.Logger):
            func = args[0]
            log = self.log_or_func.info
            return lambda *args, **kwargs: _absoluteTime(log, func, *args, **kwargs)
        else:
            func = self.log_or_func
            return _absoluteTime(print, func, *args, **kwargs)


def reduceByKey_(func, args):
    '''
    参数：func——二元函数；args——键值对列表或键值对元组
    返回：键值对列表
    解释：该函数在单机上模拟pyspark中高阶分布式计算函数reduceByKey的功能。将传入列表的键值对按照相同键压缩值
    测试用例：
    a = [(1,2),(1,5),(3,4),(3,7),(3,1),(2,2)]
    b = ZCBtools.reduceByKey(lambda x, y: ZCBtools.flatten([x, y]), a)
    c = dict(b)
    d = ZCBtools.reduceByKey(lambda x, y: x + y, a)
    e = ZCBtools.reduceByKey(lambda x, y: x - y, a)
    f = ZCBtools.reduceByKey(lambda x, y: x * y, a)
    g = ZCBtools.reduceByKey(lambda x, y: x / y, a)
    print('a = {}\\nb = {}\\nc = {}\\nd = {}\\ne = {}\\nf = {}\\ng = {}'.format(a, b, c, d, e, f, g))
    '''
    def f(arg1, arg2) -> list: # 各初始元素一定要是tuple类型的
        if isinstance(arg1, (tuple)): # 如果arg1是tuple类型
            if arg1[0] == arg2[0]: # 首次调用且key一样时使用
                return [(arg1[0], func(arg1[1],arg2[1]))]
            else: # 首次调用且key不一样时使用
                return [arg1,arg2] 
        else:
            bool = 1 # 标记是否匹配
            for list_one in arg1:
                if list_one[0] == arg2[0]:
                    arg1[arg1.index(list_one)] = (list_one[0], func(list_one[1],arg2[1])) # key相同时
                    bool = 0 
                    break # 每次最多有一个key相同
            if bool: # 不匹配，添加
                arg1.append(arg2)
            return arg1

    return reduce(f, args)



_flatten1 = lambda x: (y for l in x for y in _flatten1(l)) if isinstance(x, (list,tuple)) else (x,)
def _flatten2(args) -> list: # 非递归的循环方案
    '''
    参数：args——高维列表或高维元组
    返回：一维元组
    解释：flatten的非递归的循环方案，测试时执行效率是递归版的3.5倍
    '''
    if not isinstance(args, (Iterator,list,dict,tuple)): # 各种可迭代对象，除了str
        return [args]

    stack = [] # 初始化栈
    result = [] # 初始化结果
    s = iter(args) # 将各种可迭代对象转化为迭代器，内含__next__方法，可用next函数遍历

    while True: # 与其每循环一次都判断是否栈空和指针越界，倒不如写成死循环让程序自行报错退出
        try:
            s0 = next(s)
        except StopIteration: # 指针越界，next到终点
            try:
                s = stack.pop() # 出栈
            except IndexError: # stack栈空
                return result
        else:
            if isinstance(s0, (Iterator,list,dict,tuple)):
                stack.append(s) # 把存有当前未遍历元素的s入栈
                s = iter(s0) # 转换成迭代器
            else:
                result.append(s0)

# @absoluteTime
def flatten(args, recursion=False) -> tuple:
    '''
    参数：args——高维列表或高维元组，recursion=False表示默认不使用递归版本
    返回：一维元组
    解释：可参照Mathematica的Flatten函数。该函数将传入的列表、元组或基本类型参数压平成一维元组。
         测试时非递归版本的执行效率是递归版本的5倍以上
         不可将各级字符串对象压平至同一字符串对象内，对于这种需求，要使用
         strings = ('abc',(('def')),('ghi','jkl'))
         string = reduce(lambda x, y: x + y, flatten(strings))
         print(string)
    测试用例：print(f'{ZCBtools.flatten([(1,2),(3,(4,(5,(9,0),8)))]) =}')
    '''
    if recursion:
        return tuple(_flatten1(args))
    return tuple(_flatten2(args))


def reduceByKey(func, iterable, BLOCK:Union[int,float]=float('inf')):
    ''' 
    参数：func——二元函数；iterable——可迭代型数据；
         BLOCK——块大小，需为非±1的int或±inf，
         BLOCK=inf（默认）时，即只有1块，此时直接调用reduceByKey_
         BLOCK=0时，会令BLOCK=int(sqrt(len(iterable)))
         BLOCK=-inf时，会令BLOCK=-int(sqrt(len(iterable)))
         BLOCK>0时，块大小即为BLOCK，且进行压缩汇总
         BLOCK<0时，块大小为-BLOCK，且不进行压缩汇总
         经分析，BLOCK=int(sqrt(len(iterable)))时，能较好平衡时间开销和数据浪费的影响
    返回：与参数func返回值一致
    解释：分块reduceByKey函数，可应对数据量大的情况
    由于reduce函数时间复杂度为o(n^2)，所以块越小消耗时间越短，目前测试：
    BLOCK=100时，耗时21s；BLOCK=1k时，耗时50s
    BLOCK=1w时，耗时320s；BLOCK=3w时，耗时18min
    块划分的缺点是，由于块是定长的，所以可能将来自同一键的两个不同值被划分到不同块中，
    导致块汇总成flattenData的时候，存在同键不同值的情况，而后续dict(flattenData)过程，
    遇到相同键会以新键值对取代就键值对的形式处理，造成该键值对信息浪费。
    鉴于块划分越小，相邻块间键值对信息浪费越多，因此在时间条件允许的情况下，不宜划分过细。
    但若在返回前进行一次整体汇总的压缩的话，则可以消灭块大小对键值对信息浪费的影响，即令BLOCK>0。
    '''
    if not isinstance(iterable, (Iterator,list,dict,tuple)): # 若iterable不可迭代
        raise ValueError('参数iterable需为可迭代变量')
    if BLOCK == float('inf'): # 直接调用_reduceByKey
        return reduceByKey_(func, iterable)
    if not isinstance(BLOCK, int) and BLOCK != float('-inf') or abs(BLOCK) == 1 : # 若非除inf外非±1的整数
        raise ValueError('参数BLOCK需为非±1的int或float("±inf")')
    if not hasattr(iterable, '__len__'): # 如果iterable中没有__len__方法，即不可求长
        iterable = list(iterable)
    N = len(iterable)
    if BLOCK == 0:
        BLOCK = int(sqrt(N))
    elif BLOCK == float('-inf'):
        BLOCK = -int(sqrt(N))
    N = N // abs(BLOCK) + (N%abs(BLOCK) != 0) # 不能整除时加1个块
    flattenData = []
    pbar = tqdm(total=N, ascii=True)
    for i in range(N):
        temp = reduceByKey_(func, iterable[i*abs(BLOCK) : (i+1)*abs(BLOCK)]) # 大于索引时仅索引到最后一个
        flattenData += list(temp)
        pbar.update()
    # flattenData = iter(flattenData)
    # 若在返回前进行一次整体汇总的压缩的话，则可以消灭块大小对键值对信息浪费的影响
    if BLOCK > 0:
        print('数据分块压缩完成，汇总数据个数为', len(flattenData))
        flattenData = reduceByKey_(func, flattenData)
        print('数据整体压缩完成')
    return flattenData


def map_(f, *args):
    '''
    内置map函数的人工实现
    '''
    for var in zip(*args): # 注意用zip(*args)运行可能会报错，因为args可能是一个生成器，不能被*运算作用，应用apply(zip, args)
        yield f(*var)


#队列实现
def _showtqdm(q:Queue, max_) -> None:
    '''
    参数：q——进程通信消息队列Queue；max_——进度条最大值
    解释：为最大值max_显示进度条
    '''
    with tqdm(total=max_, ascii=True) as t:
        j = 1
        q.get()
        while j < max_:
            t.update(1)
            j += 1
            q.get() # 当队列为空是，会阻塞等待
        t.update(1)


def _f(mq, f, iter):
    '''
    为函数f添加通讯队列，但注意不可用装饰器实现
    '''
    mq.put(1)
    # print(999,'|',os.getpid())
    return f(iter)


def pmap(f, iterable, processes=4):
    '''
    参数：f——一元函数；iterable——列表或元组等迭代器；processes——进程池数量，默认跑4个CPU，用os.cpu_count()可查看CPU总数
    返回：与map(f, iterable)返回值相同，是一个生成器generator
    解释：可参照Mathematica的ParallelMap函数。
         pmap是一个多进程并行的map，将f作用在不同的内核和处理器之间
         pmap将给出和map(f, iterable)相同的结果，除了计算过程中的副作用
         对于不能并行的pmap，用map执行计算
    测试用例：暂无
    '''

    if not hasattr(iterable, '__len__'): # 如果iterable中没有__len__方法，即不可求长
        iterable = list(iterable)

    # multiprocessing 的 Queue 不能用于 Pool ( 进程池)，需要用multiprocessing的Manager 的 Queue，需要实例化：queue = Manager().Queue()
    mq = Manager().Queue()

    p0 = Process(target=_showtqdm, args=(mq, len(iterable)))
    p1 = Pool(processes)

    # starmap_async是f有多参数的map_async；zip,starmap_async,map_async均返回可迭代对象，类似内建的map返回值一样
    result = p1.starmap_async(_f, ((mq, f, i) for i in iterable))

    p0.start()
    p1.close()

    p1.join()
    p0.join()

    return result


# 返回该函数往上数第 level 层调用的调用文件名
# 默认 level = 1 表示当前调用层的文件名
def get_sup_model(level=1) -> str:
    # 根路径
    root_file_full_path = inspect.stack()[-1].filename
    root_dir = os.path.dirname(root_file_full_path)
    # 调用文件的路径
    sup_file_full_path = inspect.stack()[level].filename
    # 相对路径
    relative_path = os.path.relpath(sup_file_full_path, root_dir)
    # 先去掉末尾的 .py, 再转换路径分隔符
    python_path = relative_path[:-3].replace('/', '.').replace('\\', '.')
    return python_path


def getLogger(filename='./log.txt',
              name:Union[str,None]=None,
              fmt='%(asctime)s - %(name)s - %(funcName)s - %(levelname)s - %(message)s',
              level='DEBUG') -> logging.Logger:
    name = get_sup_model(2) if name is None else name
    # 设置日志器的名字
    log = logging.getLogger(name)
    # 设置全局日志级别
    log.setLevel('DEBUG')
    # 创建日志记录的格式
    log_formatter = logging.Formatter(fmt)

    # 创建文件日志记录器，指明日志保存的路径，每个日志文件的最大值，保存的日志文件个数上限
    file_handle = logging.FileHandler(filename)
    # 设置文件日志级别
    file_handle.setLevel(level)
    # 为创建的日志记录器设置日志记录格式
    file_handle.setFormatter(log_formatter)
    # 为全局的日志工具对象添加日志记录器
    log.addHandler(file_handle)

    # 设置输出日志到控制台
    control_handler = logging.StreamHandler()
    # 设置控制台日志级别
    control_handler.setLevel(level)
    # 为创建的日志记录器设置日志记录格式
    control_handler.setFormatter(log_formatter)
    # 把控制台日志对象添加到logger中
    log.addHandler(control_handler)

    return log


if __name__ == '__main__':
    strings = ((3,6),(1,2),(1,3),(1,4),(2,8),(2,10))
    string = reduceByKey(lambda x, y: flatten([x, y]), strings, float('-inf'))
    print((string))
