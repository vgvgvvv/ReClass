#pragma once

#if RECLASS_FOR_UNREAL
namespace ReClassSystem
{
    namespace Detail
    {
        template <typename TClassName>
        struct BaseObjectWrapper<TClassName, typename TEnableIf<TIsDerivedFrom<TClassName, UObject>::Value>::Type>
        {
            static bool IsValidRawOrValidUObject(TClassName* Obj)
            {
                if (UObject* UObj = Cast<UObject>(Obj))
                {
                    return IsValid(UObj);
                }
                return false;
            }

            static TClassName* New()
            {
                return NewObject<TClassName>();
            }
        };

        template <typename TClassName, typename = void>
        struct GetUClassWrapper
        {
            static UClass* GetUClass()
            {
                return nullptr;
            }

            static constexpr bool IsUClass()
            {
                return false;
            }
        };

        template <typename TClassName>
        struct GetUClassWrapper<TClassName, typename TEnableIf<TIsDerivedFrom<TClassName, UObject>::Value>::Type>
        {
            static UClass* GetUClass()
            {
                return TClassName::StaticClass();
            }

            static constexpr bool IsUClass()
            {
                return true;
            }
        };
    }

    template <typename TClassName>
    UClass* GetUClass()
    {
        return Detail::GetUClassWrapper<TClassName>::GetUClass();
    }

    template <typename TClassName>
    bool IsUClass()
    {
        return Detail::GetUClassWrapper<TClassName>::IsUClass();
    }

    namespace Detail
    {
        // SFINAE test
        template <typename T>
        class HasStaticStruct
        {
            typedef char one;
            struct two { char x[2]; };

            template <typename C>
            static one test( decltype(&C::StaticStruct) ) ;
            template <typename C>
            static two test(...);

        public:
            enum { Value = sizeof(test<T>(0)) == sizeof(char) };
        };

        template <typename T, typename = void>
        struct GetStaticUStructWrapper
        {
            static UScriptStruct* GetUStruct()
            {
                return nullptr;
            }

            static bool IsUStruct()
            {
                return false;
            }
        };

        template <typename T>
        struct GetStaticUStructWrapper<T, typename TEnableIf<HasStaticStruct<T>::Value>::Type>
        {
            static UScriptStruct* GetUStruct()
            {
                return T::StaticStruct();
            }

            static bool IsUStruct()
            {
                return true;
            }
        };
    }

    template <typename TClassName>
    UScriptStruct* GetUStruct()
    {
        return Detail::GetStaticUStructWrapper<TClassName>::GetUStruct();
    }

    template <typename TClassName>
    bool IsUStruct()
    {
        return Detail::GetStaticUStructWrapper<TClassName>::IsUStruct();
    }

    inline FString ExpandEnumString(const FString& InName, const FString& EnumName)
    {
        FString expanded(InName);
        FString spaceLetter("");
        FString spaceNumber("");
        FString search("");
        expanded.ReplaceInline(*EnumName, TEXT(""), ESearchCase::CaseSensitive);
        expanded.ReplaceInline(TEXT("::"), TEXT(""), ESearchCase::CaseSensitive);
        for (TCHAR letter = 'A'; letter <= 'Z'; ++letter)
        {
            search = FString::Printf(TEXT("%c"), letter);
            spaceLetter = FString::Printf(TEXT(" %c"), letter);
            expanded.ReplaceInline(*search, *spaceLetter, ESearchCase::CaseSensitive);
        }
        for (TCHAR number = '0'; number <= '9'; ++number)
        {
            search = FString::Printf(TEXT("%c"), number);
            spaceNumber = FString::Printf(TEXT(" %c"), number);
            expanded.ReplaceInline(*search, *spaceNumber, ESearchCase::CaseSensitive);
        }
        expanded.ReplaceInline(TEXT("_"), TEXT(" -"), ESearchCase::CaseSensitive);
        expanded = expanded.RightChop(1).TrimStartAndEnd();
        return expanded;
    }

    template<typename T>
    FString EnumToString(const FString& enumName, const T value, const FString& defaultValue)
    {
        UEnum* pEnum = FindObject<UEnum>(ANY_PACKAGE, *enumName, true);
        return pEnum
            ? ExpandEnumString(pEnum->GetNameByIndex(static_cast<uint8>(value)).ToString(), enumName)
            : defaultValue;
    }

    inline UEnum* GetUEnum(const FString& InEnumName)
    {
        UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *InEnumName, true);
        return EnumPtr;
    }
}
#endif // RECLASS_FOR_UNREAL

