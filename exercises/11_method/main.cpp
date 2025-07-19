#include "../exercise.h" // 假设这个头文件包含了 ASSERT 宏和 std::cout

struct Fibonacci {
    unsigned long long cache[128]; // 缓存数组，最多存储 fib(0) 到 fib(127)
    int cached; // 记录下一个需要计算的斐波那契数的索引

    // TODO: 实现正确的缓存优化斐波那契计算
    unsigned long long get(int i) {
        // 1. 边界条件或已缓存的基准项处理
        // 如果 i 小于当前已缓存的范围，直接返回缓存值
        if (i < cached) {
            return cache[i];
        }

        // 2. 如果请求的 i 超出了缓存的最大容量
        // cache 数组大小是 128，索引范围是 0-127。
        // 如果 i >= 128，我们无法在当前缓存中存储结果。
        // 在实际应用中，这里可能需要抛出异常或返回错误码，
        // 或者切换到不使用缓存的计算方式。
        // 对于这个练习，我们假设 i 不会超过 127。
        ASSERT(i < 128, "Requested Fibonacci number index exceeds cache capacity.");


        // 3. 计算并填充缓存
        // 循环从当前已缓存的下一个索引开始，直到请求的 i。
        // 这样可以确保按顺序填充缓存，避免重复计算。
        for (int k = cached; k <= i; ++k) {
            // 斐波那契公式：F(k) = F(k-1) + F(k-2)
            cache[k] = cache[k - 1] + cache[k - 2];
        }

        // 4. 更新 cached 状态
        // 现在我们已经计算并缓存了从 fib(0) 到 fib(i) 的所有值。
        // 所以下一个需要计算的值的索引是 i + 1。
        cached = i + 1;

        // 5. 返回请求的斐波那契数
        return cache[i];
    }
};

int main(int argc, char **argv) {
    // TODO: 初始化缓存结构体，使计算正确
    Fibonacci fib;

    // 初始化斐波那契数列的基准项
    fib.cache[0] = 0;
    fib.cache[1] = 1;
    // 设置 cached 为 2，表示 fib(0) 和 fib(1) 已经缓存，下一个是 fib(2)
    fib.cached = 2; 

    // 使用 ASSERT 来验证计算结果
    ASSERT(fib.get(10) == 55, "fibonacci(10) should be 55");
    std::cout << "fibonacci(10) = " << fib.get(10) << std::endl;
    
    // 再次调用，验证缓存是否生效
    ASSERT(fib.get(5) == 5, "fibonacci(5) should be 5"); // 此时 fib(5) 应该已经缓存
    std::cout << "fibonacci(5) = " << fib.get(5) << std::endl;

    ASSERT(fib.get(20) == 6765, "fibonacci(20) should be 6765");
    std::cout << "fibonacci(20) = " << fib.get(20) << std::endl;
    
    // 如果 cache 足够大，可以测试更大的值
    ASSERT(fib.get(90) == 2880067194370816120ull, "fibonacci(90) should be a large number"); // 注意：90项可能需要unsigned long long
    std::cout << "fibonacci(90) = " << fib.get(90) << std::endl;

    return 0;
}