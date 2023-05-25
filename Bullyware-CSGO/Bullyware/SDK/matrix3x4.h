#pragma once
#include "Vector.h"

struct matrix3x4_t
{
	float* operator[](int i)
	{
		return m_matrix[i];
	}

	Vector GetVector(int i)
	{
		return Vector(this[i][0][3], this[i][1][3], this[i][2][3]);
	}

	float m_matrix[3][4];
};
