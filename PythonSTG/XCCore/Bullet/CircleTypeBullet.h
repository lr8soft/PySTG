#pragma once
#ifndef _CIRCLE_TYPE_BULLET_H_
#define _CIRCLE_TYPE_BULLET_H_
#include "Bullet.h"
#include <glm/glm.hpp>
#include "../../util/GameTimer.h"
#include "../XCRender/XCImageHelper.h"
class CircleTypeBullet :public Bullet {
private:
	/*
	float velocity = 0.0f, acceleration = 0.0f, angle = 0.0f;
	float NowPosition[3] = { 0.0f,  0.0f, 0.0f };
	bool isWorkFinish = false;
	bool isFinishTime = false;
	*/
	std::string imagePath;
	glm::vec4 divideInfo;
	glm::vec3 scaleInfo;
	glm::vec3 collideSize;
	int  reBoundTime = 0;
	float velocity = 0.0f, acceleration = 0.0f, angle = 0.0f, increaseAngle = 0.0f;

	int particleDensity = 3;
	float particleFinishTime = 0.0f, particleVelocity = 0.6f, particleSize = 15.0f;
	glm::vec4 particleColor;

	XCGameTimer timer;
	XCImageHelper* image;
	
	bool aimToPlayer = false, haveCheckGraze = false;
	
public:
	/*#£ĦAttention scaleInfo will work both collide and render.
#£ĦrenderSize: scaleX * widthRate : scaleY * heightRate  ,collideSize : scaleX : scaleY*/
	CircleTypeBullet(std::string bulletImagePath, glm::vec4 divideInfo, glm::vec3 scaleInfo, glm::vec3 collideSize ,glm::vec3 initCoord,
		float velocity, float acceleration, float angle, float increateAngle, int reboundTime, bool aimPlayer, 
		int particleDensity, float particleFinishTime, float particleVelocity, float particleSize, glm::vec4 particleColor);

	virtual void Init()  override;
	virtual void Render() override;
	virtual void Release() override;

	virtual bool BulletCollideWithPoint(float x, float y, bool &haveGraze) override;
	virtual void setBulletTerminate() override;
};
#endif