#include "../exercise.h" // 假设包含 ASSERT 宏和 std::cout

// READ: 静态字段 <https://zh.cppreference.com/w/cpp/language/static>
// READ: 虚析构函数 <https://zh.cppreference.com/w/cpp/language/destructor>




struct A {
    static int num_a; // 声明静态字段

    A() {
        ++num_a;
    }
    // 修正：将基类的析构函数声明为虚函数，以确保多态删除时调用正确的析构函数链
    virtual ~A() { 
        --num_a;
    }

    virtual char name() const {
        return 'A';
    }
};

struct B final : public A { // final 关键字表示 B 不能再被继承
    static int num_b; // 声明静态字段

    B() {
        ++num_b;
    }
    // B 的析构函数会被正确调用，然后自动调用 A 的析构函数
    ~B() override { // 可以加上 override 明确这是覆盖基类的虚析构函数
        --num_b;
    }

    // name() 函数被 B 覆盖，并标记为 final，表示 C 及 D 不能再覆盖它
    char name() const final { 
        return 'B';
    }
};


// 在类外部定义并初始化静态成员变量

int A::num_a = 0; 
int B::num_b = 0;

int main(int argc, char **argv) {
    auto a = new A; // 构造 A 对象
    auto b = new B; // 构造 B 对象（会先构造 A 部分）

    // 此时状态：
    // num_a: A 对象构造 (+1) + B 对象基类 A 部分构造 (+1) = 2
    // num_b: B 对象构造 (+1) = 1
    ASSERT(A::num_a == 2, "Fill in the correct value for A::num_a");
    ASSERT(B::num_b == 1, "Fill in the correct value for B::num_b");
    ASSERT(a->name() == 'A', "Fill in the correct value for a->name()");
    ASSERT(b->name() == 'B', "Fill in the correct value for b->name()");

    delete a; // 析构 A 对象
    // 此时状态：
    // num_a: 2 - 1 = 1
    delete b; // 析构 B 对象 (会先调用 B::~B()，再调用 A::~A() 因为 A::~A() 是虚函数)
    // 此时状态：
    // num_b: 1 - 1 = 0
    // num_a: 1 - 1 = 0
    ASSERT(A::num_a == 0, "Every A was destroyed");
    ASSERT(B::num_b == 0, "Every B was destroyed");

    A *ab = new B; // 再次构造 B 对象，通过 A* 指针持有
    // 此时状态：
    // num_a: 0 + 1 = 1
    // num_b: 0 + 1 = 1
    ASSERT(A::num_a == 1, "Fill in the correct value for A::num_a");
    ASSERT(B::num_b == 1, "Fill in the correct value for B::num_b");
    ASSERT(ab->name() == 'B', "Fill in the correct value for ab->name()"); // 虚函数调用，根据实际类型 B

    // TODO: 基类指针无法随意转换为派生类指针，补全正确的转换语句
    // 使用 dynamic_cast 进行安全的向下转型，因为 A 是一个多态基类
    B &bb = dynamic_cast<B&>(*ab); 
    ASSERT(bb.name() == 'B', "Fill in the correct value for bb->name()"); // 直接通过 B& 引用调用

    // TODO: ---- 以下代码不要修改，通过改正类定义解决编译问题 ----
    // delete ab; 将正确调用 B 的析构函数，因为 A 的析构函数被声明为 virtual
    delete ab; 
    // 此时状态：
    // num_b: 1 - 1 = 0
    // num_a: 1 - 1 = 0
    ASSERT(A::num_a == 0, "Every A was destroyed");
    ASSERT(B::num_b == 0, "Every B was destroyed");

    return 0;
}