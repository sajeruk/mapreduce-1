#include <iostream>
#include <map>
#include <algorithm>
#include <sstream>

int main(int argc, char** argv) {

    // std::cerr << "Map params:";
    // for(int i = 0; i < argc; ++i){
    //     std::cerr << " " << argv[i];
    // }
    // std::cerr << std::endl;

    std::string str;
    std::stringstream sstr;
    while(std::cin >> str){
        sstr.clear();
        sstr.str(str);
        while(sstr >> str){
            std::cout << str << "\t" << 1 << std::endl;
        }
    }
}