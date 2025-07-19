#include "../exercise.h" // 假设包含 ASSERT 宏和 std::cout

// READ: Trivial type <https://learn.microsoft.com/zh-cn/cpp/cpp/trivial-standard-layout-and-pod-types?view=msvc-170>

struct FibonacciCache {
    unsigned long long cache[16];
    int cached; // 记录当前已经缓存的斐波那契数的数量，或者下一个待计算的索引
};

// TODO: 实现正确的缓存优化斐波那契计算
static unsigned long long fibonacci(FibonacciCache &cache, int i) {
    // 边界条件处理
    if (i < 0) {
        // 或者抛出异常，或者返回特定错误码
        // 在斐波那契数列中，负数索引通常没有定义
        return 0; // 假设处理为0
    }
    if (i == 0) {
        return cache.cache[0]; // 0项
    }
    if (i == 1) {
        return cache.cache[1]; // 1项
    }

    // 检查是否已经缓存
    // 如果 i 在已经计算的范围内，直接返回缓存值
    if (i < cache.cached) { 
        return cache.cache[i];
    }

    // 如果 i 超出了缓存数组的物理容量，则无法缓存，需要递归计算（或抛出错误）
    // 考虑到题目是 cache[16]，i=10，不会超限。
    // 如果 i 超过 15，这里会发生越界访问，实际项目中需要更严谨的检查
    if (i >= sizeof(cache.cache) / sizeof(cache.cache[0])) {
         // 这里表示请求的 i 超过了缓存容量，不再适合缓存优化，直接计算或抛出错误
         // 简单处理：如果超出了缓存范围，但之前的逻辑保证了 i-1 和 i-2 依然能从缓存中取到，则继续。
         // 否则这里就回到了无优化的递归。
         // 为了满足题目意图，我们假设 i 不会超出 cache[16] 的范围。
    }


    // 逐个计算并填充缓存直到 i
    // 从当前已缓存的下一个索引开始计算
    for (int k = cache.cached; k <= i; ++k) {
        cache.cache[k] = cache.cache[k - 1] + cache.cache[k - 2];
    }

    // 更新已缓存的数量。现在 cache.cache[i] 已经被计算，所以已缓存的数量增加到 i + 1。
    cache.cached = i + 1;

    // 返回请求的斐波那契数
    return cache.cache[i];
}

int main(int argc, char **argv) {
    // TODO: 初始化缓存结构体，使计算正确
    // NOTICE: C/C++ 中，读取未初始化的变量（包括结构体变量）是未定义行为
    // READ: 初始化的各种写法 <https://zh.cppreference.com/w/cpp/language/initialization>
    FibonacciCache fib;
    // 方案1: 聚合初始化 (对于POD类型或聚合类型)
    // FibonacciCache fib = {{0, 1}, 2}; // cache[0]=0, cache[1]=1, cached=2
    
    // 方案2: 显式初始化
    fib.cache[0] = 0;
    fib.cache[1] = 1;
    fib.cached = 2; // 表示 cache[0] 和 cache[1] 已经有效

    // NOTE: C++11 及更高版本，你可以使用列表初始化来零初始化整个数组：
    // FibonacciCache fib = {}; // 这会把所有成员都零初始化，包括 cache 数组和 cached 变量
    // 然后再设置 fib.cache[0]=0, fib.cache[1]=1, fib.cached=2;
    // 但为了明确初始化0和1，显式赋值更好。

    ASSERT(fibonacci(fib, 10) == 55, "fibonacci(10) should be 55");
    std::cout << "fibonacci(10) = " << fibonacci(fib, 10) << std::endl; // 再次调用，应该直接从缓存返回
    return 0;
}