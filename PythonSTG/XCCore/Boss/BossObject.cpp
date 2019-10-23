#include "BossObject.h"
#include <GL3/gl3w.h>
#include "../../XCFrameInfo.h"
BossObject::BossObject(IRenderHelper * helper, glm::vec2 dInfo, glm::vec2 sInfo, int sbRow, int wRow, int aRow)
{
	divideInfo = dInfo;
	scaleInfo = sInfo;
	standByRow = sbRow;
	walkRow = wRow;
	attackRow = aRow;

	bossImage = helper;

	setCurrentType(BossType);
}

void BossObject::Init()
{
	if (!isInit) {
		damageNormal = AudioHelper::loadWavFromFile("assets/SE/se_damage00.wav");
		damageFinal = AudioHelper::loadWavFromFile("assets/SE/se_damage01.wav");
		bossExplode = AudioHelper::loadWavFromFile("assets/SE/se_enep01.wav");

		isInit = true;
	}
}

void BossObject::Render()
{
	if (isInit) {
		timer.Tick();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		if (nowImageIndex < divideInfo[0]) {
			nowImageIndex += 3.0f * timer.getDeltaFrame();
		}
		else {
			nowImageIndex = 0.0f;
		}
		switch (nowState) {
		case BossObject::StandByState:
			nowImageState = standByRow;
			break;
		case BossObject::WalkState:
			nowImageState = walkRow;
			break;
		case BossObject::AttackState:
			nowImageState = attackRow;
			break;

		}
		if (timer.getAccumlateTime() < moveTime) {
			float pi = 3.1415926f;
			NowPosition.x += velocity * cos(angle / 180.0f * pi) * timer.getDeltaFrame();
			NowPosition.y += velocity * sin(angle / 180.0f * pi) * timer.getDeltaFrame();
			nowState = WalkState;
		}
		else {
			nowState = StandByState;
		}

		bossImage->Render(glm::vec3(NowPosition,0.0f), glm::vec4(1.0f), 0.0f, glm::vec3(0, 0, 1), glm::vec3(scaleInfo[0], scaleInfo[1], 1.0f),
			IRenderHelper::GetSpecificTexWithRate(XCFrameInfo::FrameRight, XCFrameInfo::FrameTop, divideInfo[0], divideInfo[1], nowImageIndex, nowImageState));
		glDisable(GL_BLEND);
	}
}

void BossObject::Release()
{
	if (isInit) {
		bossImage->Release();
		delete bossImage;
	
	}
}

void BossObject::setBossHitPoint(float hitPoint)
{
	nowHitPoint = hitPoint;
	maxHitPoint = hitPoint;
}

void BossObject::setBossSpellCardCount(int count)
{
	spellCardCount = count;
}

void BossObject::setBossSpellCardTime(float time)
{
	spellCardTime = time;
}

void BossObject::setMovement(const glm::vec2& movement)
{
	velocity = speed;
	float dist = pow(pow(movement.y - NowPosition.y, 2) + pow(movement.x - NowPosition.x, 2), 0.5f);
	float k = (movement.y - NowPosition.y) / (movement.x - NowPosition.x);
	float pi = 3.1415926f;
	float theta = 0.0f;
	if (movement.x != NowPosition.x) {
		theta = atan(k);
	}
	else {
		if (movement.y > NowPosition.y) {
			theta = pi / 2.0f;
		}
		else {
			theta = (pi *3.0f) / 2.0f;
		}
	}
	if (movement.x < NowPosition.x)
		velocity = -velocity;
	angle = (180.0f / pi) * theta;
	moveTime = dist / abs(velocity);

	timer.Clear();
}
