#include <iostream>
#include <map>
#include <string>

using namespace std;

struct Cache{
    map<string,string> table;
};

void write_cache(Cache* cache,string path,string& buf){
    cache->table[path]=buf;
}

void read_cache(Cache* cache,string path,string& buf){
    auto itr = cache->table.find(path);
    if(itr!=cache->table.end()){
        buf = cache->table[path];
    }else{
        buf =  path + " is not found.";
    }
    
}

void print_cache(Cache* cache){
    for(auto itr = cache->table.begin(); itr != cache->table.end(); itr++) {
        cout << "path = " << itr->first << ", val = " << itr->second << "\n";
    }
}

void write_file(Cache* cache){
    string path,buf;
    cout << "input path : ";
    cin >> path ;
    cout << "input contents : ";
    cin >> buf;
    write_cache(cache,path,buf);
}

void read_file(Cache* cache){
    string path,buf;
    cout << "input path : ";
    cin >> path;
    read_cache(cache,path,buf);
    cout << buf << endl;
}

int main(){
    Cache* cache = new Cache;
    string command,path,buf;
    while(true){
        cout << "input command (write,read,exit) : ";
        cin >> command;
        if(command == "write"){
            write_file(cache);
        }else if(command == "read"){
            read_file(cache);
        }else if(command == "exit"){
            break;
        }
    }
    print_cache(cache);
    delete cache;
    return 0;
}