SAMPGDK_NATIVE(int, CreateDynamicObject(int modelid, float x, float y, float z, float rx, float ry, float rz, int worldid, int interiorid, int playerid, float streamdistance, float drawdistance, int areaid, int priority)) {
	static AMX_NATIVE native;
	cell retval;
	cell params[15];
	if (SAMPGDK_UNLIKELY(native == NULL)) {
		native = sampgdk_native_find_warn_stub("CreateDynamicObject");
	}
	params[0] = 14 * sizeof(cell);
	params[1] = (cell)modelid;
	params[2] = amx_ftoc(x);
	params[3] = amx_ftoc(y);
	params[4] = amx_ftoc(z);
	params[5] = amx_ftoc(rx);
	params[6] = amx_ftoc(ry);
	params[7] = amx_ftoc(rz);
	params[8] = (cell)worldid;
	params[9] = (cell)interiorid;
	params[10] = (cell)playerid;
	params[11] = amx_ftoc(streamdistance);
	params[12] = amx_ftoc(drawdistance);
	params[13] = (cell)areaid;
	params[14] = (cell)priority;
	retval = native(sampgdk_fakeamx_amx(), params);
	return (int)retval;
}

SAMPGDK_NATIVE(int, CreateDynamicObjectEx(int modelid, float x, float y, float z, float rx, float ry, float rz, float drawdistance, float streamdistance, int worlds[], int interiors[], int players[], int areas[], int priority, int maxworlds, int maxinteriors, int maxplayers, int maxareas)) {
	static AMX_NATIVE native;
	cell retval;
	cell params[19];
	if (SAMPGDK_UNLIKELY(native == NULL)) {
		native = sampgdk_native_find_warn_stub("CreateDynamicObjectEx");
	}
	params[0] = 18 * sizeof(cell);
	params[1] = (cell)modelid;
	params[2] = amx_ftoc(x);
	params[3] = amx_ftoc(y);
	params[4] = amx_ftoc(z);
	params[5] = amx_ftoc(rx);
	params[6] = amx_ftoc(ry);
	params[7] = amx_ftoc(rz);
	params[8] = amx_ftoc(drawdistance);
	params[9] = amx_ftoc(streamdistance);
	cell iWorldAddr, iInteriorAddr, iPlayerAddr, iAreaAddr;
	cell * iWorldPhysAddr, * iInteriorPhysAddr, * iPlayerPhysAddr, * iAreaPhysAddr;
	amx_Allot(sampgdk_fakeamx_amx(), maxworlds, &iWorldAddr, &iWorldPhysAddr);
	memcpy ( iWorldPhysAddr, worlds, maxworlds * sizeof(cell) );
	amx_Allot(sampgdk_fakeamx_amx(), maxinteriors, &iInteriorAddr, &iInteriorPhysAddr);
	memcpy ( iInteriorPhysAddr, interiors, maxinteriors * sizeof(cell) );
	amx_Allot(sampgdk_fakeamx_amx(), maxplayers, &iPlayerAddr, &iPlayerPhysAddr);
	memcpy ( iPlayerPhysAddr, players, maxplayers * sizeof(cell) );
	amx_Allot(sampgdk_fakeamx_amx(), maxareas, &iAreaAddr, &iAreaPhysAddr);
	memcpy(iAreaPhysAddr, areas, maxareas * sizeof(cell));
	params[10] = iWorldAddr;
	params[11] = iInteriorAddr;
	params[12] = iPlayerAddr;
	params[13] = iAreaAddr;
	params[14] = priority;
	params[15] = maxworlds;
	params[16] = maxinteriors;
	params[17] = maxplayers;
	params[18] = maxareas;
	retval = native(sampgdk_fakeamx_amx(), params);
	amx_Release(sampgdk_fakeamx_amx(),  iWorldAddr);
	amx_Release(sampgdk_fakeamx_amx(),  iInteriorAddr);
	amx_Release(sampgdk_fakeamx_amx(),  iPlayerAddr);
	return (int)retval;
}

SAMPGDK_NATIVE(bool, DestroyDynamicObject ( int objectid ) ) {
	static AMX_NATIVE native;
	cell retval;
	cell params[2];
	if (SAMPGDK_UNLIKELY(native == NULL)) {
		native = sampgdk_native_find_warn_stub("DestroyDynamicObject");
	}
	params[0] = 1 * sizeof(cell);
	params[1] = (cell)objectid;
	retval = native(sampgdk_fakeamx_amx(), params);
    return !!(retval);
}

