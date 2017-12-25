#ifndef _MESH_H_
#define _MESH_H_
#include <iostream>
#include <sstream>
#include <vector>
#include <glm\glm.hpp>
#include <GLEW\glew.h>
#include "shader.h"

using namespace std;

/************Vertex***************/
struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};
/************Texture**************/
struct Texture
{
	GLuint id;
	string type;//纹理类型，diffuse或者specular
	aiString path;
};

/***********************************************
*                Mesh网格                      *
************************************************/
class Mesh{
public:
	vector<Vertex> vertices;
	vector<GLuint> indices;//顶点索引
	vector<Texture> textures;
	Mesh(vector<Vertex> vertices, vector<GLuint> indices,vector<Texture> textures);
	void Draw(Shader shader);
private:
	GLuint VAO, VBO, EBO;
	void setupMesh();
};

Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures){
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	this->setupMesh();
}

void Mesh::setupMesh(){
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof Vertex,&vertices[0],GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof GLuint,&indices[0],GL_STATIC_DRAW);

	//setup vertex position attribute point
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof Vertex, (GLvoid*)0);

	//setup vertex normal attribute point
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof Vertex, (GLvoid*)offsetof(Vertex, Normal));

	//setup vertex texCoords attribute point
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof Vertex, (GLvoid*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}

void Mesh::Draw(Shader shader){
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	for (size_t i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		//检索纹理序号
		stringstream ss;
		string number;
		string name = textures[i].type;
		if (name == "texture_diffuse"){
			ss << diffuseNr++;
		}else if (name == "texture_specular"){
			ss << specularNr++;
		}
		number = ss.str();
		//传入采样器uniform
		glUniform1i(glGetUniformLocation(shader.programId, ("material." + name + number).c_str()), i);
		glBindTexture(GL_TEXTURE_2D,textures[i].id);
	}
	// Also set each mesh's shininess property to a default value (if you want you could extend this to another mesh property and possibly change this value)
	glUniform1f(glGetUniformLocation(shader.programId, "material.shininess"),16.0f);
	//draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	// Always good practice to set everything back to defaults once configured.
	for (size_t i = 0; i < textures.size(); i++){
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
#endif