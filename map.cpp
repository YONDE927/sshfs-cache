#include <iostream>
#include <map>
#include <string>
using namespace std;

void write(map<string,string> *mp,const string path,string* val){
    (*mp)[path]=*val;
}

int main(){
    map<string,string> mp;
    string path1 = "abc";
    string val1 = "123";
    string path2 = "def";
    string val2 = "456";
    write(&mp,path1,&val1);
    write(&mp,path2,&val2);
    for(auto itr=mp.begin();itr != mp.end();itr++){
        cout << itr->first << " " << itr->second << endl;
    }
    return 0;
}