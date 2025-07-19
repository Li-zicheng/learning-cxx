#include "../exercise.h"
#include <algorithm> // For std::transform
#include <string>    // For std::string and std::to_string
#include <vector>    // For std::vector
#include <iterator>  // For std::back_inserter

// READ: `std::transform` <https://zh.cppreference.com/w/cpp/algorithm/transform>
// READ: `std::vector::begin` <https://zh.cppreference.com/w/cpp/container/vector/begin>

int main(int argc, char **argv) {
    std::vector<int> val{8, 13, 21, 34, 55};
    
    // TODO: 调用 `std::transform`，将 `v` 中的每个元素乘以 2，并转换为字符串，存入 `ans`
    std::vector<std::string> ans; // 声明目标向量

    // 使用 std::transform 算法
    // val.begin() 和 val.end() 定义了输入范围
    // std::back_inserter(ans) 是一个输出迭代器，它会在 ans 的末尾插入元素
    // [](int x){ return std::to_string(x * 2); } 是一个 lambda 表达式，作为单目操作函数
    // 它接收 val 中的每个 int 元素 x，将其乘以 2，然后转换为 std::string 返回
    std::transform(val.begin(), val.end(), 
                   std::back_inserter(ans), 
                   [](int x){ return std::to_string(x * 2); });

    ASSERT(ans.size() == val.size(), "ans size should be equal to val size");
    ASSERT(ans[0] == "16", "ans[0] should be 16");
    ASSERT(ans[1] == "26", "ans[1] should be 26");
    ASSERT(ans[2] == "42", "ans[2] should be 42");
    ASSERT(ans[3] == "68", "ans[3] should be 68");
    ASSERT(ans[4] == "110", "ans[4] should be 110");
    return 0;
}
