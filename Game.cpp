#include "Window.h"

#include "Character.h"
#include "DXCore.h"

#include "ShaderManager.h"
#include "Texture.h"
#include "Camera.h"
#include "Sampler.h"

#include "Reflection.h"

#include "Light.h"


#include <fstream>
#include <vector>

#include "Model.h"
#include "InstanceMesh.h"

#include "Font.h"


Vec3 splitString(std::string input) {
	std::stringstream ssin(input);
	Vec3 output;
	ssin >> output.x;
	ssin >> output.y;
	ssin >> output.z;
	return output;
}

void load(std::string filename) {
	std::ifstream infile(filename);
	int tilenum;
	std::string line;

	Vec3 temp;
	while (std::getline(infile, line)) {
		temp = splitString(line);
		///trees.push_back(temp);
	}
	infile.close();
}


class MouseManager {
public:
	int oldX = 0;
	int oldY = 0;

	int dx = 0;
	int dy = 0;


	MouseManager() {

	};

	MouseManager(int _x, int _y) :oldX(_x), oldY(_y) {};

	void update(int newX, int newY) {
		dx = newX - oldX;
		dy = newY - oldY;

		oldX = newX;
		oldY = newY;
	}

	float getMouseDeltaX() {
		return static_cast<float>(dx);
	}
	
	float getMouseDeltaY() {
		return static_cast<float>(dy);
	}

};
float light_rot = 0.f;
void updateLight(SphericalCamera& camera, float dt, ShaderManager &shaders, DXCOre* core, TextureManager* textures)
{
	Matrix light_matrix = Matrix::rotateY(light_rot);

	light_rot += 1.57f * dt;


	Vec3 camPos = camera.getPos();

	float dist_from_origin = 1.0f;

	Vec4 lightPos = Vec4(cos(light_rot) * dist_from_origin, 3.1f, sin(light_rot) * dist_from_origin, 1.0f);

	float LightRad = 4.f;
	Vec4 lightDir = Vec4(light_matrix.getZDirection(), 1);

	shaders.updateConstantPS("LitStaticModel", "lightAttributes", "m_light_direction", &lightDir);
	shaders.updateConstantPS("LitStaticModel", "lightAttributes", "m_camera_position", &camPos);
	shaders.updateConstantPS("LitStaticModel", "lightAttributes", "m_light_position", &lightPos);
	shaders.updateConstantPS("LitStaticModel", "lightAttributes", "m_light_radius", &LightRad);

	shaders.updateTexturePS("LitStaticModel", core, "tex", textures->find("Textures/sky2.png"));

	shaders.apply("LitStaticModel", core);
}

void updateLight2(SphericalCamera& camera, float dt, ShaderManager& shaders, DXCOre* core, TextureManager* textures)
{
	Matrix light_matrix = Matrix::rotateY(light_rot);

	light_rot += 1.57f * dt;


	Vec3 camPos = camera.getPos();

	float dist_from_origin = 1.0f;

	Vec4 lightPos = Vec4(cos(light_rot) * dist_from_origin, 3.1f, sin(light_rot) * dist_from_origin, 1.0f);

	float LightRad = 4.f;
	Vec4 lightDir = Vec4(light_matrix.getZDirection(), 1);

	Vec4 ambientColor = Vec4(0.15f, 0.15f, 0.15f, 1.0f);
	Vec4 diffuseColor = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
	Vec3 light_direction = Vec3(0.0f, -1.0f, 0.5f);

	shaders.updateConstantPS("LitStaticModel2", "lightAttributes", "lightDirection", &light_direction);
	shaders.updateConstantPS("LitStaticModel2", "lightAttributes", "ambientColor", &ambientColor);
	shaders.updateConstantPS("LitStaticModel2", "lightAttributes", "diffuseColor", &diffuseColor);

	shaders.updateTexturePS("LitStaticModel2", core, "tex", textures->find("Textures/sky2.png"));

	shaders.apply("LitStaticModel2", core);
}



