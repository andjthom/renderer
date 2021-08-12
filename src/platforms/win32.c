#include <windows.h>
#include <assert.h>
#include <stdlib.h>
#include "../platform.h"
#include "../render.h"

struct P_Window {
	HWND handle;
	HDC memdc;
	R_Surface *surface;
	int quit;
};

static int g_initialized = 0;

#ifdef UNICODE
static const wchar_t *const WINDOW_CLASS_NAME = L"Window Class";
static const wchar_t *const WINDOW_PROP_NAME = L"Window Prop";
#else /* UNICODE */
static const char *const WINDOW_CLASS_NAME = "Window Class";
static const char *const WINDOW_PROP_NAME = "Window Prop";
#endif /* UNICODE */

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	P_Window *window = GetProp(hwnd, WINDOW_PROP_NAME);

	if (window == NULL) {
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	else if (msg == WM_CLOSE) {
		window->quit = 1;
		return 0;
	}
	else {
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
}

void P_Init()
{
	assert(g_initialized == 0);

	WNDCLASS window_class = {
		.style = CS_HREDRAW | CS_VREDRAW,
		.lpfnWndProc = WndProc,
		.cbClsExtra = 0,
		.cbWndExtra = 0,
		.hInstance = GetModuleHandle(NULL),
		.hIcon = LoadIcon(NULL, IDI_APPLICATION),
		.hCursor = LoadCursor(NULL, IDC_ARROW),
		.hbrBackground = GetStockObject(WHITE_BRUSH),
		.lpszMenuName = NULL,
		.lpszClassName = WINDOW_CLASS_NAME
	};

	ATOM atom = RegisterClass(&window_class);
	assert(atom != 0);

	g_initialized = 1;
}

void P_Quit()
{
	UnregisterClass(WINDOW_CLASS_NAME, GetModuleHandle(NULL));
	g_initialized = 0;
}

P_Window *P_CreateWindow(const char *title, const int width, const int height)
{
	assert(g_initialized == 1);

	P_Window *window = malloc(sizeof(P_Window));
	assert(window != NULL);
	window->quit = 0;

	DWORD style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	RECT rect = {
		.left = 0,
		.top = 0,
		.right = width,
		.bottom = height
	};

	AdjustWindowRect(&rect, style, 0);
	int new_width = rect.right - rect.left;
	int new_height = rect.bottom - rect.top;

	window->handle = CreateWindow(WINDOW_CLASS_NAME, title, style, CW_USEDEFAULT, CW_USEDEFAULT,
								  new_width, new_height, NULL, NULL, GetModuleHandle(NULL), NULL);

	window->surface = R_CreateSurface(width, height, 4);
	free(window->surface->buffer);
	window->surface->buffer = NULL;

	HDC windc = GetDC(window->handle);
	window->memdc = CreateCompatibleDC(windc);
	ReleaseDC(window->handle, windc);

	BITMAPINFOHEADER bi = {
		.biSize = sizeof(BITMAPINFOHEADER),
		.biWidth = width,
		.biHeight = -height,
		.biPlanes = 1,
		.biBitCount = 32,
		.biCompression = BI_RGB
	};

	HBITMAP bitmap = CreateDIBSection(window->memdc, (BITMAPINFO *)&bi, DIB_RGB_COLORS,
									  (void **)&window->surface->buffer, NULL, 0);
	assert(bitmap != 0);
	HBITMAP old_bitmap = SelectObject(window->memdc, bitmap);
	assert(old_bitmap != 0);
	DeleteObject(old_bitmap);

	SetProp(window->handle, WINDOW_PROP_NAME, window);
	ShowWindow(window->handle, SW_SHOW);

	return window;
}

void P_DestroyWindow(P_Window *window)
{
	ShowWindow(window->handle, SW_HIDE);
	RemoveProp(window->handle, WINDOW_PROP_NAME);
	DeleteDC(window->memdc);
	DestroyWindow(window->handle);
	window->surface->buffer = NULL;
	R_DestroySurface(window->surface);
	free(window);
}

void P_PollEvents()
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void P_SwapWindow(const P_Window *window, const R_Framebuffer *framebuffer)
{
	R_BlitBGR(framebuffer, window->surface);

	HDC windc = GetDC(window->handle);
	int width = window->surface->width;
	int height = window->surface->height;
	BitBlt(windc, 0, 0, width, height, window->memdc, 0, 0, SRCCOPY);
	ReleaseDC(window->handle, windc);
}

int P_WindowShouldClose(const P_Window *window)
{
	return window->quit;
}

R_Framebuffer *P_CreateCompatibleFramebuffer(const P_Window *window)
{
	R_Surface *surface = window->surface;
	return R_CreateFramebuffer(surface->width, surface->height);
}
