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

const int MAX_BYTES = 50'000'000;

void readLines(std::ifstream& in, int bytesize, std::vector<std::pair<std::string, std::string> >& res){
    std::string key, value;
    char* str = new char[bytesize];
    in.read(str, bytesize);
    std::string data(str);
    char c;
    if(!in.eof() && data[data.length() - 1] != '\n'){
        do {
            in >> c;
            data += c;
        } while(c != '\n');
    }
    std::stringstream sstr(data);
    std::string s;
    while(!sstr.eof()) {
        std::getline(sstr, s);
        int delim = s.find('\t');
        key = s.substr(0, delim);
        value = s.substr(delim + 1, s.length() - delim - 1);
        if(key == ""){
            continue;
        }
        res.push_back({key, value});
    }
    delete [] str;
}

void writeLines(std::ofstream& out, std::vector<std::pair<std::string, std::string> >& lines){
    std::string key, value;
    std::string s;
    for(auto p: lines){
        out << p.first << "\t" << p.second << "\n";
    }
}

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


std::string generateFileName(std::string previousName) {
    std::string s;
    std::ifstream in;
    s = getRandomString(5) + "_" + previousName;
    in.open(s);
    while(in.good()){
        in.close();
        s = getRandomString(5) + "_" + previousName;
        in.open(s);
    }
    in.close();
    return s;
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


struct Comp {
    std::vector<std::pair<int, std::vector<std::pair<std::string, std::string> > > >& all_small_parts;
    Comp(std::vector<std::pair<int, std::vector<std::pair<std::string, std::string> > > >& ref): all_small_parts(ref) {}
    bool operator() (int& idx1, int& idx2) {
        int s1 = all_small_parts[idx1].second.size();
        int s2 = all_small_parts[idx1].second.size();
        if(s2 == 0){
            return true;
        }
        if(s1 == 0){
            return false;
        }
        return all_small_parts[idx1].second[all_small_parts[idx1].first].first > all_small_parts[idx2].second[all_small_parts[idx2].first].first;
    }
};

void sortFile(std::string path, std::string new_path) {
    std::ifstream in(path);
    std::vector<std::pair<std::string, std::string> > one_big_part;
    std::vector<std::string> fileNames;
    std::vector<std::pair<int, std::vector<std::pair<std::string, std::string> > > > small_parts;
    std::vector<std::ifstream> streams;
    std::vector<std::pair<std::string, std::string> > output;
    std::vector<int> temp;
    while(!in.eof()){
        one_big_part.clear();
        readLines(in, MAX_BYTES, one_big_part);
        std::sort(one_big_part.begin(), one_big_part.end());
        fileNames.push_back(generateFileName());
        // std::cerr << fileNames.back() << std::endl;
        std::ofstream out2(fileNames.back());
        writeLines(out2, one_big_part);
        out2.close();
    }
    in.close();
    Comp comp(small_parts);
    std::priority_queue<int, std::vector<int>, Comp> pq(comp);
    std::ofstream out;
    out.open(new_path, std::ofstream::out | std::ofstream::trunc);
    output.clear();
    const int SMALL_PART_SIZE = MAX_BYTES / (fileNames.size() + 1);
    small_parts.resize(fileNames.size());
    for(int i = 0; i < fileNames.size(); ++i){
        streams.push_back(std::ifstream(fileNames[i]));
        readLines(streams.back(), SMALL_PART_SIZE, small_parts[i].second);
        pq.push(i);
    }
    int outputBytes = 0;
    while(!pq.empty()){
        int idx = pq.top();
        pq.pop();
        output.push_back(small_parts[idx].second[small_parts[idx].first]);
        outputBytes += output.back().first.length() + output.back().second.length() + 2;
        if(outputBytes >= MAX_BYTES){
            writeLines(out, output);
            output.clear();
            outputBytes = 0;
        }
        small_parts[idx].first++;
        if(small_parts[idx].first == small_parts[idx].second.size()){
            small_parts[idx].second.clear();
            small_parts[idx].first = 0;
            if(!streams[idx].eof()){
                readLines(streams[idx], SMALL_PART_SIZE, small_parts[idx].second);
                pq.push(idx);
            }
        }else{
            pq.push(idx);
        }
    }
    writeLines(out, output);
    output.clear();
    outputBytes = 0;
    out.close();
    for(int i = 0; i < fileNames.size(); ++i){
        streams[i].close();
        remove(fileNames[i].c_str());
    }   
}

int main(int argc, char** argv) {
    srand(time(nullptr));
    char* type;
    int parent_child[2];
    int child_parent[2];
    if (argc >= 2){
        type = argv[1];
        if(strcmp(type, "help") == 0){
            std::cerr << std::endl;
            std::cerr << "This program implements MapReduce for small or rather large amount of data." << std::endl;
            std::cerr << "Format:" << std::endl;
            std::cerr << "Map:\tpath_to_program map path_to_map_script (script parameters) input_file output_file" << std::endl;
            std::cerr << "Reduce:\tpath_to_program reduce path_to_reduce_script (script parameters) input_file output_file" << std::endl;
            std::cerr << "Help:\tpath_to_program help" << std::endl;
            std::cerr << std::endl;
            std::cerr << "Each line of input file must contain tab-separated values: \"key \\t value\"" << std::endl;
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
        std::string old_file(src_file);
        std::string new_file = generateFileName(std::string(src_file));
        sortFile(old_file, new_file);

        fclose(stdin);
        freopen(new_file.c_str(), "r", stdin);

        bool firstKey = true;
        std::string lastKey;
        while (std::getline(std::cin, str)) {
            int div = str.find('\t');
            key = str.substr(0, div);
            value = str.substr(div + 1, str.length() -  div - 1);
            if(!firstKey && key != lastKey){ // if key == lastKey ended in sorted file
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
                for(auto val: values[lastKey]){
                    write(parent_child[1], lastKey.data(), lastKey.size());
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
            lastKey = key;
            values[key].push_back(value);
            if(firstKey){
                firstKey = false;
            }
        }

        if(lastKey.length() > 0){
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
            for(auto val: values[lastKey]){
                write(parent_child[1], lastKey.data(), lastKey.size());
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