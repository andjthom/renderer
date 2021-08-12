#ifndef RENDER_H
#define RENDER_H

#include "linear.h"

typedef struct {
	int width;
	int height;
	int channels;
	unsigned char *buffer;
} R_Surface;

typedef struct {
	int width;
	int height;
	unsigned char *color_buffer;
	float *depth_buffer;
} R_Framebuffer;

R_Surface *R_CreateSurface(const int width, const int height, const int channels);
void R_DestroySurface(R_Surface *surface);

R_Framebuffer *R_CreateFramebuffer(const int width, const int height);
void R_DestroyFramebuffer(R_Framebuffer *framebuffer);

void R_BlitBGR(const R_Framebuffer *const src, const R_Surface *dest);
void R_FramebufferClearColor(const R_Framebuffer *framebuffer, const Vec4f color);
void R_FramebufferClearDepth(const R_Framebuffer *framebuffer, const float depth);

void R_DrawPoint2D(const R_Framebuffer *framebuffer, const Vec4f color, const Vec2f point);
void R_DrawLine2D(const R_Framebuffer *framebuffer, const Vec4f color, const Vec2f pts[2]);
void R_DrawTriangle2D(const R_Framebuffer *framebuffer, const Vec4f color, const Vec2f pts[3]);

void R_RasterizeTriangle(const R_Framebuffer *framebuffer, const unsigned char color[4],
						 const Vec2i pts[3]);

#endif /* !RENDER_H */
