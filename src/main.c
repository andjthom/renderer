#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include "platform.h"
#include "render.h"
#include "obj.h"

void ModelTest(const R_Framebuffer *framebuffer)
{
	OBJ_Mesh *mesh = OBJ_CreateMesh("data/african_head.obj");

	int width = framebuffer->width;
	int height = framebuffer->height;
	Vec3f world_coords[3];
	Vec2i screen_coords[3];
	unsigned char color[4];
	Vec3f light_dir = {0.0f, 0.0f, -1.0f};
	for (int i = 0; i < mesh->num_faces; i++) {
		Vec3u *face = &mesh->indices[i*3];

		for (int j = 0; j < 3; j++) {
			world_coords[j][0] = mesh->positions[face[j][0]][0];
			world_coords[j][1] = mesh->positions[face[j][0]][1];
			world_coords[j][2] = mesh->positions[face[j][0]][2];
			screen_coords[j][0] = (int)((world_coords[j][0] + 1.0) * width / 2.0);
			screen_coords[j][1] = (int)((world_coords[j][1] + 1.0) * height / 2.0);
		}

		Vec3f a, b, normal;
		Vec3f_Sub(world_coords[2], world_coords[0], a);
		Vec3f_Sub(world_coords[1], world_coords[0], b);
		Vec3f_Cross(a, b, normal);
		Vec3f_Normalize(normal, normal);
		float intensity = Vec3f_Dot(normal, light_dir);
		if (intensity > 0) {
			for (int j = 0; j < 3; j++) {
				color[j] = 255 * intensity;
			}

			R_RasterizeTriangle(framebuffer, color, screen_coords);
		}
	}
}

int main()
{
	srand((unsigned int)time(NULL));
	P_Init();

	P_Window *window = P_CreateWindow("Renderer", 640, 640);
	R_Framebuffer *framebuffer = P_CreateCompatibleFramebuffer(window);

	Vec4f background_color = {0.7f, 0.7f, 1.0f, 1.0f};
	R_FramebufferClearColor(framebuffer, background_color);

	ModelTest(framebuffer);

	while (!P_WindowShouldClose(window)) {
		P_PollEvents();
		P_SwapWindow(window, framebuffer);
	}

	R_DestroyFramebuffer(framebuffer);
	P_DestroyWindow(window);
	P_Quit();
	return 0;
}
