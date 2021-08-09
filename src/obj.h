#ifndef OBJ_H
#define OBJ_H

#include "linear.h"

typedef struct {
	Vec3f *positions;
	float *_positions;
	int num_positions;

	Vec2f *texcoords;
	float *_texcoords;
	int num_texcoords;

	Vec3f *normals;
	float *_normals;
	int num_normals;

	int *faces_vertices;
	int num_faces;

	Vec3u *indices;
	int *_indices;
} OBJ_Mesh;

OBJ_Mesh *OBJ_CreateMesh(const char *path);
void OBJ_DestroyMesh(OBJ_Mesh *mesh);

#endif /* OBJ_H */
