#pragma once

enum class MemberAccessType
{
	None,
	Public,
	Protected,
	Private
};

enum class MemberFlag
{
	None = 0,
	Static = 1,
	Abstruct = 1 << 1,
	Virtual = 1 << 2,
	Overrided = 1 << 3,
	Const = 1 << 4
};

inline MemberFlag operator|(MemberFlag a, MemberFlag b)
{
	return static_cast<MemberFlag>(static_cast<int>(a) | static_cast<int>(b));
}

inline MemberFlag operator|=(MemberFlag a, MemberFlag b)
{
	return static_cast<MemberFlag>(static_cast<int>(a) | static_cast<int>(b));
}

enum class MemberOwnerType
{
	None,
	ClassMember,
	Global
};


class MemberInfo
{
public:
	virtual ~MemberInfo() = default;

	MemberInfo(std::string&& InName
	           , MemberAccessType InAccessType
	           , MemberFlag InFlag
	           , MemberOwnerType InOwnerType)
		: Name(std::move(Name))
		, AccessType(InAccessType)
		, Flag(InFlag)
		, OwnerType(InOwnerType)
	{
	}

	const std::string& GetName() const
	{
		return Name;
	}


	MemberAccessType GetAccessType() const
	{
		return AccessType;
	}

	MemberFlag GetMemberFlag() const
	{
		return Flag;
	}

	const MemberOwnerType GetOwnerType() const
	{
		return OwnerType;
	}

	virtual bool IsField()
	{
		return false;
	}

	virtual bool IsMethod()
	{
		return false;
	}

private:
	const std::string Name;
	const MemberAccessType AccessType;
	const MemberFlag Flag;
	const MemberOwnerType OwnerType;
};