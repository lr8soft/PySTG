#pragma once
#ifndef _COLLISIONINFO_H_
#define _COLLISIONINFO_H_
#include "CollideHelper.h"
class CollideInfo {
	static CollideHelper *collideHelperP1;
public:
	static void setCollideHelperP1(CollideHelper* helper);
	static CollideHelper* getCollideHelperP1();
};
#endif