#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "obj.h"
#include "stretchy.h"

#define BUFFER_SIZE 65536
#define MAX_POWER 20

typedef struct {
	OBJ_Mesh *mesh;
} OBJ_State;

static const double POW10_POS[MAX_POWER] = {1.0e0, 1.0e1, 1.0e2, 1.0e3, 1.0e4, 1.0e5, 1.0e6, 1.0e7,
	1.0e8, 1.0e9, 1.0e10, 1.0e11, 1.0e12, 1.0e13, 1.0e14, 1.0e15, 1.0e16, 1.0e17, 1.0e18, 1.0e19};

static const double POW10_NEG[MAX_POWER] = {1.0e0, 1.0e-1, 1.0e-2, 1.0e-3, 1.0e-4, 1.0e-5, 1.0e-6,
	1.0e-7, 1.0e-8, 1.0e-9, 1.0e-10, 1.0e-11, 1.0e-12, 1.0e-13, 1.0e-14, 1.0e-15, 1.0e-16, 1.0e-17,
	1.0e-18, 1.0e-19};

static int IsWhitespace(const char c)
{
	return (c == ' ' || c == '\t' || c == '\r');
}

static int IsDigit(const char c)
{
	return (c >= '0' && c <= '9');
}

static int IsExponent(const char c)
{
	return (c == 'e' || c == 'E');
}

static int IsNewline(const char c)
{
	return (c == '\n');
}

static const char *SkipWhitespace(const char *p)
{
	while (IsWhitespace(*p)) {
		p++;
	}

	return p;
}

static const char *SkipLine(const char *p)
{
	while (!IsNewline(*p)) {
		p++;
	}

	p++;
	return p;
}

static const char *ParseInt(const char *p, int *result)
{
	int sign = 1;
	if (*p == '-') {
		sign = -1;
		p++;
	}

	int n = 0;
	while (IsDigit(*p)) {
		n = 10 * n + (*p++ - '0');
	}

	*result = sign * n;
	return p;
}

static const char *ParseFloat(const char *p, float *result)
{
	p = SkipWhitespace(p);

	double sign = 1.0;
	if (*p == '-') {
		sign = -1.0;
		p++;
	}
	else if (*p == '+') {
		p++;
	}

	double n = 0.0;
	while (IsDigit(*p)) {
		n = 10.0 * n + ((double)*p++ - '0');
	}

	if (*p == '.') {
		p++;
	}

	double f = 0.0;
	double d = 1.0;
	while (IsDigit(*p)) {
		f = 10.0 * f + ((double)*p++ - '0');
		d *= 10.0;
	}

	n += f / d;

	if (IsExponent(*p)) {
		p++;

		const double *pows = POW10_POS;
		if (*p == '-') {
			pows = POW10_NEG;
			p++;
		}
		else if (*p == '+') {
			p++;
		}

		int x = 0;
		while (IsDigit(*p)) {
			x = 10 * x + (*p++ - '0');
		}

		n *= (x >= MAX_POWER ? 0.0 : pows[x]);
	}

	*result = (float)(sign * n);
	return p;
}

static const char *ParsePosition(OBJ_State *state, const char *p)
{
	float v;
	for (int i = 0; i < 3; i++) {
		p = ParseFloat(p, &v);
		SB_PUSH(state->mesh->_positions, v);
	}

	return p;
}

static const char *ParseTexcoord(OBJ_State *state, const char *p)
{
	float v;
	for (int i = 0; i < 2; i++) {
		p = ParseFloat(p, &v);
		SB_PUSH(state->mesh->_texcoords, v);
	}

	return p;
}

static const char *ParseNormal(OBJ_State *state, const char *p)
{
	float v;
	for (int i = 0; i < 3; i++) {
		p = ParseFloat(p, &v);
		SB_PUSH(state->mesh->_normals, v);
	}

	return p;
}

