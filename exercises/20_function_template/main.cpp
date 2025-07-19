#include "../exercise.h"
#include <cmath> // For std::abs

// READ: 函数模板 <https://zh.cppreference.com/w/cpp/language/function_template>
// TODO: 将这个函数模板化
template <typename T> // 定义一个函数模板，T 是一个类型参数
T plus(T a, T b) {
    return a + b;
}

int main(int argc, char **argv) {
    ASSERT(plus(1, 2) == 3, "Plus two int");
    ASSERT(plus(1u, 2u) == 3u, "Plus two unsigned int");

    // THINK: 浮点数何时可以判断 ==？何时必须判断差值？
    // 答：只有当浮点数能被精确表示且计算结果也精确时，才可能判断 ==。
    //    几乎所有涉及浮点数运算的结果都可能存在精度误差，此时必须判断差值。

    ASSERT(plus(1.25f, 2.5f) == 3.75f, "Plus two float"); // 1.25f 和 2.5f 可以在二进制中精确表示，结果 3.75f 也可以
    ASSERT(plus(1.25, 2.5) == 3.75, "Plus two double");   // 同上

    // TODO: 修改判断条件使测试通过
    // 对于 0.1 和 0.2 这样的浮点数，在二进制中无法精确表示，因此 0.1 + 0.2 不会精确等于 0.3
    // 需要使用一个小的容忍度（epsilon）来比较它们的差值
    const double EPSILON = 1e-9; // 定义一个很小的容忍度，例如 10的-9次方

    // 使用 std::abs 来获取差值的绝对值，并判断是否小于 EPSILON
    ASSERT(std::abs(plus(0.1, 0.2) - 0.3) < EPSILON, "How to make this pass?");

    return 0;
}
