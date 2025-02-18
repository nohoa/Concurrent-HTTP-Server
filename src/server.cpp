#include <cstddef>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <thread>
#include <sstream>
#include<pthread.h>
#include<zlib.h>
void send_client(int conn_fd, std :: string msg){
    char message[msg.length()] ;
           for(int i = 0 ;i < msg.length() ;i ++){
              message[i] = msg[i];
          } 
          //std :: cout << message[0] << std :: endl;
        send(conn_fd,message,strlen(message),0) ;
}
std::string gzip_compress(const std::string &data) {
  std :: cout << "data is : " <<  data << std :: endl; 
  z_stream zs;
  memset(&zs, 0, sizeof(zs));
  if (deflateInit2(&zs, Z_BEST_COMPRESSION, Z_DEFLATED, 15 + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK) { 
      throw std::runtime_error("deflateInit2 failed while compressing.");
  }
  zs.next_in = (Bytef *)data.data();
  zs.avail_in = data.size();
  int ret;
  char outbuffer[32768]; 
  std::string outstring;  
  do {
      zs.next_out = reinterpret_cast<Bytef *>(outbuffer); // Set output buffer
      zs.avail_out = sizeof(outbuffer);                  // Set available space
      ret = deflate(&zs, Z_FINISH);  // Compress the data
      if (outstring.size() < zs.total_out) {
          // Append newly compressed data to the output string
          outstring.append(outbuffer, zs.total_out - outstring.size());
      }
  } while (ret == Z_OK);
  deflateEnd(&zs);  // Free zlib's internal memory
  if (ret != Z_STREAM_END) {
      throw std::runtime_error("Exception during zlib compression: (" + std::to_string(ret) + ") " + zs.msg);
  }
  return outstring;
  }
int  handle_connect(int conn_fd , struct sockaddr_in client_addr,int client_addr_len, std ::string argu) { 
    std::string client_message(65536, '\0');
    char msg[65536] = {};
  if (recvfrom(conn_fd, msg, sizeof(msg)-1, 0, (struct sockaddr *) &client_addr, (socklen_t *) &client_addr_len) == SO_ERROR){
    std::cerr << "listen failed\n";
    return 1;
  }
  bool compress_header = false;
  for(int i = 0 ;i < 300 ;i ++){
    if(msg[i] == 'g' && msg[i+1] =='z' ){
        compress_header = true ;
    }
  }
  std :: string all_value = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n" ;
  if(compress_header == true){
      all_value = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Encoding: gzip\r\n" ;
  }
  if(msg[0] == 'P'){ 
     std::string endpoint = "";
     std :: string len = "";
     std :: string data = "";
     int content_len = 0 ;
     //146-151
      int curr_id = 12 ;
      while(msg[curr_id] != ' ') {
        endpoint += msg[curr_id];
        curr_id ++ ;
      }
      int content_id = 0 ;
      for(int i = 0 ;i < 105 ;i ++){
        if(msg[i] =='L' && msg[i+1] =='e' && msg[i+2] =='n') {
          content_id = i+8;
          break;
        }
      }
      while(msg[content_id] != '\r'){
        len += msg[content_id];
        content_id ++;
      }
      content_len = std :: stoi(len);
      int data_id = content_id ;
      while(msg[data_id] != ':') data_id ++;
      while(msg[data_id] != '\n') data_id ++;
      while(msg[data_id] == '\r' || msg[data_id] == '\n' ) data_id ++;
      int start = 0 ;
      while(start < content_len){
        start ++ ;
        data += msg[data_id];
        data_id ++;
      }
     std :: string end_dir = argu + endpoint;
     std:: ofstream ofs (end_dir);
     if(ofs.good()){
       ofs << data ;
       ofs.close() ;
     }
      std :: string message_value = "HTTP/1.1 201 Created\r\n\r\n"; 
      send_client(conn_fd, message_value);
    return 0;
  }
  if(msg[5] != ' ') {
    std::string endpoint = "";
      int curr_id = 5 ;
      while(msg[curr_id] != ' ') {
        endpoint += msg[curr_id];
        curr_id ++ ;
      }
      //std :: cout << endpoint << std :: endl;
      if(endpoint.length() < 5 || endpoint.substr(0,4).compare("echo") != 0 )  {
      if(endpoint.compare("user-agent") == 0) {
            int curr_id = 60;
            std :: string agent = "";
            while(msg[curr_id] != '\r'){
                agent += msg[curr_id];
                curr_id ++ ;
            }
            std :: string message_value = all_value;
            message_value += "Content-Length:" + std ::to_string(agent.length()) + "\r\n\r\n" + agent ; 
            send_client(conn_fd, message_value);
      }
      else if(endpoint.substr(0,5).compare("files") == 0){
        std::string fileName = endpoint.substr(6);
        std:: ifstream ifs (argu+fileName);
        if(ifs.good()){
          std::stringstream cnt;
          cnt  << ifs.rdbuf();
          std :: string message_value =  "HTTP/1.1 200 OK\r\nContent-Type: application/octet-stream\r\nContent-Length: " + std::to_string(cnt.str().length()) + "\r\n\r\n" + cnt.str() + "\r\n";
          if(compress_header == true ) {
            message_value = "HTTP/1.1 200 OK\r\nContent-Type: application/octet-stream\r\nContent-Encoding: gzip\r\nContent-Length: " + std::to_string(cnt.str().length()) + "\r\n\r\n" + cnt.str() + "\r\n";
          }
          //std :: cout << val << std :: endl; 
          send_client(conn_fd, message_value);
        }
        else {
          char res[] = "HTTP/1.1 404 Not Found\r\n\r\n";
          send(conn_fd,res,strlen(res),0) ;
        }
      }
      else {
      char res[] = "HTTP/1.1 404 Not Found\r\n\r\n";
      send(conn_fd,res,strlen(res),0) ;
        }
      }
      else {
        std :: string message_value = all_value;
          std :: cout << endpoint.substr(5) << std :: endl;
        std::string compressed_body = gzip_compress(endpoint.substr(5));
          //std :: cout << endpoint.substr(6) << std :: endl;
          std :: cout << "comp is: " << compressed_body << std :: endl;
          //message_value = all_value;
           message_value = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n" "Content-Length: " + std::to_string(endpoint.substr(5).length()) + "\r\n\r\n" + endpoint.substr(5);
          if(compress_header == true )
          message_value = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Encoding: gzip\r\n" "Content-Length: " + std::to_string(compressed_body.size()) + "\r\n\r\n" + compressed_body;
        send(conn_fd, message_value.c_str(), message_value.length(), 0);
      }
  }
  else { 
     char res[] = "HTTP/1.1 200 OK\r\n\r\n";
     send(conn_fd,res,strlen(res),0) ;
  }
  return 0;
}
int main(int argc, char **argv) {
  // Flush after every std::cout / std::cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  
  // You can use print statements as follows for debugging, they'll be visible when running tests.
  std::cout << "Logs from your program will appear here!\n";
  // Uncomment this block to pass the first stage
  //
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
   std::cerr << "Failed to create server socket\n";
   return 1;
  }
  
  // Since the tester restarts your program quite often, setting SO_REUSEADDR
  // ensures that we don't run into 'Address already in use' errors
  int reuse = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
    std::cerr << "setsockopt failed\n";
    return 1;
  }
  
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(4221);
  
  if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0) {
    std::cerr << "Failed to bind to port 4221\n";
    return 1;
  }
  
  int connection_backlog = 5;
  if (listen(server_fd, connection_backlog) != 0) {
    std::cerr << "listen failed\n";
    return 1;
  }
  
  struct sockaddr_in client_addr;
  int client_addr_len = sizeof(client_addr);
  
  std::cout << "Waiting for a client to connect...\n";
  
   std::string dir;
  if (argc == 3 && strcmp(argv[1], "--directory") == 0)
  {
  	dir = argv[2];
  }
  std :: cout << dir << std :: endl; 
// int conn_fd = accept(server_fd, (struct sockaddr *) &client_addr, (socklen_t *) &client_addr_len);
// std::cout << "Client connected\n";
//  int status = handle_connect(conn_fd, client_addr, client_addr_len, dir) ;
  while(true){
  int conn_fd = accept(server_fd, (struct sockaddr *) &client_addr, (socklen_t *) &client_addr_len);
  std::cout << "Client connected\n";
  std::thread th(handle_connect,conn_fd,client_addr,client_addr_len,dir);
  th.detach();
  }
  close(server_fd);
  return 0;
}