#include "Stage.h"
#include "../../XCInterpreter/ScriptLoader.h"
#include "../../XCCore/XCRender/XCImageHelper.h"
#include "../../XCCore/Item/Player.h"

#include "../../XCCore/Bullet/BulletHelper.h"
#include <GL3/gl3w.h>
GLenum Stage::parseAlphaFunc(int src)
{
	switch (src)
	{
	case 0:
		return GL_NONE;
	case 1:
		return GL_ONE_MINUS_SRC_ALPHA;
	default:
		return GL_NONE;
	}
}
GLenum Stage::parseColorFunc(int src)
{
	switch (src)
	{
	case 0:
		return GL_NONE;
	case 1:
		return GL_ONE_MINUS_SRC_COLOR;
	default:
		return GL_NONE;
	}
}
Stage::Stage(std::string uuid, PyObject* item)
{
	itemStage = item;
	stageUuid = uuid;
}

void Stage::stageInit()
{
	if (!getStageInit()) {
		MultiThreadDefine
			
		PyObject* itemSizeObj = PyObject_CallMethod(itemStage, "_cpp_getItemSize", NULL);
		int itemSize = ScriptLoader::getSingleArg<int>(itemSizeObj);
		if (itemSize > 0) {
			for (int i = 0; i < itemSize; i++) {
				PyObject *pItem, *itemObj = PyObject_CallMethod(itemStage, "_cpp_getItem", NULL);
				PyArg_Parse(itemObj, "O", &pItem);
				if (pItem!=nullptr) {
					auto imageInfo = PyObject_CallMethod(pItem, "_cpp_getInitRenderInfo" ,NULL);
					auto scaleInfo = PyObject_CallMethod(pItem, "_cpp_getScaleSize", NULL);
					auto rotateInfo = PyObject_CallMethod(pItem, "_cpp_getRotateInfo", NULL);
					auto blendInfo = PyObject_CallMethod(pItem, "_cpp_getBlendInfo", NULL);
					auto initCoordInfo = PyObject_CallMethod(pItem, "_cpp_getInitCoord", NULL);

					const char* imagePath; int divideFormat[4], isFlexible;
					PyArg_ParseTuple(imageInfo, "s(iiii)p", &imagePath, &divideFormat[0], &divideFormat[1], &divideFormat[2], &divideFormat[3], &isFlexible);

					float scaleSize[3];
					PyArg_ParseTuple(scaleInfo, "fff", &scaleSize[0], &scaleSize[1], &scaleSize[2]);

					float rotateAngle; int rotateWork[3];
					PyArg_ParseTuple(rotateInfo, "f(iii)", &rotateAngle, &rotateWork[0], &rotateWork[1], &rotateWork[2]);

					int useBlend, blendColorFunc, blendAlphaFunc;
					PyArg_ParseTuple(blendInfo, "i(ii)", &useBlend, &blendColorFunc, &blendAlphaFunc);


					float initCoord[3];
					PyArg_ParseTuple(initCoordInfo, "fff", &initCoord[0], &initCoord[1], &initCoord[2]);

					XCImageHelper* image = new XCImageHelper(imagePath, isFlexible);
					Item* pItem = new Item( image, glm::vec4(divideFormat[0], divideFormat[1], divideFormat[2], divideFormat[3]),
							glm::vec4(1.0f), glm::vec3(scaleSize[0], scaleSize[1], scaleSize[2]), glm::vec3(rotateWork[0], rotateWork[1], rotateWork[2]), rotateAngle);
					pItem->setPosition(initCoord[0], initCoord[1], initCoord[2]);
					pItem->ItemInit();
					itemStruct renderItem;
					renderItem.item = pItem;
					renderItem.useBlend = useBlend;
					renderItem.colorFunc = parseColorFunc(blendColorFunc);
					renderItem.alphaFunc = parseAlphaFunc(blendAlphaFunc);
					stageItemGroup.push_back(renderItem);
				}
			}
		}
		PyObject* bulletSizeObj = PyObject_CallMethod(itemStage, "_cpp_getBulletSize", NULL);
		int bulletSize = ScriptLoader::getSingleArg<int>(bulletSizeObj);
		if (bulletSize > 0) {
			for (int i = 0; i < bulletSize; i++) {
				PyObject* pBullet, *bulletObject = PyObject_CallMethod(itemStage, "_cpp_getBullet", NULL);
				PyArg_Parse(bulletObject, "O", &pBullet);
				if (pBullet!=nullptr) {
					auto imageInfo = PyObject_CallMethod(pBullet, "_cpp_getInitRenderInfo", NULL);
					auto bulletColorInfo = PyObject_CallMethod(pBullet, "_cpp_getBulletColor", NULL);
					auto bulletGenerateInfo = PyObject_CallMethod(pBullet, "_cpp_getGenerateInfo", NULL);
					auto bulletCoordInfo = PyObject_CallMethod(pBullet, "_cpp_getInitCoord", NULL);

					const char* bulletType; int divideInfo[4]; float scaleInfo[3];
					PyArg_ParseTuple(imageInfo, "s(iiii)(fff)", &bulletType, &divideInfo[0], &divideInfo[1], &divideInfo[2], &divideInfo[3], &scaleInfo[0], &scaleInfo[1], &scaleInfo[2]);

					int bulletColor;
					PyArg_Parse(bulletColorInfo, "i", &bulletColor);

					float velocity, acceleration, angle, increaseAngle; int aimPlayer, reboundTime;
					PyArg_ParseTuple(bulletGenerateInfo, "ffffip", &velocity, &acceleration, &angle, &increaseAngle, &reboundTime, &aimPlayer);

					float initCoord[3];
					PyArg_ParseTuple(bulletCoordInfo, "fff", &initCoord[0], &initCoord[1], &initCoord[2]);

					Bullet* bullet = BulletHelper::getNewBulletObject(bulletType,bulletColor, glm::vec4(divideInfo[0], divideInfo[1], divideInfo[2], divideInfo[3])
						, glm::vec3(scaleInfo[0], scaleInfo[1], scaleInfo[2]), glm::vec3(initCoord[0], initCoord[1], initCoord[2])
						, velocity, acceleration, angle, increaseAngle, reboundTime,aimPlayer);
					if (bullet!=nullptr) {
						bullet->BulletInit();
						stageBulletGroup.push_back(bullet);
					}
				}
			}
		}

		MultiThreadDefineEnd
		isStageInit = true;
	}
}

