// 引入GLEW库 定义静态链接
#define GLEW_STATIC
#include <Windows.h>
#include <debugapi.h>
#include <stdio.h>
#include <GLEW/glew.h>
// 引入GLFW库
#include <GLFW/glfw3.h>
#include <SOIL\SOIL.h>
#include <glm\glm.hpp> //引入opengl 数学库
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <iostream>
#include <vector>

// 包含着色器加载库
#include "shader.h"
#include "Camera.h"

// 键盘回调函数声明
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
//鼠标移动回调函数声明
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//鼠标滚轮滚动
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void mouse_outIncallback(GLFWwindow* window, int flag);
//综合处理键盘按键，解决卡顿
void Do_Movement();

// 定义程序常量
const float WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
float mixvalue = 0.5;
bool keys[1024] = { false };
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//下面2个变量用于计算帧间隔
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
bool mouseFirst = true;
double lastx, lasty;

int main(int argc, char** argv)
{
	/***********************************************************************************/
	if (!glfwInit())	// 初始化glfw库
	{
		std::cout << "Error::GLFW could not initialize GLFW!" << std::endl;
		return -1;
	}

	// 开启OpenGL 3.3 core profile
	std::cout << "Start OpenGL core profile version 3.3" << std::endl;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// 创建窗口
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
		"Lighting", NULL, NULL);
	if (!window)
	{
		std::cout << "Error::GLFW could not create winddow!" << std::endl;
		glfwTerminate();
		return -1;
	}
	// 创建的窗口的context指定为当前context
	glfwMakeContextCurrent(window);

	// 注册窗口键盘事件回调函数
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorEnterCallback(window, mouse_outIncallback);
	//窗口鼠标事件回调
	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetScrollCallback(window, scroll_callback);

	// 初始化GLEW 获取OpenGL函数
	glewExperimental = GL_TRUE; // 让glew获取所有拓展函数
	GLenum status = glewInit();
	if (status != GLEW_OK)
	{
		std::cout << "Error::GLEW glew version:" << glewGetString(GLEW_VERSION)
			<< " error string:" << glewGetErrorString(status) << std::endl;
		glfwTerminate();
		return -1;
	}

	// 设置视口参数
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	glEnable(GL_DEPTH_TEST);
	// 开始游戏主循环
	while (!glfwWindowShouldClose(window))
	{
		GLfloat curFrameTime = (GLfloat)glfwGetTime();
		deltaTime = curFrameTime - lastFrame;
		lastFrame = curFrameTime;

		glfwPollEvents(); // 处理例如鼠标 键盘等事件
		Do_Movement();

		// 清除颜色缓冲区 重置为指定颜色
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwSwapBuffers(window); // 交换缓存
	}
	glfwTerminate();
	return 0;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (mouseFirst)
	{
		mouseFirst = false;
		lastx = xpos;
		lasty = ypos;
	}

	double xoffset = xpos - lastx;
	double yoffset = lasty - ypos;
	lastx = xpos;
	lasty = ypos;

	camera.processMouseMovement(xoffset, yoffset);

}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);

}
//综合处理键盘按键，解决卡顿
void Do_Movement()
{
	if (keys[GLFW_KEY_W])
		camera.processKeyboard(Camera_Movement::FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.processKeyboard(Camera_Movement::BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.processKeyboard(Camera_Movement::LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.processKeyboard(Camera_Movement::RIGHT, deltaTime);

}

void mouse_outIncallback(GLFWwindow* window, int flag)
{
	if (flag == GL_FALSE)
	{
		mouseFirst = true;
	}
}