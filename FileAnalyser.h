#ifndef SOFTSERVE_TASK_3_FILEANALYSER_H
#define SOFTSERVE_TASK_3_FILEANALYSER_H
#include <string>
#include <vector>
#include <dirent.h>
using namespace std;

class FileAnalyser {
private:
    vector<char*> pathList;
    static bool isDirectory(char* path){
        DIR* dp = opendir(path);
        if(dp== nullptr){
            return false;
        }else{
            closedir(dp);
            delete dp;
            return true;
        }
    }

    static bool isCorrectExtensionFile(char* path){
        string filename(path);
        string extension = filename.substr(filename.find_last_of(".") + 1);
        if(extension=="c" || extension=="cpp" || extension=="h" || extension=="hpp")
            return true;
        else
            return false;
    }
public:
    vector<char* > getPathList();
    void analyse(char* basePath);
};


#endif //SOFTSERVE_TASK_3_FILEANALYSER_H
