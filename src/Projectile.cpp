#include "Projectile.h"
#define PI              3.14159265
#define UZAKLIK         230.0
float GetVehicleSpeed(int vehicleID);
float UzaklikCal(float mx, float my, float mz, float x, float y, float z);
Projectile::Projectile(int player, int vehid, int vworld)
{
    this->exceptveh = vehid;
    this->playerid = player;
    float x,y,z,a,qw,qx,qy,qz;
    int don = 0;
    //char logchr[144];
    int sureinms;
    int vid = GetPlayerVehicleID(playerid);
	GetVehiclePos(vid,&x,&y,&z);
	GetVehicleZAngle(vid,&a);
	GetVehicleRotationQuat(vid,&qw,&qx,&qy,&qz);
	float CollisionPos[3];
    float StartPos[3];
    float EndPos[3];
    StartPos[0] = (x + (6.0*(sin((-a)*PI/180.0))));
    StartPos[1] = (y + (6.0*(cos((-a)*PI/180.0))));
    StartPos[2] = (z + (12.0 *((qy*qz)-(qw*qx))))+0.1;
    this->x = StartPos[0];
    this->y = StartPos[1];
    this->z = StartPos[2];
    EndPos[0]   = (x + (UZAKLIK * sin((-a)*PI/180.0)));
    EndPos[1]   = (y + (UZAKLIK * cos((-a)*PI/180.0)));
    EndPos[2]   = (z + (UZAKLIK * 2.0 * ((qy*qz) - (qw*qx))))+0.1;
    objectid = CreateDynamicObject(345 ,StartPos[0],StartPos[1],StartPos[2],(atan(2.0 * ((qy*qz) - (qw*qx)))*180/PI),0,a,vworld);
    try
    {
        don = CA_RayCastLine(StartPos[0], StartPos[1], StartPos[2], EndPos[0], EndPos[1], EndPos[2], &CollisionPos[0], &CollisionPos[1], &CollisionPos[2]);
    }
    catch(...)
    {
        sampgdk::logprintf("CA_RayCastLine Error");
    }
    if(don > 0)
    {
        sureinms = MoveDynamicObject(objectid, CollisionPos[0], CollisionPos[1], CollisionPos[2],(50.0 + (GetVehicleSpeed(vid)/5.0)),-1000.0,-1000.0,-1000.0);
        // = ( UzaklikCal(StartPos[0],StartPos[1],StartPos[2],CollisionPos[0], CollisionPos[1], CollisionPos[2]) / (50.0 + (GetVehicleSpeed(vid)/5.0)));
        sx = (CollisionPos[0] - StartPos[0]) / sureinms;
        sy = (CollisionPos[1] - StartPos[1]) / sureinms;
        sz = (CollisionPos[2] - StartPos[2]) / sureinms;
        sx *= 1000;
        sy *= 1000;
        sz *= 1000;
        ex = CollisionPos[0];
        ey = CollisionPos[1];
        ez = CollisionPos[2];
        /*snprintf(logchr,144,"Sure:%d Artis: %f,%f,%f",sureinms,sx,sy,sz);
        SendClientMessageToAll(-1,logchr);*/
    }
    else
    {
        sureinms = MoveDynamicObject(objectid, EndPos[0], EndPos[1], EndPos[2],(50.0 + (GetVehicleSpeed(vid)/5.0)),-1000.0,-1000.0,-1000.0);
        sx = (EndPos[0] - StartPos[0]) / sureinms;
        sy = (EndPos[1] - StartPos[1]) / sureinms;
        sz = (EndPos[2] - StartPos[2]) / sureinms;
        sx *= 1000;
        sy *= 1000;
        sz *= 1000;
        ex = EndPos[0];
        ey = EndPos[1];
        ez = EndPos[2];
        /*snprintf(logchr,144,"Sure:%d Artis: %f,%f,%f",sureinms,sx,sy,sz);
        SendClientMessageToAll(-1,logchr);*/
    }
    this->sendtime = duration_cast< milliseconds >(
		system_clock::now().time_since_epoch()
    );
    this->endtime = sureinms;
}

float UzaklikCal(float mx, float my, float mz, float x, float y, float z)
{
    float ax,ay,az;
    ax = x - mx;
    ay = y - my;
    az = z - mz;
    return sqrt((ax * ax) + (ay * ay)+ (az * az));
}

Projectile::~Projectile()
{
    DestroyDynamicObject(objectid);
}
void Projectile::GetProjectilePos(float* ax, float* ay, float*az)
{
    *ax = this->ex;
    *ay = this->ey;
    *az = this->ez;
}
void Projectile::UpdateProjectilePos(milliseconds time, float* ax, float* ay, float*az)
{
    duration<double> tm = (time - this->sendtime);
    *ax = (this->x + (this->sx * tm.count()));
    *ay = (this->y + (this->sy * tm.count()));
    *az = (this->z + (this->sz * tm.count()));
    /*char logchr[144];
    snprintf(logchr,144,"Sure:%lf Koord: %f,%f,%f",tm.count(),*ax,*ay,*az);
    SendClientMessageToAll(-1,logchr); */
}
int Projectile::GetExceptVehicle()
{
    return this->exceptveh;
}
int Projectile::GetProjectileID()
{
    return this->objectid;
}
int Projectile::GetProjectileOwner()
{
    return this->playerid;
}
float GetVehicleSpeed(int vehicleID)
{
	float vehiclesVelocity[3];
	GetVehicleVelocity(vehicleID, &vehiclesVelocity[0], &vehiclesVelocity[1], &vehiclesVelocity[2]);
	return sqrt((vehiclesVelocity[0] * vehiclesVelocity[0] + vehiclesVelocity[1] * vehiclesVelocity[1] + vehiclesVelocity[2] * vehiclesVelocity[2])) * 200.0;
}
