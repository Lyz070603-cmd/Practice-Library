#include <iostream>
using std::cout;
using std::endl;
class Myclass{
public:

    Myclass(int x)
    :_x(x){

    }

    void print(){
        cout << _x << endl;

    }

    ~Myclass(){

    }

private:
    int _x;
};

int main()
{
    Myclass my(1);
    my.print();
    return 0;
}

