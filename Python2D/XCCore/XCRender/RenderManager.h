#pragma once
#ifndef _RenderManager_H_
#define _RenderManager_H_
#include <vector>
#include <string>
#include <mutex>
#include <map>
#include "IRenderHelper.h"
#include "../UserInterface/IUserInterface.h"
#include "../Stage/Stage.h"
#include "../Item/Player.h"
#include "../Background/Background.h"
struct StaticRenderItem {
	std::string renderType;
	std::string imagePath;
	IRenderHelper *image;
	glm::vec3 renderPos;
	glm::vec4  renderColor;
	glm::vec3  scaleSize;
	int divideInfo[4];//col row scol srow
	
	bool visible = true;
	bool flexible = false;
	bool init = false;
};
class RenderManager {
private:
	std::vector<Stage*> stageQueue;
	std::vector<StaticRenderItem> staticQueue;
	std::map<std::string, IUserInterface*> uiGroup;

	bool shouldGamePause = false;
	Background *renderBackground = nullptr;
	Player* playerP1;

	static RenderManager* pRManager;
	RenderManager();
public:
	static RenderManager* getInstance();
	void AddStaticWork(StaticRenderItem work);
	void AddStageItem(Stage* stage);
	void AddUserInterface(std::string uiName, IUserInterface* ui);

	void RenderWork();

	Player* getPlayerP1();
	void setPlayerP1(Player* p1);
};
#endif