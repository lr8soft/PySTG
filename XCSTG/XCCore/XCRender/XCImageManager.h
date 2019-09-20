#pragma once
#ifndef _XCIMAGE_MANAGER_H_
#define _XCIMAGE_MANAGER_H_
#include <GL/glcorearb.h>
#include <map>
#include <glm/glm.hpp>
class XCImageManager {
private:
	bool isFlexible;
	GLuint vao, vbo, ebo, tbo;
	std::string imagePath;
	static GLuint ProgramHandle, ProgramHandleFx;
	static bool haveProgramInit;
	static std::map<std::string,GLuint> textureGroup;
public:
	XCImageManager() = delete;
	/*isRenderFlexible=true�£���Ⱦʱ���texuturePos16xFloat��ֵ*/
	XCImageManager(std::string path, bool isRenderFlexible = false);

	/*��flexible״̬�£�texuturePos16xFloatָ��Ӧ��ΪGetSpecificTexture����GetSpecificTexWithWidthAndHeight�ķ���ֵ*/
	void ImageRender(glm::vec3 renderPos,glm::vec4 coverColor, 
		glm::vec3 scaleSize=glm::vec3(1.0f, 1.0f, 1.0f), float *texuturePos16xFloat=nullptr);
	void ImageRelease();

	/*��column�У�row�У�ȡ��x�� ��y�У��Ը����������Ͻ�����Ϊ׼��*/
	static float* GetSpecificTexture(int column, int row, int x, int y);
	/*��column�У�row�У�ȡ��x�� ��y�У��Ը����������Ͻ�����Ϊ׼��*/
	static float* GetSpecificTexWithRate(float width_rate, float height_rate, int column, int row, int x, int y);
	static float* GetPointSpriteVertex(float size);
};

#endif