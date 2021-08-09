#include "platform.h"
#include "render.h"

int main()
{
	P_Init();

	P_Window *window = P_CreateWindow("Renderer", 640, 480);
	R_Surface *surface = P_CreateCompatibleSurface(window);

	Vec4f background_color = {0.7f, 0.7f, 1.0f, 1.0f};
	R_FillSurface(surface, background_color);

	Vec4f line_color = {1.0f, 1.0f, 1.0f, 1.0f};
	Vec2f p0 = {0.0f, 0.0f};
	Vec2f p1 = {0.3f, 0.9f};

	R_DrawLine(surface, line_color, p0, p1);

	while (!P_WindowShouldClose(window)) {
		P_PollEvents();
		P_PresentSurface(window, surface);
	}

	R_DestroySurface(surface);
	P_DestroyWindow(window);
	P_Quit();
	return 0;
}
