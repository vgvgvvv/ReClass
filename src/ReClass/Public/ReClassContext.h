#pragma once
#include "ReClassPrefix.h"


namespace ReClassSystem
{
	class RECLASS_API IClassContext
	{
	protected:
		virtual ~IClassContext() = default;
	public:

		static IClassContext& Get();

		virtual void RegisterClassMap(const String& name, Class* type) = 0;
		virtual void RegisterTypeMap(const String& name, Type* type) = 0;

		virtual Class* GetClass(const String& name) = 0;
		virtual Class* GetClassByHash(uint64 hash) = 0;
		virtual void GetClassOf(const Class* type, Vector<Class*>* out) = 0;
		virtual Type* GetType(const String& name) = 0;

		virtual SharedPtr<void> Create(const String& name) = 0;
		template<class T>
		SharedPtr<T> CreateT(const String& name)
		{
			SharedPtr<void> result = Create(name);
			return RECLASS_SHAREDPTR_STATIC_CAST<T>(result);
		}

	};
}
