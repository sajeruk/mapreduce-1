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
#include <sys/wait.h>

int main(int argc, char** argv) {
    char* type;
    int parent_child[2];
    int child_parent[2];
    if (argc >= 2){
        type = argv[1];
        if(strcmp(type, "help") == 0){
            std::cerr << std::endl;
            std::cerr << "This program implements MapReduce for small amount of data." << std::endl;
            std::cerr << "Format:" << std::endl;
            std::cerr << "Map:\tpath_to_program map path_to_map_script (script parameters) input_file output_file" << std::endl;
            std::cerr << "Reduce:\tpath_to_program reduce path_to_reduce_script (script parameters) input_file output_file" << std::endl;
            std::cerr << "Help:\tpath_to_program help" << std::endl;
            std::cerr << std::endl;
            std::cerr << "Each line of input file for reduce operation must contain tab-separated values: \"key \\t value\"" << std::endl;
            std::cerr << "Each line of output file will contain tab-separated values: \"key \\t value\"" << std::endl;
            std::cerr << std::endl;
            return 0;
        }
    }
    if (argc < 5){
        std::cerr << "Invalid format: at least 4 arguments required" << std::endl;
        return 10010;
    }
    char* script_path = argv[2];
    char* src_file = argv[argc - 2];
    char* dst_file = argv[argc - 1];
    char** params = new char* [argc - 4];
    for(int i = 0; i < argc - 4; ++i){
        params[i] = new char[100];
        strcpy(params[i], argv[i + 2]);
    }
    freopen(src_file, "r", stdin);
    freopen(dst_file, "w", stdout);

    if (strcmp(type, "map") == 0){

        pipe(parent_child);
        pipe(child_parent);
        int pid = fork();
        if (pid == 0) {
            close(parent_child[1]);
            close(child_parent[0]);
            dup2(parent_child[0], 0);
            dup2(child_parent[1], 1);
            if (execvp(script_path, params)) {
                perror("execvp");
            }
            exit(1);
        }
        std::string str;
        while (std::getline(std::cin, str)) {
            str += '\n';
            write(parent_child[1], str.data(), str.size());
        }
        close(parent_child[1]);
        wait(NULL);
        char buf[1000];
        memset(buf, 0, 1000);
        while(true){
            read(child_parent[0], buf, sizeof(buf));
            std::cout << buf;
            if(strlen(buf) < 1000){
                break;
            }
            memset(buf, 0, 1000);
        }
        close(child_parent[0]);

    } else if (strcmp(type, "reduce") == 0) {

        std::string str, key, value;
        std::map<std::string, std::vector<std::string>> values;
        std::stringstream sstr;

        while (std::getline(std::cin, str)) {
            sstr.clear();
            sstr.str(str);
            sstr >> key >> value;
            values[key].push_back(value);
        }

        for(auto entry: values){
            if (entry.second.size() == 0){
                continue;
            }
            pipe(parent_child);
            pipe(child_parent);
            int pid = fork();
            if (pid == 0) {
                close(parent_child[1]);
                close(child_parent[0]);
                dup2(parent_child[0], 0);
                dup2(child_parent[1], 1);
                if (execvp(script_path, params)) {
                    perror("execvp");
                }
                exit(1);
            }
            for(auto val: entry.second){
                write(parent_child[1], entry.first.data(), entry.first.size());
                write(parent_child[1], "\t", 1);
                write(parent_child[1], val.data(), val.size());
                write(parent_child[1], "\n", 1);
            }
            close(parent_child[1]);
            wait(NULL);
            char buf[1000];
            memset(buf, 0, 1000);
            while(true){
                read(child_parent[0], buf, sizeof(buf));
                std::cout << buf;
                if(strlen(buf) < 1000){
                    break;
                }
                memset(buf, 0, 1000);
            }
            close(child_parent[0]);
        }

    } else {
        fclose(stdin);
        fclose(stdout);
        std::cerr << "Invalid format: 1st parameter must be either \"map\" or \"reduce\"" << std::endl;
        return 10010;
    }

    fclose(stdin);
    fclose(stdout);

    return 0;
}