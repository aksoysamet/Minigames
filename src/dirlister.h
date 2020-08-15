#ifndef DIRLISTER_H_INCLUDED
#define DIRLISTER_H_INCLUDED
#include "dirMap.h"
class dir_list
{
    public:
    dir_list(const char * dir);
    ~dir_list();
    int GetDirCount();
    vector <dirMap*> dirlist;
    private:
    int dircount = 0;
};


#endif // DIRLISTER_H_INCLUDED
