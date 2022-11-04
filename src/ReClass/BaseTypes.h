#pragma once
#include "Class.h"
#include "ReClass_API.h"


ReClass_API extern Type const* GetTypeImpl(TypeTag<void>) noexcept;
ReClass_API extern Type const* GetTypeImpl(TypeTag<bool>) noexcept;
ReClass_API extern Type const* GetTypeImpl(TypeTag<char>) noexcept;
ReClass_API extern Type const* GetTypeImpl(TypeTag<short>) noexcept;
ReClass_API extern Type const* GetTypeImpl(TypeTag<int>) noexcept;
ReClass_API extern Type const* GetTypeImpl(TypeTag<long>) noexcept;
ReClass_API extern Type const* GetTypeImpl(TypeTag<long long>) noexcept;
ReClass_API extern Type const* GetTypeImpl(TypeTag<float>) noexcept;
ReClass_API extern Type const* GetTypeImpl(TypeTag<double>) noexcept;
ReClass_API extern Type const* GetTypeImpl(TypeTag<long double>) noexcept;
ReClass_API extern Type const* GetTypeImpl(TypeTag<unsigned char>) noexcept;
ReClass_API extern Type const* GetTypeImpl(TypeTag<unsigned short>) noexcept;
ReClass_API extern Type const* GetTypeImpl(TypeTag<unsigned int>) noexcept;
ReClass_API extern Type const* GetTypeImpl(TypeTag<unsigned long>) noexcept;
ReClass_API extern Type const* GetTypeImpl(TypeTag<unsigned long long>) noexcept;


template<typename T>
Type const* GetType()
{
	return GetTypeImpl(TypeTag<T>());
}

template<typename T>
Class const * GetClass()
{
	return T::StaticClass();
}