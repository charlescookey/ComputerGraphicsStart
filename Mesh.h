#pragma once

#include "Math.h"
#include "DXCore.h"


#include "GEMLoader.h"
#include "ShaderManager.h"


#include "Texture.h"



struct Vertex
{
	Vec3 position;
	Colour colour;
	Vertex() {};
};

struct STATIC_VERTEX
{
	Vec3 pos;
	Vec3 normal;
	Vec3 tangent;
	float tu;
	float tv;
};

struct ANIMATED_VERTEX
{
	Vec3 pos;
	Vec3 normal;
	Vec3 tangent;
	float tu;
	float tv;
	unsigned int bonesIDs[4];
	float boneWeights[4];
};

class Mesh {
public:
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* vertexBuffer;
	int indicesSize;
	UINT strides;

	void init(DXCOre* core ,void* vertices, int vertexSizeInBytes, int numVertices, unsigned int* indices, int numIndices) {
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
		indicesSize = numIndices;
		strides = vertexSizeInBytes;
	}

	template<typename T>
	void init(DXCOre* core, std::vector<T> vertices, std::vector<unsigned int> indices)
	{
		init(core,&vertices[0], sizeof(T), vertices.size(), &indices[0], indices.size());
	}

	void draw( DXCOre* core) {

		UINT offsets = 0;
		core->devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		core->devicecontext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
		core->devicecontext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		core->devicecontext->DrawIndexed(indicesSize, 0, 0);
	}
};

class CustomMesh {
public:
	Mesh geometry;
	Matrix w;

	STATIC_VERTEX addVertex(Vec3 p, Vec3 n, float tu, float tv)
	{
		STATIC_VERTEX v;
		v.pos = p;
		v.normal = n;
		//Frame frame;
		//frame.fromVector(n);c
		//v.tangent = frame.u; // For now
		v.tangent = Vec3(0, 0, 0); // For now
		v.tu = tu;
		v.tv = tv;
		return v;
	}

	void _init(DXCOre* core) {
		std::vector<STATIC_VERTEX> vertices;
		vertices.push_back(addVertex(Vec3(-15, 0, -15), Vec3(0, 1, 0), 0, 0));
		vertices.push_back(addVertex(Vec3(15, 0, -15), Vec3(0, 1, 0), 1, 0));
		vertices.push_back(addVertex(Vec3(-15, 0, 15), Vec3(0, 1, 0), 0, 1));
		vertices.push_back(addVertex(Vec3(15, 0, 15), Vec3(0, 1, 0), 1, 1));
		std::vector<unsigned int> indices;
		indices.push_back(2); indices.push_back(1); indices.push_back(0);
		indices.push_back(1); indices.push_back(2); indices.push_back(3);
		geometry.init(core, vertices, indices);

	}

	void draw(ShaderManager& shaders, std::string shadername, DXCOre* core) {
		shaders.updateConstantVS(shadername, "staticMeshBuffer", "W", &w);
		shaders.apply(shadername, core);

		geometry.draw(core);
	}

	//void move(const Vec3& v, ShaderManager& shaders, std::string shadername, DXCOre* core)
	void move(float v, ShaderManager& shaders, std::string shadername, DXCOre* core)
	{
		w.a[0][3] += v;
		//w = Matrix::translation(v);

		shaders.updateConstantVS(shadername, "staticMeshBuffer", "W", &w);
		shaders.apply(shadername, core);
	}
	
	void translate(const Vec3& v, ShaderManager& shaders, std::string shadername, DXCOre* core)
	{
		w = Matrix::translation(v);

		shaders.updateConstantVS(shadername, "staticMeshBuffer", "W", &w);
		shaders.apply(shadername, core);
	}

	void scale(const Vec3& v, ShaderManager& shaders, std::string shadername, DXCOre* core)
	{
		w = Matrix::scaling(v);
		shaders.updateConstantVS(shadername, "staticMeshBuffer", "W", &w);
		shaders.apply(shadername, core);
	}

	void init(DXCOre* core , std::string filename) {

		std::vector<STATIC_VERTEX> vertices;
		std::vector<unsigned int> indices;


		std::ifstream infile(filename);
		int tilenum;
		std::string line;

		Vec3 vertex;
		Vec3 normal;
		float tu; float tv;

		for (int i = 0; i < 4; i++) {
			std::getline(infile, line);
		}

		int i = 0;
		while (std::getline(infile, line)) {
			std::stringstream ssin(line);
			
			ssin >> vertex.x;
			ssin >> vertex.y;
			ssin >> vertex.z;

			ssin >> tu;
			ssin >> tv;
			
			ssin >> normal.x;
			ssin >> normal.y;
			ssin >> normal.z;



			vertices.push_back(addVertex(vertex, normal, tu, tv));
			indices.push_back(i++);
		}
		infile.close();
		geometry.init(core, vertices, indices);
	}
};

class Triangle {
public:
	ID3D11Buffer* vertexBuffer;
	Vertex vertices[3];

	//just cause
	int N = 3;

	void init(DXCOre *core) {
		vertices[0].position = Vec3(0, 1.0f, 0);
		vertices[0].colour = Colour(0.f, 1.0f, 0.f, 1.f);
		vertices[1].position = Vec3(-1.0f, -1.0f, 0);
		vertices[1].colour = Colour(1.0f, 0.f, 0.f, 1.f);
		vertices[2].position = Vec3(1.0f, -1.0f, 0);
		vertices[2].colour = Colour(0.f, 0.f, 1.0f, 1.f);


		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA uploadData;
		bd.ByteWidth = sizeof(Vertex) * N;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		uploadData.pSysMem = vertices;
		uploadData.SysMemPitch = 0;
		uploadData.SysMemSlicePitch = 0;
		core->device->CreateBuffer(&bd, &uploadData, &vertexBuffer);
	}

	void draw(DXCOre* core) {
		UINT offsets;
		offsets = 0;
		UINT strides = sizeof(Vertex);
		core->devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		core->devicecontext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
		core->devicecontext->Draw(3, 0);
	}

};