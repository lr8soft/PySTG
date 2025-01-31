#include "Stage.h"
#include "../../XCInterpreter/ScriptLoader.h"
#include "../XCRender/XCImageHelper.h"
#include "../Player/Player.h"

#include "../Task/TaskHelper.h"
#include "../Bullet/BulletHelper.h"
#include "../Boss/BossHelper.h"
#include <GL3/gl3w.h>

#include "../UserInterface/GameInfoInterface.h"
Stage::Stage(std::string uuid, PyObject* item)
{
	itemStage = item;
	stageUuid = uuid;
}

void Stage::addTask(Task * pTask)
{
	stageTaskGroup.insert(std::make_pair(pTask->getTaskUUID(), pTask));
}

void Stage::removeTask(std::string & uuid)
{
	auto task = stageTaskGroup.find(uuid);
	if (task != stageTaskGroup.end()) {
		task->second->TaskRelease();
		delete task->second;
		stageTaskGroup.erase(task);
	}
}

void Stage::setTaskInitCoord(std::string & uuid, float x, float y)
{
	auto task = stageTaskGroup.find(uuid);
	if (task != stageTaskGroup.end()) {
		task->second->setUnitInitCoord(x, y);
	}
}

ALuint Stage::getStageMusic()
{
	return bgmBuffer;
}

void Stage::stageInit()
{
	if (!isStageInit) {
		MultiThreadDefine
		PyObject* taskSizeObj = PyObject_CallMethod(itemStage, "_cpp_getTaskSize", NULL);
		PyObject* bossSizeObj = PyObject_CallMethod(itemStage, "_cpp_getBossSize", NULL);
		PyObject* stageInfoObj = PyObject_CallMethod(itemStage, "_cpp_getStageInfo", NULL);
		int taskSize = ScriptLoader::getSingleArg<int>(taskSizeObj);
		int bossSize = ScriptLoader::getSingleArg<int>(bossSizeObj);

		const char* stageName, *stageMusic;
		PyArg_ParseTuple(stageInfoObj, "siis", &stageName, &stageRank, &stageBackgroundID, &stageMusic);
		bgmBuffer = AudioHelper::loadWavByAlut(stageMusic);
		
		if (taskSize > 0) {

			for (int i = 0; i < taskSize; i++) {
				PyObject *pItem, *itemObj = PyObject_CallMethod(itemStage, "_cpp_getTaskItem", NULL);
				PyArg_Parse(itemObj, "O", &pItem);
				
				Task* task = TaskHelper::parseTaskFromObject(pItem);
				if (task!=nullptr) {
					stageTaskGroup.insert(std::make_pair(task->getTaskUUID(), task));
				}
			}
		}

		if (bossSize > 0) {
			for (int i = 0; i < bossSize; i++) {
				PyObject *pItem, *itemObj = PyObject_CallMethod(itemStage, "_cpp_getBossItem", NULL);
				PyArg_Parse(itemObj, "O", &pItem);

				Boss* boss = BossHelper::parseBossFromObject(pItem);
				if (boss != nullptr) {
					stageTaskGroup.insert(std::make_pair(boss->getTaskUUID(), boss));
				}
			}
		}
		MultiThreadDefineEnd
		stageBackground = BackgroundHelper::getBackgroundByID(stageBackgroundID);
		if (stageBackground != nullptr) {
			stageBackground->BackgroundInit();
		}
		
		isStageInit = true;

		
	}
}

void Stage::stageWork()
{
	if (isStageInit) {
		timer.Tick();
		bool allTaskWaitTarget = true;
		GameInfoInterface::getInstance()->setRank(stageRank);
		AudioHelper::playFromBuffer(bgmBuffer);
		for (auto task = stageTaskGroup.begin(); task != stageTaskGroup.end(); task++) {
			if (!task->second->getTaskInit())
				task->second->TaskInit();

			if (!task->second->getTaskFinish()) {
				task->second->TaskWork();
				if (!task->second->getIsTaskWaitingForTarget()) {
					allTaskWaitTarget = false;
				}
			}
			if (task->second->getTaskFinish()) {
				task->second->TaskRelease();
				delete task->second;
				if (std::next(task) == stageTaskGroup.end())
				{
					stageTaskGroup.erase(task);
					break;
				}
				else {
					task = stageTaskGroup.erase(task);

				}
			}
		}
		if (allTaskWaitTarget) {
			auto taskEnd = stageTaskGroup.end();
			for (auto task = stageTaskGroup.begin(); task != taskEnd; task++) {
				task->second->TaskRelease();
				delete task->second;
			}
			stageTaskGroup.clear();
		}
		if (stageTaskGroup.empty()) {
			AudioHelper::stopByBuffer(bgmBuffer);
			stageFinish = true;
		}
	}
}

void Stage::stageRelease()
{
	alDeleteBuffers(1, &bgmBuffer);
	if (stageBackground != nullptr) {
		
		stageBackground->BackgroundRelease();
		delete stageBackground;
	}
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

Background * Stage::getBackgroundPointer()
{
	return stageBackground;
}
