#pragma once

struct matrix3x4_t
{
	float* operator[](int i)
	{
		return m_matrix[i];
	}

	float m_matrix[3][4];
};
