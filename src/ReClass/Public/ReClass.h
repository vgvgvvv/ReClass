#pragma once
#include "ReClassPrefix.h"
#include "ReClassContext.h"
#include "ReClass/Private/TemplateUtility.h"

namespace ReClassSystem
{
	template<class T>
	struct TypeTag
	{
		typedef T value;
	};
	template<class T>
	struct ClassTag
	{
		typedef T value;
	};

	namespace ClassDetail
	{
		static constexpr uint64 kFNV1aValue = 0xcbf29ce484222325;
		static constexpr uint64 kFNV1aPrime = 0x100000001b3;

		inline constexpr uint64 Hash(char const* const str, uint64_t const value = kFNV1aValue) noexcept
		{
			return (str[0] == '\0')
				   ? value
				   : Hash(&str[1], (value ^ static_cast<uint64>(str[0])) * kFNV1aPrime);
		}
	}


	enum class EClassFlag
	{
		None = 0,
		Abstruct = 1 << 2,
		Final = 1 << 3
	};

	class Type
	{

	public:
		virtual ~Type() = default;

		Type(int32 size, const char* name) noexcept
			: TypeSize(size)
			, TypeHash(ClassDetail::Hash(name))
			, TypeName(name)
		{

		}

		RECLASS_NO_DISCARD virtual bool IsClass() const noexcept { return false; }
		RECLASS_NO_DISCARD virtual bool IsEnum() const noexcept { return false; }
		RECLASS_NO_DISCARD virtual bool IsPointer() const noexcept { return false; }

		RECLASS_NO_DISCARD uint64 GetSize() const noexcept { return TypeSize; }
		RECLASS_NO_DISCARD uint64 GetHash() const noexcept { return TypeHash; }
		RECLASS_NO_DISCARD const char* GetName() const noexcept { return TypeName; }
		RECLASS_NO_DISCARD bool IsValid() const noexcept { return TypeHash != 0 && TypeSize != 0; }


		bool operator==(Type const& other) const noexcept
		{
			return TypeHash == other.TypeHash;
		}

		bool operator!=(Type const& other) const noexcept
		{
			return !(*this == other);
		}


	protected:
		uint64 TypeSize;
		uint64 TypeHash;
		const char * TypeName;
	};

	inline uint32 GetTypeHash(Type type)
	{
		return type.GetHash();
	}

	using AddressOffset = Detail::ChooseClass<sizeof(void*) == 8, int64, int32>::Result;
	using InternalClassInfo = Pair<const Class*, AddressOffset>;

	class Class : public Type
	{
	public:

#pragma region Construction
		Class(
			int32 InSize,
			Function<InternalClassInfo()> InBaseClassGetter,
			const char* InName,
			EClassFlag InFlag = EClassFlag::None) noexcept
				: Type(InSize, InName)
				, GetBaseClassFunc(RECLASS_MOVE(InBaseClassGetter))
				, ClassFlag(InFlag)
		{
			IClassContext::Get().RegisterClassMap(InName, this);
			bDefined = true;
		}

		template<class Lambda>
		Class(int InSize
			, Function<InternalClassInfo()> InBaseClassGetter
			, const char * InName
			, EClassFlag InFlag
			, Lambda&& InCtor) noexcept
				: Type(InSize, InName)
				, GetBaseClassFunc(RECLASS_MOVE(InBaseClassGetter))
				, ClassFlag(InFlag)
		{
			InCtor(this);
			IClassContext::Get().RegisterClassMap(InName, this);
			bDefined = true;
		}

#pragma endregion // Construction

#pragma region ClassInfo
		RECLASS_NO_DISCARD bool IsClass() const noexcept override { return true; };
		RECLASS_NO_DISCARD const Class* GetBaseClass() const { return RECLASS_GET_FROM_PAIR(GetBaseClassFunc(), 0); }
		RECLASS_NO_DISCARD bool HasFlag(EClassFlag InFlag) const { return static_cast<int>(ClassFlag) & static_cast<int>(InFlag); }
		RECLASS_NO_DISCARD const Function<void(void*)>& GetDest() const { return Dest; }
		RECLASS_NO_DISCARD const Function<void*()>& GetCtor() const { return Ctor; }

