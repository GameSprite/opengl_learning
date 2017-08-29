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
const int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
float mixvalue = 0.5;
bool keys[1024] = { false };
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//下面2个变量用于计算帧间隔
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
bool mouseFirst = true;
double lastx, lasty;

//灯的位置
glm::vec3 lightPos(1.2f, 1.0f, 1.0f);
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

	// 指定顶点属性数据 顶点位置
	GLfloat vertices[] =
	{
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
	};
	// 创建缓存对象
	GLuint VAOId, VBOId;
	glGenVertexArrays(1, &VAOId);
	glGenBuffers(1, &VBOId);

	glBindBuffer(GL_ARRAY_BUFFER, VBOId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAOId);

	// 指定解析方式  并启用顶点属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)0); //
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT),(GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBOId);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	// Section2 准备着色器程序
	Shader shader("box.vertex", "box.frag");
	Shader lampshader("light.vertex", "light.frag");

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

		// 这里填写场景绘制代码
		shader.use();

		GLint lightPosLoc = glGetUniformLocation(shader.programId, "lightPos");
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);

		GLint viewPosLoc = glGetUniformLocation(shader.programId, "viewPos");
		glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);

		GLint objectColorLoc = glGetUniformLocation(shader.programId, "objectColor");
		GLint lightColorLoc = glGetUniformLocation(shader.programId, "lightColor");
		glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);// 珊瑚红
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f); //把光源设置为白色

		GLint modelLoc = glGetUniformLocation(shader.programId, "model");
		GLint viewLoc = glGetUniformLocation(shader.programId, "view");
		GLint projectionLoc = glGetUniformLocation(shader.programId, "projection");

		glm::mat4 view;
		view = camera.getViewMatrix();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 projection;
		projection = camera.getPerspectiveMatrix(WINDOW_WIDTH, WINDOW_HEIGHT);
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//draw the container
		glBindVertexArray(VAOId);
		glm::mat4 model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		//draw the lamp,again build the appropriate shader
		lampshader.use();
		modelLoc = glGetUniformLocation(lampshader.programId,"model");
		viewLoc = glGetUniformLocation(lampshader.programId,"view");
		projectionLoc = glGetUniformLocation(lampshader.programId,"projection");
		//set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));//make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glfwSwapBuffers(window); // 交换缓存
	}
	// 释放资源
	glDeleteVertexArrays(1, &VAOId);
	glDeleteVertexArrays(1, &lightVAO);

	glDeleteBuffers(1, &VBOId);
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