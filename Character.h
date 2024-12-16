#pragma once
#include "SkeletalMesh.h"


enum AnimState
{
	Idle,
	Run,
	Attack,
	Fire,
	React,
	Death,
	WalkForward,
	WalkForwardLeft,
	WalkForwardRight,
	WalkBackward,
	WalkBackwardLeft,
	WalkBackwardRight,
	WalkLeft,
	WalkRight,
};

std::string AnimStateToString(AnimState state) {
	switch (state) {
	case AnimState::Idle: return "Idle";
	case AnimState::Run: return "Run";
	case AnimState::WalkForward: return "walk forward";
	case AnimState::WalkForwardLeft: return "walk forward left";
	case AnimState::WalkForwardRight: return "walk forward right";
	case AnimState::WalkBackward: return "walk backward";
	case AnimState::WalkBackwardLeft: return "walk backward left";
	case AnimState::WalkBackwardRight: return "walk backward right";
	case AnimState::WalkLeft: return "walk left";
	case AnimState::WalkRight: return "walk right";
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

	float shootTime = 0.f;
	bool shoot = false;

	Ray testRay;
	Line testLine;

public:
	AnimatedModel animModel;

	void init(DXCOre* core, std::string filename) {
		animModel.init(core, filename);
		animInstance.animation = &animModel.animation;
		health = 100;
		state = AnimState::Idle;
	}

	void drawLine(DXCOre* core) {
		testLine.init(core, testRay.at(30.f), testRay.o);
		testLine.draw(core);
	}

	void update(float dt, float x, float y, DXCOre* core) {
		if (dead) {
			deathTime += dt;

			if (deathTime > 4.f) {
				deathTime = 0.f;
				show = false;
				state = AnimState::Idle;
			}
		}
		else if (react) {
			reactTime += dt;
			if (reactTime > 0.5f) {
				reactTime = 0.f;
				react = false;
				state = AnimState::Idle;
			}
		}
		else if (shoot) {
			shootTime += dt;
			if (shootTime > 0.5f) {
				shootTime = 0.f;
				shoot = false;
				state = AnimState::Idle;
			}
			else {
				drawLine(core);
			}
		}
		else if (x > 0) {
			if (y > 0) {
				state = AnimState::WalkForwardLeft;
			}
			else if (y < 0) {
				state = AnimState::WalkForwardRight;
			}
			else {
				state = AnimState::WalkForward;
			}
		}
		else if (x < 0) {
			if (y > 0) {
				state = AnimState::WalkBackwardLeft;
			}
			else if (y < 0) {
				state = AnimState::WalkBackwardRight;
			}
			else {
				state = AnimState::WalkBackward;
			}
		}
		else if (y > 0) {
				state = AnimState::WalkLeft;
		}
		else if (y < 0) {
				state = AnimState::WalkRight;
		}
		else {
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

	void shootAt(Character &Trex, Vec3 camDir) {
		state = AnimState::Fire;
		shoot = true;

		testRay.init(animModel.getPos_Add_Y(), camDir * -1);
		float TimeOfIntersection = 0.f;
		if (Trex.animModel.checkRayCollision(testRay, TimeOfIntersection)) {
			std::cout << "Ray collided at " << TimeOfIntersection << "\n";
			Trex.takeDamage(5);
		}
	}

	void draw(ShaderManager& shaders, std::string shadername, DXCOre* core, TextureManager* textures, std::string texVarname) {
		if (!show)return;		
		//animInstance.update("Idle", dt);
		shaders.updateConstantVS(shadername, "animatedMeshBuffer", "bones", animInstance.matrices);
		animModel.drawTexture(shaders, shadername, core, textures, "animatedMeshBuffer", "W", texVarname);
	}

	void drawTextureNormal(ShaderManager& shaders, std::string shadername, DXCOre* core, TextureManager* textures, std::string buffername, std::string worldVarname, std::string texVarname, std::string NormalVarname, std::string NormaltexVarname) {
		if (!show)return;
		shaders.updateTexturePS(shadername, core, NormalVarname, textures->find(NormaltexVarname));
		shaders.updateConstantVS(shadername, "animatedMeshBuffer", "bones", animInstance.matrices);
		shaders.apply(shadername, core);

		animModel.drawTexture(shaders, shadername, core, textures, "animatedMeshBuffer", "W", texVarname);
	}


};

