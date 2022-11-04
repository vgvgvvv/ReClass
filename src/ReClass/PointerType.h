#pragma once

#include "Class.h"

struct PointerType : public Type
{
public:
	PointerType(Type* InRawType)
		: Type(sizeof(int*)
			, InRawType->Name()"*")
		, RawType(InRawType)
		
	{
	}

	const Type& GetRawType()
	{
		return *RawType;
	}

	void* RawValue(void* PointObj)
	{
		return *PointObj;
	}

	virtual bool IsPointer() const noexcept { return true; }

private:
	Type* RawType;
};

template<typename T>
Type GetPointerType()
{
	return PointerType(T::StaticClass());
}