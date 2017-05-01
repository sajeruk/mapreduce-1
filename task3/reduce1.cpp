#include <iostream>
#include <map>
#include <algorithm>
#include <sstream>

int main(int argc, char** argv) {


	std::string str;
	std::string key;
	std::string value;
	int val;
	std::string result = "";
	for(; std::getline(std::cin, str);){
		int div = str.find('\t');
		key = str.substr(0, div);
		value = str.substr(div + 1, str.length() -  div - 1);
		if(result.length() > 0){
			result += "#";
		}
		result += value;
	}
	std::cout << key << "\t" << result << std::endl;
}