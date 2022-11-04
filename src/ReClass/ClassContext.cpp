#include "ClassContext.h"

#include "ClassMisc.h"


void ClassContext::RegisterClassMap(const std::string& name, Class* type)
{
	ClassMap.insert(std::make_pair(name, type));
}

Class* ClassContext::GetClass(const std::string& name)
{
	if(ClassMap.contains(name))
	{
		return ClassMap.at(name);
	}
	return nullptr;
}

void ClassContext::GetClassOf(const Class* type, std::vector<Class*>* out)
{
	for (auto& pair : ClassMap)
	{
		if(ClassLib::IsA(pair.second, type))
		{
			out->push_back(pair.second);
		}
	}
}

std::shared_ptr<void> ClassContext::Create(const std::string& name)
{
	auto type = GetClass(name);
	if(type)
	{
		return type->Create();
	}
	return  nullptr;
}

void ClassContext::RegisterTypeMap(const std::string& name, Type* type)
{
	TypeMap.insert(std::make_pair(name, type));
}

Type* ClassContext::GetType(const std::string& name)
{
	if (TypeMap.contains(name))
	{
		return TypeMap.at(name);
	}
	return GetClass(name);
}

ClassContext& ClassContext::Get()
{
	static ClassContext Instance;
	return Instance;
}
