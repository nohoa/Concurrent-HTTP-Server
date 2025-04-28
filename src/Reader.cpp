#include "Reader.hpp"
#include<iostream>
#include <string>

Request HTTP_Reader :: parse(char msg[1024], int size){
    Request request_reader ;
    std::map<std::string,std::string> mp;
    int id = 0 ;
    for(int i = 0; i < size ;i ++){
        std::cout << msg[i] ;
    }
    std::string request_command ;
    while(id < size && msg[id] != ' '){
        request_command += msg[id];
        id ++;
    }
    request_reader.request_name = std::move(request_command);
    std::string value ;
    id ++;
    while(id < size && msg[id] != ' '){
        value += msg[id];
        id ++;
    }
    request_reader.file_directory = std::move(value);
    while(id < size && msg[id] != '\n') id ++;
    id++;
    std::string key ;
    while(id < size){
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
        if(key == "Content-length"){
            request_reader.content_len = std::stoi(value);
        }
        else if(key == "Accept-encoding"){
            if(value == "gzip") request_reader.accept_encoding = true;
        }

        mp[key] = value;
    }
    return request_reader;
}