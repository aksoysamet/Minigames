

#ifndef SAMPGDK_STREAMER_H
#define SAMPGDK_STREAMER_H

SAMPGDK_NATIVE(int, CreateDynamicObject(int modelid, float x, float y, float z, float rx, float ry, float rz, int worldid, int interiorid, int playerid, float streamdistance, float drawdistance, int areaid, int priority));
SAMPGDK_NATIVE(bool, DestroyDynamicObject ( int objectid ) );
SAMPGDK_NATIVE(int, CreateDynamicObjectEx(int modelid, float x, float y, float z, float rx, float ry, float rz, float drawdistance, float streamdistance, int worlds[], int interiors[], int players[], int areas[], int priority, int maxworlds, int maxinteriors, int maxplayers, int maxareas));
SAMPGDK_NATIVE(bool, IsValidDynamicObject ( int objectid ) );
SAMPGDK_NATIVE(bool, SetDynamicObjectPos ( int objectid, float x, float y, float z ) );
SAMPGDK_NATIVE(bool, GetDynamicObjectPos ( int objectid, float * x, float * y, float * z ) );
SAMPGDK_NATIVE(bool, SetDynamicObjectRot ( int objectid, float rx, float ry, float rz ) );
SAMPGDK_NATIVE(bool, GetDynamicObjectRot ( int objectid, float * rx, float * ry, float * rz ) );
//Sonradan eklenenler
SAMPGDK_NATIVE(bool, Streamer_Update ( int playerid, int type ) );

SAMPGDK_NATIVE(int, CreateDynamic3DTextLabel(const char * text,int color, float x, float y, float z, float drawdistance, int attachedplayer, int attachedvehicle, int testlos, int worldid, int interiorid, int playerid, float streamdistance, int areaid, int priority) );
SAMPGDK_NATIVE(bool, DestroyDynamic3DTextLabel ( int id) );
SAMPGDK_NATIVE(bool, IsValidDynamic3DTextLabel ( int id) );

SAMPGDK_NATIVE(int, CreateDynamicPickup(int modelid, int type, float x, float y, float z, int worldid, int interiorid, int playerid, float streamdistance, int areaid, int priority) );
SAMPGDK_NATIVE(bool, DestroyDynamicPickup ( int id) );
SAMPGDK_NATIVE(bool, IsValidDynamicPickup ( int id) );
SAMPGDK_NATIVE(int, CA_RayCastLine ( float StartX, float StartY, float StartZ, float EndX, float EndY, float EndZ, float * x, float * y, float * z) );
SAMPGDK_NATIVE(int, CA_CreateObject ( int modelid, float x, float y, float z, float rx, float ry, float rz, bool add) );
SAMPGDK_NATIVE(bool, CA_DestroyObject ( int index) );
SAMPGDK_NATIVE(int, MoveDynamicObject ( int objectid, float x, float y, float z, float speed, float rx, float ry, float rz ) );
SAMPGDK_NATIVE(bool, SyncRepairVehicle ( int vehid) );
SAMPGDK_NATIVE(bool, SyncSetVehicleHealth ( int vehid, float health) );

//#ifndef DOXYGEN

//#if defined SAMPGDK_CPP_WRAPPERS && !defined IN_SAMPGDK

//namespace sampgdk {

inline int CreateDynamicObject(int modelid, float x, float y, float z, float rx, float ry, float rz, int worldid=-1, int interiorid=-1, int playerid=-1, float streamdistance=300.0, float drawdistance=0.0, int areaid = -1, int priority = 0) {
	return sampgdk_CreateDynamicObject(modelid,x,y,z,rx,ry,rz,worldid,interiorid, playerid, streamdistance, drawdistance, areaid, priority);
}

inline int DestroyDynamicObject(int objectid) {
	return sampgdk_DestroyDynamicObject(objectid);
}

/*inline int CreateDynamicObjectEx(int modelid, float x, float y, float z, float rx, float ry, float rz, int worlds[],  int maxworlds) {
	static int static_interiors[] = { -1 };
	static int static_players[] = { -1 };
	return sampgdk_CreateDynamicObjectEx(modelid, x, y, z, rx, ry, rz, 0.0f, 200.0f, worlds, static_interiors, static_players, maxworlds, 1, 1);
}    */
//Sonradan eklenenler
inline int GetDynamicObjectPos ( int objectid, float * x, float * y, float * z ) {
	return sampgdk_GetDynamicObjectPos(objectid,x,y,z);
}

inline int MoveDynamicObject(int objectid, float x, float y, float z, float speed, float rx = -1000.0, float ry = -1000.0, float rz = -1000.0) {
	return sampgdk_MoveDynamicObject(objectid,x,y,z,speed,rx,ry,rz);
}
inline int Streamer_Update(int playerid, int type=-1) {
	return sampgdk_Streamer_Update(playerid,type);
}
inline int CreateDynamic3DTextLabel(const char * text,int color, float x, float y, float z, float drawdistance, int attachedplayer = INVALID_PLAYER_ID, int attachedvehicle = INVALID_VEHICLE_ID, int testlos = 0, int worldid = -1, int interiorid = -1, int playerid = -1, float streamdistance = 300.0, int areaid = -1, int priority = 0) {
	return sampgdk_CreateDynamic3DTextLabel(text, color, x, y, z, drawdistance, attachedplayer, attachedvehicle, testlos, worldid, interiorid, playerid, streamdistance, areaid, priority);
}

inline int CreateDynamicPickup(int modelid, int type, float x, float y, float z, int worldid = -1, int interiorid = -1, int playerid = -1, float streamdistance = 300.0, int areaid = -1, int priority = 0) {
	return sampgdk_CreateDynamicPickup(modelid, type, x, y, z, worldid, interiorid, playerid, streamdistance, areaid, priority);
}
inline int DestroyDynamicPickup(int objectid) {
	return sampgdk_DestroyDynamicPickup(objectid);
}
inline int DestroyDynamic3DTextLabel(int objectid) {
	return sampgdk_DestroyDynamic3DTextLabel(objectid);
}
inline int CA_RayCastLine ( float StartX, float StartY, float StartZ, float EndX, float EndY, float EndZ, float * x, float * y, float * z) {
	return sampgdk_CA_RayCastLine ( StartX, StartY, StartZ, EndX, EndY, EndZ, x, y, z);
}
inline int CA_CreateObject ( int modelid, float x, float y, float z, float rx, float ry, float rz, bool add) {
	return sampgdk_CA_CreateObject ( modelid, x, y, z, rx, ry, rz, add);
}
inline int CA_DestroyObject ( int index) {
	return sampgdk_CA_DestroyObject ( index);
}

inline int SyncRepairVehicle ( int vehid) {
	return sampgdk_SyncRepairVehicle ( vehid);
}
inline int SyncSetVehicleHealth ( int vehid, float health)  {
	return sampgdk_SyncSetVehicleHealth ( vehid, health);
}


//} // namespace sampgdk

//#else /* SAMPGDK_CPP_WRAPPERS && !IN_SAMPGDK */

//#undef  CreateDynamicObject
//#define CreateDynamicObject sampgdk_CreateDynamicObject

//#endif /* !SAMPGDK_CPP_WRAPPERS || IN_SAMPGDK */
//#endif /* !DOXYGEN */
#endif /* SAMPGDK_STREAMER_H */

//native CreateDynamicObjectEx(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, Float:drawdistance = 0.0, Float:streamdistance = 200.0, worlds[] = { -1 }, interiors[] = { -1 }, players[] = { -1 }, maxworlds = sizeof worlds, maxinteriors = sizeof interiors, maxplayers = sizeof players);
