#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <algorithm>
#include <fstream>
#include "FileAnalyser.h"
#include "FileStatGetter.h"
using namespace std;

const int filePathLenght = 256;
const int numberOfThreads = 6;

int main() {
    auto start = chrono::steady_clock::now();
    char* readPath = new char [filePathLenght];
    cout<<"Enter filepath to directory to read: ";
    gets(readPath);
    FileAnalyser analyser;
    analyser.analyse(readPath);
    cout<<"Reading done!\n";
    FileStatGetter fileStatGetter(numberOfThreads,analyser.getPathList());
    fileStatGetter.analyseAll();
    char* writePath = new char [filePathLenght];
    cout<<"Enter file to write results: ";
    gets(writePath);
    ofstream outputFile(writePath);
    if(!outputFile){
        perror("Something wrong with output!\n");
        return 1;
    }
    fileStatGetter.outputResults(outputFile);
    auto end = chrono::steady_clock::now();
    outputFile<<"=====================\n";
    outputFile << "Elapsed time in milliseconds: "
    << chrono::duration_cast<chrono::milliseconds>(end - start).count()
    << " ms" << endl;
    cout<<"Results written!\n";
    outputFile.close();
}
