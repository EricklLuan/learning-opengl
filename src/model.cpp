#include "model.hpp"

#include <iostream>
#include <stb_image.h>

void Model::load(std::string path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << "\n";
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}

	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
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
		nVertex.position = position;

		if (mesh->HasNormals()) {
			glm::vec3 normals = {
				mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z
			};
			nVertex.normals = normals;
		}

		if (mesh->mTextureCoords[0]) {
			glm::vec2 textCoords = {
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y,
			};
			
			nVertex.texCoords = textCoords;
		} else nVertex.texCoords = { 0.0f, 0.0f };

		vertices.push_back(nVertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
		
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");

		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	}

	return Mesh(vertices, indices, textures);
}

std::vector<texture> Model::loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName) {

	std::vector<texture> textures;
	for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
		aiString str;
		material->GetTexture(type, i, &str);
		
		bool skip = false;
		for (unsigned int j = 0; j < texture_cache.size(); j++) {
			if (std::strcmp(str.C_Str(), texture_cache[j].path.data()) == 0) {
				textures.push_back(texture_cache[j]);
				skip = true;
				break;
			}
		}

		if (!skip) {
			texture tex;
			tex.ID = textureFromFile(str.C_Str(), directory);
			tex.type = typeName;
			tex.path = str.C_Str();
			textures.push_back(tex);
			texture_cache.push_back(tex);
		}
	}

	return textures;
}

unsigned int Model::textureFromFile(const char* name, const std::string& directory, bool game) {
	std::string filename = std::string(name);
	filename = directory + '/' + filename;

	unsigned int ID;
	glGenTextures(1, &ID);

	int width, height, nrChannels;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);

	if (data) {
		GLenum format{};

		if (nrChannels == 1) format = GL_RED;
		else if (nrChannels == 3) format = GL_RGB;
		else if (nrChannels == 4) format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, ID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	} else {
		std::cout << "Texture failed to load in path: " << filename << "\n";
		stbi_image_free(data);
	}

	return ID;
}

Model::Model(const char* path) {
	load(path);
}

void Model::draw(Shader& shader) {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].draw(shader);
	}
}