#if RECLASS_FOR_UNREAL

namespace ReClassSystem
{
	namespace Detail
	{
		template<typename TValue, typename TOwner, typename = void>
		struct GetPropertyWrapper
		{
			static const TValue* GetPropertyValuePtr(const FProperty* Property, TOwner* Owner, int ArrayIndex = 0)
			{
				auto ValuePtr = Property->ContainerPtrToValuePtr<TValue>(Owner);
				return ValuePtr;
			}
		};

		template<typename TValue, typename TOwner>
		struct GetPropertyWrapper<TValue, TOwner, typename TEnableIf<HasStaticStruct<TValue>::Value>::Type>
		{
			static const TValue* GetPropertyValuePtr(const FProperty* Property, TOwner* Owner, int ArrayIndex = 0)
			{
				auto StructProperty = CastField<FStructProperty>(Property);
				if(StructProperty == nullptr)
				{
					return nullptr;
				}
				auto ValuePtr = StructProperty->ContainerPtrToValuePtr<TValue>(Owner);
				return ValuePtr;
			}
		};

		template<typename TValue, typename TOwner>
		struct GetPropertyWrapper<TValue, TOwner, typename TEnableIf<TIsDerivedFrom<TValue, UObject>::Value>::Type>
		{
			static const TValue* GetPropertyValuePtr(const FProperty* Property, TOwner* Owner, int ArrayIndex = 0)
			{
				auto ObjectProperty = CastField<FObjectProperty>(Property);
				if(ObjectProperty == nullptr)
				{
					return nullptr;
				}
				const UObject* ValuePtr = ObjectProperty->GetPropertyValue_InContainer(Owner);
				return Cast<TValue>(ValuePtr);
			}
		};

		template<typename TValue, typename TOwner>
		struct GetPropertyWrapper<TValue, TOwner, typename TEnableIf<TIsEnum<TValue>::Value>::Type>
		{
			static const TValue* GetPropertyValuePtr(const FProperty* Property, TOwner* Owner, int ArrayIndex = 0)
			{
				auto EnumProperty = CastField<FEnumProperty>(Property);
				auto ValuePtr = EnumProperty->ContainerPtrToValuePtr<TValue>(Owner, ArrayIndex);
				return static_cast<TValue*>(ValuePtr);
			}
		};

		template<typename TValue, typename TOwner>
		struct GetPropertyWrapper<TValue, TOwner, typename TEnableIf<TIsSame<TValue, FString>::Value>::Type>
		{
			static const TValue* GetPropertyValuePtr(const FProperty* Property, TOwner* Owner, int ArrayIndex = 0)
			{
				auto StrProperty = Cast<FStrProperty>(Property);
				if(StrProperty == nullptr)
				{
					return nullptr;
				}
				auto ValuePtr = StrProperty->GetPropertyValuePtr(Owner);
				return static_cast<TValue*>(ValuePtr);
			}
		};

		template<typename TValue, typename TOwner>
		struct GetPropertyWrapper<TValue, TOwner, typename TEnableIf<TIsSame<TValue, FName>::Value>::Type>
		{
			static const TValue* GetPropertyValuePtr(const FProperty* Property, TOwner* Owner, int ArrayIndex = 0)
			{
				auto NameProperty = CastField<FNameProperty>(Property);
				if(NameProperty == nullptr)
				{
					return nullptr;
				}
				auto ValuePtr = NameProperty->GetPropertyValuePtr(Owner);
				return static_cast<TValue*>(ValuePtr);
			}
		};

		template<typename TValue, typename TOwner>
		struct GetPropertyWrapper<TValue, TOwner, typename TEnableIf<TIsIntegral<TValue>::Value>::Type>
		{
			static const TValue* GetPropertyValuePtr(const FProperty* Property, TOwner* Owner, int ArrayIndex = 0)
			{
				auto NumberProperty = CastField<FNumericProperty>(Property);
				if(NumberProperty == nullptr)
				{
					return nullptr;
				}
				auto ValuePtr = NumberProperty->ContainerPtrToValuePtr<TValue>(Owner);
				return static_cast<TValue*>(ValuePtr);
			}
		};

		template<typename TValue, typename TOwner>
		struct GetPropertyWrapper<TValue, TOwner, typename TEnableIf<TIsFloatingPoint<TValue>::Value>::Type>
		{
			static const TValue* GetPropertyValuePtr(const FProperty* Property, TOwner* Owner, int ArrayIndex = 0)
			{
				auto NumberProperty = CastField<FNumericProperty>(Property);
				if(NumberProperty == nullptr)
				{
					return nullptr;
				}
				auto ValuePtr = NumberProperty->ContainerPtrToValuePtr<TValue>(Owner);
				return static_cast<TValue*>(ValuePtr);
			}
		};

