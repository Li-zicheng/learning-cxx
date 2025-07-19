#include "../exercise.h"
#include <cstring>   // For std::memcmp, std::strcmp
#include <vector>    // For std::vector
#include <iostream>  // For std::cout (if needed for debugging, though not strictly required by ASSERTs)

// READ: std::vector <https://zh.cppreference.com/w/cpp/container/vector>

// TODO: 将下列 `?` 替换为正确的代码
int main(int argc, char **argv) {
    {
        std::vector<int> vec{1, 2, 3, 4, 5};
        // vec.size() 返回 vector 中当前元素的数量
        ASSERT(vec.size() == 5, "Fill in the correct value.");
        // THINK: `std::vector` 的大小是什么意思？与什么有关？
        // 答：`sizeof(std::vector)` 返回的是 `std::vector` 对象本身在内存中占用的字节数，
        //    它通常是一个固定值，与存储的元素数量无关，因为它只包含管理底层动态数组的指针、大小和容量成员。
        //    在 64 位系统上，通常是 3 个指针大小（或 1 个指针 + 2 个 size_t），即 24 字节。
        ASSERT(sizeof(vec) == 24, "Fill in the correct value."); // 假设 64 位系统，通常是 3 * 8 字节
        int ans[]{1, 2, 3, 4, 5};
        // vec.data() 返回指向底层数组的指针
        // std::memcmp 比较两块内存区域的内容
        ASSERT(std::memcmp(vec.data(), ans, sizeof(ans)) == 0, "Fill in the correct values.");
    }
    {
        std::vector<double> vec{1, 2, 3, 4, 5};
        {
            ASSERT(vec.size() == 5, "Fill in the correct value.");
            // `sizeof(vec)` 同样是 `std::vector` 对象本身的大小，与元素类型无关
            ASSERT(sizeof(vec) == 24, "Fill in the correct value."); // 假设 64 位系统
            double ans[]{1, 2, 3, 4, 5};
            ASSERT(std::memcmp(vec.data(), ans, sizeof(ans)) == 0, "Fill in the correct values.");
        }
        {
            vec.push_back(6); // 添加一个元素
            ASSERT(vec.size() == 6, "Fill in the correct value.");
            // `sizeof(vec)` 不变，因为它只反映 `std::vector` 对象本身的大小，不包括其管理的动态内存
            ASSERT(sizeof(vec) == 24, "Fill in the correct value."); // 假设 64 位系统
            vec.pop_back(); // 移除最后一个元素
            ASSERT(vec.size() == 5, "Fill in the correct value.");
            // `sizeof(vec)` 依然不变
            ASSERT(sizeof(vec) == 24, "Fill in the correct value."); // 假设 64 位系统
        }
        {
            vec[4] = 6; // 修改索引 4 的元素
            ASSERT(vec[0] == 1, "Fill in the correct value.");
            ASSERT(vec[1] == 2, "Fill in the correct value.");
            ASSERT(vec[2] == 3, "Fill in the correct value.");
            ASSERT(vec[3] == 4, "Fill in the correct value.");
            ASSERT(vec[4] == 6, "Fill in the correct value."); // 已经被修改为 6
        }
        {
            // THINK: `std::vector` 插入删除的时间复杂度是什么？
            // 答：
            //   - 在末尾插入/删除 (push_back/pop_back)：通常是 O(1) 均摊时间复杂度。
            //     当容量不足时，会重新分配内存并复制元素，此时是 O(N)。
            //   - 在中间插入/删除 (insert/erase)：O(N) 时间复杂度，因为需要移动插入点之后的所有元素。

            // vec = {1, 2, 3, 4, 6}
            // vec.begin() + 1 指向索引 1 的位置（值为 2）
            vec.insert(vec.begin() + 1, 1.5); // 在索引 1 处插入 1.5
            // vec 现在是 {1, 1.5, 2, 3, 4, 6}
            ASSERT((vec == std::vector<double>{1, 1.5, 2, 3, 4, 6}), "Make this assertion pass.");
            // vec.begin() + 3 指向索引 3 的位置（值为 3）
            vec.erase(vec.begin() + 3); // 移除索引 3 的元素（值为 3）
            // vec 现在是 {1, 1.5, 2, 4, 6}
            ASSERT((vec == std::vector<double>{1, 1.5, 2, 4, 6}), "Make this assertion pass.");
        }
        {
            vec.shrink_to_fit(); // 尝试减少容量以匹配当前大小
            ASSERT(vec.capacity() == 5, "Fill in the correct value."); // size 是 5，capacity 应该缩减到 5
            vec.clear(); // 清空所有元素，size 变为 0，但通常不改变 capacity
            ASSERT(vec.empty(), "`vec` is empty now.");
            ASSERT(vec.size() == 0, "Fill in the correct value.");
            // clear() 通常不释放内存，所以 capacity 保持不变（除非之前 capacity 已经很小）
            // 在这里，capacity 仍然是 5，因为它在 shrink_to_fit 后已经匹配了 size。
            // 不同的编译器和标准库实现可能会有差异，但通常 clear 不会改变 capacity。
            ASSERT(vec.capacity() == 5, "Fill in the correct value."); 
        }
    }
    {
        // TODO: 调用正确的构造函数
        // std::vector<char> vec(count, value); 构造一个包含 count 个 value 的 vector
        std::vector<char> vec(48, 'z'); 
        ASSERT(vec[0] == 'z', "Make this assertion pass.");
        ASSERT(vec[47] == 'z', "Make this assertion pass."); // 0 到 47 共 48 个元素
        ASSERT(vec.size() == 48, "Make this assertion pass.");
        // sizeof(vec) 同样是 `std::vector` 对象本身的大小
        ASSERT(sizeof(vec) == 24, "Fill in the correct value."); // 假设 64 位系统
        {
            auto capacity = vec.capacity(); // 记录当前容量
            vec.resize(16); // 改变 size 到 16，如果 size 变小，多余元素被销毁；如果 size 变大，新元素默认构造
            ASSERT(vec.size() == 16, "Fill in the correct value.");
            // resize 改变 size，但不一定会改变 capacity。如果新的 size 小于当前 capacity，capacity 不变。
            ASSERT(vec.capacity() == capacity, "Fill in a correct identifier."); // capacity 保持不变
        }
        {
            vec.reserve(256); // 预留至少 256 个元素的容量，如果当前容量不足，会重新分配
            ASSERT(vec.size() == 16, "Fill in the correct value."); // reserve 不改变 size
            // reserve 会确保 capacity 至少为 256。如果原来容量小于 256，则会增大到 256 或更大。
            // 如果原来容量已经大于等于 256，则 capacity 不变。
            ASSERT(vec.capacity() == 256, "Fill in the correct value."); 
        }
        {
            vec.push_back('a'); // size: 16 -> 17
            vec.push_back('b'); // size: 17 -> 18
            vec.push_back('c'); // size: 18 -> 19
            vec.push_back('d'); // size: 19 -> 20
            ASSERT(vec.size() == 20, "Fill in the correct value.");
            // capacity 在 reserve(256) 后是 256，push_back 不会改变它直到超过 256
            ASSERT(vec.capacity() == 256, "Fill in the correct value.");
            // resize(16) 后，vec[0]到vec[15]是'z'
            ASSERT(vec[15] == 'z', "Fill in the correct value."); // 原始的第 16 个元素 (索引 15) 仍然是 'z'
            // 新添加的元素从原 size 之后开始
            ASSERT(vec[16] == 'a', "Fill in the correct value.");
            ASSERT(vec[17] == 'b', "Fill in the correct value.");
            ASSERT(vec[18] == 'c', "Fill in the correct value.");
            ASSERT(vec[19] == 'd', "Fill in the correct value.");
        }
    }

    return 0;
}
