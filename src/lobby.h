#ifndef LOBBY_H
#define LOBBY_H
#include "main.h"
#include "maploader.h"
#include "dirlister.h"
#include "Projectile.h"
#include "Trap.h"
#define INVALID_MAP_ID (65535)
struct VehicleData
{
    int vehicleid;
    int vehiclemodel;
    float vehsize;
    float vehicledamage;
    int lastshotplayerid = INVALID_PLAYER_ID;
    int lasthitvehicle = INVALID_VEHICLE_ID;
    int TireCount;
    float TirePos1[3],
          TirePos2[3],
          TirePos3[3],
          TirePos4[3],
          TireVeh1[3],
          TireVeh2[3];
};
class lobby
{
    public:
        lobby(const char * direc, int virtualid);
        virtual ~lobby();
        bf_map * LoadLobbyMap(int mapid, int flags);
        bf_map * LoadLobbyMapRandom(int flags);
        int AddPlayerToLobby(int playerid);
        int DelPlayerToLobby(int playerid);
        bool IsPlayerInLobby(int playerid);
        int AddVehicleToLobby(int playerid);
        int DelVehicleToLobby(int playerid);
        bool IsVehicleInLobby(int playerid);
        bool AddProjectile(int playerid, int vehid);
        bool DeleteMap(int mapid);
        void SetLobbyCheckCol(bool col);
        int GetVehicleLastHit(int vehid);
        bool ResetVehicleLastHit(int vehid);
        int GetVehicleLastShooter(int vehid);
        bool ResetVehicleLastShooter(int vehid);
        int GetMapCount();
        int AddTrap(int playerid);
        bool DestroyTrap(Trap *trap);
        void DestroyAllTrap();
        void    ProcessTick();
        dir_list * dir;
        bf_map * lobbymap = nullptr;
        vector < int >      m_LobbyPlayers;
        vector < VehicleData > m_LobbyVehicles;
        vector < Projectile*> m_Projectiles;
        vector < Trap*> m_Traps;
        bool processrun = false;
        int currentmapid = INVALID_MAP_ID;
    private:
        char * dirfolder;
        int virtaulworldid;
        bool VehicleCol = false;
};

#endif // LOBBY_H
