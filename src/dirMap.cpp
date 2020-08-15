#include "dirMap.h"
#include <string.h>
#include <sys/stat.h>
#if defined __linux__ || defined __FreeBSD__ || defined __OpenBSD__
#include <unistd.h>
#endif
inline bool exists_test3 (const std::string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

dirMap::dirMap(const char *dirpath, const char *dirl)
{
    this->dirloc = new char[255];
    this->dirPath = new char[128];
    this->mapName = new char[128];
    this->dirDPath = new char[255];
    this->fLastStart = new char[255];
    snprintf(this->dirloc, 255, "%s/%s/meta.xml",dirpath,dirl);
    snprintf(this->dirDPath, 255, "%s/%s/deleted.txt",dirpath,dirl);
    snprintf(this->fLastStart, 255, "%s/%s/laststart.txt",dirpath,dirl);
    strncpy(this->dirPath, dirl, 128);
    struct tm newyear;
    newyear.tm_hour = 0; newyear.tm_min = 0; newyear.tm_sec = 1;
    newyear.tm_mon = 0;  newyear.tm_mday = 1;newyear.tm_year = 1970;
    this->laststarttime =  mktime(&newyear);
}
bool dirMap::LoadDataForMap()
{
	if (exists_test3(this->dirDPath))return false;
    rapidxml::file<>* 	m_rxMetaFile;
    rapidxml::xml_document<> m_rxMetaDocument;
    try
    {
        m_rxMetaFile = new rapidxml::file<>(this->dirloc);
    }
	catch (const runtime_error& e)
	{
		sampgdk::logprintf("File: %s | Runtime error was: Open File:%s", this->dirloc, e.what());
		return false;
	}
	catch (const rapidxml::parse_error& e)
	{
		sampgdk::logprintf("File: %s | Parse error was: Open File:%s", this->dirloc, e.what());
		return false;
	}
	catch (const exception& e)
	{
		sampgdk::logprintf("File: %s | Error was: Open File:%s", this->dirloc, e.what());
		return false;
	}
	catch (...)
	{
		sampgdk::logprintf("File: %s | Open File:An unknown error occurred.", this->dirloc);
		return false;
	}

    try
    {
        m_rxMetaDocument.parse<0>(m_rxMetaFile->data());
    }
	catch (std::runtime_error e)
	{
		sampgdk::logprintf("File: %s | Runtime error was: Parse File:%s", this->dirloc, e.what());
		return false;
	}
	catch (const rapidxml::parse_error& e)
	{
		sampgdk::logprintf("File: %s | Parse error was: Parse File:%s", this->dirloc, e.what());
		return false;
	}
	catch (const exception& e)
	{
		sampgdk::logprintf("File: %s | Error was: Parse File:%s", this->dirloc, e.what());
		return false;
	}
	catch (...)
	{
		sampgdk::logprintf("File: %s | Parse File:An unknown error occurred.", this->dirloc);
		return false;
	}
	FILE* fi;
	fi = fopen(this->fLastStart, "r");
    if(fi != NULL)
    {
        int laststart;
        struct tm y2k = {0};
        fscanf(fi,"%d",&laststart);
        y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = laststart;
        y2k.tm_year = 0; y2k.tm_mon = 0; y2k.tm_mday = 0;
        this->laststarttime = mktime(&y2k);
        fclose(fi);
    }
	rapidxml::xml_node<> * rxMetaRootNode = m_rxMetaDocument.first_node();
	if(rxMetaRootNode == NULL)return false;
	rapidxml::xml_node<> * rxMetaInfoNode = rxMetaRootNode->first_node("info");
    std::string strMapName;
	if(rxMetaInfoNode->first_attribute("name") != 0)strMapName = rxMetaInfoNode->first_attribute("name")->value();
	else
	{
		strMapName = this->dirPath;
	}
	memcpy(this->mapName, strMapName.c_str(), (128)*sizeof(char));
	delete m_rxMetaFile;
    return true;
}
dirMap::~dirMap()
{
    delete[] this->dirloc;
    delete[] this->dirPath;
    delete[] this->mapName;
    delete[] this->dirDPath;
}
bool dirMap::Destroy()
{
    if(!exists_test3(this->dirDPath))
    {
		FILE* fi;
		fi = fopen(this->dirDPath, "a");
        if(fi != NULL)fclose(fi);
        return true;
    }
    return false;
}
char* dirMap::GetMapName()
{
    return this->mapName;
}
char* dirMap::GetMapLocation()
{
    return this->dirPath;
}
void dirMap::SetStartTimeNow()
{
    this->laststarttime = time(0);
	FILE* fi;
	fi = fopen(this->fLastStart, "w");
    if(fi != NULL)
    {
        fprintf(fi,"%d",(int)difftime(this->laststarttime,0));
        fclose(fi);
    }
}
double dirMap::GetLastStartTime()
{
    return difftime(time(0), this->laststarttime);
}
