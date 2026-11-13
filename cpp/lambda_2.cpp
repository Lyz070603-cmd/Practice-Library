#include <iostream>

void test()
{
    int num = 10;
    int age = 100;
    string name("wangdao");
    [](string value)
    {
        /* cout << "num = " << num << endl; */
        /* cout << "name = " << name<< endl; */
        cout << "gNum = " << gNum << endl;
        cout << "value = " << value << endl;
    }("lili");
    //值捕获
    [num, name](string value)
    {
        cout << "num = " << num << endl;
        cout << "name = " << name<< endl;
        cout << "value = " << value << endl;
    }("lili");
    //引用捕获
    cout << endl;
    [&num, &name](string value)
    {
        num = 100;
        name = "nice";
        cout << "num = " << num << endl;
        cout << "name = " << name<< endl;
        cout << "value = " << value << endl;
    }("wangdao");
    cout << "num = " << num
        << ", name = " << name << endl;
    cout << endl << endl;
    //全部用值捕获，name用引用捕获
    [=, &name]()
    {
        /* num = 1000;//error */
        name = "welcome";
        cout << "num = " << num << endl;
        cout << "age = " << age << endl;
        cout << "name = " << name << endl;
    }();
    cout << endl << endl;
    //全部引用捕获，name值捕获
    [&, name]()
    {
        num = 1000;//ok
        age = 100;//ok
        /* name = "welcome";//error */
        cout << "num = " << num << endl;
        cout << "age = " << age << endl;
        cout << "name = " << name << endl;
    }();
}
