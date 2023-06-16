#pragma once
#include <stdint.h>

class Vector
{
public:
	float x, y, z;

	Vector& operator += (const Vector& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}

	Vector operator + (const Vector& vec)
	{
		return { x + vec.x, y + vec.y, z + vec.z };
	}

	float Dist2DSqr(const Vector& vec)
	{
		float delta = (x - vec.x) * (x - vec.x) + (y - vec.y) * (y - vec.y);
		return delta;
	}

	float Dist2D(const Vector& vec)
	{
		float delta = (x - vec.x) * (x - vec.x) + (y - vec.y) * (y - vec.y);
		return sqrt(delta);
	}

	Vector(float X = 0.f, float Y = 0.f, float Z = 0.F)
	{
		x = X; y = Y; z = Z;
	}

	operator D3DXVECTOR2()
	{
		return { x, y };
	}

	operator D3DXVECTOR2*()
	{
		return *this;
	}
};

class QAngle : public Vector
{

};