#include <sstream>
#include <GL3/gl3w.h>
#include "XCFrame.h"
#include "XCCore/XCAudio/AudioHelper.h"
#include "XCCore/Task/TaskManager.h"
#include "XCCore/XCRender/RenderManager.h"
#include "XCFrameInfo.h"
#include "util/ConfigManager.h"

#include "util/ImageLoader.h"
XCFrame* XCFrame::pInstance = nullptr;
int XCFrame::FrameWidth, XCFrame::FrameHeight;

XCInterpreter *XCFrame::interpreter = nullptr;
void XCFrame::FrameInit()
{
	ScriptLoader::initPythonEvon();
	interpreter = new XCInterpreter;
	InitInfo* info = interpreter->getInitInfo();

	XCFrameInfo::ScreenHeight = FrameHeight;
	XCFrameInfo::ScreenWidth = FrameWidth;

	float smallSize = FrameWidth > FrameHeight ? FrameHeight : FrameWidth;
	float absWidth = smallSize / (float)FrameWidth;
	float absHeight = smallSize / (float)FrameHeight;

	XCFrameInfo::ScreenOriginTitle = info->winTitle;
	XCFrameInfo::FrameRight = absWidth;
	XCFrameInfo::FrameLeft = -absWidth;
	XCFrameInfo::FrameTop = absHeight;
	XCFrameInfo::FrameBottom = -absHeight;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//����ģʽţ ��
	glfwWindowHint(GLFW_RESIZABLE, false);//No resizable.
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, false);//Auto change size

	GLFWmonitor* primaryMonitor = isFullScreen ? glfwGetPrimaryMonitor() : nullptr;
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	tscreen = glfwCreateWindow(1, 1, "ThreadInitHelper", nullptr, nullptr);
	glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
	pscreen = glfwCreateWindow(FrameWidth, FrameHeight, XCFrameInfo::ScreenOriginTitle.c_str(), primaryMonitor, tscreen);

	glfwSetWindowIcon(pscreen, 1, getApplicationIcon());
	glfwMakeContextCurrent(pscreen);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwSetWindowPos(pscreen, (mode->width - FrameWidth) / 2, (mode->height - FrameHeight) / 2);
	if (pscreen == nullptr) {
		std::cerr << "[ERROR] Failed to create window!\n";
		exit(-1);
	}
	glfwSwapInterval(1);
	glfwSetFramebufferSizeCallback(pscreen, FrameResize);
	glfwSetWindowPosCallback(pscreen, FramePos);
	gl3wInit();

	AudioHelper::EvonInit();
}
void XCFrame::FrameLoop()
{	
	while (!glfwWindowShouldClose(pscreen)) {
		timer.Tick();
		TaskManager::getInstance()->TaskWork();
		RenderManager::getInstance()->RenderWork();

		glfwSwapBuffers(pscreen);
		glfwPollEvents();
		glfwSetWindowTitle(pscreen, 
			(XCFrameInfo::ScreenOriginTitle + "  | RenderObject: " + std::to_string(RenderManager::getInstance()->GetRenderObjectCount())
				+  "  FPS: "+std::to_string(timer.getFramePerSecond())).c_str());
	}
	isFrameTerminate = true;
	glfwDestroyWindow(pscreen);
	glfwDestroyWindow(tscreen);
	glfwTerminate();
}
void XCFrame::FrameFinalize()
{
	ScriptLoader::unloadPythonEvon();
	AudioHelper::UnloadEvon();
	delete pInstance;
}
void XCFrame::FrameResize(GLFWwindow * screen, int w, int h)
{
	if (w > 0 && h > 0) {
		glViewport(0, 0, w, h);
		FrameWidth = w;
		FrameHeight = h;

		float smallSize = FrameWidth > FrameHeight ? FrameHeight : FrameWidth;
		float absWidth = smallSize / (float)FrameWidth;
		float absHeight = smallSize / (float)FrameHeight;

		XCFrameInfo::FrameRight = absWidth;
		XCFrameInfo::FrameLeft = -absWidth;
		XCFrameInfo::FrameTop = absHeight;
		XCFrameInfo::FrameBottom = -absHeight;

		XCFrameInfo::ScreenHeight = h;
		XCFrameInfo::ScreenWidth = w;

	}
}

void XCFrame::FramePos(GLFWwindow * screen, int x, int y)
{
	
}

XCFrame * XCFrame::getInstance()
{
	if (pInstance==nullptr) {
		pInstance = new XCFrame;
	}
	return pInstance;
}

GLFWwindow * XCFrame::getScreen()
{
	return pscreen;
}
GLFWwindow * XCFrame::getThreadScreen()
{
	return tscreen;
}
bool XCFrame::getFrameTerminate()
{
	return isFrameTerminate;
}
XCFrame::XCFrame() {
	xcstd::ConfigManager cfg("pystg.cfg");
	std::cout << "[INFO]Internal version:"<< PythonSTGVersion << std::endl;
	if (cfg.IsFirstRun()) {
		cfg.AddNewInfo("up", std::to_string(GLFW_KEY_UP));
		cfg.AddNewInfo("down", std::to_string(GLFW_KEY_DOWN));
		cfg.AddNewInfo("left", std::to_string(GLFW_KEY_LEFT));
		cfg.AddNewInfo("right", std::to_string(GLFW_KEY_RIGHT));
		cfg.AddNewInfo("shoot", std::to_string(GLFW_KEY_Z));
		cfg.AddNewInfo("special", std::to_string(GLFW_KEY_X));
		cfg.AddNewInfo("item", std::to_string(GLFW_KEY_C));
		cfg.AddNewInfo("slow", std::to_string(GLFW_KEY_LEFT_SHIFT));
		cfg.AddNewInfo("fullScreen", std::to_string(0));
		cfg.AddNewInfo("resolution", "1280x720");
		cfg.AddNewInfo("volume", std::to_string(0.08));

		FrameWidth = 1280;
		FrameHeight = 720;
	}
	else {
		XCFrameInfo::keyUp = atoi(cfg.GetValue("up").c_str());
		XCFrameInfo::keyDown = atoi(cfg.GetValue("down").c_str());
		XCFrameInfo::keyLeft = atoi(cfg.GetValue("left").c_str());
		XCFrameInfo::keyRight = atoi(cfg.GetValue("right").c_str());
		XCFrameInfo::keyShoot = atoi(cfg.GetValue("shoot").c_str());
		XCFrameInfo::keySlow = atoi(cfg.GetValue("slow").c_str());
		AudioHelper::setVolume(atof(cfg.GetValue("volume").c_str()));

		isFullScreen = atoi(cfg.GetValue("fullScreen").c_str());
		std::string screenInfo = cfg.GetValue("resolution");

		int divideIndex = screenInfo.find('x', 0);
		std::string w = screenInfo.substr(0, divideIndex);
		std::string h = screenInfo.substr(divideIndex + 1, screenInfo.size());

		FrameWidth = atoi(w.c_str());
		FrameHeight = atoi(h.c_str());
	}
	
}

GLFWimage * XCFrame::getApplicationIcon()
{
	static GLFWimage image;

	int w = 0, h = 0, c = 0;
	void* imageInfo = xc_ogl::ImageLoader::getTexturePointer("assets/UI/p2dicon.png", w, h, c);
	image.height = h;
	image.width = w;
	image.pixels = (unsigned char*)imageInfo;
	return &image;
}
