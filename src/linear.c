#include <math.h>
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

void Vec3f_Sub(const Vec3f a, const Vec3f b, Vec3f result)
{
	for (int i = 0; i < 3; i++) {
		result[i] = a[i] - b[i];
	}
}

float Vec3f_Dot(const Vec3f a, const Vec3f b)
{
	float result = 0.0f;
	for (int i = 0; i < 3; i++) {
		result += a[i] * b[i];
	}

	return result;
}

void Vec3f_Cross(const Vec3f a, const Vec3f b, Vec3f result)
{
	result[0] = a[1] * b[2] - a[2] * b[1];
	result[1] = a[2] * b[0] - a[0] * b[2];
	result[2] = a[0] * b[1] - a[1] * b[0];
}

float Vec3f_Length(const Vec3f a)
{
	return sqrtf(Vec3f_Dot(a, a));
}

void Vec3f_Scale(const Vec3f a, const float f, Vec3f result)
{
	for (int i = 0; i < 3; i++) {
		result[i] = a[i] * f;
	}
}

void Vec3f_Normalize(const Vec3f a, Vec3f result)
{
	float f = 1.0f / Vec3f_Length(a);
	Vec3f_Scale(a, f, result);
}
