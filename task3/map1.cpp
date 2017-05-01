#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>
#include <map>
#include <algorithm>
#include <vector>
#include <unistd.h>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <queue>
#include <sys/wait.h>
#include <ctime>

int main() {
	std::string str, key, value;
	int i = 0;
	while(std::getline(std::cin, str)){
		i++;
		int div = str.find('\t');
		key = str.substr(0, div);
		value = str.substr(div + 1, str.length() -  div - 1);
		std::cout << key << "\t" << "#" << std::endl;
	}
}