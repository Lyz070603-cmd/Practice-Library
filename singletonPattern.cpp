#include <iostream>
#include <string>
#include <string.h>
using std::string;
using std::cout;
using std::endl;
using std::cin;


class Student{
public:
    static Student *getStudent(const char * str,int id){
        if (_p == nullptr){
            _p = new Student(str,id);
        } 
        return _p;
    }

    void print(){
        cout << _strp << endl;
        cout << _id << endl;

    }


private:
    Student(const char * str,int id)
    :_strp(new char[strlen(str) + 1])
    ,_id(id){
        _strp = str;    
    }  
    static  Student * _p;
    string _strp;
    int _id;
};
Student * Student::_p = nullptr;




int main()
{
    Student *s = Student::getStudent("zhangsan",34);
    s->print();
    return 0;
}

