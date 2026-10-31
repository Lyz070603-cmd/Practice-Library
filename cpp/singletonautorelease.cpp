#include <iostream>
#include <string>
using namespace std;
//第一种，通过另一对象的生命周期
class Singleton_1;

class Auto_1{
public:
    Auto_1(Singleton_1 *p)
    :_p(p){
        cout << "Auto_1" << endl;
    }

    ~Auto_1(){
        if (_p != nullptr){
            delete _p;
            _p = nullptr;
        }
        cout << "~Auto_1" << endl;
    }

private:
    Singleton_1 *_p;
};

class Singleton_1{
public:
    static Singleton_1 * getInstance(){
        if (_pInstance == nullptr){
            _pInstance = new Singleton_1();
            static Auto_1 ar(_pInstance);
        }       

        return _pInstance;
    }

    void print(){
        cout << "hello" << endl;
    }

private:
    Singleton_1(){cout << "Singleton_1" << endl;}
    ~Singleton_1(){cout << "~Singleton_1" << endl;}

    static Singleton_1 *_pInstance;
};
Singleton_1 * Singleton_1::_pInstance = nullptr;




void test1(){
    Singleton_1::getInstance()->print();
}

//方式二 利用嵌套类加静态对象
class Singleton
{
public:
    static Singleton *getInstance()
    {
        if(nullptr == _pInstance)
        {
            //使用构造函数
            _pInstance = new Singleton();
        }
        return _pInstance;
    }

private:
    class AutoRelese
    {
    public:
        AutoRelese()
        {
            cout << "AutoRelese()" << endl;
        }

        ~AutoRelese()
        {
            cout << "~AutoRelese()" << endl;
            if(_pInstance)
            {
                delete _pInstance;
                _pInstance = nullptr;
            }
        }
    };
private:
    Singleton()
    {
        cout << "Singleton()" << endl;
    }

    ~Singleton()
    {
        cout << " ~Singleton()" << endl;
    }

private:
    static Singleton *_pInstance;
    static AutoRelese _ar;//对象数据成员

};

Singleton *Singleton::_pInstance = nullptr;
Singleton::AutoRelese Singleton::_ar;

void test2(){
    Singleton *ps1 = Singleton::getInstance();
    
}
int main()
{
    test1();
    cout << "--------------------" << endl;
    test2();
    return 0;
}

