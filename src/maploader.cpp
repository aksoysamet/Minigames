#include "maploader.h"
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <set>
extern std::set<AMX*> amxList;
extern bf_map* lastmap;
#ifndef PATH_MAX
#define PATH_MAX 256
#endif
using std::string;
bf_map::bf_map(const char * dfolder, const char * szPath, int  arrVirtualWorld, int flags) {
	this->m_szPath = new char[PATH_MAX];
	this->m_szMetaPath = new char[PATH_MAX];
	this->m_szMapPath = new char[PATH_MAX];
	snprintf(this->m_szPath, PATH_MAX, "./scriptfiles/%s/%s", dfolder, szPath);
	snprintf(this->m_szMetaPath, PATH_MAX, "./scriptfiles/%s/%s/meta.xml", dfolder, szPath);
	this->m_arrVirtualWorld = arrVirtualWorld;
	this->m_flags = flags;
	this->m_iDuration = 600;
	this->m_iWeather = 10;
	this->m_iHour = 0;
    this->m_iMinute = 0;
}

bf_map::~bf_map() {
	delete[] this->m_szPath;
	delete[] this->m_szMetaPath;
	delete[] this->m_szMapPath;
	if (this->m_szMapName != NULL) {
		delete[] this->m_szMapName;
	}
}
void WarnForError(cell id)
{
	cell ret;
	int g_pMapLoadedIdx;
	for (std::set<AMX*>::iterator a = amxList.begin(); a != amxList.end(); ++a)
	{
		g_pMapLoadedIdx = 0;
		if (!amx_FindPublic(*a, "OnMapLoadError", &g_pMapLoadedIdx))
		{
			amx_Push(*a, id);
			amx_Exec(*a, &ret, g_pMapLoadedIdx);
		}
	}        
}
bool bf_map::load() {
	try
    {
        this->m_rxMetaFile = new rapidxml::file<>(this->m_szMetaPath);
    }
    catch (const runtime_error& e)
    {
        sampgdk::logprintf("File: %s | Runtime error was: Open File:%s", this->m_szMetaPath, e.what());
        WarnForError((cell)this);
        return false;
    }
    catch (const rapidxml::parse_error& e)
    {
        sampgdk::logprintf("File: %s | Parse error was: Open File:%s", this->m_szMetaPath, e.what());
        WarnForError((cell)this);
        return false;
    }
    catch (const exception& e)
    {
        sampgdk::logprintf("File: %s | Error was: Open File:%s", this->m_szMetaPath, e.what());
        WarnForError((cell)this);
        return false;
    }
    catch (...)
    {
        sampgdk::logprintf("File: %s | Open File:An unknown error occurred.", this->m_szMetaPath);
        WarnForError((cell)this);
        return false;
    }

    try
    {
        this->m_rxMetaDocument.parse<0>(this->m_rxMetaFile->data());
    }
    catch (const runtime_error& e)
    {
        sampgdk::logprintf("File: %s | Runtime error was: Parse File:%s", this->m_szMetaPath, e.what());
        WarnForError((cell)this);
        return false;
    }
    catch (const rapidxml::parse_error& e)
    {
        sampgdk::logprintf("File: %s | Parse error was: Parse File:%s", this->m_szMetaPath, e.what());
        WarnForError((cell)this);
        return false;
    }
    catch (const exception& e)
    {
        sampgdk::logprintf("File: %s | Error was: Parse File:%s", this->m_szMetaPath, e.what());
        WarnForError((cell)this);
        return false;
    }
    catch (...)
    {
        sampgdk::logprintf("File: %s | Parse File:An unknown error occurred.", this->m_szMetaPath);
        WarnForError((cell)this);
        return false;
    }
	rapidxml::xml_node<> * rxMetaRootNode = this->m_rxMetaDocument.first_node();
	rapidxml::xml_node<> * rxMetaInfoNode = rxMetaRootNode->first_node("info");
	rapidxml::xml_node<> * rxMetaMapNode = rxMetaRootNode->first_node("map");
	rapidxml::xml_node<> * rxSettingsNode = rxMetaRootNode->first_node("settings");
	//Map src
	if(rxMetaMapNode->first_attribute("src") != 0)snprintf(this->m_szMapPath, PATH_MAX, "%s/%s", this->m_szPath, rxMetaMapNode->first_attribute("src")->value());
	else
	{
        WarnForError((cell)this);
		return false;
	}
	// Map Name
	string strMapName;
	if(rxMetaInfoNode->first_attribute("name") != 0)strMapName = rxMetaInfoNode->first_attribute("name")->value();
	else
	{
		strMapName = rxMetaMapNode->first_attribute("src")->value();
		strMapName = strMapName.substr(0, strMapName.size() - 4);
	}
	int iMapNameLength = strMapName.length();
	this->m_szMapName = new char[iMapNameLength + 1];
	memcpy(this->m_szMapName, strMapName.c_str(), (iMapNameLength + 1)*sizeof(char));
	// Settings
	rapidxml::xml_node<> * rxSettingNode = rxSettingsNode->first_node("setting");
	string strSettingNode;
	char * szSettingName;
	char * szSettingValue;
	for (; rxSettingNode; rxSettingNode = rxSettingNode->next_sibling("setting")) {
        if(!(rxSettingNode->first_attribute("name")))continue;
		szSettingName = rxSettingNode->first_attribute("name")->value();
		if (!strcmp(szSettingName, "#duration")) {
			if (rxSettingsNode != NULL) {
                if(rxSettingNode->first_attribute("value") != 0)
                {
                    strSettingNode = rxSettingNode->first_attribute("value")->value();
                    if (strSettingNode[0] == '[') {
                        strSettingNode[0] = ' ';
                        strSettingNode[strSettingNode.length() - 1] = ' ';
                    }
                    try
                    {
                        this->m_iDuration = stoi(strSettingNode);
                    }
                    catch (const exception& e)
                    {
                        sampgdk::logprintf("File: %s#duration | Error was: %s", this->m_szMetaPath, e.what());
                        this->m_iDuration = 600;
                    }
                }

			}
		}
		else if (!strcmp(szSettingName, "#weather")) {
			if (rxSettingsNode != NULL) {
                if(rxSettingNode->first_attribute("value") != 0)
                {
                    strSettingNode = rxSettingNode->first_attribute("value")->value();
                    if (strSettingNode[0] == '[') {
                        strSettingNode[0] = ' ';
                        strSettingNode[strSettingNode.length() - 1] = ' ';
                    }
                    try
                    {
                        this->m_iWeather = stoi(strSettingNode);
                    }
                    catch (const exception& e)
                    {
                        sampgdk::logprintf("File: %s#weather | Error was: %s", this->m_szMetaPath, e.what());
                        this->m_iWeather = 10;
                    }
                }
			}
		}
		else if (!strcmp(szSettingName, "#time")) {
			if (rxSettingsNode != NULL) {
				if(rxSettingNode->first_attribute("value") != 0)
				{
                    szSettingName = rxSettingNode->first_attribute("value")->value();
                    try
                    {
                        szSettingValue = strtok(szSettingName, ":");
                        this->m_iHour = stoi(szSettingValue);
                        szSettingValue = strtok(NULL, ":");
                        this->m_iMinute = stoi(szSettingValue);
                    }
                    catch (const exception& e)
                    {
                        sampgdk::logprintf("File: %s#time | Error was: %s", this->m_szMetaPath, e.what());
                        this->m_iHour = 0;
                        this->m_iMinute = 0;
                    }
                }
			}
		}
	}
	// Map Loading
	float * daObjInformation;
    try
    {
        this->m_rxFile = new rapidxml::file<>(this->m_szMapPath);
    }
	catch (const runtime_error& e)
    {
        sampgdk::logprintf("File: %s | Runtime error was: Open File:%s", this->m_szMapPath, e.what());
        WarnForError((cell)this);
        return false;
    }
    catch (const rapidxml::parse_error& e)
    {
        sampgdk::logprintf("File: %s | Parse error was: Open File:%s", this->m_szMapPath, e.what());
        WarnForError((cell)this);
        return false;
    }
    catch (const exception& e)
    {
        sampgdk::logprintf("File: %s | Error was: Open File:%s", this->m_szMapPath, e.what());
        WarnForError((cell)this);
        return false;
    }
    catch (...)
    {
        sampgdk::logprintf("File: %s | Open File:An unknown error occurred.", this->m_szMapPath);
        WarnForError((cell)this);
        return false;
    }
	try
	{
        this->m_rxDocument.parse<0>(this->m_rxFile->data());
	}
	catch (const runtime_error& e)
    {
        sampgdk::logprintf("File: %s | Runtime error was: Parse File:%s", this->m_szMapPath, e.what());
        WarnForError((cell)this);
        return false;
    }
    catch (const rapidxml::parse_error& e)
    {
        sampgdk::logprintf("File: %s | Parse error was: Parse File:%s", this->m_szMapPath, e.what());
        WarnForError((cell)this);
        return false;
    }
    catch (const exception& e)
    {
        sampgdk::logprintf("File: %s | Error was: Parse File:%s", this->m_szMapPath, e.what());
        WarnForError((cell)this);
        return false;
    }
    catch (...)
    {
        sampgdk::logprintf("File: %s | Parse File:An unknown error occurred.", this->m_szMapPath);
        WarnForError((cell)this);
        return false;
    }
	rapidxml::xml_node<> * rxRootNode = this->m_rxDocument.first_node();
	// Objects
	rapidxml::xml_node<> * rxObjectNode = rxRootNode->first_node("object");
	int model;
	int itemid = 0;
	float ObjectPosRot[6];
	if (rxObjectNode != 0)
	{
		for (; rxObjectNode; rxObjectNode = rxObjectNode->next_sibling("object")) {

            try
            {
                if(rxObjectNode->first_attribute("model") != 0)model = stoi(rxObjectNode->first_attribute("model")->value());
                else model = 0;
                if(rxObjectNode->first_attribute("posX") != 0)ObjectPosRot[0] = stof(rxObjectNode->first_attribute("posX")->value());
                else ObjectPosRot[0] = 0.0f;
                if(rxObjectNode->first_attribute("posY") != 0)ObjectPosRot[1] = stof(rxObjectNode->first_attribute("posY")->value());
                else ObjectPosRot[1] = 0.0f;
                if(rxObjectNode->first_attribute("posZ") != 0)ObjectPosRot[2] = stof(rxObjectNode->first_attribute("posZ")->value());
                else ObjectPosRot[2] = 0.0f;
                if(rxObjectNode->first_attribute("rotX") != 0)ObjectPosRot[3] = stof(rxObjectNode->first_attribute("rotX")->value());
                else ObjectPosRot[3] = 0.0f;
                if(rxObjectNode->first_attribute("rotY") != 0)ObjectPosRot[4] = stof(rxObjectNode->first_attribute("rotY")->value());
                else ObjectPosRot[4] = 0.0f;
                if(rxObjectNode->first_attribute("rotZ") != 0)ObjectPosRot[5] = stof(rxObjectNode->first_attribute("rotZ")->value());
                else ObjectPosRot[5] = 0.0f;
                itemid++;
            }
            catch (const exception& e)
            {
                sampgdk::logprintf("File: %s | Error was: O_%d:%s", this->m_szMapPath, itemid, e.what());
                WarnForError((cell)this);
                return false;
            }
			if(model > 0 && model < 20000)
			{
                m_vSAMPObjects.push_back(CreateDynamicObject(model,
                    ObjectPosRot[0],
                    ObjectPosRot[1],
                    ObjectPosRot[2],
                    ObjectPosRot[3],
                    ObjectPosRot[4],
                    ObjectPosRot[5],
                    this->m_arrVirtualWorld));
                if(this->m_flags & FLAG_COLOBJE)
                {
                    m_vColObjects.push_back(CA_CreateObject ( model, ObjectPosRot[0], ObjectPosRot[1], ObjectPosRot[2], ObjectPosRot[3], ObjectPosRot[4], ObjectPosRot[5], true));
                }
			}
		}
	}
	itemid = 0;
	// Spawnpoints
	if(this->m_flags & FLAG_PED)
	{
        rxObjectNode = rxRootNode->first_node("ped");
        if (rxObjectNode != 0)
        {
            for (; rxObjectNode; rxObjectNode = rxObjectNode->next_sibling("ped")) {
                daObjInformation = new float[4];
                try
                {
                    if(rxObjectNode->first_attribute("posX") != 0)daObjInformation[0] = stof(rxObjectNode->first_attribute("posX")->value());
                    else daObjInformation[0] = 0.0f;
                    if(rxObjectNode->first_attribute("posY") != 0)daObjInformation[1] = stof(rxObjectNode->first_attribute("posY")->value());
                    else daObjInformation[1] = 0.0f;
                    if(rxObjectNode->first_attribute("posZ") != 0)daObjInformation[2] = stof(rxObjectNode->first_attribute("posZ")->value());
                    else daObjInformation[2] = 0.0f;
                    if(rxObjectNode->first_attribute("rotZ") != 0)daObjInformation[3] = stof(rxObjectNode->first_attribute("rotZ")->value());
                    else if(rxObjectNode->first_attribute("rotation"))daObjInformation[3] = stof(rxObjectNode->first_attribute("rotation")->value());
                    else daObjInformation[3] = 0.0f;
                    this->m_vSpawnpoints.push_back(daObjInformation);
                    itemid++;
                }
                catch (const exception& e)
                {
                    delete[] daObjInformation;
                    sampgdk::logprintf("File: %s | Error was: P_%d:%s", this->m_szMapPath, itemid, e.what());
                    WarnForError((cell)this);
                    return false;
                }
            }
        }
	}else
	{
        rxObjectNode = rxRootNode->first_node("spawnpoint");
        if (rxObjectNode != 0)
        {
            for (; rxObjectNode; rxObjectNode = rxObjectNode->next_sibling("spawnpoint")) {
                daObjInformation = new float[5];
                try
                {
                    if(rxObjectNode->first_attribute("vehicle") != 0)daObjInformation[0] = stof(rxObjectNode->first_attribute("vehicle")->value());
                    else daObjInformation[0] = 0.0f;
                    if(rxObjectNode->first_attribute("posX") != 0)daObjInformation[1] = stof(rxObjectNode->first_attribute("posX")->value());
                    else daObjInformation[1] = 0.0f;
                    if(rxObjectNode->first_attribute("posY") != 0)daObjInformation[2] = stof(rxObjectNode->first_attribute("posY")->value());
                    else daObjInformation[2] = 0.0f;
                    if(rxObjectNode->first_attribute("posZ") != 0)daObjInformation[3] = stof(rxObjectNode->first_attribute("posZ")->value());
                    else daObjInformation[3] = 0.0f;
                    if(rxObjectNode->first_attribute("rotZ") != 0)daObjInformation[4] = stof(rxObjectNode->first_attribute("rotZ")->value());
                    else if(rxObjectNode->first_attribute("rotation"))daObjInformation[4] = stof(rxObjectNode->first_attribute("rotation")->value());
                    else daObjInformation[4] = 0.0f;
                    if(daObjInformation[0] >= 400 && daObjInformation[0] <= 611)this->m_vSpawnpoints.push_back(daObjInformation);
                    else
                    {
                        delete[] daObjInformation;
                    }
                    itemid++;
                }
                catch (const exception& e)
                {
                    delete[] daObjInformation;
                    sampgdk::logprintf("File: %s | Error was: S_%d:%s", this->m_szMapPath, itemid, e.what());
                    WarnForError((cell)this);
                    return false;
                }
            }
        }
	}
	itemid = 0;
	// Race Pickups
	if(this->m_flags & FLAG_PICKUP)
	{
        rxObjectNode = rxRootNode->first_node("pickup");
        if (rxObjectNode != 0)
        {
            for (; rxObjectNode; rxObjectNode = rxObjectNode->next_sibling("pickup")) {
                daObjInformation = new float[3];
                try
                {
                    if(rxObjectNode->first_attribute("posX") != 0)daObjInformation[0] = stof(rxObjectNode->first_attribute("posX")->value());
                    else daObjInformation[0] = 0.0f;
                    if(rxObjectNode->first_attribute("posY") != 0)daObjInformation[1] = stof(rxObjectNode->first_attribute("posY")->value());
                    else daObjInformation[1] = 0.0f;
                    if(rxObjectNode->first_attribute("posZ") != 0)daObjInformation[2] = stof(rxObjectNode->first_attribute("posZ")->value());
                    else daObjInformation[2] = 0.0f;
                    this->m_vPickups.push_back(daObjInformation);
                    itemid++;
                }
                catch (const exception& e)
                {
                    delete[] daObjInformation;
                    sampgdk::logprintf("File: %s | Error was: P_%d:%s", this->m_szMapPath, itemid, e.what());
                    WarnForError((cell)this);
                    return false;
                }
            }
        }
	}else
	{
        rxObjectNode = rxRootNode->first_node("racepickup");
        char aName[64];
        char AracIsimler[212][18] = {
        {"Landstalker"},{"Bravura"},{"Buffalo"},{"Linerunner"},{"Perrenial"},{"Sentinel"},{"Dumper"},{"Firetruck"},{"Trashmaster"},{"Stretch"},{"Manana"},{"Infernus"},{"Voodoo"},{"Pony"},{"Mule"},{"Cheetah"},{"Ambulance"},{"Leviathan"},{"Moonbeam"},{"Esperanto"},{"Taxi"},{"Washington"},
        {"Bobcat"},{"Mr Whoopee"},{"BF Injection"},{"Hunter"},{"Premier"},{"Enforcer"},{"Securicar"},{"Banshee"},{"Predator"},{"Bus"},{"Rhino"},{"Barracks"},{"Hotknife"},{"Trailer 1"},{"Previon"},{"Coach"},{"Cabbie"},{"Stallion"},{"Rumpo"},{"RC Bandit"},{"Romero"},{"Packer"},{"Monster"},
        {"Admiral"},{"Squalo"},{"Seasparrow"},{"Pizzaboy"},{"Tram"},{"Trailer 2"},{"Turismo"},{"Speeder"},{"Reefer"},{"Tropic"},{"Flatbed"},{"Yankee"},{"Caddy"},{"Solair"},{"Berkley's RC Van"},{"Skimmer"},{"PCJ-600"},{"Faggio"},{"Freeway"},{"RC Baron"},{"RC Raider"},{"Glendale"},{"Oceanic"},
        {"Sanchez"},{"Sparrow"},{"Patriot"},{"Quad"},{"Coastguard"},{"Dinghy"},{"Hermes"},{"Sabre"},{"Rustler"},{"ZR-350"},{"Walton"},{"Regina"},{"Comet"},{"BMX"},{"Burrito"},{"Camper"},{"Marquis"},{"Baggage"},{"Dozer"},{"Maverick"},{"News Chopper"},{"Rancher"},{"FBI Rancher"},{"Virgo"},{"Greenwood"},
        {"Jetmax"},{"Hotring"},{"Sandking"},{"Blista Compact"},{"Police Maverick"},{"Boxville"},{"Benson"},{"Mesa"},{"RC Goblin"},{"Hotring Racer A"},{"Hotring Racer B"},{"Bloodring Banger"},{"Rancher"},{"Super GT"},{"Elegant"},{"Journey"},{"Bike"},{"Mountain Bike"},{"Beagle"},{"Cropdust"},{"Stunt"},
        {"Tanker"}, {"Roadtrain"},{"Nebula"},{"Majestic"},{"Buccaneer"},{"Shamal"},{"Hydra"},{"FCR-900"},{"NRG-500"},{"HPV1000"},{"Cement Truck"},{"Tow Truck"},{"Fortune"},{"Cadrona"},{"FBI Truck"},{"Willard"},{"Forklift"},{"Tractor"},{"Combine"},{"Feltzer"},{"Remington"},{"Slamvan"},
        {"Blade"},{"Freight"},{"Streak"},{"Vortex"},{"Vincent"},{"Bullet"},{"Clover"},{"Sadler"},{"Firetruck LA"},{"Hustler"},{"Intruder"},{"Primo"},{"Cargobob"},{"Tampa"},{"Sunrise"},{"Merit"},{"Utility"},{"Nevada"},{"Yosemite"},{"Windsor"},{"Monster A"},{"Monster B"},{"Uranus"},{"Jester"},
        {"Sultan"},{"Stratum"},{"Elegy"},{"Raindance"},{"RC Tiger"},{"Flash"},{"Tahoma"},{"Savanna"},{"Bandito"},{"Freight Flat"},{"Streak Carriage"},{"Kart"},{"Mower"},{"Duneride"},{"Sweeper"},{"Broadway"},{"Tornado"},{"AT-400"},{"DFT-30"},{"Huntley"},{"Stafford"},{"BF-400"},{"Newsvan"},
        {"Tug"},{"Trailer 3"},{"Emperor"},{"Wayfarer"},{"Euros"},{"Hotdog"},{"Club"},{"Freight Carriage"},{"Trailer 3"},{"Andromada"},{"Dodo"},{"RC Cam"},{"Launch"},{"Police Car (LSPD)"},{"Police Car (SFPD)"},{"Police Car (LVPD)"},{"Police Ranger"},{"Picador"},{"S.W.A.T. Van"},{"Alpha"},{"Phoenix"},{"Glendale"},
        {"Sadler"},{"Luggage Trailer A"},{"Luggage Trailer B"},{"Stair Trailer"},{"Boxville"},{"Farm Plow"},{"Utility Trailer"}
        };
        if (rxObjectNode != 0)
        {

            for (; rxObjectNode; rxObjectNode = rxObjectNode->next_sibling("racepickup")) {
                daObjInformation = new float[8];
                switch (rxObjectNode->first_attribute("type")->value_size()) {
                case 6: // repair
                    daObjInformation[0] = 0.0f;
                    break;
                case 5: // nitro
                    daObjInformation[0] = 1.0f;
                    break;
                case 13: // vehiclechange
                    daObjInformation[0] = 2.0f;
                    break;
                default:
                    daObjInformation[0] = 0.0f;
                    break;
                }
                try
                {

                    if(rxObjectNode->first_attribute("vehicle") != 0) {
                        daObjInformation[1] = stof(rxObjectNode->first_attribute("vehicle")->value());
                    }else {
                        daObjInformation[1] = 522.0f;
                    }
                    if(rxObjectNode->first_attribute("posX") != 0)daObjInformation[2] = stof(rxObjectNode->first_attribute("posX")->value());
                    else daObjInformation[2] = 0.0f;
                    if(rxObjectNode->first_attribute("posY") != 0)daObjInformation[3] = stof(rxObjectNode->first_attribute("posY")->value());
                    else daObjInformation[3] = 0.0f;
                    if(rxObjectNode->first_attribute("posZ") != 0)daObjInformation[4] = stof(rxObjectNode->first_attribute("posZ")->value());
                    else daObjInformation[4] = 0.0f;
                    if(daObjInformation[2] > 3000.0f || daObjInformation[2] < -3000.0f || daObjInformation[3] > 3000.0f || daObjInformation[3] < -3000.0f)
                    {
                        if(daObjInformation[0] == 2.0f) {
                            daObjInformation[5] = CreateDynamicObject(19132, daObjInformation[2], daObjInformation[3], daObjInformation[4], 0,0,0, m_arrVirtualWorld);
                            snprintf(aName, 64, "{FFFFFF}| {25FF25}%s {FFFFFF}|",AracIsimler[(int)daObjInformation[1]-400]);
                            daObjInformation[7] = CreateDynamic3DTextLabel(aName, -1, daObjInformation[2], daObjInformation[3], daObjInformation[4], 100.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, m_arrVirtualWorld);
                        }
                        else if(daObjInformation[0] == 1.0f) {
                            daObjInformation[5] = CreateDynamicObject(1010, daObjInformation[2], daObjInformation[3], daObjInformation[4], 0,0,0, m_arrVirtualWorld);
                            daObjInformation[7] = CreateDynamic3DTextLabel("10{FF2525}X {2525FF}Nitro", -1, daObjInformation[2], daObjInformation[3], daObjInformation[4], 100.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, m_arrVirtualWorld);
                        }
                        else daObjInformation[5] = CreateDynamicObject(3096, daObjInformation[2], daObjInformation[3], daObjInformation[4], 0,0,0, m_arrVirtualWorld);
                        daObjInformation[6] = 0.0f;

                    }else
                    {
                        if(daObjInformation[0] == 2.0f) {
                            daObjInformation[5] = CreateDynamicPickup(19132, 23, daObjInformation[2], daObjInformation[3], daObjInformation[4], m_arrVirtualWorld);
                            snprintf(aName, 64, "{FFFFFF}| {25FF25}%s {FFFFFF}|",AracIsimler[(int)daObjInformation[1]-400]);
                            daObjInformation[7] = CreateDynamic3DTextLabel(aName, -1, daObjInformation[2], daObjInformation[3], daObjInformation[4], 100.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, m_arrVirtualWorld);
                        }
                        else if(daObjInformation[0] == 1.0f) {
                            daObjInformation[5] = CreateDynamicPickup(1010, 23, daObjInformation[2], daObjInformation[3], daObjInformation[4], m_arrVirtualWorld);
                            daObjInformation[7] = CreateDynamic3DTextLabel("10{FF2525}X {2525FF}Nitro", -1, daObjInformation[2], daObjInformation[3], daObjInformation[4], 100.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, m_arrVirtualWorld);
                        }
                        else daObjInformation[5] = CreateDynamicPickup(3096, 23, daObjInformation[2], daObjInformation[3], daObjInformation[4], m_arrVirtualWorld);
                        daObjInformation[6] = 1.0f;
                    }

                    this->m_vRacePickups.push_back(daObjInformation);
                    itemid++;
                }
                catch (const exception& e)
                {
                    delete[] daObjInformation;
                    sampgdk::logprintf("File: %s | Error was: R_%d:%s", this->m_szMapPath, itemid, e.what());
                    WarnForError((cell)this);
                    return false;
                }
            }
        }
    }
    itemid = 0;
	// Markers
	if(this->m_flags & FLAG_MARKERS)
	{
        rxObjectNode = rxRootNode->first_node("marker");
        if (rxObjectNode != 0)
        {
            for (; rxObjectNode; rxObjectNode = rxObjectNode->next_sibling("marker")) {
                try
                {
                    if(rxObjectNode->first_attribute("posX") != 0)ObjectPosRot[0] = stof(rxObjectNode->first_attribute("posX")->value());
                    else ObjectPosRot[0] = 0.0f;
                    if(rxObjectNode->first_attribute("posY") != 0)ObjectPosRot[1] = stof(rxObjectNode->first_attribute("posY")->value());
                    else ObjectPosRot[1] = 0.0f;
                    if(rxObjectNode->first_attribute("posZ") != 0)ObjectPosRot[2] = stof(rxObjectNode->first_attribute("posZ")->value());
                    else ObjectPosRot[2] = 0.0f;
                    m_vSAMPObjects.push_back(CreateDynamicObject(1559,
                        ObjectPosRot[0],
                        ObjectPosRot[1],
                        ObjectPosRot[2],
                        0.0,
                        0.0,
                        0.0,
                        this->m_arrVirtualWorld));
                    itemid++;
                }
                catch (const exception& e)
                {
                    sampgdk::logprintf("File: %s | Error was: M_%d:%s", this->m_szMapPath, itemid, e.what());
                    WarnForError((cell)this);
                    return false;
                }
            }
        }
	}
	itemid = 0;
    if(this->m_flags & FLAG_CHECKPS)
	{
        rxObjectNode = rxRootNode->first_node("checkpoint");
        if (rxObjectNode != 0)
        {
            for (; rxObjectNode; rxObjectNode = rxObjectNode->next_sibling("checkpoint")) {
                daObjInformation = new float[5];
                try
                {
                    if(rxObjectNode->first_attribute("posX") != 0)daObjInformation[0] = stof(rxObjectNode->first_attribute("posX")->value());
                    else daObjInformation[0] = 0.0f;
                    if(rxObjectNode->first_attribute("posY") != 0)daObjInformation[1] = stof(rxObjectNode->first_attribute("posY")->value());
                    else daObjInformation[1] = 0.0f;
                    if(rxObjectNode->first_attribute("posZ") != 0)daObjInformation[2] = stof(rxObjectNode->first_attribute("posZ")->value());
                    else daObjInformation[2] = 0.0f;
                    if(rxObjectNode->first_attribute("type") != 0){
                        if(strcmp(rxObjectNode->first_attribute("type")->value(), "checkpoint"))daObjInformation[3] = 1.0f;
                        else daObjInformation[3] = 0.0f;
                    }else daObjInformation[3] = 0.0f;
                    if(rxObjectNode->first_attribute("vehicle") != 0)daObjInformation[4] = stof(rxObjectNode->first_attribute("vehicle")->value());
                    else daObjInformation[4] = INVALID_VEHICLE_ID;
                    m_vRaceCheckps.push_back(daObjInformation);
                    itemid++;
                }
                catch (const exception& e)
                {
                    delete[] daObjInformation;
                    sampgdk::logprintf("File: %s | Error was: C_%d:%s", this->m_szMapPath, itemid, e.what());
                    WarnForError((cell)this);
                    return false;
                }
            }
        }
	}
	// Cleaning
	delete this->m_rxMetaFile;
	delete this->m_rxFile;
	this->create();
	return true;
}
/*
cell m_iPickupAddr;
cell m_iCheckpointAddr;
cell m_iEmptypointAddr;
int m_iCopyAddr = 0;

if (this->m_flags & FLAG_PICKUP)
{
cell * m_iPickupPhysAddr;
cell m_iPickupInfo[3];
int m_iPickupSize = this->m_vPickups.size();

amx_Push(*a, m_iPickupSize);
amx_Allot(*a, m_iPickupSize * 3, &m_iPickupAddr, &m_iPickupPhysAddr);
auto spInformation = this->m_vPickups.begin();
for (; spInformation != this->m_vPickups.end(); ++spInformation) {
daObjInformation = *(spInformation);

m_iPickupInfo[0] = amx_ftoc(daObjInformation[0]);
m_iPickupInfo[1] = amx_ftoc(daObjInformation[1]);
m_iPickupInfo[2] = amx_ftoc(daObjInformation[2]);
memcpy(m_iPickupPhysAddr + m_iCopyAddr, m_iPickupInfo, sizeof(cell) * 3);
m_iCopyAddr += 3;
}
this->m_vPickups.clear();
amx_Push(*a, m_iPickupAddr);

}else if (this->m_flags & FLAG_CHECKPS)
{
cell * m_iCheckpointPhysAddr;
cell m_iCheckpointInfo[5];
m_iCopyAddr = 0;
int m_iCheckpointsSize = this->m_vRaceCheckps.size();

amx_Push(*a, m_iCheckpointsSize);
amx_Allot(*a, m_iCheckpointsSize * 5, &m_iCheckpointAddr, &m_iCheckpointPhysAddr);
auto scInformation = this->m_vRaceCheckps.begin();
for (; scInformation != this->m_vRaceCheckps.end(); ++scInformation) {
daObjInformation = *(scInformation);

m_iCheckpointInfo[0] = amx_ftoc(daObjInformation[0]);
m_iCheckpointInfo[1] = amx_ftoc(daObjInformation[1]);
m_iCheckpointInfo[2] = amx_ftoc(daObjInformation[2]);
m_iCheckpointInfo[3] = amx_ftoc(daObjInformation[3]);
m_iCheckpointInfo[4] = amx_ftoc(daObjInformation[4]);
memcpy(m_iCheckpointPhysAddr + m_iCopyAddr, m_iCheckpointInfo, sizeof(cell) * 5);
m_iCopyAddr += 5;
}
this->m_vRaceCheckps.clear();
amx_Push(*a, m_iCheckpointAddr);
}
else
{
cell* m_iEmptyPhysAddr;
amx_Push(*a, (cell)0);
amx_Allot(*a, 0, &m_iEmptypointAddr, &m_iEmptyPhysAddr);
amx_Push(*a, m_iEmptypointAddr);
}

// Spawnpoints
int m_iSpawnpointSize = this->m_vSpawnpoints.size();

amx_Push(*a, m_iSpawnpointSize);
cell m_iSpawnpointAddr;
cell * m_iSpawnpointPhysAddr;
cell m_iSpawnpointInfo[5];
m_iCopyAddr = 0;
if (this->m_flags & FLAG_PED)
{

amx_Allot(*a, m_iSpawnpointSize * 4, &m_iSpawnpointAddr, &m_iSpawnpointPhysAddr);
auto spInformation = this->m_vSpawnpoints.begin();
for (; spInformation != this->m_vSpawnpoints.end(); ++spInformation) {
daObjInformation = *(spInformation);

m_iSpawnpointInfo[0] = amx_ftoc(daObjInformation[0]);
m_iSpawnpointInfo[1] = amx_ftoc(daObjInformation[1]);
m_iSpawnpointInfo[2] = amx_ftoc(daObjInformation[2]);
m_iSpawnpointInfo[3] = amx_ftoc(daObjInformation[3]);
memcpy(m_iSpawnpointPhysAddr + m_iCopyAddr, m_iSpawnpointInfo, sizeof(cell) * 4);
m_iCopyAddr += 4;
}
}
else
{
amx_Allot(*a, m_iSpawnpointSize * 5, &m_iSpawnpointAddr, &m_iSpawnpointPhysAddr);
auto spInformation = this->m_vSpawnpoints.begin();
for (; spInformation != this->m_vSpawnpoints.end(); ++spInformation) {
daObjInformation = *(spInformation);

m_iSpawnpointInfo[0] = amx_ftoc(daObjInformation[0]);
m_iSpawnpointInfo[1] = amx_ftoc(daObjInformation[1]);
m_iSpawnpointInfo[2] = amx_ftoc(daObjInformation[2]);
m_iSpawnpointInfo[3] = amx_ftoc(daObjInformation[3]);
m_iSpawnpointInfo[4] = amx_ftoc(daObjInformation[4]);
memcpy(m_iSpawnpointPhysAddr + m_iCopyAddr, m_iSpawnpointInfo, sizeof(cell) * 5);
m_iCopyAddr += 5;
}
}
this->m_vSpawnpoints.clear();
amx_Push(*a, m_iSpawnpointAddr);

amx_Exec

amx_Release(*a, m_iSpawnpointAddr);
if (this->m_flags & FLAG_CHECKPS)
{
amx_Release(*a, m_iCheckpointAddr);
}else if (this->m_flags & FLAG_PICKUP)
{
amx_Release(*a, m_iPickupAddr);
}
else
{
amx_Release(*a, m_iEmptypointAddr);
}
*/
// OnMapLoad ( index, name[], duration, weather, time_hour, time_minute, objecount, pickupcount);
bool bf_map::create() {
	lastmap = this;
	int g_pMapLoadedIdx;
	float * daObjInformation;
	for (std::set<AMX*>::iterator a = amxList.begin(); a != amxList.end(); ++a)
	{
		g_pMapLoadedIdx = 0;
		if (!amx_FindPublic(*a, "OnMapLoad", &g_pMapLoadedIdx))
		{
			cell ret;
			if (this->m_flags & FLAG_PICKUP)
				amx_Push(*a, (cell)this->m_vPickups.size());
			else if (this->m_flags & FLAG_CHECKPS)
				amx_Push(*a, (cell)this->m_vRaceCheckps.size());
			else
				amx_Push(*a, (cell)0);
			if (this->m_flags & FLAG_PED)
				amx_Push(*a, (cell)this->m_vSpawnpoints.size());
			else
				amx_Push(*a, (cell)this->m_vSpawnpoints.size());
			amx_Push(*a, (cell)this->m_vRacePickups.size());
			amx_Push(*a, (cell)this->m_vSAMPObjects.size());
			amx_Push(*a, (cell)this->m_iMinute);
			amx_Push(*a, (cell)this->m_iHour);
			amx_Push(*a, (cell)this->m_iWeather);
			amx_Push(*a, (cell)this->m_iDuration);
			cell   m_iMapNameAddr;
			cell * m_iMapNamePhysAddr;
			amx_PushString(*a, &m_iMapNameAddr, &m_iMapNamePhysAddr, this->m_szMapName, 0, 0);
			amx_Push(*a, (cell)this);
			amx_Exec(*a, &ret, g_pMapLoadedIdx);
			amx_Release(*a, m_iMapNameAddr);
		}
	}
	if (this->m_flags & FLAG_PICKUP)
	{
		auto spInformation = this->m_vPickups.begin();
		for (; spInformation != this->m_vPickups.end(); ++spInformation) {
			daObjInformation = *(spInformation);
			delete[] daObjInformation;
		}
	}else if (this->m_flags & FLAG_CHECKPS)
	{
		auto scInformation = this->m_vRaceCheckps.begin();
		for (; scInformation != this->m_vRaceCheckps.end(); ++scInformation) {
			daObjInformation = *(scInformation);
			delete[] daObjInformation;
		}
	}
	if (this->m_flags & FLAG_PED)
	{
		auto spInformation = this->m_vSpawnpoints.begin();
		for (; spInformation != this->m_vSpawnpoints.end(); ++spInformation) {
			daObjInformation = *(spInformation);
			delete[] daObjInformation;
		}
	}
	else
	{
		auto spInformation = this->m_vSpawnpoints.begin();
		for (; spInformation != this->m_vSpawnpoints.end(); ++spInformation) {
			daObjInformation = *(spInformation);
			delete[] daObjInformation;
		}
	}
	this->m_vPickups.clear();
	this->m_vRaceCheckps.clear();
	this->m_vSpawnpoints.clear();
	this->m_bLoaded = true;
	lastmap = nullptr;
	return true;
}


bool bf_map::destroy() {
	if (!m_bLoaded) { return false; }
	vector<int>::iterator pObjIter = this->m_vSAMPObjects.begin();
	for (; pObjIter != this->m_vSAMPObjects.end(); ++pObjIter) {
				DestroyDynamicObject(*(pObjIter));
	}
	if(this->m_flags & FLAG_COLOBJE)
    {
        vector<int>::iterator pObjIter = this->m_vColObjects.begin();
        for (; pObjIter != this->m_vColObjects.end(); ++pObjIter) {
                    CA_DestroyObject(*(pObjIter));
        }
    }
	auto pPicIter = this->m_vRacePickups.begin();
	for (; pPicIter != this->m_vRacePickups.end(); ++pPicIter) {
        if((*pPicIter)[6] == 1.0)DestroyDynamicPickup((int)(*pPicIter)[5]);
        else DestroyDynamicObject((int)(*pPicIter)[5]);
        if((*pPicIter)[0] > 0.0)DestroyDynamic3DTextLabel((int)(*pPicIter)[7]);
        delete[] *(pPicIter);
	}
    this->m_vRacePickups.clear();
	this->m_vSAMPObjects.clear();
	this->m_vColObjects.clear();
	this->m_bLoaded = false;
	return true;
}

