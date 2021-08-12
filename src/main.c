#include <assert.h>
#include "platform.h"
#include "render.h"
#include "obj.h"

void TriangleTest(const R_Surface *surface)
{
	Vec4f red = {1.0f, 0.0f, 0.0f, 1.0f};
	Vec4f white = {1.0f, 1.0f, 1.0f, 1.0f};
	Vec4f green = {0.0f, 1.0f, 0.0f, 1.0f};
	Vec4f black = {0.0f, 0.0f, 0.0f, 1.0f};

	Vec2f t0[3] = {{-0.9f, -0.3f}, {-0.5f, 0.6f}, {-0.3f, -0.2f}};
	Vec2f t1[3] = {{0.8f, -0.5f}, {0.5f, -0.99f}, {-0.3f, 0.8f}};
	Vec2f t2[3] = {{0.8f, 0.5f}, {0.2f, 0.6f}, {0.3f, 0.8f}};

	R_RasterizeTriangle(surface, red, t0);
	R_RasterizeTriangle(surface, white, t1);
	R_RasterizeTriangle(surface, green, t2);

	R_DrawTriangle(surface, black, t0);
	R_DrawTriangle(surface, black, t1);
	R_DrawTriangle(surface, black, t2);
}

int main()
{
	P_Init();

	P_Window *window = P_CreateWindow("Renderer", 640, 640);
	R_Surface *surface = P_CreateCompatibleSurface(window);

	Vec4f background_color = {0.7f, 0.7f, 1.0f, 1.0f};
	R_FillSurface(surface, background_color);

	TriangleTest(surface);

	while (!P_WindowShouldClose(window)) {
		P_PollEvents();
		P_PresentSurface(window, surface);
	}

	R_DestroySurface(surface);
	P_DestroyWindow(window);
	P_Quit();
	return 0;
}
