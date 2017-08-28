// ����GLEW�� ���徲̬����
#define GLEW_STATIC
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

// ���̻ص�����ԭ������
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// ���������
const int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;

float mixvalue = 0.5;
int main(int argc, char** argv)
{
	/*
	  glm ʹ�ò���	
	*/
	/*
	//��һ����(1,0,0)λ��(1,1,0)
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 trans; //Ĭ�Ϲ����һ��4ά��λ����
	trans = glm::translate(trans,glm::vec3(1.0f,1.0f,0.0f));
	vec = trans * vec;
	std::cout << vec.x << vec.y << vec.z << vec.w << std::endl; //(2,1,0,1)
	*/
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
		"Demo of triangle", NULL, NULL);
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

	// Section1 ׼����������
	// ָ�������������� ����λ��
	GLfloat vertices[] = {
		//---- λ�� ----       ---- ��ɫ ----     - �������� -
		 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // ����
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // ����
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // ����
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f    // ����
	};
	GLuint indices[] = {
		3, 2, 1, //��һ��������
		3, 0, 1  //�ڶ���������
	};
	// �����������
	GLuint VAOId, VBOId, EBOId;
	glGenVertexArrays(1, &VAOId);
	glGenBuffers(1, &VBOId);
	glGenBuffers(1, &EBOId);

	glBindVertexArray(VAOId);

	glBindBuffer(GL_ARRAY_BUFFER, VBOId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//��������EBO����

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// ָ��������ʽ  �����ö�������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)0); //λ������
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GLfloat)));//��ɫ����
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(6 * sizeof(GLfloat)));//������������
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	//ʹ������
	GLuint texture1;

	glGenTextures(1, &texture1); //GL_TEXTURE0Ĭ�����Ǳ������
	glBindTexture(GL_TEXTURE_2D, texture1);
	//����wrap����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	/*GLfloat borderColor[] = { 0.0, 0.0, 0.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);*/
	//���ò�������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//���ش��� ��������mipmap�༶��Զ����
	int width, height;
	unsigned char* image = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);// Unbind texture when done, so we won't accidentily mess up our texture.
	//ʹ�õ�2������
	GLuint texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	//����wrap����
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	
	//���ò�������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	image = SOIL_load_image("awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	// Section2 ׼����ɫ������
	Shader shader("triangle.vertex", "triangle.frag");

	/*
	//�����һ������0.5��,Ȼ����ʱ����ת90��(��z��)�ı任����
	glm::mat4 trans;
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	trans = glm::scale(trans, glm::vec3(0.5,0.5,0.5));
	*/

	//����ģ�;�������ʹ�ö�����X����תһ���Ƕȣ�����ת������������ϵ
	glm::mat4 model;
	model = glm::rotate(model, glm::radians(-55.5f), glm::vec3(1.0, 0.0, 0.0));
	//����۲��������ƶ�(���������)���൱�ڳ���������һ��
	glm::mat4 view;
	view = glm::translate(view, glm::vec3(0.0, 0.0, -3.0));
	//Ȼ����һ��͸��ͶӰ����
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);

	// ��ʼ��Ϸ��ѭ��
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents(); // ����������� ���̵��¼�

		// �����ɫ������ ����Ϊָ����ɫ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// ������д�������ƴ���
		shader.use();

		//bind Textures using texture 
		//glActiveTexture(GL_TEXTURE);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(shader.programId, "ourTexture"), 0);
		//����ڶ�������Ԫ
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(shader.programId,"outTexture2"), 1);
		glUniform1f(glGetUniformLocation(shader.programId,"mixvalue"),mixvalue);
		//
		GLint modelLoc = glGetUniformLocation(shader.programId, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		GLint viewLoc = glGetUniformLocation(shader.programId, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		GLint projectionLoc = glGetUniformLocation(shader.programId, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		
		
		glBindVertexArray(VAOId);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		glfwSwapBuffers(window); // ��������
	}
	// �ͷ���Դ
	glDeleteVertexArrays(1, &VAOId);
	glDeleteBuffers(1, &VBOId);
	glfwTerminate();
	return 0;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE); // �رմ���
	}
	else if (key == GLFW_KEY_UP){
		mixvalue = mixvalue + 0.1 > 1 ? 1 : mixvalue + 0.1;
	}
	else if (key == GLFW_KEY_DOWN){
		mixvalue = mixvalue - 0.1 < 0 ? 0 : mixvalue - 0.1;
	}
}