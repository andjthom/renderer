#ifndef LINEAR_H
#define LINEAR_H

typedef unsigned int Vec3u[3];

typedef int Vec2i[2];
typedef int Vec4i[4];

typedef float Vec2f[2];
typedef float Vec3f[3];
typedef float Vec4f[4];

void Vec2i_Min(const Vec2i a, const Vec2i b, Vec2i result);
void Vec2i_Max(const Vec2i a, const Vec2i b, Vec2i result);
void Vec2i_Sub(const Vec2i a, const Vec2i b, Vec2i result);
int Vec2i_Dot(const Vec2i a, const Vec2i b);

void Vec3f_Cross(const Vec3f a, const Vec3f b, Vec3f result);

#endif /* !LINEAR_H */
