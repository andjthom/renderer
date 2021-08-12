#include <assert.h>
#include "platform.h"
#include "render.h"
#include "obj.h"

void TriangleTest(const R_Framebuffer *framebuffer)
{
	Vec4f red = {1.0f, 0.0f, 0.0f, 1.0f};
	Vec4f white = {1.0f, 1.0f, 1.0f, 1.0f};
	Vec4f green = {0.0f, 1.0f, 0.0f, 1.0f};
	Vec4f black = {0.0f, 0.0f, 0.0f, 1.0f};

	Vec2f t0[3] = {{-0.9f, -0.3f}, {-0.5f, 0.6f}, {-0.3f, -0.2f}};
	Vec2f t1[3] = {{0.8f, -0.5f}, {0.5f, -0.99f}, {-0.3f, 0.8f}};
	Vec2f t2[3] = {{0.8f, 0.5f}, {0.2f, 0.6f}, {0.3f, 0.8f}};

	R_RasterizeTriangle(framebuffer, red, t0);
	R_RasterizeTriangle(framebuffer, white, t1);
	R_RasterizeTriangle(framebuffer, green, t2);

	R_DrawTriangle(framebuffer, black, t0);
	R_DrawTriangle(framebuffer, black, t1);
	R_DrawTriangle(framebuffer, black, t2);
}

int main()
{
	P_Init();

	P_Window *window = P_CreateWindow("Renderer", 640, 640);
	R_Framebuffer *framebuffer = P_CreateCompatibleFramebuffer(window);

	Vec4f background_color = {0.7f, 0.7f, 1.0f, 1.0f};
	R_FramebufferClearColor(framebuffer, background_color);

	TriangleTest(framebuffer);

	while (!P_WindowShouldClose(window)) {
		P_PollEvents();
		P_SwapWindow(window, framebuffer);
	}

	R_DestroyFramebuffer(framebuffer);
	P_DestroyWindow(window);
	P_Quit();
	return 0;
}
