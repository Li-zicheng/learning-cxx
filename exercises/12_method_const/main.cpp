#include "../exercise.h" // 假设这个头文件包含了 ASSERT 宏和 std::cout

// READ: 有 cv 限定符的成员函数 <https://zh.cppreference.com/w/cpp/language/member_functions>

struct Fibonacci {
    int numbers[11]; // 存储斐波那契数列的数组
    
    // TODO: 修改方法签名和实现，使测试通过
    // 将方法标记为 const 成员函数，因为它不修改类的任何成员变量。
    // 这允许 const/constexpr 对象调用此方法。
    int get(int i) const { 
        // 简单地返回数组中对应索引的值
        // 在实际应用中，这里应该加入边界检查，确保 i 在有效范围内 (0 到 10)
        ASSERT(i >= 0 && i < 11, "Index i out of bounds for numbers array");
        return numbers[i];
    }
};

int main(int argc, char **argv) {
    // Fibonacci constexpr FIB{{0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55}};
    // 另一种初始化语法，效果相同
    constexpr Fibonacci FIB{ .numbers = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55} };

    ASSERT(FIB.get(10) == 55, "fibonacci(10) should be 55");
    std::cout << "fibonacci(10) = " << FIB.get(10) << std::endl;
    
    // 也可以测试其他索引
    ASSERT(FIB.get(0) == 0, "fibonacci(0) should be 0");
    ASSERT(FIB.get(5) == 5, "fibonacci(5) should be 5");

    return 0;
}