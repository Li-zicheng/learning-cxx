#include "../exercise.h"

class Fibonacci {
    size_t cache[16];
    int cached;

public:
    // 构造函数：初始化 cache 数组和 cached 变量
    Fibonacci() : cached(2) {
        cache[0] = 0;  // fib(0) = 0
        cache[1] = 1;  // fib(1) = 1
    }

    // 计算斐波那契数列，使用缓存优化
    size_t get(int i) {
        // 检查输入范围
        if (i < 0 || i >= 16) {
            return 0; // 或者可以抛出异常
        }
        
        // 计算直到需要的索引
        for (; cached <= i; ++cached) {
            cache[cached] = cache[cached - 1] + cache[cached - 2];
        }
        return cache[i];
    }
};

int main(int argc, char **argv) {
    Fibonacci fib;
    ASSERT(fib.get(10) == 55, "fibonacci(10) should be 55");
    std::cout << "fibonacci(10) = " << fib.get(10) << std::endl;
    return 0;
}