		RECLASS_NO_DISCARD bool Implemented(const Class& Interface) const;
		RECLASS_NO_DISCARD bool IsA(const Class* targetClass) const;
		RECLASS_NO_DISCARD bool IsA(const Class& TargetClass) const;
		RECLASS_NO_DISCARD void* GetInterface(void* InInstance, const Class& InInterfaceClass) const;
#pragma endregion

#pragma region ClassInit
		void SetCtorAndDest(Function<void*()>&& InCtor, Function<void(void*)>&& InDest)
		{
			RECLASS_CHECK(!bDefined);
			Ctor = InCtor;
			Dest = InDest;
		}

		void AppendInterface(InternalClassInfo&& InInfo)
		{
			RECLASS_CHECK(!bDefined);
			if (InInfo.first == nullptr)
			{
				return;
			}
			Interfaces.emplace_back(InInfo);
		}

#pragma endregion // ClassInit

#pragma region Create Functions
		RECLASS_NO_DISCARD SharedPtr<void> Create() const;
		template<typename T>
		RECLASS_NO_DISCARD SharedPtr<T> Create() const;
		template<class T>
		RECLASS_NO_DISCARD T* CreateRaw() const;

		template<typename T>
		class InternalClassDeleter
		{
		public:
			explicit InternalClassDeleter(Function<void(void*)>& deleter)
				: Deleter(deleter)
			{
			}
			void operator()(T* Obj) const
			{
				Deleter(Obj);
			}
			Function<void(void*)>& Deleter;
		};

		template<typename T>
		UniquePtr<T> CreateUnique() const;
		template<typename T>
		static SharedPtr<T> Create(const String& ClassName)
		{
			auto cls = IClassContext::Get().GetClass(ClassName);
			if(!cls)
			{
				return nullptr;
			}
			return cls->Create<T>();
		}
#pragma endregion // Create Functions

		template<typename TSelfClass, typename TInternalClass>
		InternalClassInfo ToInternalClassInfo()
		{
			return InternalClassInfo
			{
				&TSelfClass::RECLASS_STATIC_CLASS_FUNCNAME(),
				reinterpret_cast<AddressOffset>(reinterpret_cast<char const*>(static_cast<TInternalClass*>(reinterpret_cast<TSelfClass*>(1)))) - 1
			};
		}

		inline constexpr static InternalClassInfo NullInternalClassInfo{nullptr, -1};

	protected:

		Function<InternalClassInfo()> GetBaseClassFunc;
		EClassFlag ClassFlag;
		Function<void*()> Ctor;
		Function<void(void*)> Dest;
		bool bDefined;
		Vector<InternalClassInfo> Interfaces;

	};

	inline uint32 GetTypeHash(Class cls)
	{
		return cls.GetHash();
	}

	struct TemplateArgument
	{
		enum class ETag {
			Type,
			Number,
		};

		ETag Tag;
		union {
			const Type* Type;
			uint64 Number;
		};
	};

	class ClassWithTemplate : public Class
	{
	public:
		ClassWithTemplate(
		int32 InSize,
		Function<InternalClassInfo()> InBaseClassGetter,
		const char* InName,
		EClassFlag InFlag,
		TemplateArgument* InTemplateArgs,
		TemplateArgument* InTemplateArgsEnd)
			: Class(
				InSize,
				RECLASS_MOVE(InBaseClassGetter),
				InName,
				InFlag)
			, TemplateArgs(InTemplateArgs)
			, TemplateArgsEnd(InTemplateArgsEnd)
		{}

		template<class Lambda>
		ClassWithTemplate(
			int InSize,
			Function<InternalClassInfo()> InBaseClassGetter,
			char const* InName,
			EClassFlag InFlag,
			Lambda&& InCtor,
			TemplateArgument* InTemplateArgs,
			TemplateArgument* InTemplateArgsEnd)
			: Class(
				InSize,
				RECLASS_MOVE(InBaseClassGetter),
				InName,
				InFlag,
				InCtor)
			, TemplateArgs(InTemplateArgs)
			, TemplateArgsEnd(InTemplateArgsEnd)
		{}

	public:
		TemplateArgument* TemplateArgs;
		TemplateArgument* TemplateArgsEnd;
	};

#include "ReClass/Private/ReClass.Implement.inl"
}
