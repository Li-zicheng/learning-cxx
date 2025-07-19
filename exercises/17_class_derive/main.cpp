#include "../exercise.h"
#include <iostream> // For std::cout
#include <cstddef>  // For std::size_t, though not strictly needed for sizeof with int

// READ: 派生类 <https://zh.cppreference.com/w/cpp/language/derived_class>

static int i = 0; // Global counter for tracking constructor/destructor calls

struct X {
    int x;

    X(int x_) : x(x_) {
        std::cout << ++i << ". " << "X(" << x << ')' << std::endl;
    }
    X(X const &other) : x(other.x) {
        std::cout << ++i << ". " << "X(X const &) : x(" << x << ')' << std::endl;
    }
    ~X() {
        std::cout << ++i << ". " << "~X(" << x << ')' << std::endl;
    }
};
struct A {
    int a;

    A(int a_) : a(a_) {
        std::cout << ++i << ". " << "A(" << a << ')' << std::endl;
    }
    A(A const &other) : a(other.a) {
        std::cout << ++i << ". " << "A(A const &) : a(" << a << ')' << std::endl;
    }
    ~A() {
        std::cout << ++i << ". " << "~A(" << a << ')' << std::endl;
    }
};
struct B : public A { // B 继承自 A
    X x; // B 包含一个 X 类型的成员

    B(int b) : A(1), x(b) { // B 的构造函数：首先调用 A 的构造函数 A(1)，然后调用 X 的构造函数 x(b)
        std::cout << ++i << ". " << "B(" << a << ", X(" << x.x << "))" << std::endl;
    }
    B(B const &other) : A(other.a), x(other.x) { // B 的拷贝构造函数：首先拷贝构造基类 A，然后拷贝构造成员 x
        std::cout << ++i << ". " << "B(B const &) : A(" << a << "), x(X(" << x.x << "))" << std::endl;
    }
    ~B() { // B 的析构函数：首先执行 B 自己的析构逻辑，然后自动调用成员 x 的析构函数，最后调用基类 A 的析构函数
        std::cout << ++i << ". " << "~B(" << a << ", X(" << x.x << "))" << std::endl;
    }
};

int main(int argc, char **argv) {
    X x = X(1); // 构造 X(1)
    A a = A(2); // 构造 A(2)
    B b = B(3); // 构造 B(3)，内部会构造 A(1) 和 X(3)

    // TODO: 补全三个类型的大小
    // 假设 int 为 4 字节。编译器通常会进行内存对齐，但对于只包含 int 的简单结构体，通常是其成员大小之和。
    static_assert(sizeof(X) == 4, "There is an int in X"); // X 包含一个 int
    static_assert(sizeof(A) == 4, "There is an int in A"); // A 包含一个 int
    static_assert(sizeof(B) == 8, "B is an A with an X"); // B 包含 A 的部分（int）和 X 的成员（另一个 int），总共 4 + 4 = 8 字节

    i = 0; // 重置计数器
    std::cout << std::endl
              << "-------------------------" << std::endl
              << std::endl;

    // 这是不可能的，A 无法提供 B 增加的成员变量的值
    // B ba = A(4); // 编译错误：不能将基类对象直接转换为派生类对象

    // 这也是不可能的，因为 A 是 B 的一部分，就好像不可以把套娃的外层放进内层里。
    A ab = B(5); // 然而这个代码可以编译和运行！ --> 这里发生了对象切片 (Object Slicing)
    // THINK: 观察打印出的信息，推测把大象放进冰箱分几步？
    // (见上面的详细分析)
    // 1. 构造临时 B(5) 对象：A(1) -> X(5) -> B(1, X(5))
    // 2. 拷贝构造 A ab：从临时 B 对象中，只复制其 A 部分，调用 A(A const &)
    // 3. 销毁临时 B(5) 对象：~B(1, X(5)) -> ~X(5) -> ~A(1)
    // 4. (main 函数结束时) 销毁 A ab 对象：~A(1)

    // THINK: 这样的代码是“安全”的吗？
    // (见上面的详细分析)
    // 不安全！会导致信息丢失（B 中 X 成员的数据被丢弃），并且可能破坏多态性，
    // 如果 B 管理资源，可能导致资源泄漏（X 的析构函数不会被 ab 调用）。
    // 在本例中，temp_B 的 X 会被正确析构，但 ab 自身没有 X 成员，所以 X 的析构不会因为 ab 的销毁而发生。
    // 如果 B 包含虚函数，那么通过 ab （A类型）调用的将是 A 版本的虚函数，而不是 B 版本的，即使原始对象是 B。

    i = 0; // 重置计数器，尽管这个i在main结束前不会再次打印，但保持习惯
    std::cout << std::endl
              << "-------------------------" << std::endl
              << std::endl;

    return 0; // 程序结束时，局部变量 x, a, b, ab 会被销毁，调用它们的析构函数
}

/*
预期控制台输出（可能因编译器和平台略有不同，但构造/析构顺序和切片行为一致）：

1. X(1)
2. A(2)
3. A(1)
4. X(3)
5. B(1, X(3))

-------------------------

6. A(1)            // B(5)的A部分构造
7. X(5)            // B(5)的X部分构造
8. B(1, X(5))      // B(5)构造体执行

9. A(A const &) : a(1) // 对象切片：ab 通过拷贝构造函数从临时B对象中复制A部分

10. ~B(1, X(5))    // 临时B(5)对象析构
11. ~X(5)        // 临时B(5)对象中X成员析构
12. ~A(1)        // 临时B(5)对象中A基类部分析构

-------------------------

// main函数结束时，局部变量的析构顺序与构造顺序相反 (ab, b, a, x)
13. ~A(1)        // ab 析构
14. ~B(1, X(3))    // b 析构
15. ~X(3)        // b 中 X 析构
16. ~A(1)        // b 中 A 析构
17. ~A(2)        // a 析构
18. ~X(1)        // x 析构

*/