SAMPGDK_NATIVE(bool, IsValidDynamicObject ( int objectid ) ) {
	static AMX_NATIVE native;
	cell retval;
	cell params[2];
	if (SAMPGDK_UNLIKELY(native == NULL)) {
		native = sampgdk_native_find_warn_stub("IsValidDynamicObject");
	}
	params[0] = 1 * sizeof(cell);
	params[1] = (cell)objectid;
	retval = native(sampgdk_fakeamx_amx(), params);
	return !!(retval);
}

SAMPGDK_NATIVE(bool, SetDynamicObjectPos ( int objectid, float x, float y, float z ) ) {
	static AMX_NATIVE native;
	cell retval;
	cell params[5];
	if (SAMPGDK_UNLIKELY(native == NULL)) {
		native = sampgdk_native_find_warn_stub("SetDynamicObjectPos");
	}
	params[0] = 4 * sizeof(cell);
	params[1] = (cell)objectid;
	params[2] = amx_ftoc(x);
	params[3] = amx_ftoc(y);
	params[4] = amx_ftoc(z);
	retval = native(sampgdk_fakeamx_amx(), params);
	return !!(retval);
}

SAMPGDK_NATIVE(bool, GetDynamicObjectPos ( int objectid, float * x, float * y, float * z ) ) {
	static AMX_NATIVE native;
	cell retval;
	cell params[5];
	cell x_ , y_, z_;
	if (SAMPGDK_UNLIKELY(native == NULL)) {
		native = sampgdk_native_find_warn_stub("GetDynamicObjectPos");
	}
	sampgdk_fakeamx_push(1, &x_);
	sampgdk_fakeamx_push(1, &y_);
	sampgdk_fakeamx_push(1, &z_);
	params[0] = 4 * sizeof(cell);
	params[1] = (cell)objectid;
	params[2] = x_;
	params[3] = y_;
	params[4] = z_;
	retval = native(sampgdk_fakeamx_amx(), params);
	sampgdk_fakeamx_get_float(x_, x);
	sampgdk_fakeamx_get_float(y_, y);
	sampgdk_fakeamx_get_float(z_, z);
	sampgdk_fakeamx_pop(z_);
	sampgdk_fakeamx_pop(y_);
	sampgdk_fakeamx_pop(x_);
	return !!(retval);
}

