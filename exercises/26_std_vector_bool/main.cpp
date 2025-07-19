#include "../exercise.h"
#include <vector>
#include <iostream>
#include <cmath> // <--- 添加这一行

enum class DataType {
    Float,
    Double,
};

struct TaggedUnion {
    DataType type;
    union {
        float f;
        double d;
    };
};

template <typename T>
T sigmoid(T x) {
    return 1 / (1 + std::exp(-x));
}

TaggedUnion sigmoid_dyn(TaggedUnion x) {
    TaggedUnion ans{x.type};
    if (x.type == DataType::Float) {
        ans.f = sigmoid(x.f);
    } else if (x.type == DataType::Double) {
        ans.d = sigmoid(x.d);
    }
    return ans;
}

int main(int argc, char **argv) {
    std::vector<bool> vec(100, true); 
    
    ASSERT(vec[0], "Make this assertion pass.");
    ASSERT(vec[99], "Make this assertion pass.");
    ASSERT(vec.size() == 100, "Make this assertion pass.");

    std::cout << "sizeof(std::vector<bool>) = " << sizeof(std::vector<bool>) << std::endl;
    ASSERT(sizeof(vec) == 40, "Fill in the correct value."); 

    {
        vec[20] = false;
        ASSERT(!vec[20], "Fill in `vec[20]` or `!vec[20]`.");
    }
    {
        vec.push_back(false);
        ASSERT(vec.size() == 101, "Fill in the correct value.");
        ASSERT(!vec[100], "Fill in `vec[100]` or `!vec[100]`.");
    }
    {
        auto ref = vec[30];
        ASSERT(ref, "Fill in `ref` or `!ref`");
        ref = false;
        ASSERT(!ref, "Fill in `ref` or `!ref`");
        ASSERT(!vec[30], "Fill in `vec[30]` or `!vec[30]`.");
    }
    return 0;
}