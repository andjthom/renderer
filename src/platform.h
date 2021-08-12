#ifndef PLATFORM_H
#define PLATFORM_H

#include "render.h"

typedef struct P_Window P_Window;

void P_Init();
void P_Quit();

P_Window *P_CreateWindow(const char *title, const int width, const int height);
void P_DestroyWindow(P_Window *window);
void P_SwapWindow(const P_Window *window, const R_Framebuffer *framebuffer);
int P_WindowShouldClose(const P_Window *window);

void P_PollEvents();
R_Framebuffer *P_CreateCompatibleFramebuffer(const P_Window *window);

#endif /* !PLATFORM_H */
