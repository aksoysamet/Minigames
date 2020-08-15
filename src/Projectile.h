#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "../src/main.h"
#include <chrono>
using namespace std::chrono;
class Projectile
{
    public:
        Projectile(int player, int vehid, int vworld);
        virtual ~Projectile();
        void GetProjectilePos(float* ax, float* ay, float*az);
        int GetExceptVehicle();
        int GetProjectileID();
        int GetProjectileOwner();
        void UpdateProjectilePos(milliseconds time, float* ax, float* ay, float*az);
        milliseconds sendtime;
        int endtime;
    protected:
    private:
    int playerid;
    int exceptveh;
    int objectid;
    float sx,sy,sz;
    float x,y,z;
    float ex,ey,ez;
};

#endif // PROJECTILE_H
