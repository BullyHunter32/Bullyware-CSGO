#pragma once
#include "C_Entity.h"

class IClientEntityList
{
public:
	virtual void* GetClientNetworkable(int entnum) = 0;
	virtual void* GetClientNetworkableFromHandle(void* hEnt) = 0;
	virtual void* GetClientUnknownFromHandle(void* hEnt) = 0;
	virtual C_Entity* GetClientEntity(int entnum) = 0;
	virtual void* GetClientEntityFromHandle(void* hEnt) = 0;
	virtual int					NumberOfEntities(bool bIncludeNonNetworkable) = 0;
	virtual int					GetHighestEntityIndex(void) = 0;
	virtual void				SetMaxEntities(int maxents) = 0;
	virtual int					GetMaxEntities() = 0;
};
