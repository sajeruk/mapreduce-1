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
	std::string value;
	int val;
	int result = 0;
	std::stringstream sstr;
	for(; std::getline(std::cin, str);){
		int div = str.find('\t');
		key = str.substr(0, div);
		value = str.substr(div + 1, str.length() -  div - 1);
		sstr.clear();
		sstr.str(value);
		sstr >> val;
		result += val;
	}
	std::cout << key << "\t" << result << std::endl;
}