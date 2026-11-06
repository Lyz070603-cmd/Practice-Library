#include <iostream>
using namespace std;

class FunctionObject{
 public:
     //第一个括号表示运算符（函数调用运算符）
     //第二个括号表示参数列表（无参形式）
     int operator()(){
         cout << "int operator()()" << endl;
         ++_cnt;
         return 1;
     }
 
     //可以定义多种函数调用运算符重载函数
     void operator()(int x){
         cout << "void operator(int)" << endl;
         ++_cnt;
         cout << x << endl;
     }
 
     int _cnt = 0;
 };
 
 void test0(){
     FunctionObject fo; 
     
     cout << fo() << endl;  //让对象像一个函数一样被调用
     cout << fo.operator()() << endl; //本质
 
     fo(5);
     fo.operator()(5);//本质
 }
