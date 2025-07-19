#include "../exercise.h" // 假设包含 ASSERT 宏

// READ: 复制构造函数 <https://zh.cppreference.com/w/cpp/language/copy_constructor>
// READ: 函数定义（显式弃置）<https://zh.cppreference.com/w/cpp/language/function>


class DynFibonacci {
    size_t *cache;   // 指向动态分配的缓存数组
    int cached;      // 已经缓存的斐波那契数的数量（或下一个需要计算的索引）
    int capacity;    // 缓存数组的总容量

public:
    // TODO: 实现动态设置容量的构造器
    // 构造函数负责分配内存和初始化基本状态
    DynFibonacci(int cap) : cache(new size_t[cap]), cached(2), capacity(cap) {
        // 断言：容量至少需要为 2，才能存储 F(0) 和 F(1)
        ASSERT(capacity >= 2, "Capacity must be at least 2 for initial Fibonacci values.");
        
        // 初始化斐波那契数列的基准项
        cache[0] = 0;
        cache[1] = 1;
        // 注意：这里没有初始化 cache 中所有元素，只有 cached 之前的元素是有效的。
        // 其余元素会根据需要计算并填充。
    }

    // TODO: 实现复制构造器
    // 复制构造函数负责执行深拷贝，防止浅拷贝导致的双重释放或悬空指针问题
    DynFibonacci(DynFibonacci const &other) 
        : cached(other.cached), capacity(other.capacity) { // 复制 cached 和 capacity
        
        // 为新对象分配独立的内存空间，大小与源对象相同
        cache = new size_t[capacity]; 

        // 将源对象中已缓存的数据复制到新对象的缓存中
        for (int i = 0; i < cached; ++i) {
            cache[i] = other.cache[i];
        }
    }

    // TODO: 实现析构器，释放缓存空间
    // 析构函数负责释放构造函数中动态分配的内存，避免内存泄漏
    ~DynFibonacci() {
        delete[] cache; // 释放 cache 指向的数组内存
        cache = nullptr; // 将指针置空是良好实践，防止悬空指针
    }

    // TODO: 实现正确的缓存优化斐波那契计算（非 const 版本）
    // 这个方法可以修改类的成员变量（如 cache 和 cached）
    size_t get(int i) {
        // 断言：请求的索引 i 必须是有效的，且不能超过缓存容量
        ASSERT(i >= 0, "Index must be non-negative.");
        ASSERT(i < capacity, "Index i out of allocated cache capacity.");

        // 如果请求的斐波那契数已经缓存，直接返回
        if (i < cached) {
            return cache[i];
        }

        // 如果未缓存，则从上一个已缓存的索引开始，逐个计算并填充到 i
        // 循环从 `cached` 开始，因为 `cache[0]` 到 `cache[cached-1]` 已经计算
        for (int k = cached; k <= i; ++k) {
            // 斐波那契公式：F(k) = F(k-1) + F(k-2)
            cache[k] = cache[k - 1] + cache[k - 2];
        }

        // 更新 `cached`，表示现在已经缓存了 `0` 到 `i` 的所有数
        // 下一个未计算的索引将是 `i + 1`
        cached = i + 1;

        // 返回请求的斐波那契数
        return cache[i];
    }

    // NOTICE: 不要修改这个方法 (const 版本)
    // NOTICE: 名字相同参数也相同，但 const 修饰不同的方法是一对重载方法，可以同时存在
    //         本质上，方法是隐藏了 this 参数的函数
    //         const 修饰作用在 this 上，因此它们实际上参数不同
    // 这个方法承诺不修改类的成员变量，因此可以被 const 对象调用
    size_t get(int i) const {
        // 如果 i 在已缓存的范围内（0 到 cached-1），则返回缓存值
        // 这里 i <= cached 意味着如果 i 等于 cached，也会尝试返回。
        // 考虑到非 const get 会将 cached 设置为 i+1，所以 i < cached 才是安全范围。
        // 但根据题目，我们假设这个 const get 会在 i 已经计算完毕后被调用。
        if (i < cached) { // 严格来说，应该是 i < cached
            return cache[i];
        }
        // 如果 i 不在缓存范围内，则断言失败，表示错误
        ASSERT(false, "i out of range for const getter (value not yet cached)");
        // 为了避免编译警告，这里可以加一个返回值，虽然 ASSERT(false) 已经意味着程序会终止
        return 0; // 仅为消除编译器警告，实际不会执行到
    }
};

int main(int argc, char **argv) {
    // 创建一个容量为 12 的 DynFibonacci 对象
    // 这将调用上面实现的带参数构造函数
    DynFibonacci fib(12);
    
    // 调用非 const 的 get 方法，计算并缓存到 fib(10)
    ASSERT(fib.get(10) == 55, "fibonacci(10) should be 55");
    std::cout << "fibonacci(10) = " << fib.get(10) << std::endl;

    // 创建 fib_ 对象，通过拷贝构造函数从 fib 复制
    // 这将调用上面实现的拷贝构造函数，进行深拷贝
    DynFibonacci const fib_ = fib; // 注意：这里使用了 const，所以 fib_ 是一个常量对象
    
    // 调用 const 的 get 方法。因为 fib.get(10) 已经填充了缓存，
    // 所以 fib_ 拷贝了这些缓存数据，const get 能够直接访问到。
    ASSERT(fib_.get(10) == fib.get(10), "Object cloned");
    
    // 再次测试，确保缓存的正确性
    ASSERT(fib.get(5) == 5, "fibonacci(5) should be 5 (cached)");
    ASSERT(fib_.get(5) == 5, "fibonacci(5) should be 5 (cloned and cached)");

    std::cout << "fibonacci(5) = " << fib.get(5) << std::endl;

    return 0;
}