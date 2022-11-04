#pragma once

#include <map>
#include "FieldInfo.h"
#include "MethodInfo.h"

class ClassStorage
{
public:

	ClassStorage(const std::vector<FieldInfo>& InFieldInfos, 
		const std::vector<MethodInfo>& InMethodInfos)
	{
		for (const FieldInfo& FieldInfo : InFieldInfos)
		{
			FieldInfos.insert(std::make_pair(FieldInfo.GetName(), FieldInfo));
		}

		for(const MethodInfo& MethodInfo : InMethodInfos)
		{
			MethodInfos.insert(std::make_pair(MethodInfo.GetName(), MethodInfo));
		}
	}

	const FieldInfo* GetField(const std::string& Name) const
	{
		auto Iter = FieldInfos.find(Name);
		if(Iter != FieldInfos.end())
		{
			return &Iter->second;
		}else
		{
			return nullptr;
		}
	}

	const MethodInfo* GetMethod(const std::string& Name) const
	{
		auto Iter = MethodInfos.find(Name);
		if (Iter != MethodInfos.end())
		{
			return &Iter->second;
		}
		else
		{
			return nullptr;
		}
	}

private:
	std::map<std::string, FieldInfo> FieldInfos;
	std::map<std::string, MethodInfo> MethodInfos;
};