SAMPGDK_NATIVE(bool, SetDynamicObjectRot ( int objectid, float rx, float ry, float rz ) )  {
	static AMX_NATIVE native;
	cell retval;
	cell params[5];
	if (SAMPGDK_UNLIKELY(native == NULL)) {
		native = sampgdk_native_find_warn_stub("SetDynamicObjectRot");
	}
	params[0] = 4 * sizeof(cell);
	params[1] = (cell)objectid;
	params[2] = amx_ftoc(rx);
	params[3] = amx_ftoc(ry);
	params[4] = amx_ftoc(rz);
	retval = native(sampgdk_fakeamx_amx(), params);
	return !!(retval);
}
SAMPGDK_NATIVE(bool, GetDynamicObjectRot ( int objectid, float * rx, float * ry, float * rz ) ) {
	static AMX_NATIVE native;
	cell retval;
	cell params[5];
	cell x_ , y_, z_;
	if (SAMPGDK_UNLIKELY(native == NULL)) {
		native = sampgdk_native_find_warn_stub("GetDynamicObjectRot");
	}
	sampgdk_fakeamx_push(1, &x_);
	sampgdk_fakeamx_push(1, &y_);
	sampgdk_fakeamx_push(1, &z_);
	params[0] = 4 * sizeof(cell);
	params[1] = (cell)objectid;
	params[2] = x_;
	params[3] = y_;
	params[4] = z_;
	retval = native(sampgdk_fakeamx_amx(), params);
	sampgdk_fakeamx_get_float(x_, rx);
	sampgdk_fakeamx_get_float(y_, ry);
	sampgdk_fakeamx_get_float(z_, rz);
	sampgdk_fakeamx_pop(z_);
	sampgdk_fakeamx_pop(y_);
	sampgdk_fakeamx_pop(x_);
	return !!(retval);
}
//Sonradan eklenenler
SAMPGDK_NATIVE(bool, Streamer_Update ( int playerid, int type ) ) {
	static AMX_NATIVE native;
	cell retval;
	cell params[3];
	if (SAMPGDK_UNLIKELY(native == NULL)) {
		native = sampgdk_native_find_warn_stub("Streamer_Update");
	}
	params[0] = 2 * sizeof(cell);
	params[1] = (cell)playerid;
	params[2] = (cell)type;
	retval = native(sampgdk_fakeamx_amx(), params);
	return !!(retval);
}
SAMPGDK_NATIVE(int, CreateDynamic3DTextLabel(const char * text,int color, float x, float y, float z, float drawdistance, int attachedplayer, int attachedvehicle, int testlos, int worldid, int interiorid, int playerid, float streamdistance, int areaid, int priority) ) {
	static AMX_NATIVE native;
	cell retval;
	cell params[16];
	cell text_;
	if (SAMPGDK_UNLIKELY(native == NULL)) {
		native = sampgdk_native_find_warn_stub("CreateDynamic3DTextLabel");
	}
	sampgdk_fakeamx_push_string(text, NULL, &text_);
	params[0] = 15 * sizeof(cell);
	params[1] = text_;
	params[2] = (cell)color;
	params[3] = amx_ftoc(x);
	params[4] = amx_ftoc(y);
	params[5] = amx_ftoc(z);
	params[6] = amx_ftoc(drawdistance);
	params[7] = (cell)attachedplayer;
	params[8] = (cell)attachedvehicle;
	params[9] = (cell)testlos;
	params[10] = (cell)worldid;
	params[11] = (cell)interiorid;
	params[12] = (cell)playerid;
	params[13] = amx_ftoc(streamdistance);
	params[14] = (cell)areaid;
	params[15] = (cell)priority;
	retval = native(sampgdk_fakeamx_amx(), params);
	sampgdk_fakeamx_pop(text_);
	return (int)retval;
}
SAMPGDK_NATIVE(bool, DestroyDynamic3DTextLabel ( int id) ) {
	static AMX_NATIVE native;
	cell retval;
	cell params[2];
	if (SAMPGDK_UNLIKELY(native == NULL)) {
		native = sampgdk_native_find_warn_stub("DestroyDynamic3DTextLabel");
	}
	params[0] = sizeof(cell);
	params[1] = (cell)id;
	retval = native(sampgdk_fakeamx_amx(), params);
	return !!(retval);
}
SAMPGDK_NATIVE(bool, IsValidDynamic3DTextLabel ( int id) ) {
	static AMX_NATIVE native;
	cell retval;
	cell params[2];
	if (SAMPGDK_UNLIKELY(native == NULL)) {
		native = sampgdk_native_find_warn_stub("IsValidDynamic3DTextLabel");
	}
	params[0] = sizeof(cell);
	params[1] = (cell)id;
	retval = native(sampgdk_fakeamx_amx(), params);
	return !!(retval);
}
SAMPGDK_NATIVE(int, CreateDynamicPickup(int modelid, int type, float x, float y, float z, int worldid, int interiorid, int playerid, float streamdistance, int areaid, int priority)) {
	static AMX_NATIVE native;
	cell retval;
	cell params[12];
	if (SAMPGDK_UNLIKELY(native == NULL)) {
		native = sampgdk_native_find_warn_stub("CreateDynamicPickup");
	}
	params[0] = 11 * sizeof(cell);
	params[1] = (cell)modelid;
	params[2] = (cell)type;
	params[3] = amx_ftoc(x);
	params[4] = amx_ftoc(y);
	params[5] = amx_ftoc(z);
	params[6] = (cell)worldid;
	params[7] = (cell)interiorid;
	params[8] = (cell)playerid;
	params[9] = amx_ftoc(streamdistance);
	params[10] = (cell)areaid;
	params[11] = (cell)priority;
	retval = native(sampgdk_fakeamx_amx(), params);
	return (int)retval;
}
SAMPGDK_NATIVE(bool, DestroyDynamicPickup ( int id) ) {
	static AMX_NATIVE native;
	cell retval;
	cell params[2];
	if (SAMPGDK_UNLIKELY(native == NULL)) {
		native = sampgdk_native_find_warn_stub("DestroyDynamicPickup");
	}
	params[0] = sizeof(cell);
	params[1] = (cell)id;
	retval = native(sampgdk_fakeamx_amx(), params);
	return !!(retval);
}
SAMPGDK_NATIVE(bool, IsValidDynamicPickup ( int id) ) {
	static AMX_NATIVE native;
	cell retval;
	cell params[2];
	if (SAMPGDK_UNLIKELY(native == NULL)) {
		native = sampgdk_native_find_warn_stub("IsValidDynamicPickup");
	}
	params[0] = sizeof(cell);
	params[1] = (cell)id;
	retval = native(sampgdk_fakeamx_amx(), params);
	return !!(retval);
}
//native CA_RayCastLine(Float:StartX, Float:StartY, Float:StartZ, Float:EndX, Float:EndY, Float:EndZ, &Float:x, &Float:y, &Float:z)
SAMPGDK_NATIVE(int, CA_RayCastLine ( float StartX, float StartY, float StartZ, float EndX, float EndY, float EndZ, float * x, float * y, float * z) ) {
	static AMX_NATIVE native;
	cell retval;
	cell params[10];
	cell x_ , y_, z_;
	if (SAMPGDK_UNLIKELY(native == NULL)) {
		native = sampgdk_native_find_warn_stub("CA_RayCastLine");
	}
	sampgdk_fakeamx_push(1, &x_);
	sampgdk_fakeamx_push(1, &y_);
	sampgdk_fakeamx_push(1, &z_);
	params[0] = 9 * sizeof(cell);
	params[1] = amx_ftoc(StartX);
	params[2] = amx_ftoc(StartY);
	params[3] = amx_ftoc(StartZ);
	params[4] = amx_ftoc(EndX);
	params[5] = amx_ftoc(EndY);
	params[6] = amx_ftoc(EndZ);
	params[7] = x_;
	params[8] = y_;
	params[9] = z_;
	retval = native(sampgdk_fakeamx_amx(), params);
	sampgdk_fakeamx_get_float(x_, x);
	sampgdk_fakeamx_get_float(y_, y);
	sampgdk_fakeamx_get_float(z_, z);
	sampgdk_fakeamx_pop(z_);
	sampgdk_fakeamx_pop(y_);
	sampgdk_fakeamx_pop(x_);
	return (int)retval;
}
//CA_CreateObject(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, bool:add = false);
//CA_DestroyObject(index);
SAMPGDK_NATIVE(int, CA_CreateObject ( int modelid, float x, float y, float z, float rx, float ry, float rz, bool add) ) {
	static AMX_NATIVE native;
	cell retval;
	cell params[9];
	if (SAMPGDK_UNLIKELY(native == NULL)) {
		native = sampgdk_native_find_warn_stub("CA_CreateObject");
	}
	params[0] = 8 * sizeof(cell);
	params[1] = (cell)modelid;
	params[2] = amx_ftoc(x);
	params[3] = amx_ftoc(y);
	params[4] = amx_ftoc(z);
	params[5] = amx_ftoc(rx);
	params[6] = amx_ftoc(ry);
	params[7] = amx_ftoc(rz);
	params[8] = (cell)add;
	retval = native(sampgdk_fakeamx_amx(), params);
	return (int)retval;
}
SAMPGDK_NATIVE(bool, CA_DestroyObject ( int index) ) {
	static AMX_NATIVE native;
	cell retval;
	cell params[2];
	if (SAMPGDK_UNLIKELY(native == NULL)) {
		native = sampgdk_native_find_warn_stub("CA_DestroyObject");
	}
	params[0] = sizeof(cell);
	params[1] = (cell)index;
	retval = native(sampgdk_fakeamx_amx(), params);
	return !!(retval);
}
SAMPGDK_NATIVE(int, MoveDynamicObject ( int objectid, float x, float y, float z, float speed, float rx, float ry, float rz) ) {
	static AMX_NATIVE native;
	cell retval;
	cell params[9];
	if (SAMPGDK_UNLIKELY(native == NULL)) {
		native = sampgdk_native_find_warn_stub("MoveDynamicObject");
	}
	params[0] = 8 * sizeof(cell);
	params[1] = (cell)objectid;
    params[2] = amx_ftoc(x);
	params[3] = amx_ftoc(y);
	params[4] = amx_ftoc(z);
	params[5] = amx_ftoc(speed);
	params[6] = amx_ftoc(rx);
	params[7] = amx_ftoc(ry);
	params[8] = amx_ftoc(rz);
	retval = native(sampgdk_fakeamx_amx(), params);
	return retval;
}

SAMPGDK_NATIVE(bool, SyncRepairVehicle ( int vehid) ) {
	static AMX_NATIVE native;
	cell retval;
	cell params[2];
	if (SAMPGDK_UNLIKELY(native == NULL)) {
		native = sampgdk_native_find_warn_stub("SyncRepairVehicle");
	}
	params[0] = sizeof(cell);
	params[1] = (cell)vehid;
	retval = native(sampgdk_fakeamx_amx(), params);
	return !!(retval);
}


SAMPGDK_NATIVE(bool, SyncSetVehicleHealth ( int vehid, float health) ) {
	static AMX_NATIVE native;
	cell retval;
	cell params[3];
	if (SAMPGDK_UNLIKELY(native == NULL)) {
		native = sampgdk_native_find_warn_stub("SyncSetVehicleHealth");
	}
	params[0] = sizeof(cell)*2;
	params[1] = (cell)vehid;
	params[2] = amx_ftoc(health);
	retval = native(sampgdk_fakeamx_amx(), params);
	return !!(retval);
}
