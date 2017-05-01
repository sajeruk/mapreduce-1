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
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

std::string getRandomString(int length){
    std::string ans;
    for(int i = 0; i < length; ++i){
        if(rand() & 1){
            ans += (rand() % 26 + 'a');
        }else{
            ans += (rand() % 26 + 'A');
        }
    }
    return ans;
}

std::string generateFileName() {
    std::string s;
    std::ifstream in;
    s = "tmp_" + getRandomString(7) + ".txt";
    in.open(s);
    while(in.good()){
        in.close();
        s = "tmp_" + getRandomString(7) + ".txt";
        in.open(s);
    }
    in.close();
    return s;
}

int main(int argc, char** argv) {
	srand(time(NULL));
	if(argc < 9){
		std::cout << "Too few arguments" << std::endl;
		return 0;
	}
	const char* mapreduce = argv[1];
	const char* map1 = argv[2];
	const char* map2 = argv[3];
	const char* reduce1 = argv[4];
	const char* reduce2 = argv[5];
	const char* file_urls = argv[6];
	const char* file_words = argv[7];
	const char* outputFile = argv[8];
	const char* tmpfile1 = generateFileName().c_str();
	const char* tmpfile2 = generateFileName().c_str();
	const char* tmpfile3 = generateFileName().c_str();
	int pid;
	pid = fork();
	if (pid == 0) {
	    if (execl(mapreduce, mapreduce, "map", map1, file_words, tmpfile1, (char*)NULL)) {
	        perror("execl");
	    }
	    exit(1);
	}
	pid = fork();
	if (pid == 0) {
	    if (execl(mapreduce, mapreduce, "map", map2, file_urls, tmpfile2, (char*)NULL)) {
	        perror("execl");
	    }
	    exit(1);
	}
	pid = fork();
	if (pid == 0) {
	    if (execl(mapreduce, mapreduce, "reduce", reduce1, tmpfile2, tmpfile3, (char*)NULL)) {
	        perror("execl");
	    }
	    exit(1);
	}
	std::ifstream fin(tmpfile1);
	std::ofstream fout(tmpfile3);
	std::string s;
	fout << std::endl;
	while(getline(fin, s)){
		fout << s << std::endl;
	}
	fin.close();
	fout.close();
	pid = fork();
	if (pid == 0) {
	    if (execl(mapreduce, mapreduce, "reduce", reduce2, tmpfile3, outputFile, (char*)NULL)) {
	        perror("execl");
	    }
	    exit(1);
	}
	remove(tmpfile1);
	remove(tmpfile2);
	remove(tmpfile3);
}