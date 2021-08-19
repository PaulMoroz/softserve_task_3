#ifndef SOFTSERVE_TASK_3_FILESTATGETTER_H
#define SOFTSERVE_TASK_3_FILESTATGETTER_H
#include <map>
#include <mutex>
#include <vector>
#include "Statistics.h"
using namespace std;


class FileStatGetter {
private:
    map<char*,Statistics> m;
    int numberOfThreads;
    vector<char*> fileList;
    mutex FileGetLock;
    void analyseFiles();
public:
    FileStatGetter(int,vector<char*>);
    void analyseAll();
    map<char*,Statistics> getStatistics;
    void outputResults(ofstream &outputFile);
};

#endif //SOFTSERVE_TASK_3_FILESTATGETTER_H
