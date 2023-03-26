#pragma once

#include <glad/glad.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>

#include "mesh.hpp"
#include "shader.hpp"
#include "texture.hpp"

class Model {
private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<texture> texture_cache;

private:
	void load(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<texture> loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName);
	unsigned int textureFromFile(const char* name, const std::string& directory, bool game = false);
public:
	Model(const char* path);
	void draw(Shader &shader);
};