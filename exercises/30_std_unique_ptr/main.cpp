#include "../exercise.h"
#include <cstring>   // For std::strcmp
#include <memory>    // For std::unique_ptr, std::make_unique
#include <string>    // For std::string
#include <vector>    // For std::vector
#include <iostream>  // For std::cout (调试输出)

// READ: `std::unique_ptr` <https://zh.cppreference.com/w/cpp/memory/unique_ptr>

std::vector<std::string> RECORDS; // 全局记录析构顺序和记录字符串

class Resource {
    std::string _records; // 记录对该资源对象的操作

public:
    void record(char record_char) {
        _records.push_back(record_char);
        // std::cout << "  Resource " << this << " records: " << _records << std::endl; // 调试输出
    }

    // 析构函数：当 Resource 对象被销毁时，将其内部的 _records 字符串推入全局 RECORDS 向量
    ~Resource() {
        RECORDS.push_back(_records);
        // std::cout << "Resource " << this << " destroyed. Pushed '" << _records << "' to RECORDS." << std::endl; // 调试输出
    }
};

using Unique = std::unique_ptr<Resource>; // 定义 Unique 为 Resource 的唯一指针别名

// 重置函数：如果 ptr 非空，记录 'r'，然后返回一个新的 Resource 对象
Unique reset(Unique ptr) { // ptr 是按值传递，所有权转移到这里
    if (ptr) ptr->record('r'); 
    return std::make_unique<Resource>(); // 返回一个新的 Resource 对象的唯一指针
    // ptr (传入参数) 在这里生命周期结束，其指向的对象会被销毁（如果它拥有对象）
}

// 丢弃函数：如果 ptr 非空，记录 'd'，然后返回 nullptr（即销毁对象）
Unique drop(Unique ptr) { // ptr 是按值传递，所有权转移到这里
    if (ptr) ptr->record('d'); 
    return nullptr; // 返回空指针，导致传入的 ptr 指向的对象被销毁
    // ptr (传入参数) 在这里生命周期结束，其指向的对象会被销毁
}

// 转发函数：如果 ptr 非空，记录 'f'，然后返回 ptr（转移所有权）
Unique forward(Unique ptr) { // ptr 是按值传递，所有权转移到这里
    if (ptr) ptr->record('f'); 
    return ptr; // 返回 ptr，所有权被移动
    // ptr (传入参数) 在这里生命周期结束，但其所有权已转移给返回值，所以不会销毁对象
}

int main(int argc, char **argv) {
    std::vector<std::string> problems[3]; // 用于存储每个问题的记录结果

    // 问题 0
    drop(forward(reset(nullptr)));
    problems[0] = std::move(RECORDS); 

    // 问题 1
    forward(drop(reset(forward(forward(reset(nullptr))))));
    problems[1] = std::move(RECORDS); 

    // 问题 2
    drop(drop(reset(drop(reset(reset(nullptr))))));
    problems[2] = std::move(RECORDS); 

    // ---- 不要修改以上代码 ----

    // TODO: 分析 problems[1] 和 problems[2] 中资源的生命周期，将记录填入 `std::vector`
    // NOTICE: 此题结果依赖对象析构逻辑，平台相关，提交时以 CI 实际运行平台为准
    std::vector<const char *> answers[]{
        {"fd"}, // problems[0] 的答案，与之前一致
        {"d", "ffr"}, // problems[1] 的答案，根据你的调试输出修正
        {"d", "d", "r"}, // problems[2] 的答案，根据你的调试输出修正
    };

    // ---- 不要修改以下代码 ----

    // 验证结果
    for (auto i = 0; i < 3; ++i) {
        // 修正警告：将 i 和 j 的类型改为 size_t 或 unsigned int
        ASSERT(problems[i].size() == answers[i].size(), "wrong size");
        for (size_t j = 0; j < problems[i].size(); ++j) { // 修正循环变量类型
            ASSERT(std::strcmp(problems[i][j].c_str(), answers[i][j]) == 0, "wrong location");
        }
    }

    return 0;
}
