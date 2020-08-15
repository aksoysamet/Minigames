#include "main.h"
#include "dirlister.h"
#include <string.h>
#include <dirent.h>


dir_list::dir_list(const char * dir)
{
    char asilpath[255];
    snprintf(asilpath, 255, "./scriptfiles/%s",dir);
    dirlist.clear();
    DIR* dire;
    dire = opendir(asilpath);
    dirMap* dmap;
    dircount = 0;
    if(dire != NULL)
    {
        struct dirent *ent;
        while ((ent = readdir(dire)) != NULL)
		{
			switch (ent->d_type)
			{
				case DT_DIR:
				{
					if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, ".."))continue;
					dmap = new dirMap(asilpath, ent->d_name);
					if(dmap->LoadDataForMap())
					{
                        dirlist.push_back(dmap);
                        dircount++;
					}else
					{
                        //sampgdk::logprintf("%s/%s dosyasi hatali.",asilpath, ent->d_name);
                        delete dmap;
					}
				}
				break;
			}
		}
		closedir(dire);
    }else sampgdk::logprintf("%s file is not found!", asilpath);
//#endif
}
dir_list::~dir_list()
{
    for (auto dirl = dirlist.begin(); dirl != dirlist.end(); ++dirl)
    {
        delete (*dirl);
    };
    dirlist.clear();
}
int dir_list::GetDirCount()
{
    return dircount;
}
