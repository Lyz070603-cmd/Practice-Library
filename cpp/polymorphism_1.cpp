#include <iostream>
using namespace std;


class Base{

public:
    Base(long x)
    : _base(x)
    {}

    void display() const{
        cout << "Base::display()" << endl;
    }
private:
    long _base;
};


class Derived
: public Base
{
public:
    Derived(long base,long derived)
    : Base(base)//创建基类子对象
    , _derived(derived)
    {}

    void display() const{
        cout << "Derived::display()" << endl;
    }
private:
    long _derived;

};

void print(Base * pbase){
    pbase->display();
    pbase->show();
}

void test0(){
    Base base(10);
    Derived dd(1,2);

    print(&base);
    cout << endl;
    //用一个基类指针指向派生类对象
    //能够操纵的只有基类部分
    print(&dd);
    
    cout << "sizeof(Base):" << sizeof(Base) << endl;
    cout << "sizeof(Derived):" << sizeof(Derived) << endl;
}


