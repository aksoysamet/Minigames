#include "main.h"
using std::vector;

#define FLAG_MARKERS 1
#define FLAG_CHECKPS 2
#define FLAG_COLOBJE 4
#define FLAG_PED     8
#define FLAG_PICKUP  16
class bf_map {
public:
	bf_map(const char * dfolder, const char * szPath, int  arrVirtualWorlds, int flags);
	~bf_map();

public:
	bool	load();
	bool	create();
	bool	destroy();
	bool  m_bLoaded = false;
	int         m_flags = 0;
protected:
	char * 		m_szPath;
	char *		m_szMetaPath;
	char *		m_szMapPath;
	int  		m_arrVirtualWorld;
protected:
	char *		m_szMapName = NULL;
	int 		m_iDuration = 400;
	int			m_iWeather = 0;
	int			m_iHour = 0;
	int			m_iMinute = 0;

public:
	vector < float * > 	m_vSpawnpoints; // 7-indexed array. ( vehicle, x,y,z, rx,ry,rz )
	vector < float * > 	m_vRacePickups; // 8-indexed array. ( type, vehicle, x,y,z, rx,ry,rz ) types: (0:repair, 1:nitro, 2:vehiclechange)
    vector < float * >  m_vRaceCheckps;
    vector < float * >  m_vPickups;
public:
	vector < int > 		m_vSAMPObjects;
	vector < int >      m_vColObjects;
protected:
	rapidxml::file<> 			*	m_rxFile;
	rapidxml::file<>			* 	m_rxMetaFile;
	rapidxml::xml_document<> 		m_rxDocument;
	rapidxml::xml_document<>		m_rxMetaDocument;
};
