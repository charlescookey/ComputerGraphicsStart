#pragma once

#include "Mesh.h"
#include "Collision.h"

class StaticModel {
	std::vector<Mesh> meshes;
	std::vector<std::string> textureFilenames;

	int rings;
	int segments;
	float radius;


public:
	AABB boundingBox;

	bool checkCollision(const StaticModel* other) {
		return boundingBox.checkCollision(other->boundingBox);
	}

	void UpdateBoindingBox() {
		boundingBox.update(w);
	}

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
		boundingBox.extend(p);

		return v;
	}

	void init_model(DXCOre* core, std::string filename) {
		GEMLoader::GEMModelLoader loader;
		std::vector<GEMLoader::GEMMesh> gemmeshes;
		loader.load(filename, gemmeshes);
		for (int i = 0; i < gemmeshes.size(); i++) {
			Mesh mesh;
			std::vector<STATIC_VERTEX> vertices;
			for (int j = 0; j < gemmeshes[i].verticesStatic.size(); j++) {
				STATIC_VERTEX v;
				memcpy(&v, &gemmeshes[i].verticesStatic[j], sizeof(STATIC_VERTEX));
				vertices.push_back(v);
			}
			textureFilenames.push_back(gemmeshes[i].material.find("diffuse").getValue());
			mesh.init(core, vertices, gemmeshes[i].indices);
			meshes.push_back(mesh);
		}
	}

	void init_plane(DXCOre* core, std::string textureName) {
		Mesh mesh;
		std::vector<STATIC_VERTEX> vertices;
		vertices.push_back(addVertex(Vec3(-15, 0, -15), Vec3(0, 1, 0), 0, 0));
		vertices.push_back(addVertex(Vec3(15, 0, -15), Vec3(0, 1, 0), 1, 0));
		vertices.push_back(addVertex(Vec3(-15, 0, 15), Vec3(0, 1, 0), 0, 1));
		vertices.push_back(addVertex(Vec3(15, 0, 15), Vec3(0, 1, 0), 1, 1));
		std::vector<unsigned int> indices;
		indices.push_back(2); indices.push_back(1); indices.push_back(0);
		indices.push_back(1); indices.push_back(2); indices.push_back(3);
		mesh.init(core, vertices, indices);
		meshes.push_back(mesh);
		textureFilenames.push_back(textureName);
	}

	void init_cube(DXCOre* core, std::string textureName) {
		Mesh mesh;
		std::vector<STATIC_VERTEX> vertices;
		Vec3 p0 = Vec3(-1.0f, -1.0f, -1.0f);
		Vec3 p1 = Vec3(1.0f, -1.0f, -1.0f);
		Vec3 p2 = Vec3(1.0f, 1.0f, -1.0f);
		Vec3 p3 = Vec3(-1.0f, 1.0f, -1.0f);
		Vec3 p4 = Vec3(-1.0f, -1.0f, 1.0f);
		Vec3 p5 = Vec3(1.0f, -1.0f, 1.0f);
		Vec3 p6 = Vec3(1.0f, 1.0f, 1.0f);
		Vec3 p7 = Vec3(-1.0f, 1.0f, 1.0f);

		vertices.push_back(addVertex(p0, Vec3(0.0f, 0.0f, -1.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p1, Vec3(0.0f, 0.0f, -1.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p2, Vec3(0.0f, 0.0f, -1.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p3, Vec3(0.0f, 0.0f, -1.0f), 0.0f, 0.0f));

		vertices.push_back(addVertex(p5, Vec3(0.0f, 0.0f, 1.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p4, Vec3(0.0f, 0.0f, 1.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p7, Vec3(0.0f, 0.0f, 1.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p6, Vec3(0.0f, 0.0f, 1.0f), 0.0f, 0.0f));

		vertices.push_back(addVertex(p4, Vec3(-1.0f, 0.0f, 0.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p0, Vec3(-1.0f, 0.0f, 0.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p3, Vec3(-1.0f, 0.0f, 0.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p7, Vec3(-1.0f, 0.0f, 0.0f), 0.0f, 0.0f));


		vertices.push_back(addVertex(p1, Vec3(1.0f, 0.0f, 0.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p5, Vec3(1.0f, 0.0f, 0.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p6, Vec3(1.0f, 0.0f, 0.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p2, Vec3(1.0f, 0.0f, 0.0f), 0.0f, 0.0f));

		vertices.push_back(addVertex(p3, Vec3(0.0f, 1.0f, 0.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p2, Vec3(0.0f, 1.0f, 0.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p6, Vec3(0.0f, 1.0f, 0.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p7, Vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f));

		vertices.push_back(addVertex(p4, Vec3(0.0f, -1.0f, 0.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p5, Vec3(0.0f, -1.0f, 0.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p1, Vec3(0.0f, -1.0f, 0.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p0, Vec3(0.0f, -1.0f, 0.0f), 0.0f, 0.0f));


		std::vector<unsigned int> indices;
		indices.push_back(0); indices.push_back(1); indices.push_back(2);
		indices.push_back(0); indices.push_back(2); indices.push_back(3);
		indices.push_back(4); indices.push_back(5); indices.push_back(6);
		indices.push_back(4); indices.push_back(6); indices.push_back(7);
		indices.push_back(8); indices.push_back(9); indices.push_back(10);
		indices.push_back(8); indices.push_back(10); indices.push_back(11);
		indices.push_back(12); indices.push_back(13); indices.push_back(14);
		indices.push_back(12); indices.push_back(14); indices.push_back(15);
		indices.push_back(16); indices.push_back(17); indices.push_back(18);
		indices.push_back(16); indices.push_back(18); indices.push_back(19);
		indices.push_back(20); indices.push_back(21); indices.push_back(22);
		indices.push_back(20); indices.push_back(22); indices.push_back(23);

		mesh.init(core, vertices, indices);

		meshes.push_back(mesh);
		textureFilenames.push_back(textureName);

	}


	void init_sphere(DXCOre* core, std::string textureName, int _rings, int _segments, float _radius) {
		rings = _rings;
		segments = _segments;
		radius = _radius;
		Mesh mesh;
		std::vector<STATIC_VERTEX> vertices;
		for (int lat = 0; lat <= rings; lat++) {
			float theta = lat * M_PI / rings;
			float sinTheta = sinf(theta);
			float cosTheta = cosf(theta);
			for (int lon = 0; lon <= segments; lon++) {
				float phi = lon * 2.0f * M_PI / segments;
				float sinPhi = sinf(phi);
				float cosPhi = cosf(phi);
				Vec3 position(radius * sinTheta * cosPhi, radius * cosTheta, radius * sinTheta * sinPhi);
				Vec3 normal = position.normalize();
				float tu = 1.0f - (float)lon / segments;
				float tv = 1.0f - (float)lat / rings;
				vertices.push_back(addVertex(position, normal, tu, tv));
			}
		}

		std::vector<unsigned int> indices;
		for (int lat = 0; lat < rings; lat++)
		{
			for (int lon = 0; lon < segments; lon++)
			{
				int current = lat * (segments + 1) + lon;
				int next = current + segments + 1;
				indices.push_back(current);
				indices.push_back(next);
				indices.push_back(current + 1);
				indices.push_back(current + 1);
				indices.push_back(next);
				indices.push_back(next + 1);
			}
		}
		mesh.init(core, vertices, indices);
		meshes.push_back(mesh);
		textureFilenames.push_back(textureName);
	}

	void drawNoTexture(ShaderManager& shaders, std::string shadername, std::string buffername, std::string worldVarname, DXCOre* core) {
		shaders.updateConstantVS(shadername, buffername, worldVarname, &w);
		shaders.apply(shadername, core);

		for (int i = 0; i < meshes.size(); i++)
		{
			meshes[i].draw(core);
		}
	}

	void drawTexture(ShaderManager& shaders, std::string shadername, DXCOre* core, TextureManager* textures, std::string buffername, std::string worldVarname, std::string texVarname ) {
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

			}else{
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