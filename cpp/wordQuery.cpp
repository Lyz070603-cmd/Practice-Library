#include <iostream>
#include<map>
#include<cstring>
#include<fstream>
#include<sstream>

//测量程序执行时间
#include<chrono>

using namespace std;

class Dictionary
{
public:
    void read(const string &filename)
    {
        ifstream ifs(filename);
        if(!ifs)
        {
            cerr << "ifs open file fail" << endl;
            return;
        }
        string line;
        while(getline(ifs,line))
        {
            istringstream iss(line);
            string word;
            while(iss >> word)
            {
                //处理单词
                string newWord = dealWord(word);

                //将单词的信息插入到map
                if(newWord != string())
                {
                    //一步到位，无过失已经存在的单词，会将frequency+1
                    //如果是新的单词，会插入一个新的pair
                    //string内容为新单词，int 为0，再加为1
                    ++_map[newWord];
                }
            }
        }
        ifs.close();
    }

    string dealWord(string & word)
    {
        for(size_t idx = 0;idx != word.size(); ++idx)
        {
            if(!isalpha(word[idx]))
            {
                //返回空字符串
                return string();
            }
            else if(isupper(word[idx]))
            {
                //大写转小写
                word[idx] = tolower(word[idx]);
            }
        }
        return word;
    }

    void store(const string & filename)
    {
        ofstream ofs(filename);
        if(!ofs)
        {
            cerr << "ofs open file fail" << endl;
            return;
        }

        map<string,int>::iterator it = _map.begin();
        for(;it != _map.end();++it)
        {
            ofs << it->first << " " << it->second << endl;
        }

        ofs.close();
    }
private:
    map<string,int> _map;

};

void test0()
{
    Dictionary dict;
    dict.read("./The_Holy_Bible.txt");
    dict.store("dictMap.dat");
}

int main(int argc, char * argv[]){
    //获取程序开始时间点
    auto start = chrono::high_resolution_clock::now();
    test0();

    //获取程序结束时间点
    auto end = chrono::high_resolution_clock::now();
    //计算时间差
    auto duration = std::chrono::duration_cast<chrono::milliseconds>(end - start);

    //输出执行时间
    cout << "程序执行时间：" << duration.count() << " 毫秒" << endl;
    return 0;
}

