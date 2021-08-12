#include "linear.h"

void Vec2i_Min(const Vec2i a, const Vec2i b, Vec2i result)
{
	for (int i = 0; i < 2; i++) {
		result[i] = a[i] < b[i] ? a[i] : b[i];
	}
}

void Vec2i_Max(const Vec2i a, const Vec2i b, Vec2i result)
{
	for (int i = 0; i < 2; i++) {
		result[i] = a[i] > b[i] ? a[i] : b[i];
	}
}

void Vec2i_Sub(const Vec2i a, const Vec2i b, Vec2i result)
{
	result[0] = a[0] - b[0];
	result[1] = a[1] - b[1];
}

int Vec2i_Dot(const Vec2i a, const Vec2i b)
{
	return a[0] * b[0] + a[1] * b[1];
}

void Vec3f_Cross(const Vec3f a, const Vec3f b, Vec3f result)
{
	result[0] = a[1] * b[2] - a[2] * b[1];
	result[1] = a[2] * b[0] - a[0] * b[2];
	result[2] = a[0] * b[1] - a[1] * b[0];
}
