#include "main.h"
void InitScripting(AMX *amx)
{
	AMX_HEADER *hdr = reinterpret_cast<AMX_HEADER*>(amx->base);
	AMX_FUNCSTUBNT *natives = reinterpret_cast<AMX_FUNCSTUBNT*>(amx->base + hdr->natives);
	int index = 0;
	if (amx_FindNative(amx, "GetVehiclePos", &index) == AMX_ERR_NONE) {
		::native_GetVehiclePos = reinterpret_cast<AMX_NATIVE>(natives[index].address);
	}
	index = 0;
	if (amx_FindNative(amx, "GetPlayerVehicleID", &index) == AMX_ERR_NONE) {
		::native_GetPlayerVehicleID = reinterpret_cast<AMX_NATIVE>(natives[index].address);
	}
	index = 0;
	if (amx_FindNative(amx, "GetVehicleZAngle", &index) == AMX_ERR_NONE) {
		::native_GetVehicleZAngle = reinterpret_cast<AMX_NATIVE>(natives[index].address);
	}
	index = 0;
	if (amx_FindNative(amx, "GetVehicleRotationQuat", &index) == AMX_ERR_NONE) {
		::native_GetVehicleRotationQuat = reinterpret_cast<AMX_NATIVE>(natives[index].address);
	}
	index = 0;
	if (amx_FindNative(amx, "GetVehicleModel", &index) == AMX_ERR_NONE) {
		::native_GetVehicleModel = reinterpret_cast<AMX_NATIVE>(natives[index].address);
	}
	index = 0;
	if (amx_FindNative(amx, "GetVehicleDamageStatus", &index) == AMX_ERR_NONE) {
		::native_GetVehicleDamageStatus = reinterpret_cast<AMX_NATIVE>(natives[index].address);
	}
	index = 0;
	if (amx_FindNative(amx, "UpdateVehicleDamageStatus", &index) == AMX_ERR_NONE) {
		::native_UpdateVehicleDamageStatus = reinterpret_cast<AMX_NATIVE>(natives[index].address);
	}
	index = 0;
	if (amx_FindNative(amx, "GetVehicleModelInfo", &index) == AMX_ERR_NONE) {
		::native_GetVehicleModelInfo = reinterpret_cast<AMX_NATIVE>(natives[index].address);
	}
	index = 0;
	if (amx_FindNative(amx, "GetVehicleHealth", &index) == AMX_ERR_NONE) {
		::native_GetVehicleHealth = reinterpret_cast<AMX_NATIVE>(natives[index].address);
	}
	index = 0;
	if (amx_FindNative(amx, "IsValidVehicle", &index) == AMX_ERR_NONE) {
		::native_IsValidVehicle = reinterpret_cast<AMX_NATIVE>(natives[index].address);
	}
	index = 0;
	if (amx_FindNative(amx, "IsPlayerInAnyVehicle", &index) == AMX_ERR_NONE) {
		::native_IsPlayerInAnyVehicle = reinterpret_cast<AMX_NATIVE>(natives[index].address);
	}
	index = 0;
	if (amx_FindNative(amx, "IsPlayerConnected", &index) == AMX_ERR_NONE) {
		::native_IsPlayerConnected = reinterpret_cast<AMX_NATIVE>(natives[index].address);
	}
	index = 0;
	if (amx_FindNative(amx, "GetVehicleDistanceFromPoint", &index) == AMX_ERR_NONE) {
		::native_GetVehicleDistanceFromPoint = reinterpret_cast<AMX_NATIVE>(natives[index].address);
	}
	index = 0;
	if (amx_FindNative(amx, "CreateExplosionForPlayer", &index) == AMX_ERR_NONE) {
		::native_CreateExplosionForPlayer = reinterpret_cast<AMX_NATIVE>(natives[index].address);
	}
	index = 0;
	if (amx_FindNative(amx, "GetPlayerState", &index) == AMX_ERR_NONE) {
		::native_GetPlayerState = reinterpret_cast<AMX_NATIVE>(natives[index].address);
	}
	index = 0;
	if (amx_FindNative(amx, "IsPlayerInRangeOfPoint", &index) == AMX_ERR_NONE) {
		::native_IsPlayerInRangeOfPoint = reinterpret_cast<AMX_NATIVE>(natives[index].address);
	}
	index = 0;
	if (amx_FindNative(amx, "AddVehicleComponent", &index) == AMX_ERR_NONE) {
		::native_AddVehicleComponent = reinterpret_cast<AMX_NATIVE>(natives[index].address);
	}
	index = 0;
	if (amx_FindNative(amx, "GetPVarInt", &index) == AMX_ERR_NONE) {
		::native_GetPVarInt = reinterpret_cast<AMX_NATIVE>(natives[index].address);
	}
	index = 0;
	if (amx_FindNative(amx, "SetPVarInt", &index) == AMX_ERR_NONE) {
		::native_SetPVarInt = reinterpret_cast<AMX_NATIVE>(natives[index].address);
	}
	index = 0;
	if (amx_FindNative(amx, "PlayerPlaySound", &index) == AMX_ERR_NONE) {
		::native_PlayerPlaySound = reinterpret_cast<AMX_NATIVE>(natives[index].address);
	}
	index = 0;
	if (amx_FindNative(amx, "CA_CreateObject", &index) == AMX_ERR_NONE) {
		::native_CA_CreateObject = reinterpret_cast<AMX_NATIVE>(natives[index].address);
	}
	index = 0;
	if (amx_FindNative(amx, "CreateDynamicObject", &index) == AMX_ERR_NONE) {
		::native_CreateDynamicObject = reinterpret_cast<AMX_NATIVE>(natives[index].address);
	}
	index = 0;
	if (amx_FindNative(amx, "CreateDynamic3DTextLabel", &index) == AMX_ERR_NONE) {
		::native_CreateDynamic3DTextLabel = reinterpret_cast<AMX_NATIVE>(natives[index].address);
	}
	index = 0;
	if (amx_FindNative(amx, "CreateDynamicPickup", &index) == AMX_ERR_NONE) {
		::native_CreateDynamicPickup = reinterpret_cast<AMX_NATIVE>(natives[index].address);
	}
	index = 0;
	if (amx_FindNative(amx, "DestroyDynamicObject", &index) == AMX_ERR_NONE) {
		::native_DestroyDynamicObject = reinterpret_cast<AMX_NATIVE>(natives[index].address);
	}
	index = 0;
	if (amx_FindNative(amx, "DestroyDynamicPickup", &index) == AMX_ERR_NONE) {
		::native_DestroyDynamicPickup = reinterpret_cast<AMX_NATIVE>(natives[index].address);
	}
	index = 0;
	if (amx_FindNative(amx, "CA_RayCastLine", &index) == AMX_ERR_NONE) {
		::native_CA_RayCastLine = reinterpret_cast<AMX_NATIVE>(natives[index].address);
	}
}
