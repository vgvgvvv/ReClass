#pragma once
#include <cstdint>
#include <utility>
#include "ClassContext.h"

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
	static constexpr uint64_t kFNV1aValue = 0xcbf29ce484222325;
	static constexpr uint64_t kFNV1aPrime = 0x100000001b3;

	inline constexpr uint64_t
	Hash(char const* const str, uint64_t const value = kFNV1aValue) noexcept
	{
		return (str[0] == '\0')
			   ? value
			   : Hash(&str[1], (value ^ uint64_t(str[0])) * kFNV1aPrime);
	}
}


class Type;

struct TemplateArgument
{
	enum class Tag {
		Type,
		Number,
	};

	Tag tag;
	union {
		Type const* type;
		uint64_t number;
	};
};

enum class ClassFlag
{
	None = 0,
	Abstruct = 1 << 2
};

class Type
{

public:
	virtual ~Type() = default;

	Type(
			int size,
			char const* name) noexcept
			: TypeSize(size)
			, TypeHash(ClassDetail::Hash(name))
			, TypeName(name)
	{

	}

	virtual bool IsClass() const noexcept { return false; }

	virtual bool IsEnum() const noexcept { return false; }

	virtual bool IsPointer() const noexcept { return false; }

	uint64_t
	Size() const noexcept
	{
		return TypeSize;
	}

	uint64_t
	Hash() const noexcept
	{
		return TypeHash;
	}

	char const*
	Name() const noexcept
	{
		return TypeName;
	}

	bool
	operator==(Type const& other) const noexcept
	{
		return TypeHash == other.TypeHash;
	}

	bool
	operator!=(Type const& other) const noexcept
	{
		return !(*this == other);
	}


protected:
	uint64_t TypeSize;
	uint64_t TypeHash;
	char const* TypeName;
};

class ClassStorage;


namespace __ClassDetail
{
	template<typename T>
	struct GetClassStorageWrapper
	{
		static const ClassStorage* GetClassStorage()
		{
			return nullptr;
		}
	};
}

template<typename T>
const ClassStorage* GetClassStorage()
{
	return __ClassDetail::GetClassStorageWrapper<T>::GetClassStorage();
}

class Class : public Type
{

public:
	Class(
			int InSize,
			const Class* InBaseClass,
			char const* InName,
			std::function<const ClassStorage* ()> InGetClassStorageFunc,
			ClassFlag InFlag = ClassFlag::None) noexcept
			: Type(InSize, InName)
			, BaseClass(InBaseClass)
			, Flag(InFlag)
			, GetClassStorageFunc(std::move(InGetClassStorageFunc))
	{
		ClassContext::Get().RegisterClassMap(InName, this);
		Defined = true;
	}

	template<class Lambda>
	Class(
			int InSize,
			const Class* InBaseClass,
			char const* InName,
			std::function<const ClassStorage* ()> InGetClassStorageFunc,
			ClassFlag InFlag,
			Lambda&& InCtor) noexcept
			: Type(InSize, InName)
			, BaseClass(InBaseClass)
			, Flag(InFlag)
			, GetClassStorageFunc(std::move(InGetClassStorageFunc))
	{
		InCtor(this);
		ClassContext::Get().RegisterClassMap(InName, this);
		Defined = true;
	}

	virtual bool IsClass() const noexcept override { return true; };


	Class const* GetBaseClass() const noexcept
	{
		return BaseClass;
	}

	const ClassStorage* GetClassStorage() const noexcept
	{
		return GetClassStorageFunc();
	}

	bool IsA(const Class* targetClass) const
	{
		if(targetClass == nullptr)
		{
			return false;
		}
		if(*this == *targetClass)
		{
			return true;
		}
		const auto baseClass = GetBaseClass();
		if (baseClass == nullptr)
		{
			return false;
		}
		return baseClass->IsA(targetClass);
	}

	bool HasFlag(ClassFlag flag) const
	{
		return (int)Flag & (int)flag;
	}

	std::shared_ptr<void> Create() const
	{
		return Ctor();
	}

	template<class T>
	std::shared_ptr<T> Create() const
	{
		return std::static_pointer_cast<T>(Ctor());
	}

public:

	void SetCtor(std::function<std::shared_ptr<void>()>&& newCtor)
	{
		Ctor = newCtor;
	}

protected:
	const Class* BaseClass;
	ClassFlag Flag;
	std::function<std::shared_ptr<void>()> Ctor;
	std::function<const ClassStorage* ()> GetClassStorageFunc;
	bool Defined;
};

class ClassTemplate : public Class
{
public:
	ClassTemplate(
			int InSize,
			const Class* InBaseClass,
			char const* Name,
			std::function<const ClassStorage* ()> InGetClassStorageFunc,
			ClassFlag Flag,
			TemplateArgument* TemplateArgs,
			TemplateArgument* TemplateArgsEnd) noexcept
			: Class(
			InSize,
			InBaseClass,
			Name,
			std::move(InGetClassStorageFunc),
			Flag)
			, m_templateArgs(TemplateArgs)
			, m_templateArgsEnd(TemplateArgsEnd)
	{}

	template<class Lambda>
	ClassTemplate(
			int size,
			const Class* baseClass,
			char const* name,
			ClassFlag flag,
			Lambda&& ctor,
			TemplateArgument* templateArgs,
			TemplateArgument* templateArgsEnd) noexcept
			: Class(
			size,
			baseClass,
			name,
			flag,
			ctor)
			, m_templateArgs(templateArgs)
			, m_templateArgsEnd(templateArgsEnd)
	{}

public:
	TemplateArgument* m_templateArgs;
	TemplateArgument* m_templateArgsEnd;
};
