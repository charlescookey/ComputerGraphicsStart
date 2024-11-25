#include "Window.h"

#include "Mesh.h"
#include "DXCore.h"

#include "Shader.h"

#define M_PI       3.14159265358979323846


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	Window win;
	win.init("My Window" ,1024, 1024);

	DXCOre core;
	core.init(win.width, win.height, win.hwnd, false);

	Shader shader;
	shader.init("VertexShader.txt", "PixelShader.txt", &core);
	
	Triangle triangle;
	triangle.init(&core);

	Timer timer;
	float dt = 0.f;

	float WIDTH = 1024.f;
	float HEIGHT = 1024.f;

	while (true)
	{
		win.processMessages();
		core.clear();

		dt = timer.dt();
		shader.constBufferCPU2.time += dt;

		//spinning lights
		for (int i = 0; i < 4; i++)
		{
			float angle = shader.constBufferCPU2.time + (i * M_PI / 2.0f);
			shader.constBufferCPU2.lights[i] = Vec4(WIDTH / 2.0f + (cosf(angle) * (WIDTH * 0.3f)),
				HEIGHT / 2.0f + (sinf(angle) * (HEIGHT * 0.3f)),
				0, 0);
		}



		shader.apply(&core);
		triangle.draw(&core);


		core.present();
	}
	return 0;
}