static const char *ParseFace(OBJ_State *state, const char *p)
{
	p = SkipWhitespace(p);

	int v, t, n;
	int count = 0;
	while (!IsNewline(*p)) {
		v = t = n = 0;

		p = ParseInt(p, &v);
		if (*p == '/') {
			p++;
			if (*p != '/') {
				p = ParseInt(p, &t);
			}

			if (*p == '/') {
				p++;
				p = ParseInt(p, &n);
			}
		}

		if (v < 0) {
			v = (SB_LEN(state->mesh->_positions) / 3) - (int)(-v);
		}
		else {
			v = (int)v;
		}

		if (t < 0) {
			t = (SB_LEN(state->mesh->_texcoords) / 2) - (int)(-t);
		}
		else if (t > 0) {
			t = (int)t;
		}
		else {
			t = 0;
		}

		if (n < 0) {
			n = (SB_LEN(state->mesh->_normals) / 3) - (int)(-n);
		}
		else if (n > 0) {
			n = (int)n;
		}
		else {
			n = 0;
		}

		SB_PUSH(state->mesh->_indices, v);
		SB_PUSH(state->mesh->_indices, t);
		SB_PUSH(state->mesh->_indices, n);
		count++;

		p = SkipWhitespace(p);
	}

	SB_PUSH(state->mesh->faces_vertices, count);
	return p;
}

static void ParseBuffer(OBJ_State *state, const char *start, const char *end)
{
	const char *p = start;
	while (p != end) {
		p = SkipWhitespace(p);
		switch (*p) {
		case 'v':
			p++;
			switch (*p++) {
			case ' ':
			case '\t':
				p = ParsePosition(state, p);
				break;

			case 't':
				p = ParseTexcoord(state, p);
				break;

			case 'n':
				p = ParseNormal(state, p);
				break;

			default:
				p--;
				break;
			}
			break;

		case 'f':
			p++;
			switch (*p++) {
			case ' ':
			case '\t':
				p = ParseFace(state, p);
				break;
				
			default:
				p--;
				break;
			}
			break;

		case '#':
			break;
		}

		p = SkipLine(p);
	}
}

OBJ_Mesh *OBJ_CreateMesh(const char *path)
{
	FILE *file = fopen(path, "r");
	if (!file) {
		return 0;
	}

	OBJ_Mesh *mesh = malloc(sizeof(OBJ_Mesh));
	if (!mesh) {
		return 0;
	}

	mesh->_positions = NULL;
	mesh->_texcoords = NULL;
	mesh->_normals = NULL;
	mesh->faces_vertices = NULL;
	mesh->_indices = NULL;

	SB_PUSH(mesh->_positions, 0.0f);
	SB_PUSH(mesh->_positions, 0.0f);
	SB_PUSH(mesh->_positions, 0.0f);

	SB_PUSH(mesh->_texcoords, 0.0f);
	SB_PUSH(mesh->_texcoords, 0.0f);

	SB_PUSH(mesh->_normals, 0.0f);
	SB_PUSH(mesh->_normals, 0.0f);
	SB_PUSH(mesh->_normals, 1.0f);

	OBJ_State state = {
		.mesh = mesh
	};

	char *buffer = malloc(2 * BUFFER_SIZE * sizeof(char));
	if (!buffer) {
		return 0;
	}

	char *start = buffer;
	while (1) {
		int read = (int)(fread(start, 1, BUFFER_SIZE, file));
		if (read == 0 && start == buffer) {
			break;
		}

		if (read < BUFFER_SIZE) {
			if (read == 0 || start[read - 1] != '\n') {
				start[read++] = '\n';
			}
		}

		char *end = start + read;
		if (end == buffer) {
			break;
		}

		char *last = end;
		while (last > buffer) {
			last--;
			if (*last == '\n') {
				break;
			}
		}
		
		if (*last != '\n') {
			break;
		}

		last++;
		ParseBuffer(&state, buffer, last);

		int extra = (int)(end - last);
		memmove(buffer, last, extra);
		start = buffer + extra;
	}

	mesh->num_positions = SB_LEN(mesh->_positions) / 3;
	mesh->num_texcoords = SB_LEN(mesh->_texcoords) / 2;
	mesh->num_normals = SB_LEN(mesh->_normals) / 3;
	mesh->num_faces = SB_LEN(mesh->faces_vertices);

	mesh->positions = (Vec3f *)mesh->_positions;
	mesh->texcoords = (Vec2f *)mesh->_texcoords;
	mesh->normals = (Vec3f *)mesh->_normals;
	mesh->indices = (Vec3u *)mesh->_indices;

	free(buffer);
	fclose(file);
	return mesh;
}

void OBJ_DestroyMesh(OBJ_Mesh *mesh)
{
	SB_FREE(mesh->_positions);
	SB_FREE(mesh->_texcoords);
	SB_FREE(mesh->_normals);
	SB_FREE(mesh->_indices);
	SB_FREE(mesh->faces_vertices);
	free(mesh);
}
