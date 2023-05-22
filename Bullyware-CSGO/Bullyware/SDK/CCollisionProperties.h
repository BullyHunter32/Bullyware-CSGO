#pragma once
#include "Vector.h"

class CCollisionProperties
{
public:
	virtual ~CCollisionProperties();
	virtual void* GetEntity() const = 0;

	Vector& OBBMins()
	{
		return *(Vector*)((uintptr_t)this + 0x8);
	}
	Vector& OBBMaxs()
	{
		return *(Vector*)((uintptr_t)this + 0x14);
	}
};