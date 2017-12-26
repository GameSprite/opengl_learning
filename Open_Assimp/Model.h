#ifndef _MODEL_H_
#define _MODEL_H_
#include "Mesh.h"
#include <SOIL\SOIL.h>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

GLint TextureFromFile(const char* path, string directory);
/********************************************************
*				   加载、转换后的模型                     *
*********************************************************/
class Model{
public:
	Model(GLchar* path);
	void Draw(Shader shader);
private:
	vector<Mesh> meshes;
	string directory;
	vector<Texture> textures_loaded;

	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};

Model::Model(GLchar* path){
	loadModel(path);
}
void Model::Draw(Shader shader){
	for (size_t i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shader);
	}
}
void Model::loadModel(string path){
	Assimp::Importer importer;
	//后处理(post-processing)参数: 将所有图元转换为三角形 | 翻转Y轴（opengl纹理的使用都会沿着y翻转，所以翻转回来）
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
		cout << "ERROR:ASSIMP::" << importer.GetErrorString() << endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode,scene);
}
/**
*递归遍历所有的节点，拿到所有节点的mesh索引数组里的mesh索引
*/
void Model::processNode(aiNode* node, const aiScene* scene){
	for (size_t i = 0; i < node->mNumMeshes;i++){
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	//递归
	for (size_t i = 0; i < node->mNumChildren;i++){
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene){
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	for (size_t i = 0; mesh->mNumVertices; i++){
		Vertex vertex;
		vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		if (mesh->mTextureCoords[0])//网格可以没有纹理坐标
			vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		else
			vertex.TexCoords = glm::vec2(0.0, 0.0);
		vertices.push_back(vertex);
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++){
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0){//网格可以没有材质索引
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	return Mesh(vertices, indices, textures);
}
vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName){
	vector<Texture> textures;
	for (size_t i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		//先在已加载过的纹理中查询
		bool skip = false;
		for (auto iter = (this->textures_loaded).begin(); iter != this->textures_loaded.end(); iter++){
			if ((*iter).path == str){
				skip = true;
				textures.push_back(*iter);
				break;
			}
		}
		if (skip){
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			this->textures_loaded.push_back(texture);
		}
	}
	return textures;
}

GLint TextureFromFile(const char* path, string directory)
{
	string filename = directory + '/' + string(path);
	GLuint texture2D;
	glGenTextures(1, &texture2D);
	int width, height;
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, texture2D);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return texture2D;
}
#endif