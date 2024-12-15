#pragma once
#include "Mesh.h"
#include <map>
#include "Collision.h"

struct Bone
{
	std::string name;
	Matrix offset;
	int parentIndex;
};

struct Skeleton
{
	std::vector<Bone> bones;
	Matrix globalInverse;
};

struct AnimationFrame
{
	std::vector<Vec3> positions;
	std::vector<Quaternions> rotations;
	std::vector<Vec3> scales;
};

class AnimationSequence
{
public:
	std::vector<AnimationFrame> frames;
	float ticksPerSecond;
	Vec3 interpolate(Vec3 p1, Vec3 p2, float t) {
		return ((p1 * (1.0f - t)) + (p2 * t));
	}
	Quaternions interpolate(Quaternions q1, Quaternions q2, float t) {
		return Quaternions::slerp(q1, q2, t);
	}
	float duration() {
		return ((float)frames.size() / ticksPerSecond);
	}

	void calcFrame(float t, int& frame, float& interpolationFact)
	{
		interpolationFact = t * ticksPerSecond;
		frame = (int)floorf(interpolationFact);
		interpolationFact = interpolationFact - (float)frame;
		frame = min(frame, frames.size() - 1);
	}

	int nextFrame(int frame)
	{
		return min(frame + 1, frames.size() - 1);
	}

	Matrix interpolateBoneToGlobal(Matrix* matrices, int baseFrame, float interpolationFact, Skeleton* skeleton, int boneIndex) {
		int nextFrameIndex = nextFrame(baseFrame);
		Matrix scale = Matrix::scaling(interpolate(frames[baseFrame].scales[boneIndex], frames[nextFrameIndex].scales[boneIndex], interpolationFact));
		Matrix rotation = interpolate(frames[baseFrame].rotations[boneIndex], frames[nextFrameIndex].rotations[boneIndex], interpolationFact).toMatrix();
		Matrix translation = Matrix::translation(interpolate(frames[baseFrame].positions[boneIndex], frames[nextFrameIndex].positions[boneIndex], interpolationFact));
		//Matrix local = scale * rotation * translation;
		//if (skeleton->bones[boneIndex].parentIndex > -1) {
		//	Matrix global = local * matrices[skeleton->bones[boneIndex].parentIndex];
		//	return global;
		//}
		Matrix local = translation * rotation * scale;
		if (skeleton->bones[boneIndex].parentIndex > -1) {
			Matrix global = matrices[skeleton->bones[boneIndex].parentIndex] * local;
			return global;
		}
		return local;
	}


};

class Animation
{
public:
	
	std::map<std::string, AnimationSequence> animations;
	Skeleton skeleton;
	void calcFrame(std::string name, float t, int& frame, float& interpolationFact) {
		animations[name].calcFrame(t, frame, interpolationFact);
	}
	Matrix interpolateBoneToGlobal(std::string name, Matrix* matrices, int baseFrame, float interpolationFact, int boneIndex) {
		return animations[name].interpolateBoneToGlobal(matrices, baseFrame, interpolationFact, &skeleton, boneIndex);
	}
	void calcFinalTransforms(Matrix* matrices)
	{
		for (int i = 0; i < bonesSize(); i++)
		{
			//matrices[i] = skeleton.bones[i].offset * matrices[i] * skeleton.globalInverse;
			matrices[i] = matrices[i] * skeleton.bones[i].offset * skeleton.globalInverse;
		}
	}

	//confirm
	int bonesSize() {
		return skeleton.bones.size();
	}
};

class AnimationInstance
{
public:
	Animation* animation;
	std::string currentAnimation;
	float t;
	Matrix matrices[256];

	void resetAnimationTime()
	{
		t = 0;
	}
	bool animationFinished()
	{
		if (t > animation->animations[currentAnimation].duration())
		{
			return true;
		}
		return false;
	}
	void update(std::string name, float dt) {
		if (name == currentAnimation) {
			t += dt;
		}
		else {
			currentAnimation = name;  t = 0;
		}
		if (animationFinished() == true) { resetAnimationTime(); }
		int frame = 0;
		float interpolationFact = 0;
		animation->calcFrame(name, t, frame, interpolationFact);
		for (int i = 0; i < animation->bonesSize(); i++)
		{
			matrices[i] = animation->interpolateBoneToGlobal(name, matrices, frame, interpolationFact, i);
		}
		//animation->calcFinalTransforms(matrices, coordTransform);
		animation->calcFinalTransforms(matrices);
	}


};

