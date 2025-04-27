#include "Reader.hpp"
#include<iostream>

std::map<std::string,std::string>  HTTP_Reader :: parse(char msg[1024], int size){
    std::map<std::string,std::string> mp;
    int id = 0 ;
    std::string key ;
    while(id < size && msg[id] != ' '){
        key += msg[id];
        id ++;
    }
    std::string value ;
    id ++;
    while(id < size && msg[id] != '\r'){
        value += msg[id];
        id ++;
    }
    
    mp[key] = value ;
    id ++;
    id++;
    while(id < size){
        key.clear();
        value.clear();
        while(id < size && msg[id] != ':'){
            key += msg[id];
            id ++;
        }
        id ++;
        id ++;
        while(id < size && msg[id] != '\r'){
            value += msg[id];
            id ++;
        }
        if(key.empty() || value.empty()) break;
        mp[key] = value;
    }
    for(auto it : mp) std::cout << it.first <<" " << it.second << std::endl;
    return mp;
}