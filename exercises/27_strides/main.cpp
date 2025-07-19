#include "../exercise.h"
#include <vector> // 包含 std::vector

// 张量即多维数组。连续存储张量即逻辑结构与存储结构一致的张量。
// 通常来说，形状为 [d0, d1, ..., dn] 的张量，第 n 维是 dn 个连续的元素，第 n-1 维是 dn-1 个连续的 dn 个元素，以此类推。
// 张量的步长或跨度指的是张量每个维度上坐标 +1 时，数据指针跨过的范围。
// 因此，一个连续张量，其第 n 维的步长为 1，第 n-1 维的步长为 dn，第 n-2 维的步长为 dn*dn-1，以此类推。
// 例如，一个 2x3x4 张量，其步长为 [12, 4, 1]。

// READ: 类型别名 <https://zh.cppreference.com/w/cpp/language/type_alias>
using udim = unsigned int; // 定义 udim 为 unsigned int 的别名

/// @brief 计算连续存储张量的步长
/// @param shape 张量的形状
/// @return 张量每维度的访问步长
std::vector<udim> strides(std::vector<udim> const &shape) {
    // 初始化一个与形状相同大小的 vector 来存储步长
    std::vector<udim> strides_vec(shape.size());

    // 如果形状为空，则返回空的步长 vector
    if (shape.empty()) {
        return strides_vec;
    }

    // 最内层维度（即最后一个维度）的步长总是 1
    // 对应 shape[N-1] 和 strides_vec[N-1]
    strides_vec[shape.size() - 1] = 1;

    // 从倒数第二个维度开始，向前（向外层维度）计算步长
    // 步长计算公式：当前维度的步长 = 其右侧（更内层）维度的步长 * 其右侧维度的长度
    // 例如：strides[i] = strides[i+1] * shape[i+1]
    for (int i = static_cast<int>(shape.size()) - 2; i >= 0; --i) {
        strides_vec[i] = strides_vec[i + 1] * shape[i + 1];
    }

    return strides_vec;
}

// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    // 使用 ASSERT 宏来验证 strides 函数的正确性
    // 注意：std::vector 的比较运算符 (==) 会逐元素比较
    ASSERT((strides({2, 3, 4}) == std::vector<udim>{12, 4, 1}), "Make this assertion pass.");
    ASSERT((strides({3, 4, 5}) == std::vector<udim>{20, 5, 1}), "Make this assertion pass.");
    ASSERT((strides({1, 3, 224, 224}) == std::vector<udim>{150528, 50176, 224, 1}), "Make this assertion pass.");
    ASSERT((strides({7, 1, 1, 1, 5}) == std::vector<udim>{5, 5, 5, 5, 1}), "Make this assertion pass.");
    return 0;
}
