#include "main.h"
#include "lobby.h"
#include <set>
void **ppPluginData;
extern void	*pAMXFunctions;
vector < lobby * > maplar;
vector < lobby * > yuklenecekler;
typedef void(*logprintf_t)(char* format, ...);
logprintf_t logprintf;
milliseconds uptime;
milliseconds nuptime;
std::chrono::duration<double> tmt;
int tickRate = 0;
std::set<AMX*> amxList;
bf_map* lastmap = nullptr;
PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return sampgdk::Supports() | SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES | SUPPORTS_PROCESS_TICK;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];
	bool load = sampgdk::Load(ppData);
	sampgdk::logprintf("Minigames Loaded.");
	return true;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	sampgdk::logprintf("Minigames unLoaded.");
	sampgdk::Unload();
}
PLUGIN_EXPORT void PLUGIN_CALL ProcessTick()
{
    if(yuklenecekler.size() > 0)
    {
        uptime = duration_cast< milliseconds >(
            system_clock::now().time_since_epoch()
        );
        if(yuklenecekler.front()->lobbymap->load()) {
            maplar.push_back(yuklenecekler.front());
        }
        nuptime = duration_cast< milliseconds >(
        system_clock::now().time_since_epoch()
        );
        tmt = (nuptime - uptime);
        tickRate += (int)(tmt.count()*1000);
        yuklenecekler.erase(yuklenecekler.begin());
    }
    tickRate += 5;
    if(tickRate >= 50)
    {
        if(maplar.size() > 0)
        {
            auto pMaps = maplar.begin();
            for (; pMaps != maplar.end(); ++pMaps) {
                (*pMaps)->ProcessTick();
            }
        }
        tickRate = 0;
    }

}
PLUGIN_FUNCTION LoadMap(AMX* amx, cell* params)
{
    lobby * mLobby = (lobby*)params[1];
	if (params[2] >= mLobby->dir->GetDirCount() || params[2] < 0)return 0;
    cell mapid = (cell)mLobby->LoadLobbyMap(params[2], params[3]);
    return mapid;
}
PLUGIN_FUNCTION LoadRandomMap(AMX* amx, cell* params)
{
    lobby * mLobby = (lobby*)params[1];
    cell mapid = (cell)mLobby->LoadLobbyMapRandom(params[2]);
    return mapid;
}
PLUGIN_FUNCTION UnLoadMap(AMX* amx, cell* params)
{
	lobby * mLobby = (lobby*)params[1];
	if(std::find(maplar.begin(), maplar.end(), mLobby) != maplar.end() ||
       std::find(yuklenecekler.begin(), yuklenecekler.end(), mLobby) != yuklenecekler.end() )
	{
        bf_map *cmap = mLobby->lobbymap;
        if (cmap != nullptr)
        {
            if(cmap->m_bLoaded)
            {
                for (auto yukle = maplar.begin(); yukle != maplar.end(); ++yukle)
                {
                    if (mLobby == (*yukle))
                    {
                        maplar.erase(yukle);
                        break;
                    }
                }
                cmap->destroy();
            }

            else
            {
                for (auto yukle = yuklenecekler.begin(); yukle != yuklenecekler.end(); ++yukle)
                {
                    if (mLobby == (*yukle))
                    {
                        yuklenecekler.erase(yukle);
                        break;
                    }
                }
            }
            if(!(mLobby->processrun))delete cmap;
            else {
                sampgdk::logprintf("Process Tick Calisirken Unload yapilamadi!");
                return false;
            }
            mLobby->currentmapid = INVALID_MAP_ID;
            mLobby->lobbymap = nullptr;
            return true;
        }
	}

	return false;
}
PLUGIN_FUNCTION CreateLobby(AMX* amx, cell* params)
{
    char * file;
    lobby * mLobby;
    amx_StrParam(amx, params[1], file);
    if(!file[0])
    {
        return (cell)0;
    }else
    {
        mLobby = new lobby(file,params[2]);
        if(mLobby->dir->GetDirCount() == 0)sampgdk::logprintf("Map bulunamadı!");
    }
	return (cell)mLobby;
}
PLUGIN_FUNCTION DestroyLobby(AMX* amx, cell* params)
{
    lobby * mLobby = (lobby*)params[1];
    if(mLobby != NULL)
    {
        delete mLobby;
        return true;
    }
    return false;
}
PLUGIN_FUNCTION AddPlayerLobby(AMX* amx, cell* params)
{
    if(!IsPlayerConnected(params[1])) return 0;
    lobby * mLobby = (lobby*)params[2];
    if(mLobby != NULL)
    {
        return mLobby->AddPlayerToLobby(params[1]);
    }
    return false;
}
PLUGIN_FUNCTION DelPlayerLobby(AMX* amx, cell* params)
{
    lobby * mLobby = (lobby*)params[2];
    if(mLobby != NULL)
    {
		return mLobby->DelPlayerToLobby(params[1]);
	}
    return false;
}
PLUGIN_FUNCTION IsPlayerInLobby(AMX* amx, cell* params)
{
    lobby * mLobby = (lobby*)params[2];
    if(mLobby != NULL)
    {
        return mLobby->IsPlayerInLobby(params[1]);
    }
    return false;
}
PLUGIN_FUNCTION AddVehicleLobby(AMX* amx, cell* params)
{
    if(!IsValidVehicle(params[1])) return 0;
    lobby * mLobby = (lobby*)params[2];
    if(mLobby != NULL)
    {
		return mLobby->AddVehicleToLobby(params[1]);
	}
    return false;
}
PLUGIN_FUNCTION DelVehicleLobby(AMX* amx, cell* params)
{
    lobby * mLobby = (lobby*)params[2];
    if(mLobby != NULL)
	{
		return mLobby->DelVehicleToLobby(params[1]);
	}
    return false;
}
PLUGIN_FUNCTION IsVehicleInLobby(AMX* amx, cell* params)
{
    lobby * mLobby = (lobby*)params[2];
    if(mLobby != NULL)
    {
        return mLobby->IsVehicleInLobby(params[1]);
    }
    return false;
}
PLUGIN_FUNCTION AddProjectile(AMX* amx, cell* params)
{
    lobby * mLobby = (lobby*)params[1];
    if(mLobby != NULL)
    {
        return mLobby->AddProjectile(params[2],params[3]);
    }
    return false;
}
PLUGIN_FUNCTION AddTrap(AMX* amx, cell* params)
{
    lobby * mLobby = (lobby*)params[1];
    if(mLobby != NULL)
    {
        return (cell)mLobby->AddTrap(params[2]);
    }
    return 0;
}
PLUGIN_FUNCTION DestroyTrap(AMX* amx, cell* params)
{
    lobby * mLobby = (lobby*)params[1];
    if(mLobby != NULL)
    {
        Trap * trap = (Trap*)params[2];
        if(trap != NULL)
        {
            return mLobby->DestroyTrap(trap);
        }
    }
    return 0;
}
PLUGIN_FUNCTION DestroyAllTrap(AMX* amx, cell* params)
{
    lobby * mLobby = (lobby*)params[1];
    if(mLobby != NULL)
    {
        mLobby->DestroyAllTrap();
        return 1;
    }
    return 0;
}
PLUGIN_FUNCTION GetMapCount(AMX* amx, cell* params)
{
    lobby * mLobby = (lobby*)params[1];
    if(mLobby != NULL)
    {
        return mLobby->GetMapCount();
    }
    return 0;
}
PLUGIN_FUNCTION SetLobbyCheckCol(AMX* amx, cell* params)
{
    lobby * mLobby = (lobby*)params[1];
    if(mLobby != NULL)
    {
        mLobby->SetLobbyCheckCol((bool)params[2]);
        return 1;
    }
    return 0;
}
PLUGIN_FUNCTION GetVehicleLastHit(AMX* amx, cell* params)
{
    lobby * mLobby = (lobby*)params[1];
    if(mLobby != NULL)
    {
        return mLobby->GetVehicleLastHit(params[2]);
    }
    return 0;
}
PLUGIN_FUNCTION ResetVehicleLastHit(AMX* amx, cell* params)
{
    lobby * mLobby = (lobby*)params[1];
    if(mLobby != NULL)
    {
        return mLobby->ResetVehicleLastHit(params[2]);
    }
    return 0;
}
PLUGIN_FUNCTION GetVehicleLastShooter(AMX* amx, cell* params)
{
    lobby * mLobby = (lobby*)params[1];
    if(mLobby != NULL)
    {
        return mLobby->GetVehicleLastShooter(params[2]);
    }
    return 0;
}
PLUGIN_FUNCTION ResetVehicleLastShooter(AMX* amx, cell* params)
{
    lobby * mLobby = (lobby*)params[1];
    if(mLobby != NULL)
    {
        return mLobby->ResetVehicleLastShooter(params[2]);
    }
    return 0;
}
PLUGIN_FUNCTION GetMapInfo(AMX* amx, cell* params)
{
    lobby * mLobby = (lobby*)params[1];
    if(mLobby != NULL)
    {
        if (params[2] >= mLobby->dir->GetDirCount() || params[2] < 0)return 0;
        cell* addr = NULL;
        amx_GetAddr(amx, params[3], &addr);
        amx_SetString(addr, mLobby->dir->dirlist.at(params[2])->GetMapName(), 0, 0, params[4]);
    }
    return 0;
}

