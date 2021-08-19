#include "StringFunctions.h"
using namespace std;

string ltrim(const string &s){
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == string::npos) ? "" : s.substr(start);
}

string rtrim(const string &s){
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == string::npos) ? "" : s.substr(0, end + 1);
}

string trim(const string &s){
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
