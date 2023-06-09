#pragma once
#include "IClientRenderable.h"
#include "CCollisionProperties.h"

enum CS_TEAM
{
	TEAM_CT = 3,
	TEAM_T = 2
};

typedef unsigned short bone_t;

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

	// index 391
	const char* LookupBone(const char* bone)
	{
		//// ValveBiped.Bip01_Head
		//auto func = (const char* (__thiscall*)(void*, const char*))((*(void***)this)[391]);
		//const char* ret = func(this, bone);
		//return ret;

		return nullptr;
	}

	bool IsPlayer()
	{
		return Utils::CallVFunction<bool>(this, 158);
	}

	const char* GetClassname()
	{
		return Utils::CallVFunction<const char*>(this, 143);
	}

	void SetSequence(int nSequence)
	{
		return Utils::CallVFunction<void>(this, 219, nSequence);
	}
};