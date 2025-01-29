#include <types.h>
#include "racer.h"

void FUN_01ffcd84(Orientation4D* param_1, Quaternion* param_2) {
	int q_x = param_2->k;
	int q_y = param_2->j;
	int q_z = param_2->i;
	int q_w = param_2->r;

	int xx = q_x * q_x >> 0xb;
	int yy = q_y * q_y >> 0xb;
	int zz = q_z * q_z >> 0xb;

	int xy = q_x * q_y >> 0xb;
	int xz = q_x * q_z >> 0xb;
	int yz = q_y * q_z >> 0xb;
	int wx = q_w * q_x >> 0xb;
	int wy = q_w * q_y >> 0xb;
	int wz = q_w * q_z >> 0xb;
	
	(param_1->left).x = 0x1000 - (yy + zz);
	(param_1->left).y = xy + wz;
	(param_1->left).z = xz - wy;
	(param_1->up).x = xy - wz;
	(param_1->up).y = 0x1000 - (zz + xx);
	(param_1->up).z = yz + wx;
	(param_1->forward).x = xz + wy;
	(param_1->forward).y = yz - wx;
	(param_1->forward).z = 0x1000 - (xx + yy);
	(param_1->fourth).x = 0;
	(param_1->fourth).y = 0;
	(param_1->fourth).z = 0;
	return;
}
