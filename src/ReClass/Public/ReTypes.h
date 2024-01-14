#pragma once
#include "ReClassPrefix.h"
#include "ReClass.h"

namespace ReClassSystem
{
	RECLASS_API extern Type const* GetTypeImpl(TypeTag<void>) noexcept;
	RECLASS_API extern Type const* GetTypeImpl(TypeTag<bool>) noexcept;
	RECLASS_API extern Type const* GetTypeImpl(TypeTag<char>) noexcept;
	RECLASS_API extern Type const* GetTypeImpl(TypeTag<short>) noexcept;
	RECLASS_API extern Type const* GetTypeImpl(TypeTag<int>) noexcept;
	RECLASS_API extern Type const* GetTypeImpl(TypeTag<long>) noexcept;
	RECLASS_API extern Type const* GetTypeImpl(TypeTag<long long>) noexcept;
	RECLASS_API extern Type const* GetTypeImpl(TypeTag<float>) noexcept;
	RECLASS_API extern Type const* GetTypeImpl(TypeTag<double>) noexcept;
	RECLASS_API extern Type const* GetTypeImpl(TypeTag<long double>) noexcept;
	RECLASS_API extern Type const* GetTypeImpl(TypeTag<unsigned char>) noexcept;
	RECLASS_API extern Type const* GetTypeImpl(TypeTag<unsigned short>) noexcept;
	RECLASS_API extern Type const* GetTypeImpl(TypeTag<unsigned int>) noexcept;
	RECLASS_API extern Type const* GetTypeImpl(TypeTag<unsigned long>) noexcept;
	RECLASS_API extern Type const* GetTypeImpl(TypeTag<unsigned long long>) noexcept;

#if RECLASS_FOR_UNREAL
	RECLASS_API extern Type const* GetTypeImpl(TypeTag<FString>) noexcept;
	RECLASS_API extern Type const* GetTypeImpl(TypeTag<FName>) noexcept;
#endif


	template<typename T>
	Type const* GetType()
	{
		return GetTypeImpl(TypeTag<T>());
	}

	template<typename T>
	Class const * GetClass()
	{
		return T::RECLASS_STATIC_CLASS_FUNCNAME();
	}
}

