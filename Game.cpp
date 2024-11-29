#include "Window.h"

#include "Mesh.h"
#include "DXCore.h"

#include "Shader.h"


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	Window win;
	win.init("My Window" ,1024, 1024);

	DXCOre core;
	core.init(win.width, win.height, win.hwnd, false);

	Shader shader;
	shader.init("StaticMeshVertex.txt", "StaticMeshPixel.txt", &core);

	Plane p;
	p.init(&core);

	Cube c;
	c.init(&core);

	Sphere s(20, 20, 2);
	s.init(&core);

	Model model;
	model.init(&core, "acacia.gem");

	Timer timer;
	float dt = 0.f;
	float t = 0.f;
	

	//to is camera+look at direction
	//phi for strafing
	//setCuirsorPos
	//offert in bone is inveser bind pose

	float WIDTH = 1024.f;
	float HEIGHT = 1024.f;

	Matrix proj = Matrix::ProjectionMatrix(90.0, 1024 / 1024, 1000, 0.01f);

	while (true)
	{
		win.processMessages();
		core.clear();

		dt = timer.dt();
		t += dt;

		Vec3 from = Vec3(11 * cos(t), 5, 11 * sinf(t));
		Matrix v = Matrix::LookAtMatrixRot(from, Vec3(0, 1, 0), Vec3(0, 1, 0));

		Matrix w;// = Matrix::scaling(Vec3(0.1f, 0.1f, 0.1f));

		//w = Matrix::translation(Vec3(0, 0, 0));
		//w = Matrix::scaling(Vec3(0.01f, 0.01f, 0.01f));
		w = Matrix::scaling(Vec3(0.1f, 0.1f, 0.1f));


		//shader.updateConstantVS("StaticModel", "staticMeshBuffer", "W", &planeWorld);
		//shader.updateConstantVS("StaticModel", "staticMeshBuffer", "VP", &vp);

		shader.updateConstantVS( "staticMeshBuffer", "W", &w);
		Matrix vp = v.mull(proj);
		shader.updateConstantVS( "staticMeshBuffer", "VP", &vp);


		shader.apply(&core);
		p.geometry.draw(&core);
		//s.geometry.draw(&core);
		//c.geometry.draw(&core);
		//model.draw(&core);


		//w = Matrix::translation(Vec3(5, 0, 0));
		//shader.updateConstantVS("staticMeshBuffer", "W", &w);
		//shader.apply(&core);
		//c.geometry.draw(&core);


		core.present();
	}
	return 0;
}
