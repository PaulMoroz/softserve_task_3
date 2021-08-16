#include <iostream>
#include <vector>
#include <chrono>
#include <unistd.h>
#include <dirent.h>
#include <set>
#include <thread>
#include <string>
#include <cstring>
#include <map>
#include <fstream>
#include <queue>
#include <algorithm>

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

const std::string WHITESPACE = " \n\r\t\f\v";

std::string ltrim(const std::string &s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string &s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trim(const std::string &s) {
    return rtrim(ltrim(s));
}

int findFirstIndex(string in,string what){
    int n = in.size();
    int k = what.size();
    for(int i=0;i<n - k;i++){
        if(in.substr(i,k)==what)return i;
    }
    return INT_MAX;
}

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
        string s;
        getline(in,s);
        s = trim(s);
        bool openComment = false;
        if(s.empty() && !openComment){
            stat.numberOfEmptyLines++;
        }else{
            int multiLineIndexStart = findFirstIndex(s,"/*");
            int oneLineIndex = findFirstIndex(s,"//");
            int multiLineIndexEnd = findFirstIndex(s,"*/");
            bool isCode = true;
            if(multiLineIndexStart==min({multiLineIndexEnd,multiLineIndexStart,oneLineIndex}) && multiLineIndexStart<s.size()) {
                if(multiLineIndexStart!=0 && openComment==false)
                    isCode = true;
                openComment = true;
                if(multiLineIndexEnd!=INT_MAX && multiLineIndexEnd!=s.size() - 2) {
                    openComment = false;
                    isCode = true;
                }
                stat.numberOfCommentLines++;
            }else
                if(multiLineIndexEnd==min({multiLineIndexEnd,multiLineIndexStart,oneLineIndex}) && multiLineIndexEnd<s.size()){
                    openComment = false;
                    stat.numberOfCommentLines++;
                    if(multiLineIndexStart==multiLineIndexEnd+2) {
                        openComment = true;
                        isCode = false;
                    }
                    if(oneLineIndex==multiLineIndexEnd+2)
                        isCode = false;
            }else
                if(oneLineIndex==min({multiLineIndexEnd,multiLineIndexStart,oneLineIndex}) && oneLineIndex<s.size()){
                    stat.numberOfCommentLines++;
                    if(oneLineIndex!=0 && !openComment)
                        isCode = true;
                }
                stat.numberOfCodeLines+=isCode;
        }
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
        t[i] = thread(parseFile);
        t[i].join();
    }
}

int main() {
    auto start = chrono::steady_clock::now();
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
    auto end = chrono::steady_clock::now();
    outputFile<<"=====================\n";
    outputFile << "Elapsed time in milliseconds: "
    << chrono::duration_cast<chrono::milliseconds>(end - start).count()
    << " ms" << endl;
}
