#ifndef RENDER_H
#define RENDER_H

#include "linear.h"

typedef struct {
	int width;
	int height;
	int channels;
	unsigned char *buffer;
} R_Surface;

R_Surface *R_CreateSurface(const int width, const int height, const int channels);
void R_DestroySurface(R_Surface *surface);

void R_BlitBGR(const R_Surface *src, const R_Surface *dest);
void R_FillSurface(const R_Surface *surface, const Vec4f color);
void R_DrawPoint(const R_Surface *surface, const Vec4f color, const Vec2f point);
void R_DrawLine(const R_Surface *surface, const Vec4f color, const Vec2f pts[2]);
void R_DrawTriangle(const R_Surface *surface, const Vec4f color, const Vec2f pts[3]);
void R_RasterizeTriangle(const R_Surface *surface, const Vec4f color, const Vec2f pts[3]);

#endif /* !RENDER_H */