		template<typename TValue, typename TOwner>
		struct GetPropertyWrapper<TValue, TOwner, typename TEnableIf<TIsSame<TValue, bool>::Value>::Type>
		{
			static const TValue* GetPropertyValuePtr(const FProperty* Property, TOwner* Owner, int ArrayIndex = 0)
			{
				auto BoolProperty = CastField<FBoolProperty>(Property);
				auto ValuePtr = BoolProperty->ContainerPtrToValuePtr<TValue>(Owner);
				return static_cast<TValue*>(ValuePtr);
			}
		};

		template<typename TValue, typename TOwner>
		struct GetPropertyWrapper<TArray<TValue>, TOwner, void>
		{
			static const TArray<TValue>* GetPropertyValuePtr(const FProperty* Property, TOwner* Owner, int ArrayIndex = 0)
			{
				auto ArrayProperty = CastField<FArrayProperty>(Property);
				auto ValuePtr = ArrayProperty->ContainerPtrToValuePtr<TArray<TValue>>(Owner, ArrayIndex);
				return ValuePtr;
			}
		};

		template<typename TKey, typename TValue, typename TOwner>
		struct GetPropertyWrapper<TMap<TKey, TValue>, TOwner, void>
		{
			static const TMap<TKey, TValue>* GetPropertyValuePtr(const FProperty* Property, TOwner* Owner, int ArrayIndex = 0)
			{
				auto MapProperty = CastField<FMapProperty>(Property);
				auto ValuePtr = MapProperty->ContainerPtrToValuePtr<TMap<TKey, TValue>>(Owner, ArrayIndex);
				return ValuePtr;
			}
		};

		template<typename TValue, typename TOwner>
		struct GetPropertyWrapper<TSet<TValue>, TOwner, void>
		{
			static const TSet<TValue>* GetPropertyValuePtr(const FProperty* Property, TOwner* Owner, int ArrayIndex = 0)
			{
				auto SetProperty = CastField<FSetProperty>(Property);
				auto ValuePtr = SetProperty->ContainerPtrToValuePtr<TSet<TValue>>(Owner, ArrayIndex);
				return ValuePtr;
			}
		};

	}

	template<typename TValue, typename TOwner>
	const TValue* GetPropertyValuePtr(const FProperty* Property, TOwner* Owner, int ArrayIndex = 0)
	{
		return Detail::GetPropertyWrapper<TValue, TOwner>::GetPropertyValuePtr(Property, Owner, ArrayIndex);
	}

	inline void ArrayPropertyForeach(const FArrayProperty* Property,
			const void* Owner, TFunction<void(const FArrayProperty*, int, void*)> Handler)
	{
		FScriptArrayHelper_InContainer Helper(Property, Owner);

		int Num = Helper.Num();
		for(int i = 0; i < Num; i++)
		{
			auto Value = Helper.GetRawPtr(i);
			Handler(Property, i, Value);
		}
	}

	inline void MapPropertyForeach(const FMapProperty* Property,
		const void* Owner, TFunction<void(const FMapProperty*, void*, void*)> Handler)
	{
		FScriptMapHelper_InContainer Helper(Property, Owner);

		int32 Num = Helper.Num();
		int32 MapIndex = 0;
		for (int32 DynamicIndex = 0; Num; ++DynamicIndex)
		{
			if(Helper.IsValidIndex(DynamicIndex))
			{
				void* KeyPtr = Helper.GetKeyPtr(DynamicIndex);
				void* ValuePtr = Helper.GetValuePtr(DynamicIndex);

				Handler(Property, KeyPtr, ValuePtr);

				--Num;
			}
		}
	}

	inline void SetPropertyForeach(const FSetProperty* Property,
		const void* Owner, TFunction<void(const FSetProperty*, void*)> Handler)
	{
		FScriptSetHelper_InContainer Helper(Property, Owner);

		int32 Num = Helper.Num();
		for (int32 DynamicIndex = 0; Num; ++DynamicIndex)
		{
			if (Helper.IsValidIndex(DynamicIndex))
			{
				void* ValuePtr = Helper.GetElementPtr(DynamicIndex);
				Handler(Property, ValuePtr);

				--Num;
			}
		}
	}

}

#endif // RECLASS_FOR_UNREAL