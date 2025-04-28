#include<iostream>
#include<string>

struct Request {
    public:
    std::string request_name;
    std::string file_directory ;
    std::string user_agent ;
    int content_len ;
    bool accept_encoding ;
};