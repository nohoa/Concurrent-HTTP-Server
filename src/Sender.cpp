#include "Sender.hpp"

void Sender :: send_client(int conn_fd, std :: string & msg){
           send(conn_fd,msg.c_str(),msg.length(),0) ;
    }


  