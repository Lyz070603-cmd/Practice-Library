#include <string>
#include <stdio.h>


class Example
{
public:
    void print(int x, int y)
    {
        auto x1 = [] {return _number; }; // error
        auto x2 = [this] {return _number; }; // ok
        auto x3 = [this] {return _number + x + y; }; // error
        auto x4 = [this, x, y] {return _number + x + y; }; // ok
        auto x5 = [this] {return _number++; }; // ok
        auto x6 = [=] {return _number + x + y; }; // ok
        auto x7 = [&] {return _number + x + y; }; // ok
    }
    int _number = 100;
};
void test()
{
    int a = 10, b = 20;
    auto f1 = [] {return a; }; // error
    auto f2 = [&] {return a++; };
    auto f3 = [=] {return a; }; // ok
    auto f4 = [=] {return a++; }; // error
    auto f5 = [a] {return a + b; }; // error
    auto f6 = [a, &b] {return a + (b++); }; // ok
    auto f7 = [=, &b] {return a + (b++); }; // ok
}
