#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include "platform.h"
#include "render.h"
#include "obj.h"
#include "stretchy.h"

int main()
{
	P_Init();

	P_Window *window = P_CreateWindow("Renderer", 640, 480);
	R_Surface *surface = P_CreateCompatibleSurface(window);

	Vec4f background_color = {0.7f, 0.7f, 1.0f, 1.0f};
	R_FillSurface(surface, background_color);

	Vec4f line_color = {1.0f, 1.0f, 1.0f, 1.0f};
	OBJ_Mesh *mesh = OBJ_CreateMesh("data/african_head.obj");
	assert(mesh->indices != NULL);
	
	printf("%u\n", SB_LEN(mesh->indices));
	for (unsigned int i = 0; i < mesh->num_faces-1; i++) {
		Vec3u *face = &mesh->indices[i * 3];
		for (unsigned int j = 0; j < 3; j++) {
			unsigned int v0 = face[j][0];
			unsigned int v1 = face[(j+1)%3][0];
			Vec2f p0 = {mesh->positions[v0][0], mesh->positions[v0][1]};
			Vec2f p1 = {mesh->positions[v1][0], mesh->positions[v1][1]};
			R_DrawLine(surface, line_color, p0, p1);
		}
	}

	while (!P_WindowShouldClose(window)) {
		P_PollEvents();
		P_PresentSurface(window, surface);
	}

	OBJ_DestroyMesh(mesh);
	R_DestroySurface(surface);
	P_DestroyWindow(window);
	P_Quit();
	return 0;
}
