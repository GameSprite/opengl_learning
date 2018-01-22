#pragma once
#pragma execution_character_set("utf-8")

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

GLuint loadTexture(const char* path);
GLuint generateAttackmentTexture(GLboolean depth, GLboolean stencil);
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

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

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

	/**************************************************************************************/
	//vertices data
	float cubeVertices[] = {
		// positions          // texture Coords
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
	};
	float planeVertices[] = {
		// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
		5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
		-5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

		5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
		5.0f, -0.5f, -5.0f, 2.0f, 2.0f
	};

	GLfloat quadVertices[] = {   // Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// Positions   // TexCoords
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,

		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};

	//cube VAO
	GLuint cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);

	glBindVertexArray(cubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);//开启第一个顶点属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);//开启第二个顶点属性
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);


	//plane VAO
	GLuint planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);

	glBindVertexArray(planeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);

	//quad VAO
	GLuint quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);

	glBindVertexArray(quadVAO);

	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindVertexArray(0);

	//load texture
	GLuint cubeTexture = loadTexture("texture/marble.jpg");
	GLuint floorTexture = loadTexture("texture/metal.png");

	//FrameBuffer
	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	//Create a color attachment texture
	GLuint textureColorbuffer = generateAttackmentTexture(false, false);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	//Create a renderbuffer object for depth and stencil
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	// Use a single renderbuffer object for both a depth AND stencil buffer.
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Shader shader("screen.vs","screen.fs");
	Shader frameScreenShader("framebuffers.vs", "framebuffers.fs");
	
	// 开始游戏主循环
	while (!glfwWindowShouldClose(window))
	{
		GLfloat curFrameTime = (GLfloat)glfwGetTime();
		deltaTime = curFrameTime - lastFrame;
		lastFrame = curFrameTime;

		glfwPollEvents(); // 处理例如鼠标 键盘等事件
		Do_Movement();
		 
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		// 清除颜色缓冲区、深度缓冲区、模板缓冲区
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		shader.use();

		//绘制地面
		glm::mat4 model = glm::mat4();
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniform1i(glGetUniformLocation(shader.programId, "texture1"), 0);
		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//绘制2个立方体
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		/*****************************************************************/
		/*Bind to default framebuffer again and draw the quad plane with attached screen texture*/
		/*****************************************************************/
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//Clear all relevant buffers
		glClearColor(1.0, 1.0, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST); // We don't care about depth information when rendering a single quad

		//Draw Screen copied
		frameScreenShader.use();
		glBindVertexArray(quadVAO);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		glfwSwapBuffers(window); // 交换缓存
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &planeVBO);
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

GLuint loadTexture(const char* path)
{
	int width, height, channel;
	GLuint textureID;
	glGenTextures(1, &textureID);
	unsigned char *data = SOIL_load_image(path, &width, &height, &channel, SOIL_LOAD_AUTO);
	if (data)
	{
		GLenum format;
		if (channel == 1)
			format = GL_RED;//单通道
		else if (channel == 3)
			format = GL_RGB;
		else if (channel == 4)
			format = GL_RGBA;
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//param wrap
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
	}
	SOIL_free_image_data(data);
	return textureID;
}
/**
*生成紋理附件
*/
GLuint generateAttackmentTexture(GLboolean depth, GLboolean stencil){
	GLenum attachment_type;
	if (!depth && !stencil)
		attachment_type = GL_RGB;
	else if (depth && !stencil)
		attachment_type = GL_DEPTH_COMPONENT;
	else if (!depth && stencil)
		attachment_type = GL_STENCIL_INDEX;

	//Genarate texture ID and load texture data
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	if (!depth && !stencil)
		glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, WINDOW_WIDTH, WINDOW_HEIGHT, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8,NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	return textureID;
}