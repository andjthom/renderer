#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include "render.h"

static void __ConvertColor(const Vec4f color, unsigned char out[4])
{
	for (int i = 0; i < 4; i++) {
		out[i] = (unsigned char)(color[i] * 255);
	}
}

static void __ConvertPoint(const R_Surface *surface, const Vec2f point, int *x, int *y)
{
	*x = (int)((point[0] + 1.0) * surface->width / 2.0);
	*y = (int)((point[1] + 1.0) * surface->height / 2.0);
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

void R_BlitBGR(const R_Surface *const src, const R_Surface *dest)
{
	assert(src->width >= dest->width && src->height >= dest->height);

	int width = src->width;
	int height = src->height;

	for (int y = 0; y < dest->height; y++) {
		for (int x = 0; x < dest->width; x++) {
			int flipped_y = height - y - 1;
			int src_index = (y * width + x) * 4;
			int dest_index = (flipped_y * width + x) * 4;
			unsigned char *src_pixel = &src->buffer[src_index];
			unsigned char *dest_pixel = &dest->buffer[dest_index];

			dest_pixel[0] = src_pixel[2];
			dest_pixel[1] = src_pixel[1];
			dest_pixel[2] = src_pixel[0];
			dest_pixel[3] = src_pixel[3];
		}
	}
}

void R_FillSurface(const R_Surface *surface, const Vec4f color_vec)
{
	unsigned char color[4];
	__ConvertColor(color_vec, color);

	for (int y = 0; y < surface->height; y++) {
		for (int x = 0; x < surface->width; x++) {
			int index = (y * surface->width + x) * 4;
			unsigned char *pixel = &surface->buffer[index];

			pixel[0] = color[0];
			pixel[1] = color[1];
			pixel[2] = color[2];
			pixel[3] = color[3];
		}
	}
}

static void __DrawPoint(const R_Surface *surface, const unsigned char color[4],
						const int x, const int y)
{
	int index = (y * surface->width + x) * 4;
	for (int i = 0; i < 4; i++) {
		surface->buffer[index + i] = color[i];
	}
}

void R_DrawPoint(const R_Surface *surface, const Vec4f color_vec, const Vec2f point)
{
	unsigned char color[4];
	int x, y;

	__ConvertColor(color_vec, color);
	__ConvertPoint(surface, point, &x, &y);

	__DrawPoint(surface, color, x, y);
}

void R_DrawLine(const R_Surface *surface, const Vec4f color_vec, const Vec2f p0, const Vec2f p1)
{
	unsigned char color[4];
	int x0, y0, x1, y1;

	__ConvertColor(color_vec, color);
	__ConvertPoint(surface, p0, &x0, &y0);
	__ConvertPoint(surface, p1, &x1, &y1);

	int dx = abs(x1 - x0);
	int x_dir = (x0 < x1) ? 1 : -1;
	int dy = -abs(y1 - y0);
	int y_dir = (y0 < y1) ? 1 : -1;
	int error = dx + dy;

	while (1) {
		__DrawPoint(surface, color, x0, y0);

		if (x0 == x1 && y0 == y1) {
			break;
		}

		int dbl_error = 2 * error;

		if (dbl_error >= dy) {
			error += dy;
			x0 += x_dir;
		}

		if (dbl_error <= dx) {
			error += dx;
			y0 += y_dir;
		}
	}
}