class AnimatedModel {
	std::vector<Mesh> meshes;
	Matrix w;

public:
	Animation animation;
	std::vector<std::string> textureFilenames;


	AABB boundingBox;

	bool checkCollision(const AABB& other) {
		return boundingBox.checkCollision(other);
	}

	bool checkRayCollision(const Ray& other , float& t) {
		return boundingBox.rayAABB(other , t);
	}

	void UpdateBoindingBox() {
		boundingBox.update(w);
	}

	void init(DXCOre* core, std::string filename) {
		GEMLoader::GEMModelLoader loader;
		std::vector<GEMLoader::GEMMesh> gemmeshes;
		GEMLoader::GEMAnimation gemanimation;
		loader.load(filename, gemmeshes, gemanimation);
		for (int i = 0; i < gemmeshes.size(); i++) {
			Mesh mesh;
			std::vector<ANIMATED_VERTEX> vertices;
			for (int j = 0; j < gemmeshes[i].verticesAnimated.size(); j++) {
				ANIMATED_VERTEX v;
				memcpy(&v, &gemmeshes[i].verticesAnimated[j], sizeof(ANIMATED_VERTEX));
				boundingBox.extend(v.pos);
				vertices.push_back(v);
			}
			textureFilenames.push_back(gemmeshes[i].material.find("diffuse").getValue());
			mesh.init(core, vertices, gemmeshes[i].indices);
			meshes.push_back(mesh);
		}

		for (int i = 0; i < gemanimation.bones.size(); i++)
		{
			Bone bone;
			bone.name = gemanimation.bones[i].name;
			memcpy(&bone.offset, &gemanimation.bones[i].offset, 16 * sizeof(float));
			bone.parentIndex = gemanimation.bones[i].parentIndex;
			animation.skeleton.bones.push_back(bone);
		}

		for (int i = 0; i < gemanimation.animations.size(); i++)
		{
			std::string name = gemanimation.animations[i].name;
			AnimationSequence aseq;
			aseq.ticksPerSecond = gemanimation.animations[i].ticksPerSecond;
			for (int n = 0; n < gemanimation.animations[i].frames.size(); n++)
			{
				AnimationFrame frame;
				for (int index = 0; index < gemanimation.animations[i].frames[n].positions.size(); index++)
				{
					Vec3 p;
					Quaternions q;
					Vec3 s;
					memcpy(&p, &gemanimation.animations[i].frames[n].positions[index], sizeof(Vec3));
					frame.positions.push_back(p);
					memcpy(&q, &gemanimation.animations[i].frames[n].rotations[index], sizeof(Quaternions));
					frame.rotations.push_back(q);
					memcpy(&s, &gemanimation.animations[i].frames[n].scales[index], sizeof(Vec3));
					frame.scales.push_back(s);
				}
				aseq.frames.push_back(frame);
			}
			animation.animations.insert({ name, aseq });
		}		
	}

	void draw(ShaderManager& shaders, std::string shadername, DXCOre* core, TextureManager* textures) {
		//shaders.updateConstantVS(shadername, "staticMeshBuffer", "W", &w);
		//shaders.apply("StaticModel", core);
		 
		//shaders->updateConstantVS("Animated", "staticMeshBuffer", "VP", &vp);
		//shaders->updateConstantVS("Animated", "staticMeshBuffer", "bones", instance->matrices);

		shaders.updateConstantVS(shadername, "animatedMeshBuffer", "W", &w);
		shaders.apply(shadername, core);

		

		for (int i = 0; i < meshes.size(); i++)
		{
			shaders.updateTexturePS(shadername, core, "tex", textures->find(textureFilenames[i]));
			meshes[i].draw(core);
		}

	}

	void move(const Vec3& v, ShaderManager& shaders, std::string shadername, DXCOre* core)
	{
		w = Matrix::translation(v);

		shaders.updateConstantVS(shadername, "staticMeshBuffer", "W", &w);
		shaders.apply("StaticModel", core);
	}

	void scale(const Vec3& v, ShaderManager& shaders, std::string shadername, DXCOre* core)
	{
		w = Matrix::scaling(v);
		shaders.updateConstantVS(shadername, "staticMeshBuffer", "W", &w);
		shaders.apply("StaticModel", core);
	}
};