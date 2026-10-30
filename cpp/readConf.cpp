#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

int readConfig(const char * filename){
    ifstream ifs(filename);
    if (!ifs.good()){
        cout << "open file fail" << endl;
        return -1;
    }

    string key;
    string value;
    string line;
    while(getline(ifs,line)){
        istringstream  iss(line);
        
        iss >> key >> value;
        cout << key << "-------->" << value << endl;
        
    }
    return 0;
}
int main()
{
    readConfig("mysever.conf");
    return 0;
}

