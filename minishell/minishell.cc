#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void showPrompt(const std::string &PWD){
    char* username = cuserid(nullptr);
    std::cout << "\033[92m";
    while(*username != '\0'){
        std::cout << *username;
        ++username;
    }
    std::cout << '@';
    char hostname[100];
    gethostname(hostname, 100);
    for(int i = 0; hostname[i] != '\0'; ++i){
        std::cout << hostname[i];
    }
    std::cout << "\033[0m:\033[34m" << PWD << "\033[31m > \033[0m" << std::flush;

}

void getInput(std::vector<std::string> &result){
    result.resize(0);
    while(std::cin.peek() != '\n'){
        std::string currentArg;
        std::cin >> currentArg;
        result.push_back(currentArg);
    }
    std::cin.ignore(1, '\n');
}

char** convertResult(std::vector<std::string> result){
    int lastSlash = -1;
    for(int i = 0; i < result[0].size(); ++i){
        if(result[0][i] == '/') lastSlash = i;
    }
    if(lastSlash != -1) result[0].erase(result[0].begin(), result[0].begin() + lastSlash + 1);

    char** returnVal = new char*[result.size() + 1];
    for(int i = 0; i < result.size(); ++i){
        returnVal[i] = new char[result[i].size() + 1];
        for(int j = 0; j < result[i].size(); ++j){
            returnVal[i][j] = result[i][j];
        }
        returnVal[i][result[i].size()] = '\0';
    }
    returnVal[result.size()] = nullptr;
    return returnVal;
}

void cd(const std::vector<std::string> &result, std::string &PWD){
    std::string newPWD = PWD;
    if(result.size() == 1) newPWD = "/";
    else if(result[1].front() == '/') newPWD = result[1];
    else newPWD.append(result[1]);

    if(newPWD.back() != '/') newPWD.append("/");

    if(chdir(newPWD.data()) == 0){
        char* currentPWD = get_current_dir_name();
        PWD = currentPWD;
        PWD.append("/");
        free(currentPWD);
    }
    else std::cout << "error: directory does not exist" << std::endl;
}


int main(){
    std::vector<std::string> result;

    std::string PWD = "/";
    chdir(PWD.data());

    while(true){
        showPrompt(PWD);
        getInput(result);

        std::vector<int> commandBegins = {0};
        for(int i = 0; i < result.size(); ++i){
            if(result[i] == "|"){
                result.erase(result.begin()+i);
                commandBegins.push_back(i);
            }
        }

        std::vector<pid_t> childs;
        int *lastPipe = new int[2];
        int *newPipe = new int[2];
        for(int i = 0; i < commandBegins.size(); ++i){
            bool last = i+1 == commandBegins.size();
            bool first = i == 0;

            close(lastPipe[0]);
            close(lastPipe[1]);
            delete[] lastPipe;
            lastPipe = newPipe;
            if(!last){
                newPipe = new int[2];
                pipe(newPipe);
            } 

            std::vector<std::string>::iterator firstIt = result.begin() + commandBegins[i], lastIt;
            if(last) lastIt = result.end();
            else lastIt = result.begin() + commandBegins[i+1];
            std::vector<std::string> command;
            std::copy(firstIt, lastIt, std::back_inserter(command));

            if(result[commandBegins[i]] == "exit" || result[commandBegins[i]] == "quit"){
                exit(0);
            }
            else if(result[commandBegins[i]] == "cd"){
                cd(command, PWD);
                continue;
            }

            childs.push_back(fork());
            if(childs.back() == 0){
                if(!first){
                    close(lastPipe[1]);
                    dup2(lastPipe[0], 0);
                }
                if(!last){
                    close(newPipe[0]);
                    dup2(newPipe[1], 1);
                }
                char** args = convertResult(command);
                if(execvp(command[0].data(), args) != 0){
                    std::cout << "error: invalid command" << std::endl;
                    exit(1);
                }
            }
        }
        close(lastPipe[0]);
        close(lastPipe[1]);
        delete[] lastPipe;

        std::vector<int> childStatusCodes(childs.size());
        for(int i = 0; i < childs.size(); ++i){
            waitpid(childs[i], &childStatusCodes[i], 0);
            std::cout << (i+1) << ". child process exited with code " << childStatusCodes[i] << std::endl;
        }
    }
}
