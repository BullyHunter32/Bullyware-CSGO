#pragma once
#include "IClientRenderable.h"
#include "CCollisionProperties.h"

class C_Entity
{
private:
	virtual void* __method_0();
	virtual void* __method_1();
	virtual void* __method_2();
public:
	virtual CCollisionProperties* GetCollisionProperties();
	virtual void* GetClientNetworkable();
	virtual IClientRenderable* GetClientRenderable();
private:
	virtual void* __method_6();
	virtual void* __method_7();
public:
	virtual void* GetClientThinkable();
private:
	virtual void* __method_9();
public:
	virtual Vector& GetABSOrigin();

	int GetTeam()
	{
		return *(int*)((uintptr_t)this + 0xF4);
	}

	int GetHealth()
	{
		return *(int*)((uintptr_t)this + 0x100);
	}

	void* GetCSTeam()
	{
		auto func = (void*(__thiscall*)(void*))((*(void***)this)[87]);
		return func(this);
	}
};