#include <string.h>
#include <iostream>

using std::cout;
using std::endl;

class String
{
public:
    //String s1;
    String()
    : _pstr(new char[5]() + 4)//1 '\0'   2-5 RefCount  int
    {
        cout << "String()" << endl;
        *(int *)(_pstr - 4) = 1;
    }

    //String s1("hello");
    String(const char *pstr)
    : _pstr(new char[strlen(pstr) + 5]() + 4)
    {
        cout << "String(const char *)" << endl;
        strcpy(_pstr, pstr);
        initRefCount();
    }

    //String s2(s1);
    String(const String &rhs)
    : _pstr(rhs._pstr)//浅拷贝
    {
        cout << "String(const String &)" << endl;
        increaseRefCount();
    }


    //String s3("world");
    //s3 = s1
    String &operator=(const String &rhs)
    {
        if(this != &rhs)//1、自复制
        {
            //2、释放左操作
            decreaseRefCount();
            if(0 == *(int *)(_pstr - 4))
            {
                delete [] (_pstr - 4);
            }

            //3、深拷贝(浅拷贝)
            _pstr = rhs._pstr;
            increaseRefCount();
        }
        return *this;//4、返回*this
    }
private:
    class CharProxy
    {
    public:
        CharProxy(String &self,size_t idx)
        : _self(self)
        , _idx(idx)
        {

        }

        char &operator=(const char &ch);//写操作
        
        //类型转换函数
        operator char()
        {
            cout << "operator char() " << endl;
            return  _self._pstr[_idx];
        }
        /* friend std::ostream &operator << (std::ostream &os,const CharProxy &rhs); */

    private:
        String &_self;//此时string是不完整类型
        size_t _idx;
    };

public:
    //下标访问运算符
    //s3[0] = "H"
    //cout <<"s1[0] = " << s1[0] << endl;
    CharProxy operator[](size_t idx)//string *this
    {
        return CharProxy(*this,idx); 
    }

    #if 0
    //下边访问运算符
    //s3[0] = 'H'
    //cout <<"s1[0] = " << s1[0] << endl;
    char &operator[](size_t idx)
    {
        if(idx < size())
        {
            if(getRefCount() > 1)//考虑共享问题
            {
                char *ptmp = new char[size() + 5]() + 4;
                strcpy(ptmp, _pstr);
                decreaseRefCount();

                _pstr = ptmp;
                initRefCount();
            }
            return _pstr[idx];
        }
        else
        {
            static char charnull = '\0';
            return charnull;
        }
    }
    #endif
    

    //s3
    ~String()
    {
        cout << "~String()" << endl;
        release();
    }

public:
    const char *c_str() const
    {
        return _pstr;
    }

    int getRefCount() const
    {
        return *(int *)(_pstr - 4);
    }

private:
    size_t size() const
    {
        return strlen(_pstr);
    }

    void initRefCount()
    {
        *(int *)(_pstr - 4) = 1;
    }

    void increaseRefCount()
    {
        ++*(int *)(_pstr - 4);
    }

    void decreaseRefCount()
    {
        --*(int *)(_pstr - 4);
    }

    void release()
    {
        decreaseRefCount();
        if(0 == getRefCount())
        {
            delete [] (_pstr - 4);
        }
    }


    friend std::ostream &operator<<(std::ostream &os, const String &rhs);
    /* friend std::ostream &operator<<(std::ostream &os, const String::CharProxy &rhs); */
private:
    char *_pstr;
};

std::ostream &operator<<(std::ostream &os, const String &rhs)
{
    if(rhs._pstr)
    {
        os << rhs._pstr;
    }

    return os;
}

//CharProxy里面的函数
//s3[0] = 'H'
char &String::CharProxy::operator=(const char &ch)
{
    if(_idx < _self.size())
    {
        if(_self.getRefCount() > 1)
        {
            char *ptemp = new char[_self.size() + 5]() + 4;
            strcpy(ptemp,_self._pstr);
            _self.decreaseRefCount();
            
            _self._pstr = ptemp;
            _self.initRefCount();
        }
        _self._pstr[_idx] = ch;//真正的赋值操作

        return _self._pstr[_idx];
    }
    else
    {
        static char charnull = '\0';
        return charnull;
    }
}

#if 0
//双友元的设置
std::ostream &operator<<(std::ostream &os,const String::CharProxy &rhs)
{
    os << rhs._self._pstr[rhs._idx];
    return os;
}
#endif

void test()
{
    String s1("hello");
    String s2(s1);
    cout << "s1 = " << s1 << endl;
    cout << "s2 = " << s2 << endl;
    cout << "s1.getRefCount() = " << s1.getRefCount() << endl;
    cout << "s2.getRefCount() = " << s2.getRefCount() << endl;
    printf("s1'address : %p\n", s1.c_str());
    printf("s2'address : %p\n", s2.c_str());

    cout << endl;
    String s3("world");
    cout << "s3 = " << s3 << endl;
    cout << "s3.getRefCount() = " << s3.getRefCount() << endl;
    printf("s3'address : %p\n", s3.c_str());

    cout << endl << "使用s3 = s1进行赋值操作"  << endl;
    s3 = s1;
    cout << "s1 = " << s1 << endl;
    cout << "s2 = " << s2 << endl;
    cout << "s3 = " << s3 << endl;
    cout << "s1.getRefCount() = " << s1.getRefCount() << endl;
    cout << "s2.getRefCount() = " << s2.getRefCount() << endl;
    cout << "s3.getRefCount() = " << s3.getRefCount() << endl;
    printf("s1'address : %p\n", s1.c_str());
    printf("s2'address : %p\n", s2.c_str());
    printf("s3'address : %p\n", s3.c_str());

    cout << endl << "对s3[0]执行写操作" << endl;
    s3[0] = 'H';//'h' = 'H' char = char//CharProxy = char//"hello"
    cout << "s1 = " << s1 << endl;
    cout << "s2 = " << s2 << endl;
    cout << "s3 = " << s3 << endl;
    cout << "s1.getRefCount() = " << s1.getRefCount() << endl;
    cout << "s2.getRefCount() = " << s2.getRefCount() << endl;
    cout << "s3.getRefCount() = " << s3.getRefCount() << endl;
    printf("s1'address : %p\n", s1.c_str());
    printf("s2'address : %p\n", s2.c_str());
    printf("s3'address : %p\n", s3.c_str());

    cout << endl << "对s1[0]执行读操作" << endl;
    cout << "s1[0] = " << s1[0] << endl;//cout << CharProxy//CharProxy -> char;
    cout << "s1 = " << s1 << endl;
    cout << "s2 = " << s2 << endl;
    cout << "s3 = " << s3 << endl;
    cout << "s1.getRefCount() = " << s1.getRefCount() << endl;
    cout << "s2.getRefCount() = " << s2.getRefCount() << endl;
    cout << "s3.getRefCount() = " << s3.getRefCount() << endl;
    printf("s1'address : %p\n", s1.c_str());
    printf("s2'address : %p\n", s2.c_str());
    printf("s3'address : %p\n", s3.c_str());

}
int main(int argc, char **argv)
{
    test();
    return 0;
}

