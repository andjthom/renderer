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
void R_DestroySurface(const R_Surface *surface);

void R_BlitBGR(const R_Surface *src, const R_Surface *dest);
void R_FillSurface(const R_Surface *surface, const Vec4f color);

#endif /* !RENDER_H */
