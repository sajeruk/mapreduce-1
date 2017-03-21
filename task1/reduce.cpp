#include <iostream>
#include <map>
#include <algorithm>
#include <sstream>

int main(int argc, char** argv) {

    // std::cerr << "Reduce params:";
    // for(int i = 0; i < argc; ++i){
    //     std::cerr << " " << argv[i];
    // }
    // std::cerr << std::endl;

    std::string str;
    std::string key;
    int value;
    int result = 0;
    std::stringstream sstr;
    for(; std::getline(std::cin, str);){
        sstr.clear();
        sstr.str(str);
        sstr >> key;
        sstr >> value;
        result += value;
    }
    std::cout << key << "\t" << result << std::endl;
}