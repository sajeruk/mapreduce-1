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


void error(const char *msg)
{
    perror(msg);
    exit(0);
}

std::string get_contents(std::string& url, int port = 80)
{
	if(url.length() >= 8 && url.substr(0, 8) == "https://"){
		url = url.substr(8, url.length() - 8);
	}
	if(url.length() >= 7 && url.substr(0, 7) == "http://"){
		url = url.substr(7, url.length() - 7);
	}
	int div = url.find('/');
	std::string host_string, page_string;
	if(div < 0){
		page_string = "/";
		host_string = url;
	}else{
		page_string = url.substr(div, url.length() - div);
		host_string = url.substr(0, div);
	}
	const char* host = host_string.c_str();
	const char* page = page_string.c_str();
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[50000];
    portno = port;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
    }
    server = gethostbyname(host);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        error("ERROR connecting");
    }
    // printf("Please enter the message: ");
    bzero(buffer,50000);
    strcpy(buffer, "GET ");
    strcat(buffer, page);
    strcat(buffer, " HTTP/1.0\r\nHost: ");
    strcat(buffer, host);
    strcat(buffer, "\r\nUser-Agent: main.cpp\r\n\r\n");
    // fgets(buffer,255,stdin);
    // strcpy(buffer,"GET " + page + " HTTP/1.0\r\nHost: " + host + "\r\nUser-Agent: main.cpp\r\n\r\n");
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) {
        error("ERROR writing to socket");
    }
    bzero(buffer,50000);
    n = read(sockfd,buffer,50000);
    if (n < 0) {
        error("ERROR reading from socket");
    }
    close(sockfd);
    return std::string(buffer);
}

std::pair<std::string, char> get_word(std::stringstream& sstr, std::string delim){
	std::string s = "";
	char c;
	for(; !sstr.eof();){
		sstr.get(c);
		if(delim.find(c) != std::string::npos){
			return make_pair(s, c);
		}
		s += c;
	}
	return make_pair(s, 0);
}

int main() {
	std::string str, key, value, contents;
	std::stringstream sstr;
	std::string delim = " ~`!@#$%^&*()+-=[]{}<>;':,./?\\|\"";
	std::string tags_delim = " ~`!@#$%^&*()+-=[]{}<>;':,.?\\|\"";
	std::vector<std::string> good_tags = {"p", "ul", "ol", "h2", "h3", "h4", "h5", "h6", "table"};
	char ch;
	bool inside_of_tag, name_of_tag;
	std::vector<std::string> tags_stack;
	bool is_title;
	std::string title;
	int good_tag_count;
	char lastSymbol;
	int i = 0;
	while(std::getline(std::cin, str)){
		i++;
		int div = str.find('\t');
		key = str.substr(0, div);
		value = str.substr(div + 1, str.length() - div - 1);
		contents = get_contents(key);
		sstr.clear();
		sstr << contents;
		inside_of_tag = false;
		name_of_tag = false;
		good_tag_count = 0;
		is_title = false;
		while(!sstr.eof()){
			auto p = get_word(sstr, (name_of_tag ? tags_delim : delim));
			std::cerr << p.first << "\t" << p.second << std::endl;
			if(!inside_of_tag && is_title){
				title += p.first;
				if(p.second != '>' && p.second != '<'){
					title += p.second;
				}
			}
			if(!inside_of_tag && good_tag_count > 0 && p.first.length() > 3){
				std::cout << p.first << "\t" << title << std::endl;
			}
			if(p.second == '>' && p.first == "" && lastSymbol == '/'){
				tags_stack.pop_back();
			}
			if(name_of_tag){
				if(p.first[0] == '/'){
					tags_stack.pop_back();
				}else{
					tags_stack.push_back(p.first);
				}
				if(p.first == "h1"){
					is_title = true;
				}
				if(p.first == "/h1"){
					is_title = false;
				}
				if(std::find(good_tags.begin(), good_tags.end(), p.first) != good_tags.end()){
					++good_tag_count;
				}
				if(p.first[0] == '/' && std::find(good_tags.begin(), good_tags.end(), p.first.substr(1, p.first.length() - 1)) != good_tags.end()){
					--good_tag_count;
				}
			}
			name_of_tag = false;
			if(p.second == '<'){
				inside_of_tag = true;
				name_of_tag = true;
			}else if(p.second == '>'){
				inside_of_tag = false;
			}
			lastSymbol = p.second;
		}
	}
}