#include "model.hpp"

#include <iostream>

void Model::load(std::string path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << "\n";
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {

	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<vertex>		   vertices;
	std::vector<unsigned int >  indices;
	std::vector<texture>       textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		vertex nVertex;

		glm::vec3 position = {
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		};

		glm::vec3 normals = {
			mesh->mNormals[i].x,
			mesh->mNormals[i].y,
			mesh->mNormals[i].z
		};

		if (mesh->mTextureCoords[0]) {
			glm::vec2 textCoords = {
				mesh->mTextureCoords[i]->x,
				mesh->mTextureCoords[i]->y,
			};

			nVertex.texCoords = textCoords;
		} else nVertex.texCoords = { 0.0f, 0.0f };

		nVertex.position = position;
		nVertex.normals = normals;

		vertices.push_back(nVertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[i]);
		}
	}

	return Mesh(vertices, indices, textures);
}

Model::Model(const char* path) {
	load(path);
}

void Model::draw(Shader& shader) {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].draw(shader);
	}
}
