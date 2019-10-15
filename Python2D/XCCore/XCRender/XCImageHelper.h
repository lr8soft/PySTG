#pragma once
#ifndef _XCIMAGE_MANAGER_H_
#define _XCIMAGE_MANAGER_H_
#include <GL/glcorearb.h>
#include <map>
#include "IRenderHelper.h"
class XCImageHelper:public IRenderHelper {
private:
	bool isFlexible;
	GLuint vao, vbo, ebo, tbo;

	int imageWidth = 0, imageHeight = 0;
	std::string imagePath;
	static GLuint ProgramHandle, ProgramHandleFx;
	static bool haveProgramInit;
public:
	XCImageHelper() = delete;
	/*isRenderFlexible=true�£���Ⱦʱ���texuturePos16xFloat��ֵ*/
	XCImageHelper(std::string path, bool isRenderFlexible = false);

	/*��flexible״̬�£�texuturePos16xFloatָ��Ӧ��ΪGetSpecificTexture����GetSpecificTexWithWidthAndHeight�ķ���ֵ*/
	virtual void Render(glm::vec3 renderPos,glm::vec4 coverColor,float rotateAngleRadians = 0.0f ,glm::vec3 rotateWork= glm::vec3(0, 0, 0),
		glm::vec3 scaleSize=glm::vec3(1.0f, 1.0f, 1.0f), float *texuturePos16xFloat=nullptr) override;
	virtual void Release() override;

	int getImageHeight();
	int getImageWidth();
};

#endif