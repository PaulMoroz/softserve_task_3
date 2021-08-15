#include <iostream>
#include <vector>
#include <time.h>
#include <dirent.h>
#include <set>
#include <thread>
#include <string>
#include <cstring>
#include <map>
#include <fstream>
#include <queue>

using namespace std;
const int filePathLenght = 256;
const int numberOfThreads = 6;
bool possibleToGetFile = true;
vector<char*> fileList;

struct output{
    int numberOfCodeLines;
    int numberOfEmptyLines;
    int numberOfCommentLines;
};
map<char*,output> m;

bool checkDirectory(char* path){
    struct dirent *entry;
    DIR* dp = opendir(path);
    if(dp== nullptr){
        return false;
    }else{
        closedir(dp);
        delete dp;
        return true;
    }
}

void parseFile(){
    char* curr;
    while (!fileList.empty()){
        while (!possibleToGetFile){}
        if(possibleToGetFile){
            possibleToGetFile= false;
            curr = fileList[0];
            fileList.erase(fileList.begin());
            possibleToGetFile= true;
        }
        output stat;
        stat.numberOfCodeLines = 0;
        stat.numberOfCommentLines = 0;
        stat.numberOfEmptyLines = 0;
        ifstream in(curr);
        m[curr] = stat;
    }
}


bool checkFile(char* path){
    string filename(path);
    string extension = filename.substr(filename.find_last_of(".") + 1);
    if(extension=="c" || extension=="cpp" || extension=="h" || extension=="hpp")
        return true;
    else
        return false;
}

void ReadAllFiles(char* path){
    queue<char*> dirList;
    dirList.push(path);
    while (!dirList.empty()){
        struct dirent *entry;
        DIR *dp;

        dp = opendir(dirList.front());
        if (dp == NULL)
        {
            perror("opendir");
            return;
        }
        while((entry = readdir(dp))){
            //puts(entry->d_name);
            if(strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0 ){
                char* temp = new char[256];
                strcpy(temp,dirList.front());
                strcat(temp,"\\");
                strcat(temp,entry->d_name);
                cout<<temp<<endl;
                if(!checkDirectory(temp)){
                    if(checkFile(temp)){
                        fileList.push_back(temp);
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

void DispatchFiles(){
    thread t[numberOfThreads];
    for(int i=0;i<numberOfThreads;i++){

    }
}

int main() {
    char* readPath = new char [filePathLenght];
    cout<<"Enter filepath to directory to read: ";
    gets(readPath);
    ReadAllFiles(readPath);
    cout<<"Reading done!\n";


    DispatchFiles();


    char* writePath = new char [filePathLenght];
    cout<<"Enter file to write results: ";
    gets(writePath);
    ofstream outputFile(writePath);
    if(!outputFile){
        cout<<"Error in writing output to file\n";
        return 1;
    }
    for(pair<char*,output> pi : m){
        outputFile<<"=====================\n";
        outputFile<<pi.first<<endl;
        outputFile<<"Comment lines: "<<pi.second.numberOfCommentLines<<endl;
        outputFile<<"Code lines: "<<pi.second.numberOfCodeLines<<endl;
        outputFile<<"Comment lines: "<<pi.second.numberOfEmptyLines<<endl;
    }
}
