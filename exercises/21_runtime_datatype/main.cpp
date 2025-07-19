#include "../exercise.h"
#include <cmath> // For std::exp

enum class DataType {
    Float,
    Double,
};

/// @brief Tagged union 即标签化联合体，是联合体的一种常见应用。
///        Rust enum 在实现上就是标签化联合体。
struct TaggedUnion {
    DataType type;
    // NOTICE: struct/union 可以相互任意嵌套。
    union {
        float f;
        double d;
    };
};

// TODO: 将这个函数模板化用于 sigmoid_dyn
// 使用模板参数 T，使函数可以接受不同类型的数值（float 或 double）
template <typename T>
T sigmoid(T x) {
    return 1 / (1 + std::exp(-x));
}

TaggedUnion sigmoid_dyn(TaggedUnion x) {
    TaggedUnion ans{x.type}; // 初始化返回结果的类型
    // TODO: 根据 type 调用 sigmoid
    // 根据输入 x 的类型，访问联合体中对应的成员，并调用模板化的 sigmoid 函数
    if (x.type == DataType::Float) {
        ans.f = sigmoid(x.f); // 如果是 float 类型，调用 sigmoid<float>
    } else if (x.type == DataType::Double) {
        ans.d = sigmoid(x.d); // 如果是 double 类型，调用 sigmoid<double>
    }
    // 可以在这里添加一个 ASSERT 或抛出异常来处理未知 DataType 的情况，但根据题目，只有 Float 和 Double
    return ans;
}

// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    // 测试 float 类型
    TaggedUnion xf{DataType::Float};
    xf.f = 5.f;
    auto yf = sigmoid_dyn(xf);
    ASSERT(yf.type == DataType::Float, "type mismatch");
    // 注意：浮点数比较通常需要容忍度，但这里的值可能被设计为精确可表示
    ASSERT(yf.f == 1 / (1 + std::exp(-5.f)), "sigmoid float");

    // 测试 double 类型
    TaggedUnion xd{DataType::Double};
    xd.d = 5.0;
    auto yd = sigmoid_dyn(xd);
    ASSERT(yd.type == DataType::Double, "type mismatch");
    // 同上，浮点数比较的注意事项
    ASSERT(yd.d == 1 / (1 + std::exp(-5.0)), "sigmoid double");
    return 0;
}