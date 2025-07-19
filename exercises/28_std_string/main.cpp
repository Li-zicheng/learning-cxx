#include "../exercise.h"
#include <string>    // 包含 std::string
#include <type_traits> // 包含 std::is_same_v

// READ: 字符串 <https://zh.cppreference.com/w/cpp/string/basic_string>

int main(int argc, char **argv) {
    // READ: 字符串字面量 <https://zh.cppreference.com/w/cpp/string/basic_string/operator%22%22s>
    using namespace std::string_literals; // 引入 "s" 后缀字面量操作符
    auto hello = "Hello"s; // "Hello"s 创建一个 std::string 对象
    auto world = "world";  // "world" 是一个 C 风格字符串字面量 (const char*)

    // READ: `decltype` 表达式 <https://zh.cppreference.com/w/cpp/language/decltype>
    // READ: `std::is_same_v` 元编程判别 <https://zh.cppreference.com/w/cpp/types/is_same>

    // `hello` 是通过 "s" 后缀创建的 std::string 对象
    ASSERT((std::is_same_v<decltype(hello), std::string>), "Fill in the missing type.");
    // `world` 是一个普通的 C 风格字符串字面量
    ASSERT((std::is_same_v<decltype(world), const char*>), "Fill in the missing type.");

    // TODO: 将 `?` 替换为正确的字符串
    // 字符串拼接：std::string + C风格字符串 + C风格字符串 + char
    ASSERT(hello + ", " + world + '!' == "Hello, world!", "Fill in the missing string.");
    return 0;
}
