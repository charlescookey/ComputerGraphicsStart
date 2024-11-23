#include "Window.h"

#include "DXCore.h"

#include "Mesh.h"
#include "Shader.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	Window win;
	win.init("My Window" ,1024, 1024);

	DXCOre core;
	core.init(win.width, win.height, win.hwnd, false);

	Shader shader;
	shader.init("VertexShader.txt", "PixelShader.txt", &core);
	
	Triangle triangle;
	triangle.init(&core);

	while (true)
	{
		win.processMessages();
		core.clear();

		shader.apply(&core);
		triangle.draw(&core);


		core.present();
	}
	return 0;
}
