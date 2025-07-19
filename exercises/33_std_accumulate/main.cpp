#include "../exercise.h"
#include <numeric>   // 包含 std::accumulate
#include <vector>    // 包含 std::vector，虽然这里是C风格数组，但通常会用到
#include <functional> // 包含 std::multiplies，如果使用它作为二元操作
#include <iterator>   // 包含 std::begin 和 std::end，用于C风格数组的迭代器

// READ: `std::accumulate` <https://zh.cppreference.com/w/cpp/algorithm/accumulate>

int main(int argc, char **argv) {
    using DataType = float; // 定义数据类型为 float
    int shape[]{1, 3, 224, 224}; // 定义张量的形状

    // TODO: 调用 `std::accumulate` 计算：
    //       - 数据类型为 float；
    //       - 形状为 shape；
    //       - 连续存储；
    //       的张量占用的字节数
    // 计算总元素数量：使用 std::accumulate 将 shape 数组中的所有维度长度相乘。
    // 初始值为 1，因为是乘积。
    int total_elements = std::accumulate(std::begin(shape), std::end(shape), 1, 
                                         [](int acc, int dim){ return acc * dim; });
    
    // 计算总字节数：总元素数量乘以每个数据类型占用的字节数。
    int size = total_elements * sizeof(DataType);

    ASSERT(size == 602112, "4x1x3x224x224 = 602112");
    return 0;
}
