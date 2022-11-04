#pragma

#include <vector>
#include <map>
#include <string>
#include "Class.h"


class EnumFieldInfo
{
public:
	EnumFieldInfo(const std::string& Name, int64_t InValue)
		: FieldName(Name)
		, Value(InValue)
	{
	}

	const std::string& GetName() const noexcept
	{
		return FieldName;
	}

	int64_t GetValue() const noexcept
	{
		return Value;
	}

private:
	const std::string FieldName;
	const int64_t Value = 0;
};


class EnumType final : public Type
{
public:

	EnumType(int InSize,
		char const* InName,
		bool InIsEnumClass,
		std::vector<EnumFieldInfo> InFields) noexcept
		: Type(InSize, InName)
		, IsEnumClass(InIsEnumClass)
	{
		for (auto& Field : InFields)
		{
			Fields.insert(std::make_pair(Field.GetName(), Field));
		}
		ClassContext::Get().RegisterTypeMap(InName, this);
	}

	const EnumFieldInfo* FindField(const std::string& EnumName)
	{
		auto Iter = Fields.find(EnumName);
		if (Iter != Fields.end())
		{
			return &(Iter->second);
		}
		return nullptr;
	}

	bool IsEnum() const noexcept override { return true; }


private:
	std::map<const std::string, const EnumFieldInfo> Fields;
	bool IsEnumClass = false;
};

