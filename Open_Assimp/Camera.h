#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <GLEW\glew.h>
#include <glm\glm.hpp> //引入opengl 数学库
#include <glm\gtc\matrix_transform.hpp>
// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

//Default camera values
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVTY = 0.25f;
const GLfloat ZOOM = 45.0f;

class Camera
{
public:
	//Camera Attributes
	glm::vec3 Position;// 观察目标
	glm::vec3 Front;//前的方向
	glm::vec3 Up;//上方向
	glm::vec3 Right;//右方向
	glm::vec3 WorldUp;
	//Eular Angles
	GLfloat Pitch;//俯视角
	GLfloat Yaw;//航偏角
	//Camera options
	GLfloat MovementSpeed;//摄像机移动速度
	GLfloat MouseSensitivity;//鼠标灵敏度
	GLfloat Zoom;//相机缩放(视野大小)

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH)
		:Front(0.0f, 0.0f, -1.0f), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
	{
		this->Position = position;
		this->WorldUp = up;
		this->Yaw = yaw;
		this->Pitch = pitch;
	}

	glm::mat4 getViewMatrix(){
		return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
	}

	glm::mat4 getPerspectiveMatrix(float screenWidth, float screenHeight){
		return glm::perspective(this->Zoom, screenWidth / screenHeight, 1.0f, 100.0f);
	}

	void processMouseMovement(GLfloat xoffset, GLfloat yoffset, bool constrainPitch = true){
		xoffset *= this->MouseSensitivity;
		yoffset *= this->MouseSensitivity;

		this->Yaw += xoffset;
		this->Pitch += yoffset;

		if (constrainPitch)
		{
			if (this->Pitch > 89.0f)
				this->Pitch = 89.0f;
			if (this->Pitch < -89.0f)
				this->Pitch = -89.0;
		}
		this->updateCameraVectors();
	}

	void processKeyboard(Camera_Movement direction, GLfloat deltaTime){
		GLfloat velocity = this->MovementSpeed * deltaTime;
		if (direction == FORWARD)
		{
			this->Position += this->Front * velocity;
		}
		if (direction == BACKWARD)
		{
			this->Position -= this->Front * velocity;
		}
		if (direction == LEFT)
		{
			this->Position -= this->Right * velocity;
		}
		if (direction == RIGHT)
		{
			this->Position += this->Right * velocity;
		}

		this->updateCameraVectors();
	}

	void ProcessMouseScroll(GLfloat yoffset){
		yoffset *= 0.3;
		this->Zoom -= yoffset;
		if (this->Zoom > 45.0f)
		{
			this->Zoom = 45.0f;
		}
		if (this->Zoom < 20.0f)
		{
			this->Zoom = 20.0f;
		}
	}
private:
	//calculates the Front vector from Camera's (updated) Eular Angles
	void updateCameraVectors(){
		glm::vec3 front;
		front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		front.y = sin(glm::radians(this->Pitch));
		front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		this->Front = glm::normalize(front);
		//re-calculate the Right and Up vector
		this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
		this->Up = glm::normalize(glm::cross(this->Right, this->Front));
	}

};
#endif