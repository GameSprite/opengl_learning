#ifndef _MODEL_H_
#define _MODEL_H_
#include "Mesh.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
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

	}
}
#endif