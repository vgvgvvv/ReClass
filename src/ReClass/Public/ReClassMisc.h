#pragma once
#include "ReClassPrefix.h"
#include "ReClass.h"

namespace ReClassSystem
{
	inline bool IsA(const Class* self, const Class* targetClass)
	{
		if(self == nullptr)
		{
			return false;
		}
		if(targetClass == nullptr)
		{
			return true;
		}
		return self->IsA(targetClass);
	}

	inline uint64 ClassNameToHash(const String& className)
	{
		const uint64 hash = ClassDetail::Hash(className.c_str());
		return hash;
	}
}
