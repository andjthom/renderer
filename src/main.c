#include "platform.h"
#include "render.h"

int main()
{
	P_Init();

	P_Window *window = P_CreateWindow("Renderer", 640, 480);
	R_Surface *surface = P_CreateCompatibleSurface(window);

	Vec4f background_color = {0.7f, 0.7f, 1.0f, 1.0f};
	R_FillSurface(surface, background_color);

	while (!P_WindowShouldClose(window)) {
		P_PollEvents();
		P_PresentSurface(window, surface);
	}

	R_DestroySurface(surface);
	P_DestroyWindow(window);
	P_Quit();
	return 0;
}
