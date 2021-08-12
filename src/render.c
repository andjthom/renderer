#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include "render.h"

#define EPSILON 1e-5f

static void ConvertColor(const Vec4f color, unsigned char result[4])
{
	for (int i = 0; i < 4; i++) {
		result[i] = (unsigned char)(color[i] * 255);
	}
}

static void ConvertPoint(const int width, const int height, const Vec2f point, Vec2i result)
{
	result[0] = (int)((point[0] + 1.0) * width / 2.0);
	result[1] = (int)((point[1] + 1.0) * height / 2.0);
}

R_Surface *R_CreateSurface(const int width, const int height, const int channels)
{
	R_Surface *surface = malloc(sizeof(R_Surface));
	assert(surface != NULL);
	surface->width = width;
	surface->height = height;
	surface->channels = channels;

	int size = width * height * channels * sizeof(unsigned char);
	surface->buffer = malloc(size);
	assert(surface->buffer != NULL);

	return surface;
}

void R_DestroySurface(R_Surface *surface)
{
	free(surface->buffer);
	free(surface);
}

R_Framebuffer *R_CreateFramebuffer(const int width, const int height)
{
	R_Framebuffer *result = malloc(sizeof(R_Framebuffer));
	assert(result != NULL);

	int color_size = width * height * 4 * sizeof(unsigned char);
	int depth_size = width * height * sizeof(float);
	result->width = width;
	result->height = height;
	result->color_buffer = malloc(color_size);
	result->depth_buffer = malloc(depth_size);
	assert(result->color_buffer != NULL && result->depth_buffer != NULL);

	Vec4f color = {0.0f, 0.0f, 0.0f, 1.0f};
	R_FramebufferClearColor(result, color);
	R_FramebufferClearDepth(result, 1.0f);

	return result;
}

void R_DestroyFramebuffer(R_Framebuffer *framebuffer)
{
	free(framebuffer->color_buffer);
	free(framebuffer->depth_buffer);
	free(framebuffer);
}

void R_BlitBGR(const R_Framebuffer *const src, const R_Surface *dest)
{
	assert(src->width >= dest->width && src->height >= dest->height);

	int width = src->width;
	int height = src->height;

	for (int y = 0; y < dest->height; y++) {
		for (int x = 0; x < dest->width; x++) {
			int flipped_y = height - y - 1;
			int src_index = (y * width + x) * 4;
			int dest_index = (flipped_y * width + x) * 4;
			unsigned char *src_pixel = &src->color_buffer[src_index];
			unsigned char *dest_pixel = &dest->buffer[dest_index];

			dest_pixel[0] = src_pixel[2];
			dest_pixel[1] = src_pixel[1];
			dest_pixel[2] = src_pixel[0];
			dest_pixel[3] = src_pixel[3];
		}
	}
}

void R_FramebufferClearColor(const R_Framebuffer *framebuffer, const Vec4f color_vec)
{
	unsigned char color[4];
	ConvertColor(color_vec, color);

	for (int y = 0; y < framebuffer->height; y++) {
		for (int x = 0; x < framebuffer->width; x++) {
			int index = (y * framebuffer->width + x) * 4;
			unsigned char *pixel = &framebuffer->color_buffer[index];

			for (int i = 0; i < 4; i++) {
				pixel[i] = color[i];
			}
		}
	}
}

void R_FramebufferClearDepth(const R_Framebuffer *framebuffer, const float depth)
{
	for (int y = 0; y < framebuffer->height; y++) {
		for (int x = 0; x < framebuffer->width; x++) {
			int index = (y * framebuffer->width + x);
			framebuffer->depth_buffer[index] = depth;
		}
	}
}

static void DrawPoint2D(const R_Framebuffer *framebuffer, const unsigned char color[4],
					  const Vec2i point)
{
	int index = (point[1] * framebuffer->width + point[0]) * 4;
	for (int i = 0; i < 4; i++) {
		framebuffer->color_buffer[index + i] = color[i];
	}
}

void R_DrawPoint2D(const R_Framebuffer *framebuffer, const Vec4f color_vec, const Vec2f point_f)
{
	unsigned char color[4];
	Vec2i point = {0, 0};

	ConvertColor(color_vec, color);
	ConvertPoint(framebuffer->width, framebuffer->height, point_f, point);

	DrawPoint2D(framebuffer, color, point);
}

static void DrawLine2D(const R_Framebuffer *framebuffer, const unsigned char color[4],
					 const Vec2i pts[2])
{
	Vec2i p0 = {pts[0][0], pts[0][1]};
	Vec2i p1 = {pts[1][0], pts[1][1]};

	int dx = abs(p1[0] - p0[0]);
	int x_dir = (p0[0] < p1[0]) ? 1 : -1;
	int dy = -abs(p1[1] - p0[1]);
	int y_dir = (p0[1] < p1[1]) ? 1 : -1;
	int error = dx + dy;

	while (1) {
		DrawPoint2D(framebuffer, color, p0);

		if (p0[0] == p1[0] && p0[1] == p1[1]) {
			break;
		}

		int dbl_error = 2 * error;

		if (dbl_error >= dy) {
			error += dy;
			p0[0] += x_dir;
		}

		if (dbl_error <= dx) {
			error += dx;
			p0[1] += y_dir;
		}
	}
}

