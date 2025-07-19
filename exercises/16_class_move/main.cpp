#include "../exercise.h"

// READ: 左值右值（概念）<https://learn.microsoft.com/zh-cn/cpp/c-language/l-value-and-r-value-expressions?view=msvc-170>
// READ: 左值右值（细节）<https://zh.cppreference.com/w/cpp/language/value_category>
// READ: 关于移动语义 <https://learn.microsoft.com/zh-cn/cpp/cpp/rvalue-reference-declarator-amp-amp?view=msvc-170#move-semantics>
// READ: 如果实现移动构造 <https://learn.microsoft.com/zh-cn/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?view=msvc-170>

// READ: 移动构造函数 <https://zh.cppreference.com/w/cpp/language/move_constructor>
// READ: 移动赋值 <https://zh.cppreference.com/w/cpp/language/move_assignment>
// READ: 运算符重载 <https://zh.cppreference.com/w/cpp/language/operators>

class DynFibonacci {
    size_t *cache;   // 指向动态分配的缓存数组
    int cached;      // 已经缓存的斐波那契数的数量（或下一个需要计算的索引）
    int capacity;    // <--- 新增成员：缓存数组的总容量

public:
    // TODO: 实现动态设置容量的构造器
    // 构造函数负责分配内存并初始化基本状态
    DynFibonacci(int cap) : cache(new size_t[cap]), cached(2), capacity(cap) {
        // 断言：容量至少需要为 2，才能存储 F(0) 和 F(1)
        ASSERT(capacity >= 2, "Capacity must be at least 2 for initial Fibonacci values.");
        
        // 初始化斐波那契数列的基准项
        cache[0] = 0;
        cache[1] = 1;
    }

    // TODO: 实现移动构造器
    // 移动构造函数 "窃取" 源对象的资源，并将源对象置于有效但可析构的状态
    DynFibonacci(DynFibonacci &&other) noexcept 
        : cache(other.cache), cached(other.cached), capacity(other.capacity) {
        // 将源对象的资源指针置空，避免源对象析构时释放被 "窃取" 的内存
        other.cache = nullptr;
        other.cached = 0;
        other.capacity = 0;
    }

    // TODO: 实现移动赋值运算符
    // 移动赋值运算符处理将右值赋值给左值的情况，同样通过 "窃取" 资源来避免昂贵的复制操作
    // 注意：⚠ 注意移动到自身问题 ⚠
    DynFibonacci &operator=(DynFibonacci &&other) noexcept {
        // 自赋值检查：如果源对象和目标对象是同一个，直接返回，避免自我删除
        if (this == &other) {
            return *this;
        }

        // 1. 释放当前对象拥有的资源，避免内存泄漏
        delete[] cache;

        // 2. "窃取" 源对象的资源
        cache = other.cache;
        cached = other.cached;
        capacity = other.capacity;

        // 3. 将源对象置于有效但可析构的状态
        other.cache = nullptr;
        other.cached = 0;
        other.capacity = 0;

        return *this; // 返回当前对象的引用
    }

    // TODO: 实现析构器，释放缓存空间
    // 析构函数负责释放构造函数中动态分配的内存，避免内存泄漏
    ~DynFibonacci() {
        delete[] cache; // 释放 cache 指向的数组内存
        cache = nullptr; // 将指针置空是良好实践，防止悬空指针
    }

    // TODO: 实现正确的缓存优化斐波那契计算 (非 const 版本)
    // 使用 operator[] 重载替代 get 方法，可以修改类的成员变量
    size_t operator[](int i) { 
        // 断言：请求的索引 i 必须是有效的，且不能超过缓存容量
        ASSERT(i >= 0, "Index must be non-negative.");
        ASSERT(i < capacity, "Index i out of allocated cache capacity.");

        // 如果请求的斐波那契数已经缓存，直接返回
        if (i < cached) {
            return cache[i];
        }

        // 如果未缓存，则从上一个已缓存的索引开始，逐个计算并填充到 i
        for (int k = cached; k <= i; ++k) {
            cache[k] = cache[k - 1] + cache[k - 2];
        }

        // 更新 `cached`，表示现在已经缓存了 `0` 到 `i` 的所有数
        cached = i + 1;

        // 返回请求的斐波那契数
        return cache[i];
    }

    // NOTICE: 不要修改这个方法 (const 版本)
    // 这个方法承诺不修改类的成员变量，可以被 const 对象调用
    size_t operator[](int i) const {
        // 断言：i 必须在已缓存的有效范围内
        ASSERT(i < cached, "i out of range for const operator[] (value not yet cached)");
        return cache[i];
    }

    // NOTICE: 不要修改这个方法
    // 检查对象是否仍然 "拥有" 内存资源（即是否被移动走了）
    bool is_alive() const {
        return cache != nullptr; // 如果 cache 为 nullptr，则认为资源已被移走
    }
};

int main(int argc, char **argv) {
    DynFibonacci fib(12); // 调用带参数的构造函数
    ASSERT(fib[10] == 55, "fibonacci(10) should be 55"); // 调用非 const operator[]

    // 通过移动构造函数创建 fib_
    // fib 的资源（cache 指针）被转移到 fib_，fib 被置为 "空" 状态
    DynFibonacci const fib_ = std::move(fib); 
    ASSERT(!fib.is_alive(), "Object moved"); // 验证 fib 是否处于 "空" 状态
    ASSERT(fib_[10] == 55, "fibonacci(10) should be 55"); // 调用 const operator[]

    DynFibonacci fib0(6);  // 创建 fib0
    DynFibonacci fib1(12); // 创建 fib1
    fib1[10]; // 确保 fib1 已经计算到 fib(10)，这样移动后 fib0 也能有数据

    fib0 = std::move(fib1); // 调用移动赋值运算符。fib1 的资源转移到 fib0
    // fib1 会被置空。fib0 原有的资源会被释放，然后获得 fib1 的资源
    ASSERT(!fib1.is_alive(), "fib1 moved"); // 验证 fib1 是否处于 "空" 状态
    
    // 测试自赋值：将 fib0 移动给自身。正确的实现会处理这种情况，不导致数据丢失或崩溃
    fib0 = std::move(fib0); 
    ASSERT(fib0[10] == 55, "fibonacci(10) should be 55"); // 验证自赋值后数据依然正确

    return 0;
}