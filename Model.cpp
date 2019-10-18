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
//	//import scene, uses the OR operator
//	//triangulate will turn model into triangles, flip UV values along y axis, smooth normals to make things look rounded, join/merge vertices close to one another
//	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
//	
//	//test if scene is successfully imported
//	if (!scene)
//	{
//		printf("Model (%s) failed to load: %s", filename, importer.GetErrorString());
//		return;
//	}
//
//	//get the first node in the scene
//	LoadNode(scene->mRootNode, scene);
//
//	//after loading node, load materials in the scene
//	LoadMaterials(scene);
//}
//
//void Model::renderModel()
//{
//	for (size_t i = 0;  i< meshList.size(); i++)
//	{
//		unsigned int materialIndex = meshToTex[i];
//
//		if (materialIndex < textureList.size() && textureList[materialIndex])
//		{
//			textureList[materialIndex]->UseTexture();
//		}
//
//		meshList[i]->RenderMesh();
//	}
//}
//
//void Model::clearModel()
//{
//	for (size_t i = 0; i < meshList.size(); i++)
//	{
//		if (meshList[i])
//		{
//			delete meshList[i];
//			meshList[i] = nullptr;
//		}
//	}
//
//
//	for (size_t i = 0; i < textureList.size(); i++)
//	{
//		if (textureList[i])
//		{
//			delete textureList[i];
//			textureList[i] = nullptr;
//		}
//	}
//
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
//		//get meshes from scene then get mesh node ID from node
//		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
//	}
//	
//	//for each children node
//	for (size_t i = 0; i < node->mNumChildren; i++)
//	{
//		//recursion for all child nodes
//		LoadNode(node->mChildren[1], scene);
//	}
//}
//
//void Model::LoadMesh(aiMesh* Mesh, const aiScene* scene)
//{
//	//store vertices
//	std::vector<GLfloat> vertices;
//	//store indices
//	std::vector<unsigned int> indices;
//
//	for (size_t i = 0; i < Mesh->mNumVertices; i++)
//	{
//		//insert x, y, z values for an array initializer
//		vertices.insert(vertices.end(), { Mesh->mVertices[i].x,Mesh->mVertices[i].y, Mesh->mVertices[i].z });
//		
//		//check if textures e
//		if (Mesh->mTextureCoords[0])
//		{
//			vertices.insert(vertices.end(), { Mesh->mTextureCoords[0][i].x,Mesh->mTextureCoords[0][i].y});
//		}
//		else
//		{
//			vertices.insert(vertices.end(), {0.0f,0.0f});
//		}
//
//		vertices.insert(vertices.end(), { -Mesh->mNormals[i].x, -Mesh->mNormals[i].y, -Mesh->mNormals[i].z });
//	}
//
//	for (size_t i = 0; i< Mesh->mNumFaces; i++ )
//	{
//		aiFace face = Mesh->mFaces[i];
//		for (size_t j = 0; j< face.mNumIndices; j++)
//		{
//			indices.push_back(face.mIndices[j]);
//		}
//	}
//
//	mesh* newMesh = new mesh();
//	newMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
//	meshList.push_back(newMesh);
//	meshToTex.push_back(Mesh->mMaterialIndex);
//	
//}
//
////run through all materials and store them appropriately
//void Model::LoadMaterials(const aiScene* scene)
//{
//	textureList.resize(scene->mNumMaterials);
//
//	for (size_t i = 0; i< scene->mNumMaterials; i++)
//	{
//		aiMaterial* material = scene->mMaterials[i];
//		textureList[i] = nullptr;
//		if (material->GetTextureCount(aiTextureType_DIFFUSE))
//		{
//			aiString path;
//
//			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
//			{
//				//find backslash
//				int idx = std::string(path.data).rfind("\\");
//				//create new string after backslash, save in filename
//				std::string filename = std::string(path.data).substr(idx + 1);
//
//				std::string texPath = std::string("Textures/") + filename;
//
//				textureList[i] = new Texture(texPath.c_str());
//				
//				//if loading texture fails
//				if (!textureList[i]->LoadTexture())
//				{
//					printf("failed to load texture at: %s\n", texPath);
//					delete textureList[i];
//					textureList[i] = nullptr;
//				}
//			}
//		}
//
//		if (!textureList[i])
//		{
//			textureList[i] = new Texture("Texture//plain.png");
//			textureList[i]->LoadTextureA();
//		}
//	}
//
//}