void UpdateFire(DXCOre* core, Matrix& p, Matrix& v, ShaderManager& shaders, float time, TextureManager* textures, SamplerManager& samplers) {

	shaders.updateConstantVS("FireModel", "MatrixBuffer", "viewMatrix", &v);
	shaders.updateConstantVS("FireModel", "MatrixBuffer", "projectionMatrix", &p);
	

	float padding = 0.0f;
	Vec3 scroolSpeeds = Vec3(1.3f, 2.1f, 2.3f);
	Vec3 scales = Vec3(1.0f, 2.0f, 3.0f);

	Vec2 distortion1 = Vec2(0.1f, 0.2f);
	Vec2 distortion2 = Vec2(0.1f, 0.3f);
	Vec2 distortion3 = Vec2(0.1f, 0.1f);

	float distortionScale = 0.8f;
	float distortionBias = 0.5f;
	
	shaders.updateConstantVS("FireModel", "NoiseBuffer", "frameTime", &time);
	shaders.updateConstantVS("FireModel", "NoiseBuffer", "scrollSpeeds", &scroolSpeeds);
	shaders.updateConstantVS("FireModel", "NoiseBuffer", "scales", &scales);
	shaders.updateConstantVS("FireModel", "NoiseBuffer", "padding", &padding);


	shaders.updateConstantPS("FireModel", "DistortionBuffer", "distortion1", &distortion1);
	shaders.updateConstantPS("FireModel", "DistortionBuffer", "distortion2", &distortion2);
	shaders.updateConstantPS("FireModel", "DistortionBuffer", "distortion3", &distortion3);
	shaders.updateConstantPS("FireModel", "DistortionBuffer", "distortionScale", &distortionScale);
	shaders.updateConstantPS("FireModel", "DistortionBuffer", "distortionBias", &distortionBias);

	shaders.updateTexturePS("FireModel", core, "alphaTexture", textures->find("Textures/alpha01.png"));
	shaders.updateTexturePS("FireModel", core, "fireTexture", textures->find("Textures/fire01.png"));
	shaders.updateTexturePS("FireModel", core, "noiseTexture", textures->find("Textures/noise01.png"));

	shaders.updateSampler("FireModel", core, samplers.find("clamp"), 1);
	shaders.updateSampler("FireModel", core, samplers.find("wrap"), 0);

	shaders.apply("FireModel", core);
}


void updateShadowVertex(ShaderManager& shaders, Matrix& vp, Light &light1, Light &light2, float padding,std::string shadername, std::string bufferName) {
	shaders.updateConstantVS(shadername, bufferName, "VP", &vp);
	shaders.updateConstantVS(shadername, bufferName, "lightVP", &light1.vp);
	shaders.updateConstantVS(shadername, bufferName, "lightVP2", &light2.vp);

	shaders.updateConstantVS(shadername, "LightPositionBuffer", "lightPosition", &light1.position);
	shaders.updateConstantVS(shadername, "LightPositionBuffer", "lightPosition2", &light2.position);
	shaders.updateConstantVS(shadername, "LightPositionBuffer", "padding", &padding);
	shaders.updateConstantVS(shadername, "LightPositionBuffer", "padding2", &padding);
}

struct LightVariable {
	Vec4 ambientColor = Vec4(0.15f, 0.15f, 0.15f, 1.0f);
	Vec4 diffuseColor = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
	Vec3 light_direction = Vec3(0.0f, -1.0f, 0.5f);
	float shadowMapBias = 0.0022f;
	Vec3 pix_padding = Vec3(0.0f, 0.0f, 0.0f);
};


void updateShadowPixel(ShaderManager& shaders, DXCOre* core, std::string shadername, SamplerManager& samplers, TextureDraw& texDraw, TextureDraw& texDraw2, LightVariable& var) {
	shaders.updateConstantPS(shadername, "LightBuffer", "ambientColor", &var.ambientColor);
	shaders.updateConstantPS(shadername, "LightBuffer", "diffuseColor", &var.diffuseColor);
	shaders.updateConstantPS(shadername, "LightBuffer", "diffuseColor2", &var.diffuseColor);
	shaders.updateConstantPS(shadername, "LightBuffer", "bias", &var.shadowMapBias);
	shaders.updateConstantPS(shadername, "LightBuffer", "padding", &var.pix_padding);

	shaders.updateTexturePS(shadername, core, "depthMapTexture", texDraw.gettexSRV());
	shaders.updateTexturePS(shadername, core, "depthMapTexture2", texDraw2.gettexSRV());


	shaders.updateSampler(shadername, core, samplers.find("clamp"), 0);
	shaders.updateSampler(shadername, core, samplers.find("wrap"), 1);
}

