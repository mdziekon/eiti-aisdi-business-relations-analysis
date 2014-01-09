
#include <string>


int main(){
    std::string str = "/user/desktop/abc/post/";
    bool exists = str.find("/dupa") != std::string::npos;

    return exists;
}
