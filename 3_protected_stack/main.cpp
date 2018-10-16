#include <iostream>
#include <random>
#include <cassert>

#include "ProtectedStack.h"

#define ASSERT_EQ(x, y) assert((x) == (y))
#define ASSERT_TRUE(x) assert((x))
#define ASSERT_FALSE(x) assert(!(x))

void test_int()
{
    std::default_random_engine generator;
    generator.seed(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> rand_int(50, 150);

    int n = rand_int(generator);
    ProtectedStack<int> stack(n);

    for (int i = 0; i < n; ++i)
    {
        ASSERT_TRUE(stack.Push(i));
    }

    ASSERT_TRUE(stack.Ok());

    stack.Dump();

    for (int i = n - 1; i >= 0; --i)
    {
        int elem = 0;
        ASSERT_TRUE(stack.Pop(elem));
        ASSERT_EQ(elem, i);
    }

    ASSERT_TRUE(stack.IsEmpty());
    ASSERT_EQ(stack.Size(), 0);
    ASSERT_TRUE(stack.Ok());

    stack.Dump();
}

void crash()
{
    ProtectedStack<int> stack(5);
    auto* p = &stack;
    *((unsigned*)p + 10) = 0xAAAAAAAA;
    ASSERT_FALSE(stack.Ok());
    stack.Dump();
}

int main() {
    test_int();
    crash();
    return 0;
}