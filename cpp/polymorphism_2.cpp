#include <iostream>
using namespace std;


class Base{

public:
    virtual void display() const{
        cout << "Base::display()" << endl;
    }
private:
    long _base;
};


class Derived
: public Base
{
public:
    //想要在派生类中定义虚函数覆盖基类的虚函数
    //很容易打错函数名字，同时又不会报错
    //没有完成有效的覆盖
    /* void dispaly() const{   //不会报错     */
    /* void dispaly() const override   //编译器会报错   */
    void display() const override
    {
        cout << "Derived::display()" << endl;
    }
private:
    long _derived;

};


