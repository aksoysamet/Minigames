#include "Trap.h"
#define PI              3.14159265

Trap::Trap(int playerid, int vworld)
{
    float vx,vy,vz,a,qw,qx,qy,qz;
    GetVehiclePos(GetPlayerVehicleID(playerid),&vx,&vy,&vz);
    GetVehicleZAngle(GetPlayerVehicleID(playerid), &a);
    GetVehicleRotationQuat(GetPlayerVehicleID(playerid),&qw,&qx,&qy,&qz);
    float StartPos[3];
    StartPos[0] = (vx - (4.0*(sin((-a)*PI/180.0))));
    StartPos[1] = (vy - (4.0*(cos((-a)*PI/180.0))));
    StartPos[2] = (vz - (8.0 *((qy*qz)-(qw*qx))) - 0.7);
    this->playerid = playerid;
    this->z = StartPos[2];
    float length = 5.0;
	int hpmodel = GetVehicleModel(GetPlayerVehicleID(playerid));
	if(hpmodel == 427 || hpmodel == 428 || hpmodel == 490 || hpmodel == 528 || hpmodel == 599 || hpmodel == 601)this->z -= 0.4;
	this->ObjectID = CreateDynamicObject(2892, StartPos[0], StartPos[1], this->z, 0.0, 0.0, a - 90.0,vworld);

	this->x = (StartPos[0] + (length * sin((-a - 90.0)*PI/180.0)));
	this->y = (StartPos[1] + (length * cos((-a - 90.0)*PI/180.0)));
	this->x2 = (StartPos[0] - (length * sin((-a - 90.0)*PI/180.0)));
	this->y2 = (StartPos[1] - (length * cos((-a - 90.0)*PI/180.0)));

}

