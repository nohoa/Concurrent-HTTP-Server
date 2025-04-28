#include<cstring>
#include<string>
#include<map>
#include "Request.cpp"

class HTTP_Reader{
    public :
    HTTP_Reader(){};
    ~HTTP_Reader(){};
    Request parse(char msg[1024],int size);
} ;