void R_DrawLine2D(const R_Framebuffer *framebuffer, const Vec4f color_vec, const Vec2f pts_f[2])
{
	unsigned char color[4];
	Vec2i points[2] = {{0, 0}, {0, 0}};

	ConvertColor(color_vec, color);
	ConvertPoint(framebuffer->width, framebuffer->height, pts_f[0], points[0]);
	ConvertPoint(framebuffer->width, framebuffer->height, pts_f[1], points[1]);

	DrawLine2D(framebuffer, color, points);
}

static void DrawTriangle2D(const R_Framebuffer *framebuffer, const unsigned char color[4],
						 const Vec2i pts[3])
{
	Vec2i abc[3][2];
	for (int i = 0; i < 3; i++) {
		abc[i][0][0] = pts[i][0];
		abc[i][0][1] = pts[i][1];
		abc[i][1][0] = pts[(i+1)%3][0];
		abc[i][1][1] = pts[(i+1)%3][1];
	}

	DrawLine2D(framebuffer, color, abc[0]);
	DrawLine2D(framebuffer, color, abc[1]);
	DrawLine2D(framebuffer, color, abc[2]);
}

void R_DrawTriangle2D(const R_Framebuffer *framebuffer, const Vec4f color_vec, const Vec2f pts_f[3])
{
	unsigned char color[4];
	Vec2i points[3] = {{0, 0}, {0, 0}, {0, 0}};

	ConvertColor(color_vec, color);
	ConvertPoint(framebuffer->width, framebuffer->height, pts_f[0], points[0]);
	ConvertPoint(framebuffer->width, framebuffer->height, pts_f[1], points[1]);
	ConvertPoint(framebuffer->width, framebuffer->height, pts_f[2], points[2]);

	DrawTriangle2D(framebuffer, color, points);
}

static void GetBBox(const int width, const int height, const Vec2i pts[3],
					Vec2i bbox_min, Vec2i bbox_max)
{
	Vec2i_Min(pts[0], pts[1], bbox_min);
	Vec2i_Min(pts[2], bbox_min, bbox_min);
	Vec2i_Max(pts[0], pts[1], bbox_max);
	Vec2i_Max(pts[2], bbox_max, bbox_max);

	bbox_min[0] = bbox_min[0] > 0 ? bbox_min[0] : 0;
	bbox_min[1] = bbox_min[1] > 0 ? bbox_min[1] : 0;
	bbox_max[0] = bbox_max[0] < width ? bbox_max[0] : width;
	bbox_max[1] = bbox_max[1] < height ? bbox_max[1] : height;
}

/* Method described in Real-Time Collision Detection by Christer Ericson */
static void Barycentric(const Vec2i abc[3], const Vec2i p, Vec3f result)
{
	Vec2i v0, v1, v2;
	Vec2i_Sub(abc[1], abc[0], v0);
	Vec2i_Sub(abc[2], abc[0], v1);
	Vec2i_Sub(p, abc[0], v2);

	float d00 = (float)Vec2i_Dot(v0, v0);
	float d01 = (float)Vec2i_Dot(v0, v1);
	float d11 = (float)Vec2i_Dot(v1, v1);
	float d20 = (float)Vec2i_Dot(v2, v0);
	float d21 = (float)Vec2i_Dot(v2, v1);

	float d = 1 / (d00 * d11 - d01 * d01);
	result[1] = (d11 * d20 - d01 * d21) * d;
	result[2] = (d00 * d21 - d01 * d20) * d;
	result[0] = 1.0f - result[1] - result[2];
}

void R_RasterizeTriangle(const R_Framebuffer *framebuffer, const unsigned char color[4],
						 const Vec2i pts[3])
{
	int width = framebuffer->width;
	int height = framebuffer->height;

	Vec2i bbox_min, bbox_max;
	GetBBox(width, height, pts, bbox_min, bbox_max);

	Vec2i p;
	Vec3f bary;
	for (p[1] = bbox_min[1]; p[1] <= bbox_max[1]; p[1]++) {
		for (p[0] = bbox_min[0]; p[0] <= bbox_max[0]; p[0]++) {
			Barycentric(pts, p, bary);
			if (bary[0] >= -EPSILON && bary[1] >= -EPSILON && bary[2] >= -EPSILON) {
				DrawPoint2D(framebuffer, color, p);
			}
		}
	}
}