Trap::~Trap()
{
    DestroyDynamicObject(this->ObjectID);
}
int SS_SegmentsIntersect_2D(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
void SS_DistanceLineToLine_3D(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4, float *distance);
bool Trap::MonitorTire(int vid, int tire, float x1, float y1, float z1, float x2, float y2, float z2)
{
	float distance = 1.0;
	if (SS_SegmentsIntersect_2D(this->x, this->y, this->x2, this->y2, x1, y1, x2, y2))
	{
		SS_DistanceLineToLine_3D(this->x, this->y, this->z, this->x2, this->y2, this->z, x1, y1, z1, x2, y2, z2, &distance);
	}
	if (distance < 0.24)
	{
		int data1,data2,data3,data4;
		GetVehicleDamageStatus(vid, &data1, &data2, &data3, &data4);
		UpdateVehicleDamageStatus(vid, data1, data2, data3, (data4 | tire));
		return true;
	}
	return false;
}
int SS_GetTireState(int vid)
{
	int
		data,
		data2;
	GetVehicleDamageStatus(vid, &data2, &data2, &data2, &data);
	return data;
}

int SS_GetTireCount(int mid)
{
	int const tire_data[] =
	{
		4, 4, 4, 6, 4, 4, 4, 4, 6, 4,
		4, 4, 4, 4, 4, 4, 4, 0, 4, 4,
		4, 4, 4, 4, 4, 0, 4, 4, 4, 4,
		0, 6, 4, 6, 4, 4, 4, 6, 4, 4,
		4, 0, 4, 6, 4, 4, 0, 0, 2, 0,
		4, 4, 0, 0, 0, 6, 4, 4, 4, 4,
		0, 2, 2, 2, 0, 0, 4, 4, 2, 0,
		4, 4, 0, 0, 4, 4, 0, 4, 4, 4,
		4, 0, 4, 4, 0, 4, 4, 0, 0, 4,
		4, 4, 4, 0, 4, 4, 4, 0, 4, 4,
		4, 0, 4, 4, 4, 4, 4, 4, 4, 0,
		0, 0, 0, 0, 6, 6, 4, 4, 4, 0,
		0, 2, 2, 2, 6, 4, 4, 4, 4, 4,
		4, 4, 6, 4, 4, 4, 4, 0, 0, 0,
		4, 4, 4, 4, 4, 4, 4, 4, 0, 4,
		4, 4, 4, 0, 4, 4, 4, 4, 4, 4,
		4, 4, 4, 0, 0, 4, 4, 4, 4, 0,
		0, 4, 4, 4, 4, 4, 4, 0, 6, 4,
		4, 2, 4, 4, 4, 4, 2, 4, 4, 4,
		0, 4, 0, 0, 0, 0, 4, 4, 4, 4,
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
		4, 4
	};
	return tire_data[mid - 400];
}

int SS_GetTireSize(int mid, int tire, float *size)
{
	float const
		tire_size_R[] =
	{
		// Rear wheel size (from vehicles.ide)
		0.7680, 0.7749, 0.6999, 1.1000, 0.6600, 0.6999, 2.2799, 1.0000, 1.0599, 0.7500,
		0.8000, 0.6999, 0.6999, 0.7200, 0.7599, 0.6800, 0.8640, 0.0000, 0.6999, 0.6399,
		0.7879, 0.6499, 0.6999, 0.6999, 0.9200, 0.0000, 0.6999, 0.9359, 0.9139, 0.6999,
		0.0000, 1.0000, 1.2999, 1.2000, 0.8000, 1.1000, 0.6999, 1.0000, 0.6999, 0.6999,
		0.6999, 0.0000, 0.6800, 1.0820, 1.5000, 0.6800, 0.0000, 0.0000, 0.4639, 0.0000,
		1.1000, 0.7500, 0.0000, 0.0000, 0.0000, 1.2000, 0.8399, 0.5000, 0.7200, 0.6999,
		0.0000, 0.6700, 0.4639, 0.5600, 0.0000, 0.0000, 0.6999, 0.6999, 0.6200, 0.0000,
		0.8939, 0.6000, 0.0000, 0.0000, 0.6999, 0.6999, 0.0000, 0.7599, 0.6999, 0.6999,
		0.6999, 0.0000, 0.6999, 0.6600, 0.0000, 0.6000, 1.5000, 0.0000, 0.0000, 0.8999,
		0.9200, 0.6499, 0.6999, 0.0000, 0.8199, 0.9720, 0.6999, 0.0000, 0.7599, 0.8000,
		0.8000, 0.0000, 0.8199, 0.8199, 0.6999, 0.8999, 0.7730, 0.6999, 0.8000, 0.0000,
		0.0000, 0.0000, 0.0000, 0.0000, 1.1059, 1.1799, 0.7500, 0.7500, 0.6600, 0.0000,
		0.0000, 0.6800, 0.6800, 0.6700, 1.0000, 0.9200, 0.6999, 0.6999, 0.8500, 0.6999,
		0.4499, 1.2999, 1.0000, 0.6999, 0.6999, 0.7400, 0.6999, 0.0000, 0.0000, 0.0000,
		0.6999, 1.0000, 0.7400, 0.6999, 1.0000, 0.6999, 0.6999, 0.6999, 0.0000, 0.6840,
		0.7599, 0.7500, 0.8399, 0.0000, 0.8399, 0.6999, 1.5000, 1.5000, 0.7879, 0.6999,
		0.8700, 0.6999, 0.6800, 0.0000, 0.0000, 0.6399, 0.6999, 0.6999, 0.6999, 0.0000,
		0.0000, 0.2599, 0.5600, 1.1399, 0.5000, 0.6999, 0.6999, 0.0000, 1.0000, 0.8999,
		0.7799, 0.6800, 0.7699, 0.7500, 1.1200, 0.7400, 0.6539, 0.6999, 0.8600, 0.7400,
		0.0000, 1.1000, 0.0000, 0.0000, 0.0000, 0.0000, 0.6999, 0.6999, 0.6999, 0.9499,
		0.6999, 1.3660, 0.6999, 0.6999, 0.6999, 0.6999, 0.6000, 0.6000, 0.6000, 0.7599,
		0.3199, 0.6800
	};
	if (tire & (e_TIRE_RR | e_TIRE_RL))
	{
		*size = tire_size_R[mid - 400];
		return 1;
	}
	float const
		tire_size_F[] =
	{
		// Front wheel size (from vehicles.ide)
		0.7680, 0.7749, 0.6999, 1.1000, 0.6600, 0.6999, 2.2799, 1.0000, 1.0599, 0.7500,
		0.8000, 0.6999, 0.6999, 0.7200, 0.7599, 0.6800, 0.8640, 0.0000, 0.6999, 0.6399,
		0.7879, 0.6499, 0.6999, 0.6999, 0.8399, 0.0000, 0.6999, 0.9359, 0.9139, 0.6999,
		0.0000, 1.0000, 1.2999, 1.2000, 0.7200, 1.1000, 0.6999, 1.0000, 0.6999, 0.6999,
		0.6999, 0.0000, 0.6800, 1.0820, 1.5000, 0.6800, 0.0000, 0.0000, 0.4639, 0.0000,
		1.1000, 0.6999, 0.0000, 0.0000, 0.0000, 1.2000, 0.8399, 0.5000, 0.7200, 0.6999,
		0.0000, 0.6700, 0.4639, 0.7799, 0.0000, 0.0000, 0.6999, 0.6999, 0.6800, 0.0000,
		0.8939, 0.6000, 0.0000, 0.0000, 0.6999, 0.6999, 0.0000, 0.7599, 0.6999, 0.6999,
		0.6999, 0.0000, 0.6999, 0.6600, 0.0000, 0.6000, 1.5000, 0.0000, 0.0000, 0.8999,
		0.9200, 0.6499, 0.6999, 0.0000, 0.8199, 0.9720, 0.6999, 0.0000, 0.7599, 0.8000,
		0.8000, 0.0000, 0.8199, 0.8199, 0.6999, 0.8999, 0.7730, 0.6999, 0.8000, 0.0000,
		0.0000, 0.0000, 0.0000, 0.0000, 1.1059, 1.1799, 0.7500, 0.7500, 0.6600, 0.0000,
		0.0000, 0.6800, 0.6800, 0.6700, 1.1200, 0.9200, 0.6999, 0.6999, 0.8500, 0.6999,
		0.4499, 0.6800, 0.5879, 0.6999, 0.6999, 0.7400, 0.6999, 0.0000, 0.0000, 0.0000,
		0.6999, 1.0000, 0.7400, 0.6999, 1.0000, 0.6999, 0.6999, 0.6999, 0.0000, 0.6840,
		0.7599, 0.7500, 0.8399, 0.0000, 0.8399, 0.6999, 1.5000, 1.5000, 0.7879, 0.6999,
		0.8700, 0.6999, 0.6800, 0.0000, 0.0000, 0.6399, 0.6999, 0.6999, 0.5500, 0.0000,
		0.0000, 0.2599, 0.4799, 1.1399, 0.5000, 0.6999, 0.6999, 0.0000, 1.0000, 0.8999,
		0.7799, 0.6800, 0.7699, 0.6600, 1.1200, 0.7400, 0.6539, 0.6999, 0.8600, 0.7400,
		0.0000, 1.1000, 0.0000, 0.0000, 0.0000, 0.0000, 0.6999, 0.6999, 0.6999, 0.9499,
		0.6999, 1.3660, 0.6999, 0.6999, 0.6999, 0.6999, 0.6000, 0.6000, 0.6000, 0.7599,
		0.3199, 0.6800
	};
	if (tire & (e_TIRE_FR | e_TIRE_FL))
	{
		*size = tire_size_F[mid - 400];
		return 1;
	}
	return 0;
}

void SS_GetTirePos(int vid, float*px, float*py, float*pz, float ox, float oy, float oz, float matrix3x3[])
{
	GetVehiclePos(vid, px, py, pz);
	*px += ox * (1 - 2 * (matrix3x3[4] + matrix3x3[7])) + oy * (2 * (matrix3x3[1] + matrix3x3[8])) + oz * (2 * (matrix3x3[2] - matrix3x3[6])),
	*py += ox * (2 * (matrix3x3[1] - matrix3x3[8])) + oy * (1 - 2 * (matrix3x3[0] + matrix3x3[7])) + oz * (2 * (matrix3x3[5] + matrix3x3[3])),
	*pz += ox * (2 * (matrix3x3[2] + matrix3x3[6])) + oy * (2 * (matrix3x3[5] - matrix3x3[3])) + oz * (1 - 2 * (matrix3x3[0] + matrix3x3[4]));
}

void SS_GetVehicleRotationMatrix(int vid, float*matrix3x3)
{
	float qw,
          qx,
          qy,
          qz;
	GetVehicleRotationQuat(vid, &qw, &qx, &qy, &qz);
	matrix3x3[0] = qx * qx, matrix3x3[1] = qx * qy, matrix3x3[2] = qx * qz,
	matrix3x3[3] = qx * qw, matrix3x3[4] = qy * qy, matrix3x3[5] = qy * qz,
	matrix3x3[6] = qy * qw, matrix3x3[7] = qz * qz, matrix3x3[8] = qz * qw;
}


int SS_SegmentsIntersect_2D(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
	float xA = x2 - x1,
          yA = y2 - y1,
          xB = x4 - x3,
          yB = y4 - y3,
          d  = xA * yB - yA * xB;
	if (!d)return 0;
    float xC = x3 - x1,
          yC = y3 - y1,
          pA = (xC * yB - yC * xB) / d,
          pB = (xC * yA - yC * xA) / d;
	if (pA < 0 || pA > 1 || pB < 0 || pB > 1)
	{
		return 0;
	}
	return 1;
}

void SS_DistanceLineToLine_3D(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4, float *distance)
{
	float ux = x2 - x1,
          uy = y2 - y1,
          uz = z2 - z1,
          vx = x4 - x3,
          vy = y4 - y3,
          vz = z4 - z3,
          wx = x1 - x3,
          wy = y1 - y3,
          wz = z1 - z3,
          uu = ux * ux + uy * uy + uz * uz,
          uv = ux * vx + uy * vy + uz * vz,
          uw = ux * wx + uy * wy + uz * wz,
          vv = vx * vx + vy * vy + vz * vz,
          vw = vx * wx + vy * wy + vz * wz,
          d  = uu * vv - uv * uv,
          pA = (uv * vw - vv * uw) / d,
          pB = (uu * vw - uv * uw) / d,
          dx = wx + pA * ux - pB * vx,
          dy = wy + pA * uy - pB * vy,
          dz = wz + pA * uz - pB * vz;
	*distance = sqrt(dx * dx + dy * dy + dz * dz);
}
