// ����GLEW�� ���徲̬����
#define GLEW_STATIC
#include <Windows.h>
#include <debugapi.h>
#include <stdio.h>
#include <GLEW/glew.h>
// ����GLFW��
#include <GLFW/glfw3.h>
#include <SOIL\SOIL.h>
#include <glm\glm.hpp> //����opengl ��ѧ��
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <iostream>
#include <vector>

// ������ɫ�����ؿ�
#include "shader.h"
#include "Camera.h"

// ���̻ص���������
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
//����ƶ��ص���������
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//�����ֹ���
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void mouse_outIncallback(GLFWwindow* window, int flag);
//�ۺϴ�����̰������������
void Do_Movement();

// ���������
const float WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
float mixvalue = 0.5;
bool keys[1024] = { false };
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//����2���������ڼ���֡���
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
bool mouseFirst = true;
double lastx, lasty;

int main(int argc, char** argv)
{
	/***********************************************************************************/
	if (!glfwInit())	// ��ʼ��glfw��
	{
		std::cout << "Error::GLFW could not initialize GLFW!" << std::endl;
		return -1;
	}

	// ����OpenGL 3.3 core profile
	std::cout << "Start OpenGL core profile version 3.3" << std::endl;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// ��������
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
		"Lighting", NULL, NULL);
	if (!window)
	{
		std::cout << "Error::GLFW could not create winddow!" << std::endl;
		glfwTerminate();
		return -1;
	}
	// �����Ĵ��ڵ�contextָ��Ϊ��ǰcontext
	glfwMakeContextCurrent(window);

	// ע�ᴰ�ڼ����¼��ص�����
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorEnterCallback(window, mouse_outIncallback);
	//��������¼��ص�
	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetScrollCallback(window, scroll_callback);

	// ��ʼ��GLEW ��ȡOpenGL����
	glewExperimental = GL_TRUE; // ��glew��ȡ������չ����
	GLenum status = glewInit();
	if (status != GLEW_OK)
	{
		std::cout << "Error::GLEW glew version:" << glewGetString(GLEW_VERSION)
			<< " error string:" << glewGetErrorString(status) << std::endl;
		glfwTerminate();
		return -1;
	}

	// �����ӿڲ���
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	/***********************��������ͼ**************************/
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	const char* textures_faces[] = {//�������º�ǰ6�����
		"skybox/right.jpg",
		"skybox/left.jpg",
		"skybox/top.jpg",
		"skybox/bottom.jpg",
		"skybox/back.jpg",
		"skybox/front.jpg"
	};

	//����6���������
	int width, height;
	unsigned char* image;
	for (GLuint i = 0; i < 6; i++)
	{
		image = SOIL_load_image(textures_faces[i], &width, &height, NULL, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	}


	glEnable(GL_DEPTH_TEST);
	// ��ʼ��Ϸ��ѭ��
	while (!glfwWindowShouldClose(window))
	{
		GLfloat curFrameTime = (GLfloat)glfwGetTime();
		deltaTime = curFrameTime - lastFrame;
		lastFrame = curFrameTime;

		glfwPollEvents(); // ����������� ���̵��¼�
		Do_Movement();

		// �����ɫ������ ����Ϊָ����ɫ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwSwapBuffers(window); // ��������
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
//�ۺϴ�����̰������������
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