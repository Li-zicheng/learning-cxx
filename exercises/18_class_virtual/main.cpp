#include "../exercise.h"

// READ: 虚函数 <https://zh.cppreference.com/w/cpp/language/virtual>

struct A {
    virtual char virtual_name() const {
        return 'A';
    }
    char direct_name() const {
        return 'A';
    }
};
struct B : public A {
    // READ: override <https://zh.cppreference.com/w/cpp/language/override>
    char virtual_name() const override { // 覆盖 A 的虚函数
        return 'B';
    }
    char direct_name() const { // 隐藏 A 的非虚函数
        return 'B';
    }
};
struct C : public B {
    // READ: final <https://zh.cppreference.com/w/cpp/language/final>
    char virtual_name() const final { // 覆盖 B 的虚函数，并声明为最终版本，D 不能再覆盖
        return 'C';
    }
    char direct_name() const { // 隐藏 B 的非虚函数
        return 'C';
    }
};
struct D : public C {
    // 注意：D 不能覆盖 virtual_name()，因为它在 C 中被标记为 final。
    // 所以 D 会使用 C 的 virtual_name() 实现。
    char direct_name() const { // 隐藏 C 的非虚函数
        return 'D';
    }
};

int main(int argc, char **argv) {
    constexpr auto MSG = "Replace '?' with its correct name.";

    A a;
    B b;
    C c;
    D d;

    // 直接对象调用：编译时绑定，根据对象本身的类型确定调用哪个函数
    ASSERT(a.virtual_name() == 'A', MSG);
    ASSERT(b.virtual_name() == 'B', MSG);
    ASSERT(c.virtual_name() == 'C', MSG);
    ASSERT(d.virtual_name() == 'C', MSG); // D::virtual_name() 继承自 C 的 final 版本
    ASSERT(a.direct_name() == 'A', MSG);
    ASSERT(b.direct_name() == 'B', MSG);
    ASSERT(c.direct_name() == 'C', MSG);
    ASSERT(d.direct_name() == 'D', MSG);

    // 基类引用调用：
    // virtual_name() -> 动态绑定（运行时多态），看实际对象类型
    // direct_name()  -> 静态绑定（编译时多态），看引用声明类型

    A &rab = b; // A 类型引用指向 B 对象
    B &rbc = c; // B 类型引用指向 C 对象
    C &rcd = d; // C 类型引用指向 D 对象

    ASSERT(rab.virtual_name() == 'B', MSG); // rab (A&) 实际是 B 对象，调用 B 的虚函数
    ASSERT(rbc.virtual_name() == 'C', MSG); // rbc (B&) 实际是 C 对象，调用 C 的虚函数
    ASSERT(rcd.virtual_name() == 'C', MSG); // rcd (C&) 实际是 D 对象，但 D 继承 C 的 final 虚函数，调用 C 的虚函数
    ASSERT(rab.direct_name() == 'A', MSG);  // rab (A&)，调用 A 的非虚函数
    ASSERT(rbc.direct_name() == 'B', MSG);  // rbc (B&)，调用 B 的非虚函数
    ASSERT(rcd.direct_name() == 'C', MSG);  // rcd (C&)，调用 C 的非虚函数

    A &rac = c; // A 类型引用指向 C 对象
    B &rbd = d; // B 类型引用指向 D 对象

    ASSERT(rac.virtual_name() == 'C', MSG); // rac (A&) 实际是 C 对象，调用 C 的虚函数
    ASSERT(rbd.virtual_name() == 'C', MSG); // rbd (B&) 实际是 D 对象，但 D 继承 C 的 final 虚函数，调用 C 的虚函数
    ASSERT(rac.direct_name() == 'A', MSG);  // rac (A&)，调用 A 的非虚函数
    ASSERT(rbd.direct_name() == 'B', MSG);  // rbd (B&)，调用 B 的非虚函数

    A &rad = d; // A 类型引用指向 D 对象

    ASSERT(rad.virtual_name() == 'C', MSG); // rad (A&) 实际是 D 对象，但 D 继承 C 的 final 虚函数，调用 C 的虚函数
    ASSERT(rad.direct_name() == 'A', MSG);  // rad (A&)，调用 A 的非虚函数

    return 0;
}

// READ: 扩展阅读-纯虚、抽象 <https://zh.cppreference.com/w/cpp/language/abstract_class>
// READ: 扩展阅读-虚继承 <https://zh.cppreference.com/w/cpp/language/derived_class>