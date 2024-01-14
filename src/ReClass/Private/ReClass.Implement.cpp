#include "ReClass.h"

namespace ReClassSystem
{
	namespace {
		auto TypeInitClass = Type::RECLASS_STATIC_CLASS_FUNCNAME();
	}
	Class& Type::GetSelfClass()
	{
		static Class selfClass(
			sizeof(Type),
			[](){ return Class::NullInternalClassInfo; },
			"Type",
			false,
			EClassFlag::None,
			[](Class* self){
				using SelfType = Type;
			});
		return selfClass;
	}

	namespace {
		auto ClassInitClass = Class::RECLASS_STATIC_CLASS_FUNCNAME();
	}
	Class& Class::GetSelfClass()
	{
		static Class selfClass(sizeof(Class),
		[](){ return Class::ToInternalClassInfo<Class, Type>(); },
		"Class",
		false,
		EClassFlag::None,
		[](Class* self){
		using SelfType = Class;
		});
		return selfClass;
	}

	void* Class::GetInterface(void* InInstance, const Class& InInterfaceClass) const
	{
		if (InInstance == nullptr)
		{
			return nullptr;
		}

		if(*this == InInterfaceClass)
		{
			return InInstance;
		}

		for (auto& [InterfaceClass, Offset] : Interfaces)
		{
			if (InterfaceClass && InterfaceClass->IsA(InInterfaceClass))
			{
				return static_cast<uint8*>(InInstance) + Offset;
			}
		}

		if (const auto [BaseClass, BaseOffset] = GetBaseClassFunc(); BaseClass && BaseOffset >= 0)
		{
			return BaseClass->GetInterface(static_cast<uint8*>(InInstance) + BaseOffset, *BaseClass, InInterfaceClass);
		}

		return nullptr;
	}

	void* Class::GetInterface(void* InInstance, const Class& FromInterfaceClass,
							   const Class& ToInterfaceClass) const
	{
		if (InInstance == nullptr)
		{
			return nullptr;
		}

		if(*this == ToInterfaceClass)
		{
			return InInstance;
		}

		auto StartAddr = static_cast<uint8*>(InInstance);
		bool FoundFromInterfaceClass = false;
		if(this != &FromInterfaceClass)
		{
			if(GetBaseClass() == &FromInterfaceClass)
			{
				auto Offset = GetBaseClassFunc().second;
				FoundFromInterfaceClass = true;
				StartAddr -= Offset;
			}
			else
			{
				for (auto& [InterfaceClass, Offset] : Interfaces)
				{
					if (InterfaceClass && InterfaceClass->IsA(FromInterfaceClass))
					{
						FoundFromInterfaceClass = true;
						StartAddr -= Offset;
						break;
					}
				}
			}
		}
		else
		{
			FoundFromInterfaceClass = true;
		}

		if(FoundFromInterfaceClass)
		{
			for (auto& [InterfaceClass, Offset] : Interfaces)
			{
				if (InterfaceClass && InterfaceClass->IsA(ToInterfaceClass))
				{
					return StartAddr + Offset;
				}
			}
		}

		if (const auto [BaseClass, BaseOffset] = GetBaseClassFunc(); BaseClass && BaseOffset >= 0)
		{
			return BaseClass->GetInterface(static_cast<uint8*>(InInstance) + BaseOffset, FromInterfaceClass, ToInterfaceClass);
		}

		return nullptr;
	}

	const void* Class::GetInterface(const void* InInstance, const Class& FromInterfaceClass,
		const Class& ToInterfaceClass) const
	{
		return GetInterface(const_cast<void*>(InInstance), FromInterfaceClass, ToInterfaceClass);
	}
}