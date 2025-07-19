#include "../exercise.h"
#include <cstring> // 用于 std::memcpy
#include <cmath>   // 用于 std::abs，用于浮点数比较

// READ: 类模板 <https://zh.cppreference.com/w/cpp/language/class_template>

template<class T>
struct Tensor4D {
    unsigned int shape[4]; // 张量的形状，例如 [D0, D1, D2, D3]
    T *data;               // 指向张量数据的指针，数据以扁平化形式存储

    // 构造函数：动态设置张量形状并复制初始数据
    Tensor4D(unsigned int const shape_[4], T const *data_) {
        unsigned int size = 1; // 初始化总元素数量
        // TODO: 填入正确的 shape 并计算 size
        for (int i = 0; i < 4; ++i) {
            shape[i] = shape_[i]; // 复制传入的形状到类的成员
            size *= shape_[i];    // 计算所有维度长度的乘积，得到总元素数量
        }
        
        data = new T[size]; // 动态分配内存
        // 使用 std::memcpy 高效地复制数据
        std::memcpy(data, data_, size * sizeof(T));
    }

    // 析构函数：释放动态分配的内存
    ~Tensor4D() {
        delete[] data;
    }

    // 为了保持简单，禁止复制和移动（Rule of Three/Five/Zero）
    Tensor4D(Tensor4D const &) = delete;
    Tensor4D(Tensor4D &&) noexcept = delete;

    // 这个加法需要支持“单向广播”。
    // 具体来说，`others` 可以具有与 `this` 不同的形状，形状不同的维度长度必须为 1。
    // `others` 长度为 1 但 `this` 长度不为 1 的维度将发生广播计算。
    // 例如，`this` 形状为 `[1, 2, 3, 4]`，`others` 形状为 `[1, 2, 1, 4]`，
    // 则 `this` 与 `others` 相加时，3 个形状为 `[1, 2, 1, 4]` 的子张量各自与 `others` 对应项相加。
    Tensor4D &operator+=(Tensor4D const &others) {
        // TODO: 实现单向广播的加法

        // 1. 验证广播兼容性
        // 遍历所有维度，检查 others 的形状是否符合广播规则
        for (int d = 0; d < 4; ++d) {
            // 如果两个张量在某个维度上的长度不同，那么 others 在该维度上的长度必须为 1
            if (shape[d] != others.shape[d] && others.shape[d] != 1) {
                // 如果不符合规则，断言失败（在实际应用中，会抛出异常）
                ASSERT(false, "Broadcasting dimensions mismatch: others' dimension must be 1 if different.");
            }
        }

        // 2. 计算 this 张量的总元素数量
        unsigned int this_size = 1;
        for (int d = 0; d < 4; ++d) {
            this_size *= shape[d];
        }

        // 3. 计算 others 张量每个维度的步长（用于将 4D 坐标转换为扁平索引）
        unsigned int others_strides[4];
        others_strides[3] = 1; // 最内层维度（维度3）的步长为 1
        for (int d = 2; d >= 0; --d) {
            others_strides[d] = others_strides[d + 1] * others.shape[d + 1];
        }

        // 4. 遍历 this 张量的每一个元素
        for (unsigned int i = 0; i < this_size; ++i) {
            // 将 this 的扁平索引 i 转换为 4D 坐标 (n0, n1, n2, n3)
            unsigned int current_coords[4];
            unsigned int temp_idx = i;
            // 从最内层维度（维度3）开始计算坐标
            for (int d = 3; d >= 0; --d) {
                current_coords[d] = temp_idx % shape[d];
                temp_idx /= shape[d];
            }

            // 根据广播规则，计算 others 张量对应的 4D 坐标 (m0, m1, m2, m3)
            unsigned int others_coords[4];
            for (int d = 0; d < 4; ++d) {
                if (others.shape[d] == 1) {
                    // 如果 others 的该维度为 1，则无论 this 在该维度上的索引是多少，都取 others 的第 0 个元素
                    others_coords[d] = 0; 
                } else {
                    // 否则，others 的该维度与 this 相同，直接使用 this 的对应索引
                    others_coords[d] = current_coords[d]; 
                }
            }

            // 将 others 的 4D 坐标 (m0, m1, m2, m3) 转换回扁平索引 others_flat_idx
            unsigned int others_flat_idx = 0;
            for (int d = 0; d < 4; ++d) {
                others_flat_idx += others_coords[d] * others_strides[d];
            }
            
            // 执行加法操作：this 的当前元素 += others 的对应（广播后）元素
            data[i] += others.data[others_flat_idx];
        }

        return *this; // 返回当前对象的引用，支持链式操作
    }
};

// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    {
        unsigned int shape[]{1, 2, 3, 4};
        // clang-format off
        int data[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        // 显式指定模板类型参数 <int>
        auto t0 = Tensor4D<int>(shape, data); 
        auto t1 = Tensor4D<int>(shape, data); 
        t0 += t1;
        for (auto i = 0u; i < sizeof(data) / sizeof(*data); ++i) {
            ASSERT(t0.data[i] == data[i] * 2, "Tensor doubled by plus its self.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        float d0[]{
            1, 1, 1, 1,
            2, 2, 2, 2,
            3, 3, 3, 3,

            4, 4, 4, 4,
            5, 5, 5, 5,
            6, 6, 6, 6};
        // clang-format on
        unsigned int s1[]{1, 2, 3, 1}; // others 的最后一个维度为 1，将发生广播
        // clang-format off
        float d1[]{ // 对应 others 的形状 [1, 2, 3, 1]
            6, // 对应 [0,0,0,0]
            5, // 对应 [0,0,1,0]
            4, // 对应 [0,0,2,0]

            3, // 对应 [0,1,0,0]
            2, // 对应 [0,1,1,0]
            1}; // 对应 [0,1,2,0]
        // clang-format on

        auto t0 = Tensor4D<float>(s0, d0); // 显式指定模板类型参数 <float>
        auto t1 = Tensor4D<float>(s1, d1); // 显式指定模板类型参数 <float>
        t0 += t1;
        // 浮点数比较需要使用容忍度（epsilon）
        const float EPSILON_F = 1e-6f; 
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            // 预期结果：t0 的每个元素加上 t1 中对应（广播后）的元素，例如：
            // d0[0] (1) + d1[0] (6) = 7
            // d0[1] (1) + d1[0] (6) = 7 (d1[0] 广播到 d0 的第 0 维的第 0 个元素，第 0 个子张量)
            // d0[4] (2) + d1[1] (5) = 7 (d1[1] 广播到 d0 的第 0 维的第 1 个元素，第 0 个子张量)
            ASSERT(std::abs(t0.data[i] - 7.f) < EPSILON_F, "Every element of t0 should be 7 after adding t1 to it.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        double d0[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        unsigned int s1[]{1, 1, 1, 1}; // others 的所有维度都为 1
        double d1[]{1}; // 只有一个元素

        auto t0 = Tensor4D<double>(s0, d0); // 显式指定模板类型参数 <double>
        auto t1 = Tensor4D<double>(s1, d1); // 显式指定模板类型参数 <double>
        t0 += t1;
        const double EPSILON_D = 1e-9; // 浮点数比较容忍度
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            // 预期结果：t0 的每个元素都加上 t1 中唯一的元素 1
            ASSERT(std::abs(t0.data[i] - (d0[i] + 1)) < EPSILON_D, "Every element of t0 should be incremented by 1 after adding t1 to it.");
        }
    }
}
