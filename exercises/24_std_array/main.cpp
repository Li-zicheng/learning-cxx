#include "../exercise.h"
#include <array>   // For std::array
#include <cstring> // For std::memcmp and std::strcmp

// READ: std::array <https://zh.cppreference.com/w/cpp/container/array>

// TODO: 将下列 `?` 替换为正确的代码
int main(int argc, char **argv) {
    {
        std::array<int, 5> arr{{1, 2, 3, 4, 5}};
        // std::array::size() 返回数组中元素的数量
        ASSERT(arr.size() == 5, "Fill in the correct value.");
        // sizeof(std::array) 返回整个 array 对象占用的字节数，包括其内部存储的所有元素
        // 这里是 5 个 int，每个 int 通常是 4 字节，所以 5 * 4 = 20 字节
        ASSERT(sizeof(arr) == 20, "Fill in the correct value.");
        int ans[]{1, 2, 3, 4, 5};
        // std::array::data() 返回指向底层数组的指针
        // std::memcmp 比较两块内存区域的内容，需要指定比较的字节数
        // 比较的字节数是元素数量 * 每个元素的大小
        ASSERT(std::memcmp(arr.data(), ans, arr.size() * sizeof(int)) == 0, "Fill in the correct values.");
    }
    {
        std::array<double, 8> arr; // 声明一个包含 8 个 double 元素的 std::array，未初始化
        // std::array::size() 返回数组中元素的数量
        ASSERT(arr.size() == 8, "Fill in the correct value.");
        // sizeof(std::array) 返回整个 array 对象占用的字节数
        // 这里是 8 个 double，每个 double 通常是 8 字节，所以 8 * 8 = 64 字节
        ASSERT(sizeof(arr) == 64, "Fill in the correct value.");
    }
    {
        // std::array 可以用字符串字面量初始化 char 数组，会自动包含空终止符
        // "Hello, InfiniTensor!" 包含 20 个字符 + 1 个空终止符 = 21 个字符
        std::array<char, 21> arr{"Hello, InfiniTensor!"}; 
        // std::array::size() 返回数组中元素的数量
        ASSERT(arr.size() == 21, "Fill in the correct value.");
        // sizeof(std::array) 返回整个 array 对象占用的字节数
        // 这里是 21 个 char，每个 char 是 1 字节，所以 21 * 1 = 21 字节
        ASSERT(sizeof(arr) == 21, "Fill in the correct value.");
        // std::strcmp 比较两个 C 风格字符串，直到遇到空终止符
        // std::array::data() 返回指向底层数组的指针，对于 char 数组，这可以被视为 C 风格字符串
        ASSERT(std::strcmp(arr.data(), "Hello, InfiniTensor!") == 0, "Fill in the correct value.");
    }
    return 0;
}