#pragma once

#include "Mesh.h"

struct INSTANCE_DATA
{
	Vec3 position;
};

class InstanceMesh {
public:
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* instanceBuffer;

	int indicesSize;
	int vertextSize;
	int instancesSize;
	UINT strides[2];

	void init(DXCOre* core, void* vertices, int vertexSizeInBytes, int numVertices, void* instances, int instancesSizeInBytes, int numInstances, unsigned int* indices, int numIndices) {
		D3D11_BUFFER_DESC bd;
		memset(&bd, 0, sizeof(D3D11_BUFFER_DESC));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * numIndices;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		D3D11_SUBRESOURCE_DATA data;
		memset(&data, 0, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = indices;
		core->device->CreateBuffer(&bd, &data, &indexBuffer);

		bd.ByteWidth = vertexSizeInBytes * numVertices;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		data.pSysMem = vertices;
		core->device->CreateBuffer(&bd, &data, &vertexBuffer);


		bd.ByteWidth = instancesSizeInBytes * numInstances;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		data.pSysMem = instances;
		core->device->CreateBuffer(&bd, &data, &instanceBuffer);

		indicesSize = numIndices;
		instancesSize = numInstances;
		vertextSize = numVertices;
		strides[0] = vertexSizeInBytes;
		strides[1] = instancesSizeInBytes;

	}

	template<typename T>
	void init(DXCOre* core, std::vector<T> vertices, std::vector<unsigned int> indices, std::vector<INSTANCE_DATA> instances)
	{
		init(core, &vertices[0], sizeof(T), vertices.size(), &instances[0], sizeof(INSTANCE_DATA), instances.size(), &indices[0], indices.size());
	}

	void draw(DXCOre* core) {
		UINT offsets[2];
		offsets[0] = 0;
		offsets[1] = 0;

		ID3D11Buffer* buffers[2];
		buffers[0] = vertexBuffer;
		buffers[1] = instanceBuffer;

		core->devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		core->devicecontext->IASetVertexBuffers(0, 2, buffers, strides, offsets);
		core->devicecontext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		core->devicecontext->DrawIndexedInstanced(indicesSize, instancesSize, 0, 0, 0);
		//core->devicecontext->DrawInstanced(vertextSize, instancesSize, 0, 0);
	}
};


class InstanceModel {
	std::vector<InstanceMesh> meshes;
	std::vector<std::string> textureFilenames;


	std::vector<INSTANCE_DATA> instances;

public:
	Matrix w;

	Vec3 splitString(std::string input) {
		std::stringstream ssin(input);
		Vec3 output;
		ssin >> output.x;
		ssin >> output.y;
		ssin >> output.z;
		return output;
	}

	void loadInstancePosition(std::string filename) {
		std::ifstream infile(filename);
		int tilenum;
		std::string line;

		INSTANCE_DATA temp;
		while (std::getline(infile, line)) {
			temp.position = splitString(line);
			instances.push_back(temp);
		}
		infile.close();
	}

	void init_model(DXCOre* core, std::string filename) {
		GEMLoader::GEMModelLoader loader;
		std::vector<GEMLoader::GEMMesh> gemmeshes;
		loader.load(filename, gemmeshes);
		for (int i = 0; i < gemmeshes.size(); i++) {
			InstanceMesh mesh;
			std::vector<STATIC_VERTEX> vertices;
			for (int j = 0; j < gemmeshes[i].verticesStatic.size(); j++) {
				STATIC_VERTEX v;
				memcpy(&v, &gemmeshes[i].verticesStatic[j], sizeof(STATIC_VERTEX));
				vertices.push_back(v);
			}
			textureFilenames.push_back(gemmeshes[i].material.find("diffuse").getValue());

			mesh.init(core, vertices, gemmeshes[i].indices, instances);
			meshes.push_back(mesh);
		}
	}

	void drawNoTexture(ShaderManager& shaders, std::string shadername, std::string buffername, std::string worldVarname, DXCOre* core) {
		shaders.updateConstantVS(shadername, buffername, worldVarname, &w);
		shaders.apply(shadername, core);

		for (int i = 0; i < meshes.size(); i++)
		{
			meshes[i].draw(core);
		}
	}

	void drawTexture(ShaderManager& shaders, std::string shadername, DXCOre* core, TextureManager* textures, std::string buffername, std::string worldVarname, std::string texVarname) {
		shaders.updateConstantVS(shadername, buffername, worldVarname, &w);
		shaders.apply(shadername, core);

		for (int i = 0; i < meshes.size(); i++)
		{
			shaders.updateTexturePS(shadername, core, texVarname, textures->find(textureFilenames[i]));
			meshes[i].draw(core);
		}
	}

	void drawTextureSplit(ShaderManager& shaders, std::string shadername, std::string shadernamesplit, DXCOre* core, TextureManager* textures, std::string buffername, std::string buffernamesplit, std::string worldVarname, std::string texVarname, std::string texVarnamesplit, std::string textureSplit) {
		shaders.updateConstantVS(shadername, buffername, worldVarname, &w);
		shaders.updateConstantVS(shadernamesplit, buffernamesplit, worldVarname, &w);
		shaders.apply(shadername, core);
		shaders.apply(shadernamesplit, core);


		for (int i = 0; i < meshes.size(); i++)
		{
			if (textureFilenames[i] == textureSplit) {
				shaders.updateTexturePS(shadernamesplit, core, texVarnamesplit, textures->find(textureFilenames[i]));
				shaders.apply(shadernamesplit, core);

			}
			else {
				shaders.updateTexturePS(shadername, core, texVarname, textures->find(textureFilenames[i]));
				shaders.apply(shadername, core);
			}
			meshes[i].draw(core);
		}
	}

	void move(float x, float y, float z)
	{
		w.a[0][3] += x;
		w.a[1][3] += z;
		w.a[2][3] += y;
	}

	void translate(const Vec3& v)
	{
		w = Matrix::translation(v);
	}

	void scale(const Vec3& v)
	{
		w = Matrix::scaling(v);
	}

	Vec3 getPos() {
		return w.getTranslation();
	}

	void setPos(Vec3 newPos) {
		w.setTranslation(newPos);
	}
};