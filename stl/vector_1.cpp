#include <iostream>
#include <vector>
using namespace std;


int main()
{   
    vector<int> v1({1,2,3,4,5,6,7,8,9,10});
    for (auto &it : v1){
        cout << it << " ";
    }
    cout << endl;
    return 0;
}

