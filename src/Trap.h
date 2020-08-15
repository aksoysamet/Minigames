#ifndef TRAP_H
#define TRAP_H
#include "../src/main.h"

class Trap
{
    public:
        Trap(int playerid, int vworld);
        virtual ~Trap();
        bool MonitorTire(int vid, int tire, float x1, float y1, float z1, float x2, float y2, float z2);
        float x;
        float y;
        float x2;
        float y2;
        float z;
        int playerid;
        int ObjectID;
};
#define e_TIRE_RR 0b0001
#define e_TIRE_FR 0b0010
#define e_TIRE_RL 0b0100
#define e_TIRE_FL 0b1000
int SS_GetTireSize(int mid, int tire, float *size);
int SS_GetTireState(int vid);
void SS_GetVehicleRotationMatrix(int vid, float*matrix3x3);
void SS_GetTirePos(int vid, float*px, float*py, float*pz, float ox, float oy, float oz, float matrix3x3[]);
int SS_GetTireCount(int mid);
float UzaklikCal(float mx, float my, float mz, float x, float y, float z);
#endif // TRAP_H
