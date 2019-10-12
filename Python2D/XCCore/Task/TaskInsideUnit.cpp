#include "TaskInsideUnit.h"
#include "../XCRender/RenderManager.h"
TaskInsideUnit::TaskInsideUnit(std::string uuid, int wFrame, int wInterval, int rTime): parentUuid(uuid),waitFrame(wFrame), workInterval(wInterval), repeatTime(rTime)
{
	;
}

void TaskInsideUnit::UnitInit()
{
}

void TaskInsideUnit::UnitWork()
{
	if (waitFrame > 0) {
		waitFrame--;
	}
	else {
		if (!haveAddToQueue) {
			auto iterBegin = renderObjectGroup.begin();
			auto iterEnd = renderObjectGroup.end();
			for (auto object = iterBegin; object != iterEnd; object++) {
				RenderManager::getInstance()->AddRenderObject(parentUuid, *object);
			}
			haveAddToQueue = true;
		}
	}
}

void TaskInsideUnit::UnitRelease()
{
	renderObjectGroup.clear();
}

void TaskInsideUnit::addRenderObject(RenderObject * pObject)
{
	renderObjectGroup.push_back(pObject);
}

bool TaskInsideUnit::IsAddToQueue()
{
	return haveAddToQueue;
}
