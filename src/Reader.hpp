#include<cstring>
#include<string>
#include<map>

class HTTP_Reader{
    public :
    HTTP_Reader(){};
    ~HTTP_Reader(){};
    std::map<std::string,std::string> parse(char msg[1024],int size);
} ;