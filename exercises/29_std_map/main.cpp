#include "../exercise.h"
#include <map>     // 包含 std::map
#include <string>  // 包含 std::string，因为 main 函数使用了它
#include <iostream> // 包含 std::cout，虽然这里没直接用，但通常练习会用到

// READ: `std::map` <https://zh.cppreference.com/w/cpp/container/map>
// READ: `std::unordered_map` <https://zh.cppreference.com/w/cpp/container/unordered_map>

template<class k, class v>
bool key_exists(std::map<k, v> const &map, k const &key) {
    // TODO: 实现函数
    // 使用 count() 方法，如果键存在返回 1，否则返回 0
    return map.count(key) > 0;
    // 另一种实现方式：
    // return map.find(key) != map.end();
}

template<class k, class v>
void set(std::map<k, v> &map, k key, v value) {
    // TODO: 实现函数
    // 使用 operator[]，如果键不存在则插入，如果键存在则更新值
    map[key] = value;
    // 另一种实现方式 (C++17 及更高版本):
    // map.insert_or_assign(std::move(key), std::move(value));
    // 注意：如果 key 和 value 是右值引用，使用 std::move 可以提高效率。
    // 但这里参数是 k key, v value，意味着它们是按值传递的，所以直接赋值即可。
}

// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    using namespace std::string_literals; // 引入 "s" 后缀字面量

    std::map<std::string, std::string> secrets;

    set(secrets, "hello"s, "world"s);
    ASSERT(key_exists(secrets, "hello"s), "\"hello\" shoud be in `secrets`");
    ASSERT(!key_exists(secrets, "foo"s), "\"foo\" shoud not be in `secrets`");

    set(secrets, "foo"s, "bar"s);
    set(secrets, "Infini"s, "Tensor"s);
    ASSERT(secrets["hello"] == "world", "hello -> world");
    ASSERT(secrets["foo"] == "bar", "foo -> bar");
    ASSERT(secrets["Infini"] == "Tensor", "Infini -> Tensor");

    set(secrets, "hello"s, "developer"s); // 更新 "hello" 的值
    ASSERT(secrets["hello"] == "developer", "hello -> developer");

    return 0;
}
