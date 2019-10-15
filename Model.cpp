//#include "Model.h"
//
//Model::Model()
//{
//}
//
//void Model::LoadModel(const std::string& filename)
//{
//	Assimp::Importer importer;
//	
//	//import scene
//	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
//	
//	//test
//	if (!scene)
//	{
//		printf("Model (%s) failed to load: %s", filename, importer.GetErrorString());
//		return;
//	}
//
//	//
//	LoadNode(scene->mRootNode, scene);
//
//	//
//	LoadMaterials(scene);
//}
//
//void Model::renderModel()
//{
//}
//
//void Model::clearModel()
//{
//}
//
//Model::~Model()
//{
//}
//
//void Model::LoadNode(aiNode* node, const aiScene* scene)
//{
//	//load each mesh in the node
//	for (size_t i = 0; i < node->mNumMeshes; i++)
//	{
//		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
//	}
//
//	for (size_t i = 0; i < node->mNumChildren; i++)
//	{
//		LoadNode(node->mChildren[1], scene);
//	}
//}
//
//void Model::LoadMesh(aiMesh* mesh, const aiScene* scene)
//{
//	//store vertices
//	std::vector<GLfloat> vertices;
//	std::vector<unsigned int> indices;
//
//	for (size_t i = 0; i < mesh->mNumVertices; i++)
//	{
//		//insert x, y, z values
//		vertices.insert(vertices.end(), { mesh->mVertices[i].x,mesh->mVertices[i].y, mesh->mVertices[i].z });
//	}
//}
//
//void Model::LoadMaterials(const aiScene* scene)
//{
//}
