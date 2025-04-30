#include "Reader.hpp"
#include<iostream>
#include <string>
#include <utility>

Request HTTP_Reader :: parse(char msg[1024], int size){
    Request request_reader ;
    std::map<std::string,std::string> mp;
    int id = 0 ;
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
        if(value.empty()){
            request_reader.content = std::move(key);
        }
        if(key.empty() || value.empty()) break;
        if(key.find("Content-Length") != std::string::npos ){
            request_reader.content_len = std::stoi(value);
        }
        else if(key.find("Accept-Encoding") != std::string::npos ){
            if(value.find("gzip") != std::string::npos) request_reader.accept_encoding = true;
            else request_reader.accept_encoding = false;
        }
        else if(key.find("Connection") != std::string::npos){
            request_reader.close_connection = true;
        }
        else if(key.find("User-Agent") != std::string::npos){
            request_reader.user_agent = value ;
        }

        mp[key] = value;
    }
    return request_reader;
}