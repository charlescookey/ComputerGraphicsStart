#pragma once
#include "Mesh.h"

//STATIC_VERTEX

struct Alphabets
{
	float left, right;
	int size;
};

class MyFont {
public:
	//MyFont() {};
	Matrix w;
	Mesh mesh;
	std::string textureFilenames;
	float fontHeight = 32.0f;
	int spaceSize = 3;
	std::vector<Alphabets> font;
	std::vector< STATIC_VERTEX> vertices;
	Alphabets splitString(std::string input) {
		int temp;
		char temp2;
		std::stringstream ssin(input);
		Alphabets output;
		ssin >> temp;
		ssin >> temp2;
		ssin >> output.left;
		ssin >> output.right;
		ssin >> output.size;
		return output;
	}

	void loadFont(std::string filename) {
		std::ifstream infile(filename);
		int tilenum;
		std::string line;

		Alphabets temp;
		while (std::getline(infile, line)) {
			temp = splitString(line);
			font.push_back(temp);
		}
		infile.close();
	}
	STATIC_VERTEX addVertex(Vec3 p, Vec2 tex)
	{
		STATIC_VERTEX v;
		v.pos = p;
		v.normal = Vec3(1.0f, 0.0f, 0.0f);
		v.tangent = Vec3(0, 0, 0);
		v.tu = tex.x;
		v.tv = tex.y;
		return v;
	}
	void BuildVertexArray(std::string sentence, float drawX, float drawY)
	{

		int numLetters, i, letter;
		numLetters = sentence.size();


		// Draw each letter onto a quad.
		for (i = 0; i < numLetters; i++)
		{
			letter = ((int)sentence[i]) - 32;

			// If the letter is a space then just move over three pixels.
			if (letter == 0)
			{
				drawX = drawX + spaceSize;
			}
			else
			{
				// First triangle in quad.
				STATIC_VERTEX temp;
				temp = addVertex(Vec3(drawX, drawY, 0.0f), Vec2(font[letter].left, 0.0f));
				vertices.push_back(temp);

				temp = addVertex(Vec3((drawX + font[letter].size), (drawY - fontHeight), 0.0f), Vec2(font[letter].right, 1.0f));
				vertices.push_back(temp);

				temp = addVertex(Vec3(drawX, (drawY - fontHeight), 0.0f), Vec2(font[letter].left, 1.0f));
				vertices.push_back(temp);

				temp = addVertex(Vec3(drawX, drawY, 0.0f), Vec2(font[letter].left, 0.0f));
				vertices.push_back(temp);

				temp = addVertex(Vec3(drawX + font[letter].size, drawY, 0.0f), Vec2(font[letter].right, 0.0f));
				vertices.push_back(temp);

				temp = addVertex(Vec3((drawX + font[letter].size), (drawY - fontHeight), 0.0f), Vec2(font[letter].right, 1.0f));
				vertices.push_back(temp);

				drawX = drawX + font[letter].size + 1.0f;
			}
		}

		return;
	}
	void init(DXCOre* core, std::string textureName) {

		std::vector<unsigned int> indices;
		for (int i = 0; i < vertices.size(); i++) {
			indices.push_back(i);
		}
		mesh.init(core, vertices, indices);
		textureFilenames = textureName;
	}

	void drawTexture(ShaderManager& shaders, std::string shadername, DXCOre* core, TextureManager* textures, std::string buffername, std::string worldVarname, std::string texVarname) {
		shaders.updateConstantVS(shadername, buffername, worldVarname, &w);
		shaders.apply(shadername, core);

		shaders.updateTexturePS(shadername, core, texVarname, textures->find(textureFilenames));
		mesh.draw(core);

	}

};