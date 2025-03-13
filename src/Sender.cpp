#include "Sender.hpp"

void Sender :: send_client(int conn_fd, std :: string & msg){
            char message[msg.length()] ;
           for(int i = 0 ;i < msg.length() ;i ++){
              message[i] = msg[i];
          } 
            send(conn_fd,message,strlen(message),0) ;
    }