PLUGIN_FUNCTION GetMapLocation(AMX* amx, cell* params)
{
    lobby * mLobby = (lobby*)params[1];
    if(mLobby != NULL)
    {
        if (params[2] >= mLobby->dir->GetDirCount() || params[2] < 0)return 0;
        cell* addr = NULL;
        amx_GetAddr(amx, params[3], &addr);
        amx_SetString(addr, mLobby->dir->dirlist.at(params[2])->GetMapLocation(), 0, 0, params[4]);
    }
    return 0;
}
PLUGIN_FUNCTION GetMapLastStart(AMX* amx, cell* params)
{
    lobby * mLobby = (lobby*)params[1];
    if(mLobby != NULL)
    {
        if (params[2] >= mLobby->dir->GetDirCount() || params[2] < 0)return 0;
        return (cell)mLobby->dir->dirlist.at(params[2])->GetLastStartTime();
    }
    return 0;
}
PLUGIN_FUNCTION GetCurrentMapID(AMX* amx, cell* params)
{
    lobby * mLobby = (lobby*)params[1];
    if(mLobby != NULL)
    {
        return mLobby->currentmapid;
    }
    return INVALID_MAP_ID;
}
PLUGIN_FUNCTION DeleteMap(AMX* amx, cell* params)
{
    lobby * mLobby = (lobby*)params[1];
    if(mLobby != NULL)
    {
        return mLobby->DeleteMap(params[2]);
    }
    return 0;
}
//GetMapCheckPoint(cpid, checkpoints[], checkpoint_size);
PLUGIN_FUNCTION	GetMapCheckPoint(AMX* amx, cell* params)
{
	if (lastmap != nullptr)
	{
		cell *cptr = NULL;
		int size = params[3];
		size_t id = params[1];
		amx_GetAddr(amx, params[2], &cptr);
		if (lastmap->m_flags & FLAG_PICKUP)
		{
			if (id >= lastmap->m_vPickups.size() || size < 3)return false;
			auto spInformation = lastmap->m_vPickups.at(id);
			for(int i = 0; i < 3;i++)cptr[i] = amx_ftoc(spInformation[i]);
		}
		else if (lastmap->m_flags & FLAG_CHECKPS)
		{
			if (id >= lastmap->m_vRaceCheckps.size() || size < 5)return false;
			auto scInformation = lastmap->m_vRaceCheckps.at(id);
			for (int i = 0; i < 5; i++)cptr[i] = amx_ftoc(scInformation[i]);
		}
		return true;
	}
	return false;
}
//GetMapSpawnPoint(spid, spawnpoint[], spawnpoint_size);
PLUGIN_FUNCTION	GetMapSpawnPoint(AMX* amx, cell* params)
{
	if (lastmap != nullptr)
	{
		cell *cptr = NULL;
		int size = params[3];
		size_t id = params[1];
		amx_GetAddr(amx, params[2], &cptr);
		if (lastmap->m_flags & FLAG_PED)
		{
			if (id >= lastmap->m_vSpawnpoints.size() || size < 4)return false;
			auto spInformation = lastmap->m_vSpawnpoints.at(id);
			for (int i = 0; i < 4; i++)cptr[i] = amx_ftoc(spInformation[i]);
		}
		else
		{
			if (id >= lastmap->m_vSpawnpoints.size() || size < 5)return false;
			auto spInformation = lastmap->m_vSpawnpoints.at(id);
			for (int i = 0; i < 5; i++)cptr[i] = amx_ftoc(spInformation[i]);
		}
		return true;
	}
	return false;
}
const AMX_NATIVE_INFO ListNatives[] =
{
	{ "LoadMap", LoadMap },
	{ "LoadRandomMap", LoadRandomMap},
	{ "UnLoadMap", UnLoadMap},
	{ "CreateLobby", CreateLobby},
	{ "DestroyLobby", DestroyLobby},
	{ "AddPlayerLobby", AddPlayerLobby},
	{ "DelPlayerLobby", DelPlayerLobby},
	{ "IsPlayerInLobby", IsPlayerInLobby},
	{ "GetMapCount", GetMapCount},
	{ "AddVehicleLobby", AddVehicleLobby},
	{ "DelVehicleLobby", DelVehicleLobby},
	{ "IsVehicleInLobby", IsVehicleInLobby},
	{ "AddProjectile", AddProjectile},
	{ "SetLobbyCheckCol", SetLobbyCheckCol},
	{ "GetVehicleLastHit", GetVehicleLastHit},
	{ "ResetVehicleLastHit", ResetVehicleLastHit},
	{ "GetVehicleLastShooter", GetVehicleLastShooter},
	{ "ResetVehicleLastShooter", ResetVehicleLastShooter},
	{ "AddTrap", AddTrap},
	{ "DestroyTrap", DestroyTrap},
	{ "DestroyAllTrap", DestroyAllTrap},
	{ "GetMapInfo", GetMapInfo},
	{ "GetMapLastStart", GetMapLastStart},
	{ "GetCurrentMapID", GetCurrentMapID},
	{ "DeleteMap", DeleteMap},
    { "GetMapLocation", GetMapLocation},
	{ "GetMapCheckPoint", GetMapCheckPoint },
	{ "GetMapSpawnPoint", GetMapSpawnPoint },
	{ NULL, NULL }
};
PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *amx)
{
	amxList.insert(amx);
	return amx_Register(amx, ListNatives, -1);
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *amx)
{
	amxList.erase(amx);
	return AMX_ERR_NONE;
}
