//#pragma once
//
//#include <vector>
//#include <string>
//
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
//
//#include "mesh.h"
//#include "Texture.h"
//
//
//class Model
//{
//public:
//	Model();
//
//	void LoadModel(const std::string& filename);
//	void renderModel();
//	void clearModel();
//
//	~Model();
//
//private:
//
//	void LoadNode(aiNode *node, const aiScene *scene);
//	void LoadMesh(aiMesh* mesh, const aiScene* scene);
//	void LoadMaterials(const aiScene *scene);
//
//	std::vector<mesh*> meshList;
//	std::vector<Texture*> textureList;
//	std::vector<unsigned int> meshToTex; 
//
//};
//
