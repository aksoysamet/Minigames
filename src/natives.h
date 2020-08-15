#ifndef NATIVE_H_INCLUDED

#define NATIVE_H_INCLUDED

AMX_NATIVE native_GetVehiclePos;
AMX_NATIVE native_GetPlayerVehicleID;
AMX_NATIVE native_GetVehicleZAngle;
AMX_NATIVE native_GetVehicleRotationQuat;
AMX_NATIVE native_GetVehicleModel;
AMX_NATIVE native_GetVehicleDamageStatus;
AMX_NATIVE native_UpdateVehicleDamageStatus;
AMX_NATIVE native_GetVehicleModelInfo;
AMX_NATIVE native_GetVehicleHealth;
AMX_NATIVE native_IsValidVehicle;
AMX_NATIVE native_IsPlayerInAnyVehicle;
AMX_NATIVE native_IsPlayerConnected;
AMX_NATIVE native_GetVehicleDistanceFromPoint;
AMX_NATIVE native_CreateExplosionForPlayer;
AMX_NATIVE native_GetPlayerState;
AMX_NATIVE native_IsPlayerInRangeOfPoint;
AMX_NATIVE native_AddVehicleComponent;
AMX_NATIVE native_GetPVarInt;
AMX_NATIVE native_SetPVarInt;
AMX_NATIVE native_PlayerPlaySound;
AMX_NATIVE native_CA_CreateObject;
AMX_NATIVE native_CreateDynamicObject;
AMX_NATIVE native_CreateDynamic3DTextLabel;
AMX_NATIVE native_CreateDynamicPickup;
AMX_NATIVE native_DestroyDynamicObject;
AMX_NATIVE native_DestroyDynamicPickup;
AMX_NATIVE native_CA_RayCastLine;

void InitScripting(AMX *amx);
#endif