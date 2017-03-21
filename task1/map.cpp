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

	std::string str, key, value;
	std::stringstream sstr;
	while(std::getline(std::cin, str)){
		int div = str.find('\t');
		key = str.substr(0, div);
		value = str.substr(div + 1, str.length() -  div - 1);
		std::cout << value << "\t" << 1 << std::endl;
	}
}