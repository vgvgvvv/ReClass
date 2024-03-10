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

	template<typename TTo, typename TFrom>
	inline TTo* CastTo(TFrom* Obj)
	{
		if(Obj == nullptr)
		{
			return nullptr;
		}
		const Class& fromClass = Obj->RECLASS_GET_CLASS_FUNCNAME();
		void* result = fromClass.GetInterface(Obj, fromClass, TTo::RECLASS_STATIC_CLASS_FUNCNAME());
		return reinterpret_cast<TTo*>(result);
	}

	inline uint64 ClassNameToHash(const String& className)
	{
		const uint64 hash = ClassDetail::Hash(className.c_str());
		return hash;
	}
}
