#pragma once
#include "SkeletalMesh.h"


enum AnimState
{
	Idle,
	Run,
	Attack,
	Fire,
	React,
	Death
};

std::string AnimStateToString(AnimState state) {
	switch (state) {
	case AnimState::Idle: return "Idle";
	case AnimState::Run: return "Run";
	case AnimState::Attack: return "attack";
	case AnimState::Fire: return "firing rifle";
	case AnimState::React: return "roar";
	case AnimState::Death: return "death";
	default: return "Idle";
	}
}

class Character {
private:
	AnimationInstance animInstance;
	AnimState state;

	int health ;
	float reactTime = 0.f;
	bool react = false;
	
	float deathTime = 0.f;
	bool dead = false;
	bool show = true;
public:
	AnimatedModel animModel;

	void init(DXCOre* core, std::string filename) {
		animModel.init(core, filename);
		animInstance.animation = &animModel.animation;
		health = 100;
		state = AnimState::Idle;
	}

	void update(float dt) {
		if (dead) {
			deathTime += dt;
		}

		if (deathTime > 4.f) {
			deathTime = 0.f;
			show = false;
			state = AnimState::Idle;
		}

		if (react) {
			reactTime += dt;
		}

		if (reactTime > 0.5f) {
			reactTime = 0.f;
			react = false;
			state = AnimState::Idle;
		}
		animInstance.update(AnimStateToString(state), dt);
	}

	void takeDamage(int damage) {
		if (dead)return;
		state = AnimState::Attack;
		health -= damage;
		health = max(0, health);
		react = true;
		
		if (health == 0) {
			dead = true;
			react = false;
			state = AnimState::Death;
		}

		std::cout << health << "\n";
	}

	void draw(ShaderManager& shaders, std::string shadername, DXCOre* core, TextureManager* textures, std::string texVarname) {
		if (!show)return;		
		//animInstance.update("Idle", dt);
		shaders.updateConstantVS(shadername, "animatedMeshBuffer", "bones", animInstance.matrices);
		animModel.drawTexture(shaders, shadername, core, textures, "animatedMeshBuffer", "W", texVarname);
	}


};

