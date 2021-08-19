#include "FileAnalyser.h"
#include <queue>
using namespace std;
vector<char*> FileAnalyser::getPathList() {
    return this->pathList;
}

void FileAnalyser::analyse(char *basePath) {
    this->pathList.clear();
    queue<char*> dirList;
    dirList.push(basePath);
    while (!dirList.empty()){
        struct dirent *entry;
        DIR *dp;
        dp = opendir(dirList.front());
        if (dp == nullptr){
            perror("opendir");
            return;
        }
        while((entry = readdir(dp))){
            if(strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0 ){
                char* temp = new char[256];
                strcpy(temp,dirList.front());
                strcat(temp,"\\");
                strcat(temp,entry->d_name);
                if(!isDirectory(temp)){
                    if(isCorrectExtensionFile(temp)){
                        this->pathList.push_back(temp);
                    }
                }else{
                    dirList.push(temp);
                }
            }
        }
        closedir(dp);
        dirList.pop();
    }
}
