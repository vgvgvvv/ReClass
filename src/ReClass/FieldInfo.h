#pragma once
#include "Class.h"
#include "MemberInfo.h"

class FieldInfo : public MemberInfo
{
public:

	FieldInfo(std::string InFieldName
		, MemberAccessType InMemberAccessType
		, MemberFlag InMemberFlag
		, MemberOwnerType InOwnerType
		, const Type* InFieldType)
		: MemberInfo(std::move(InFieldName)
			, InMemberAccessType
			, InMemberFlag
			, InOwnerType)
		, FieldType(InFieldType)
	{
	}

	const Type* GetFieldType() const
	{
		return FieldType;
	}


	bool IsField() override
	{
		return true;
	}

private:
	const Type* FieldType = nullptr;
};