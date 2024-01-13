#pragma once

#pragma region // Implement
	template <typename T>
	SharedPtr<T> Class::Create() const
	{
		if(Ctor && Dest)
		{
			if(!Detail::IsSame<T, void>::value && !IsA(T::RECLASS_STATIC_CLASS_FUNCNAME()))
			{
				return nullptr;
			}
			return RECLASS_MAKE_SHARED(T)(static_cast<T*>(Ctor()), [Dest = Dest](void* Obj)
			{
				T::RECLASS_STATIC_CLASS_FUNCNAME().GetDest()(Obj);
			});
		}
		return nullptr;
	}

	template <class T>
	T* Class::CreateRaw() const
	{
		if(Ctor && Dest)
		{
			return static_cast<T*>(Ctor());
		}
		return nullptr;
	}

	template <typename T>
	UniquePtr<T> Class::CreateUnique() const
	{
		return RECLASS_MAKE_UNIQUE(T)(static_cast<T*>(Ctor()), InternalClassDeleter<T>());
	}

	inline SharedPtr<void> Class::Create() const
	{
		if(Ctor && Dest)
		{
			return RECLASS_MAKE_SHARED(void)(Ctor(), [Dest = Dest](void* Obj)
			{
				Dest(Obj);
			});
		}
		return nullptr;
	}

	inline bool Class::Implemented(const Class& Interface) const
	{
		for (auto& pair : Interfaces)
		{
			auto& InterfaceClass = RECLASS_GET_FROM_PAIR(pair, 0);
			if(InterfaceClass && InterfaceClass->IsA(Interface))
			{
				return true;
			}
		}
		return false;
	}

	inline bool Class::IsA(const Class* targetClass) const
	{
		if(targetClass == nullptr)
		{
			return false;
		}
		return IsA(*targetClass);
	}

	inline bool Class::IsA(const Class& TargetClass) const
	{
		if(*this == TargetClass)
		{
			return true;
		}
		if(Implemented(TargetClass))
		{
			return true;
		}
		const auto LocalBaseClass = GetBaseClass();
		if (LocalBaseClass == nullptr)
		{
			return false;
		}
		if (*LocalBaseClass == TargetClass)
		{
			return true;
		}
		return LocalBaseClass->IsA(TargetClass);
	}

	inline void* Class::GetInterface(void* InInstance, const Class& InInterfaceClass) const
	{
		if (InInstance == nullptr)
		{
			return nullptr;
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
			return BaseClass->GetInterface(static_cast<uint8*>(InInstance) + BaseOffset, InInterfaceClass);
		}

		return nullptr;
	}
#pragma endregion