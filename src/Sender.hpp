#ifndef Sender_hpp
#define Sender_hpp

#include<iostream>
#include<string>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>


class Sender {
    public : 
       virtual void send_client(int conn_fd, std :: string& msg); 

};


#endif 