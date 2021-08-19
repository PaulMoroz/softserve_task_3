#include "FileStatGetter.h"
#include <fstream>
#include <iostream>
#include "StringFunctions.h"
#include <algorithm>
#include <thread>
using namespace std;

FileStatGetter::FileStatGetter(int numberOfThreads,vector<char*> fileList) {
    this->numberOfThreads = 0;
    this->fileList = fileList;
    this->FileGetLock.unlock();
}

void FileStatGetter::analyseFiles() {
    char* curr;
    while (!this->fileList.empty()){
        this->FileGetLock.lock();
        cout<<"Processing file "<<this->fileList[0]<<" by " <<this_thread::get_id()<<endl;
        curr = this->fileList[0];
        this->fileList.erase(this->fileList.begin());
        this->FileGetLock.unlock();
        Statistics stat;
        stat.numberOfCodeLines = 0;
        stat.numberOfCommentLines = 0;
        stat.numberOfEmptyLines = 0;
        ifstream in(curr);
        string s;
        bool openComment = false;
        while(getline(in,s)){
            s = trim(s);
            if(s.empty() && !openComment){
                stat.numberOfEmptyLines++;
            }else{
                int multiLineIndexStart = findFirstIndex(s,"/*");
                int oneLineIndex = findFirstIndex(s,"//");
                int multiLineIndexEnd = findFirstIndex(s,"*/");
                bool isCode = true;
                if(multiLineIndexStart==min({multiLineIndexEnd,multiLineIndexStart,oneLineIndex}) && multiLineIndexStart<s.size()){
                    if(multiLineIndexStart!=0 && !openComment)
                        isCode = true;
                    openComment = true;
                    if(multiLineIndexEnd!=INT_MAX && multiLineIndexEnd!=s.size() - 2){
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
        }
        this->m[curr] = stat;
    }
}

void FileStatGetter::analyseAll() {
    thread th[this->numberOfThreads];
    for(int i=0;i<this->numberOfThreads;i++){
        th[i] = thread(&FileStatGetter::analyseFiles, this);
    }
    for(int i=0;i<this->numberOfThreads;i++){
        th[i].join();
    }
}

void FileStatGetter::outputResults(ofstream& outputFile) {
    if(!outputFile){
        cout<<"Error in writing output to file\n";
        return;
    }
    for(pair<char*,Statistics> pi : this->m){
        outputFile<<"=====================\n";
        outputFile<<pi.first<<endl;
        outputFile<<"Comment lines: "<<pi.second.numberOfCommentLines<<endl;
        outputFile<<"Code lines: "<<pi.second.numberOfCodeLines<<endl;
        outputFile<<"Empty lines: "<<pi.second.numberOfEmptyLines<<endl;
    }
}
