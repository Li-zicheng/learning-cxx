#include "../exercise.h"

/// @brief 任意缓存容量的斐波那契类型。
/// @details 可以在构造时传入缓存容量，因此需要动态分配缓存空间。
class DynFibonacci {
    size_t *cache;
    int cached;

public:
    // 构造函数：动态分配缓存并初始化前两项
    DynFibonacci(int capacity) : cache(new size_t[capacity]), cached(2) {
        cache[0] = 0;  // fib(0) = 0
        cache[1] = 1;  // fib(1) = 1
    }

    // 析构函数：释放动态分配的缓存空间
    ~DynFibonacci() {
        delete[] cache;
    }

    // 计算斐波那契数列，使用缓存优化
    size_t get(int i) {
        // 检查输入范围
        if (i < 0) {
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
    DynFibonacci fib(12);
    ASSERT(fib.get(10) == 55, "fibonacci(10) should be 55");
    std::cout << "fibonacci(10) = " << fib.get(10) << std::endl;
    return 0;
}