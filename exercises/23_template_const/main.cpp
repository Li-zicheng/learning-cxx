#include "../exercise.h"
#include <cstring> // For std::memset

// READ: 模板非类型实参 <https://zh.cppreference.com/w/cpp/language/template_parameters#%E6%A8%A1%E6%9D%BF%E9%9D%9E%E7%B1%BB%E5%9E%8B%E5%AE%9E%E5%8F%82>

// N 是一个非类型模板参数，表示张量的维度数量
template<unsigned int N, class T>
struct Tensor {
    unsigned int shape[N]; // 张量的形状，例如 [D0, D1, ..., DN-1]
    T *data;               // 指向张量数据的指针，数据以扁平化形式存储

    // 构造函数：根据给定的形状分配内存并初始化为零
    Tensor(unsigned int const shape_[N]) {
        unsigned int size = 1; // 初始化总元素数量
        // TODO: 填入正确的 shape 并计算 size
        for (unsigned int i = 0; i < N; ++i) {
            shape[i] = shape_[i]; // 复制传入的形状到类的成员
            size *= shape_[i];    // 计算所有维度长度的乘积，得到总元素数量
        }
        
        data = new T[size]; // 动态分配内存
        // 使用 std::memset 将所有数据初始化为零。
        // 注意：std::memset 只能用于字节级别的初始化，对于非 POD 类型或 T 不是 0 字节表示的类型，
        // 这种初始化方式是不安全的。但对于 int, float, double 等基本类型，初始化为 0 是安全的。
        std::memset(data, 0, size * sizeof(T));
    }

    // 析构函数：释放动态分配的内存
    ~Tensor() {
        delete[] data;
    }

    // 为了保持简单，禁止复制和移动
    Tensor(Tensor const &) = delete;
    Tensor(Tensor &&) noexcept = delete;

    // 重载 [] 运算符，用于非 const 对象访问元素
    T &operator[](unsigned int const indices[N]) {
        return data[data_index(indices)];
    }
    // 重载 [] 运算符，用于 const 对象访问元素
    T const &operator[](unsigned int const indices[N]) const {
        return data[data_index(indices)];
    }

private:
    // 将多维索引转换为扁平化数组中的一维索引
    unsigned int data_index(unsigned int const indices[N]) const {
        unsigned int index = 0;
        // 遍历每个维度
        for (unsigned int i = 0; i < N; ++i) {
            // 确保索引在当前维度的有效范围内
            ASSERT(indices[i] < shape[i], "Invalid index");
            // TODO: 计算 index
            // 累积索引：当前维度索引乘以当前维度之后所有维度的乘积（即步长），然后累加
            // 更简洁的迭代方法：
            // index = index * current_dimension_size + current_index_in_dimension
            index = index * shape[i] + indices[i];
        }
        return index;
    }
};

// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    {
        unsigned int shape[]{2, 3, 4, 5}; // 2x3x4x5 的张量
        auto tensor = Tensor<4, int>(shape); // N=4, T=int

        unsigned int i0[]{0, 0, 0, 0}; // 第一个元素
        tensor[i0] = 1;
        ASSERT(tensor[i0] == 1, "tensor[i0] should be 1");
        ASSERT(tensor.data[0] == 1, "tensor[i0] should be 1");

        unsigned int i1[]{1, 2, 3, 4}; // 最后一个元素
        tensor[i1] = 2;
        ASSERT(tensor[i1] == 2, "tensor[i1] should be 2");
        // 计算 (1,2,3,4) 的扁平索引：
        // (1 * (3*4*5)) + (2 * (4*5)) + (3 * (5)) + (4 * (1))
        // = (1 * 60) + (2 * 20) + (3 * 5) + (4 * 1)
        // = 60 + 40 + 15 + 4 = 119
        ASSERT(tensor.data[119] == 2, "tensor[i1] should be 2"); 
    }
    {
        unsigned int shape[]{7, 8, 128}; // 7x8x128 的张量
        auto tensor = Tensor<3, float>(shape); // N=3, T=float

        unsigned int i0[]{0, 0, 0};
        tensor[i0] = 1.f;
        ASSERT(tensor[i0] == 1.f, "tensor[i0] should be 1");
        ASSERT(tensor.data[0] == 1.f, "tensor[i0] should be 1");

        unsigned int i1[]{3, 4, 99};
        tensor[i1] = 2.f;
        ASSERT(tensor[i1] == 2.f, "tensor[i1] should be 2");
        // 计算 (3,4,99) 的扁平索引：
        // (3 * (8*128)) + (4 * (128)) + (99 * (1))
        // = (3 * 1024) + (4 * 128) + 99
        // = 3072 + 512 + 99 = 3683
        ASSERT(tensor.data[3683] == 2.f, "tensor[i1] should be 2");
    }
    return 0;
}
