#pragma once

#include <glad/glad.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>

#include "mesh.hpp"
#include "shader.hpp"	

class Model {
private:
	std::vector<Mesh> meshes;
	std::string directory;

private:
	void load(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<texture> loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName);

public:
	Model(const char* path);
	void draw(Shader &shader);
};