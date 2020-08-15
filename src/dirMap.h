#ifndef DIRMAP_H
#define DIRMAP_H
#include <time.h>
#include "../src/main.h"
class dirMap
{
    public:
        dirMap(const char *dirpath, const char *dirl);
        virtual ~dirMap();
        bool LoadDataForMap();
        char* GetMapLocation();
        char* GetMapName();
        void SetStartTimeNow();
        double GetLastStartTime();
        bool Destroy();
    protected:
    char *dirloc;
    char *mapName;
    char *dirPath;
    char *dirDPath;
    char *fLastStart;
    time_t laststarttime;
    private:
};

#endif // DIRMAP_H
