#pragma once

#pragma region // Implement
	template <typename T>
	SharedPtr<T> Class::Create() const
	{
		if(Ctor && Dest)
		{
			if(!IsSame<T, void>::value && !IsA(T::RECLASS_STATIC_CLASS_FUNCNAME()))
			{
				return nullptr;
			}
			return SharedPtr<T>(static_cast<T*>(Ctor()), [Dest = Dest](T* Obj)
			{
				Dest(Obj);
			});
		}
		else
		{
			RECLASS_CHECK(false);
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
		else
		{
			RECLASS_CHECK(false);
		}
		return nullptr;
	}

	template <typename T>
	UniquePtr<T> Class::CreateUnique() const
	{
		if(Ctor && Dest)
		{
			return RECLASS_MAKE_UNIQUE(T)(static_cast<T*>(Ctor()), InternalClassDeleter<T>(Dest));
		}
		else
		{
			RECLASS_CHECK(false);
		}
		return nullptr;
	}

	inline SharedPtr<void> Class::Create() const
	{
		if(Ctor && Dest)
		{
			return SharedPtr<void>(Ctor(), [Dest = Dest](void* Obj)
			{
				Dest(Obj);
			});
		}
		else
		{
			RECLASS_CHECK(false);
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

#pragma endregion