void Stage::stageWork()
{
	timer.Tick();
	std::vector<itemStruct>::iterator itemBegin = stageItemGroup.begin();
	std::vector<itemStruct>::iterator itemEnd = stageItemGroup.end();
	for (auto item = itemBegin; item != itemEnd; item++) {
		if (!item->item->getIsFinish()) {
			if (item->useBlend) {
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_COLOR, item->colorFunc);
				glBlendFunc(GL_SRC_ALPHA, item->alphaFunc);
			}
			item->item->ItemRender();
			if (item->useBlend) {
				glDisable(GL_BLEND);
			}
		}
		if (item->item->getIsFinish()) {
			item->item->ItemRelease();
			if (std::next(item) == stageItemGroup.end()) {
				stageItemGroup.erase(item);
				break;
			}
			else {
				item = stageItemGroup.erase(item);
				itemEnd = stageItemGroup.end();
			}
		}
	}
	std::vector<Bullet*>::iterator bulletBegin = stageBulletGroup.begin();
	std::vector<Bullet*>::iterator bulletEnd = stageBulletGroup.end();
	for (auto bullet = bulletBegin; bullet != bulletEnd; bullet++) {
		if (!(*bullet)->getIsFinish()) {
			(*bullet)->BulletRender();
		}
		if ((*bullet)->getIsFinish()) {
			(*bullet)->BulletRelease();
			if (std::next(bullet) == stageBulletGroup.end()) {
				stageBulletGroup.erase(bullet);
				break;
			}
			else {
				bullet = stageBulletGroup.erase(bullet);
				bulletEnd = stageBulletGroup.end();
			}
		}
	}
}

void Stage::stageRelease()
{
}

bool Stage::getStageInit()
{
	return isStageInit;
}

bool Stage::getStageFinish()
{
	return stageFinish;
}

std::string Stage::getUuid()
{
	return stageUuid;
}