void updateNormalPixel(ShaderManager& shaders, DXCOre* core, std::string shadername, SamplerManager& samplers, Light& light1, TextureManager& textures) {
	shaders.updateConstantPS(shadername, "NormalBuffer", "lightDir", &light1.dir);
	shaders.updateConstantPS(shadername, "NormalBuffer", "lightColour", &light1.color);

	shaders.updateSampler(shadername, core, samplers.find("default"), 0);
}


void testCollision(StaticModel& main, std::vector<StaticModel*>& meshes, AnimatedModel& animModel) {
	main.UpdateBoindingBox();

	for (StaticModel* a : meshes) {
		a->UpdateBoindingBox();
		if (main.checkCollision(a->boundingBox)) {
			std::cout << "Main has collided with something\n";
		};
	}

	animModel.UpdateBoindingBox();
	if (main.checkCollision(animModel.boundingBox)) {
		std::cout << "Main has collided with trex\n";
	};

}

//int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
int main() {
	Window win;
	win.init("My Window" ,1024, 1024);

	DXCOre core;
	core.init(win.width, win.height, win.hwnd, false);

	load("map.txt");


	ShaderManager shaders;
	shaders.init(ShaderType::StaticShader, "LineModel", "Shaders/LineVertex.txt", "Shaders/LinePixel.txt", &core);
	shaders.init(ShaderType::StaticShader, "StaticModel", "Shaders/StaticMeshVertex.txt", "Shaders/StaticMeshPixel.txt", &core);
	shaders.init(ShaderType::StaticShader, "NormalModel", "Shaders/StaticMeshVertex.txt", "Shaders/NormalMapPixel.txt", &core);
	shaders.init(ShaderType::StaticShader, "LitStaticModel", "Shaders/LitStaticMeshVertex.txt", "Shaders/LitTextureMeshPixel.txt", &core);
	shaders.init(ShaderType::StaticShader, "LitStaticModel2", "Shaders/LitStaticMeshVertex.txt", "Shaders/LitTextureMeshPixel2.txt", &core);
	shaders.init(ShaderType::StaticShader, "DepthModel", "Shaders/DepthVertex.txt", "Shaders/DepthPixel.txt", &core);
	shaders.init(ShaderType::StaticShader, "FontModel", "Shaders/FontVertex.txt", "Shaders/FontPixel.txt", &core);
	shaders.init(ShaderType::StaticShader, "ShadowModel", "Shaders/ShadowVertex.txt", "Shaders/ShadowPixel.txt", &core);
	shaders.init(ShaderType::StaticShader, "TexturedModel", "Shaders/StaticMeshVertex.txt", "Shaders/TextureMeshPixel.txt", &core);
	shaders.init(ShaderType::StaticShader, "TexturedModelWave", "Shaders/StaticMeshVertexAnimation.txt", "Shaders/TextureMeshPixel.txt", &core);
	shaders.init(ShaderType::StaticShader, "WaterModel", "Shaders/WaterVertex.txt", "Shaders/WaterPixel.txt", &core);
	shaders.init(ShaderType::StaticShader, "FireModel", "Shaders/FireVertex.txt", "Shaders/FirePixel.txt", &core);
	shaders.init(ShaderType::AnimatedShader, "AnimatedModel", "Shaders/AnimatedMeshVertex.txt", "Shaders/TextureMeshPixel.txt", &core);
	shaders.init(ShaderType::AnimatedShader, "NormalAnimatedModel", "Shaders/AnimatedMeshVertex.txt", "Shaders/NormalMapPixel.txt", &core);
	shaders.init(ShaderType::AnimatedShader, "AnimatedModelDepth", "Shaders/AnimatedMeshDepthVertex.txt", "Shaders/DepthPixel.txt", &core);
	shaders.init(ShaderType::AnimatedShader, "AnimatedModelShadow", "Shaders/AnimatedMeshShadowVertex.txt", "Shaders/ShadowPixel.txt", &core);
	shaders.init(ShaderType::InstanceShader, "ShadowModelInstance", "Shaders/ShadowVertexInstance.txt", "Shaders/ShadowPixel.txt", &core);
	shaders.init(ShaderType::InstanceShader, "DepthModelInstance", "Shaders/DepthVertexInstance.txt", "Shaders/DepthPixel.txt", &core);
	shaders.init(ShaderType::InstanceShader, "TexturedModelWaveInstance", "Shaders/StaticMeshVertexAnimationInstance.txt", "Shaders/TextureMeshPixel.txt", &core);


	SamplerManager samplers;
	samplers.init(SamplerType::Wrap, "wrap", &core);
	samplers.init(SamplerType::Clamp, "clamp", &core);
	samplers.init(SamplerType::Default, "default", &core);

	StaticModel plane;
	plane.init_plane(&core, "Textures/metal.png");

	StaticModel fireplane;
	fireplane.init_plane(&core, "Textures/metal.png");

	StaticModel cube;
	cube.init_cube(&core, "Textures/wall.png");

	StaticModel cube2;
	cube2.init_cube(&core, "Textures/wall.png");

	StaticModel cube3;
	cube3.init_model2(&core, "cube.gem","Textures/stone.png" );

	StaticModel sky;
	sky.init_sphere(&core, "Textures/sky2.png", 20, 20, 2);

	StaticModel sphere;
	sphere.init_sphere(&core, "Textures/wall.png", 20, 20, 2);

	StaticModel tree;
	tree.init_model(&core, "pine.gem");

	InstanceModel trees;
	trees.loadInstancePosition("data/TreeInstances.txt");
	trees.init_model(&core, "pine.gem");

	Character Trex;
	Trex.init(&core, "TRex.gem");
	
	Character Player;
	Player.init(&core, "Soldier1.gem");

	Player.animModel.textureFilenames[0] = "Textures/MaleDuty_3_OBJ_Happy_Packed0_Diffuse.png";


	//////FOR TESTING PURPOSES
	std::vector<StaticModel*> meshes;
	meshes.push_back(&cube2);
	meshes.push_back(&sphere);

	TextureManager textures;
	textures.load(&core, "Textures/bark09.png");
	textures.load(&core, "Textures/stump01.png");
	textures.load(&core, "Textures/pine branch.png");
	textures.load(&core, "Textures/T-rex_Base_Color.png");
	textures.load(&core, "Textures/sky2.png");
	textures.load(&core, "Textures/water01.png");
	textures.load(&core, "Textures/alpha01.png");
	textures.load(&core, "Textures/fire01.png");
	textures.load(&core, "Textures/noise01.png");
	textures.load(&core, "Textures/ice.png");
	textures.load(&core, "Textures/metal.png");
	textures.load(&core, "Textures/wall.png");
	textures.load(&core, "Textures/MaleDuty_3_OBJ_Happy_Packed0_Diffuse.png");
	textures.load(&core, "Textures/stone.png");
	textures.load(&core, "Textures/font.png");
	textures.load(&core, "Textures/stonenormal.png");
	textures.load(&core, "Textures/T-rex_Normal_OpenGL.png");


	shaders.updateSampler("TexturedModel", &core, samplers.find("default"), 0);
	shaders.updateSampler("WaterModel", &core, samplers.find("default"), 0);
	shaders.updateSampler("FireModel", &core, samplers.find("default"), 0);

	Timer timer;
	float dt = 0.f;
	float t = 0.f;

	cube2.translate(Vec3(-4.0f, 2.0f, 0.0f));
	sphere.translate(Vec3(4.0f, 2.0f, 0.0f));
	

	//to is camera+look at direction
	//phi for strafing
	//setCuirsorPos
	//offert in bone is inveser bind pose

	float WIDTH = 1024.f;
	float HEIGHT = 1024.f;

	SphericalCamera camera(Vec3(0.f, 0.f, 0.f), Vec3(0, 1, 0), SphericalCoordinates(3.14159f / 4, 3.14159f / 4, 10.f), 90.0, 1024 / 1024, 1000, 0.01f);
	//AnotherCamera camera(Vec3(0.f, 0.f, 0.f), Vec3(0, 1, 0), SphericalCoordinates(3.14159f / 4, 3.14159f / 4, 10.f), 90.0, 1024 / 1024, 1000, 0.01f);
	//ThirdPersonCamera camera(Vec3(0, 1, 0), Vec3(0, 1, 0), Vec3(0, 3, -10), 90.0f, 1024 / 1024,  0.1f, 100.0f);

	SphericalCoordinates spherical(M_PI / 4.0f, 0.0f, 10.0f); // 45° pitch, 0° yaw, distance 10 units
	//ThirdPersonCamera camera(Vec3(0, 1, 0), Vec3(0, 1, 0), spherical,45.0f, 1.f, 0.1f, 100.0f);
	//ThirdPersonCamera2 camera(45.0f, 1.f, 0.1f, 100.0f);


	Matrix proj = Matrix::ProjectionMatrix(90.0, 1024 / 1024, 1000, 0.01f);
	Matrix v;
	Matrix p2;

	tree.scale(Vec3(0.01f, 0.01f, 0.01f));
	tree.move(-5.f, 0.f, 0.f);

	trees.scale(Vec3(0.01f, 0.01f, 0.01f));
	trees.move(-5.f, 0.f, 0.f);

	fireplane.rot(25);
	fireplane.move(-40.f, 0.f, 0.f);

	plane.scale(Vec3(5.f, 1.f, 5.f));

	Player.animModel.scale(Vec3(0.02f, 0.02f, 0.02f));

	cube.move(-20, 0, 2);
	cube3.move(-20, 0, 2);
	Player.animModel.move(-20, 0, 2);
	sky.scale(Vec3(100, 100, 100));

	Matrix w;
	MouseManager mousePos(win.mousex , win.mousey) ;


	TextureDraw texDraw(1024, 1024);
	texDraw.init(&core, win.width, win.height);
	
	TextureDraw texDraw2(1024, 1024);
	texDraw2.init(&core, win.width, win.height);

	TextureDraw texDraw3(1024, 1024);
	texDraw3.init(&core, win.width, win.height);

	LightVariable var;


	Vec3 LightPosition(-5.0f, 8.0f, -5.0f);

	Light light1(Vec3(5.0f, 10.0f, -5.0f), Vec3(0.f, 0.f, 0.f), Vec3(0, 1, 0), 90.0, 1024 / 1024, 100, 1.f);
	Light light2(Vec3(-5.0f, 10.0f, -5.0f), Vec3(0.f, 0.f, 0.f), Vec3(0, 1, 0), 90.0, 1024 / 1024, 100, 1.f);


	//FOR TESTING
	float shootTime = 0.f;
	float ShowBeam = 0.f;


	MyFont testFont;
	testFont.loadFont("data/font.txt");
	testFont.BuildVertexArray("test Sentence", 0, 0);
	testFont.init(&core, "Textures/font.png");


	while (true)
	{
		win.processMessages();
		core.clear();

		if (win.keyPressed(VK_ESCAPE)) break;

		dt = timer.dt();
		t += dt;

		shaders.updateConstantVS("TexturedModelWave", "waveMeshBuffer", "time", &t);
		shaders.updateConstantVS("TexturedModelWaveInstance", "waveMeshBuffer", "time", &t);

		float x = 0.f;
		float y = 0.f;
		float z = 0.f;

		float move = 0.1f;


		if (win.keyPressed('W')) x -= move;
		if (win.keyPressed('S'))  x += move;
		if (win.keyPressed('A')) y -= move;
		if (win.keyPressed('D')) y += move;
				
		mousePos.update(win.mousex, win.mousey);

		float deltaX = mousePos.getMouseDeltaX(); 
		float deltaY = mousePos.getMouseDeltaY();
		
		//CHANGE THIS BACK!!!!!
		//texDraw3.drawToTexture(&core);

		camera.moveCameraTest(x, y , deltaX, deltaY, Player.animModel);

		v = camera.getViewMatrix();
		proj = camera.getProjectionMatrix();
		p2 = camera.getOrthMatrix();


		Matrix vp = v.mull(proj);
		shaders.updateConstantVS("TexturedModel", "staticMeshBuffer", "VP", &vp);
		shaders.updateConstantVS("StaticModel", "staticMeshBuffer", "VP", &vp);
		shaders.updateConstantVS("TexturedModelWave", "waveMeshBuffer", "VP", &vp);
		shaders.updateConstantVS("TexturedModelWaveInstance", "waveMeshBuffer", "VP", &vp);
		shaders.updateConstantVS("LitStaticModel", "staticMeshBuffer", "VP", &vp);
		shaders.updateConstantVS("WaterModel", "WaterBuffer", "VP", &vp);
		shaders.updateConstantVS("AnimatedModel", "animatedMeshBuffer", "VP", &vp);
		shaders.updateConstantVS("NormalAnimatedModel", "animatedMeshBuffer", "VP", &vp);
		shaders.updateConstantVS("NormalModel", "staticMeshBuffer", "VP", &vp);

		shaders.updateConstantVS("LineModel", "staticMeshBuffer", "VP", &vp);
		shaders.updateConstantVS("LineModel", "staticMeshBuffer", "W", &w);
		shaders.apply("LineModel", &core);

		Player.update(dt, y,x,&core);
		Trex.update(dt, 0.f,0.f,&core);

		//updateLight(camera, dt, shaders, &core, &textures);

		//CHANGE THIOS BACK
		core.SetBackBufferRenderTarget();

		LightPosition.x += 0.005f;
		if (LightPosition.x > 10.0f)
		{
			LightPosition.x = -10.0f;
		}

		light1.setPos(LightPosition);

		//CHANGE THIS BACK!!!!!
		texDraw3.drawToTexture(&core);
		shaders.updateConstantVS("FontModel", "staticMeshBuffer", "V", &v);
		shaders.updateConstantVS("FontModel", "staticMeshBuffer", "P", &p2);
		shaders.updateSampler("FontModel", &core, samplers.find("default"), 0);
		testFont.drawTexture(shaders, "FontModel", &core, &textures, "staticMeshBuffer", "W", "shaderTexture");

		core.SetBackBufferRenderTarget();


		texDraw.drawToTexture(&core);
		shaders.updateConstantVS("DepthModel", "staticMeshBuffer", "V", &light1.v);
		shaders.updateConstantVS("DepthModel", "staticMeshBuffer", "P", &light1.p);

		shaders.updateConstantVS("AnimatedModelDepth", "animatedMeshBuffer", "V", &light1.v);
		shaders.updateConstantVS("AnimatedModelDepth", "animatedMeshBuffer", "P", &light1.p);
		
		shaders.updateConstantVS("DepthModelInstance", "staticMeshBuffer", "V", &light1.v);
		shaders.updateConstantVS("DepthModelInstance", "staticMeshBuffer", "P", &light1.p);

		cube2.drawTexture(shaders, "DepthModel", &core, &textures, "staticMeshBuffer", "W", "shaderTexture");
		sphere.drawTexture(shaders, "DepthModel", &core, &textures, "staticMeshBuffer", "W", "shaderTexture");
		//tree.drawTexture(shaders, "DepthModel", &core, &textures, "staticMeshBuffer", "W", "shaderTexture");
		trees.drawTexture(shaders, "DepthModelInstance", &core, &textures, "staticMeshBuffer", "W", "shaderTexture");

		Trex.draw(shaders, "AnimatedModelDepth", &core, &textures, "shaderTexture");
		plane.drawTexture(shaders, "DepthModel", &core, &textures, "staticMeshBuffer", "W", "shaderTexture");



		core.SetBackBufferRenderTarget();


		texDraw2.drawToTexture(&core);
		shaders.updateConstantVS("DepthModel", "staticMeshBuffer", "V", &light2.v);
		shaders.updateConstantVS("DepthModel", "staticMeshBuffer", "P", &light2.p);

		shaders.updateConstantVS("DepthModelInstance", "staticMeshBuffer", "V", &light2.v);
		shaders.updateConstantVS("DepthModelInstance", "staticMeshBuffer", "P", &light2.p);

		shaders.updateConstantVS("AnimatedModelDepth", "animatedMeshBuffer", "V", &light2.v);
		shaders.updateConstantVS("AnimatedModelDepth", "animatedMeshBuffer", "P", &light2.p);

		cube2.drawTexture(shaders, "DepthModel", &core, &textures, "staticMeshBuffer", "W", "shaderTexture");
		sphere.drawTexture(shaders, "DepthModel", &core, &textures, "staticMeshBuffer", "W", "shaderTexture");
		//tree.drawTexture(shaders, "DepthModel", &core, &textures, "staticMeshBuffer", "W", "shaderTexture");
		trees.drawTexture(shaders, "DepthModelInstance", &core, &textures, "staticMeshBuffer", "W", "shaderTexture");

		Trex.draw(shaders, "AnimatedModelDepth", &core, &textures, "shaderTexture");
		plane.drawTexture(shaders, "DepthModel", &core, &textures, "staticMeshBuffer", "W", "shaderTexture");

		core.SetBackBufferRenderTarget();

		float padding = 0.0f;

		//texDraw3.drawToTexture(&core);

		updateShadowVertex(shaders, vp, light1, light2, padding,"ShadowModel", "staticMeshBuffer");
		updateShadowVertex(shaders, vp, light1, light2, padding,"ShadowModelInstance", "staticMeshBuffer");
		updateShadowVertex(shaders, vp, light1, light2, padding,"AnimatedModelShadow", "animatedMeshBuffer");


		Vec4 ambientColor = Vec4(0.15f, 0.15f, 0.15f, 1.0f);
		Vec4 diffuseColor = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
		Vec3 light_direction = Vec3(0.0f, -1.0f, 0.5f);
		float shadowMapBias = 0.0022f;
		Vec3 pix_padding = Vec3(0.0f, 0.0f, 0.0f);

		updateShadowPixel(shaders, &core, "ShadowModel", samplers, texDraw, texDraw2, var);
		updateShadowPixel(shaders, &core, "ShadowModelInstance", samplers, texDraw, texDraw2, var);
		updateShadowPixel(shaders, &core, "AnimatedModelShadow", samplers, texDraw, texDraw2, var);


		cube2.drawTexture(shaders, "ShadowModel", &core, &textures, "staticMeshBuffer", "W", "shaderTexture");
		sphere.drawTexture(shaders, "ShadowModel", &core, &textures, "staticMeshBuffer", "W", "shaderTexture");
		//tree.drawTextureSplit(shaders, "ShadowModel", "TexturedModelWave", &core, &textures, "staticMeshBuffer", "waveMeshBuffer", "W", "shaderTexture", "tex", "Textures/pine branch.png");
		trees.drawTextureSplit(shaders, "ShadowModelInstance", "TexturedModelWaveInstance", &core, &textures, "staticMeshBuffer", "waveMeshBuffer", "W", "shaderTexture", "tex", "Textures/pine branch.png");
		plane.drawTexture(shaders, "ShadowModel", &core, &textures, "staticMeshBuffer", "W", "shaderTexture");


		UpdateFire(&core, proj, v, shaders,t, &textures, samplers);
		fireplane.drawNoTexture(shaders, "FireModel", "MatrixBuffer", "worldMatrix", &core);



		updateNormalPixel(shaders, &core, "NormalModel", samplers,light1, textures);
		//cube.drawTexture(shaders, "StaticModel", &core, &textures, "staticMeshBuffer", "W", "tex");
		//cube3.drawTexture(shaders, "NormalModel", &core, &textures, "staticMeshBuffer", "W", "tex");
		
		//cube3.drawTextureNormal(shaders, "NormalModel", &core, &textures, "staticMeshBuffer", "W", "tex", "normalsTexture", "Textures/stonenormal.png");
		//UpdateFire(&core, w, proj, v, shaders, t, &textures, camera, samplers);
		sky.drawTexture(shaders, "TexturedModel", &core, &textures, "staticMeshBuffer", "W", "tex");


		Trex.draw(shaders, "AnimatedModelShadow", &core, &textures, "shaderTexture");
		Player.draw(shaders, "AnimatedModelShadow", &core, &textures, "shaderTexture");
		//Trex.drawTextureNormal(shaders, "NormalAnimatedModel", &core, &textures, "animatedMeshBuffer", "W", "tex", "normalsTexture", "Textures/T-rex_Normal_OpenGL.png");

		//testCollision(cube, meshes, animModel);

		shootTime += dt;
		if (win.keyPressed('F')) {
			if (shootTime > 0.5f) {
				Player.shootAt(Trex, camera.getDir());
				shootTime = 0.f;
			}
		}


		core.SetBackBufferRenderTarget();

		core.present();
	}
	return 0;
}
