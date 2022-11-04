#pragma once
#include <functional>
#include <memory>
#include <unordered_map>

#include "ReClass_API.h"

class Type;
class Class;

class ReClass_API ClassContext
{
public:

	static ClassContext& Get();

	void RegisterClassMap(const std::string& name, Class* type);

	Class* GetClass(const std::string& name);

	void GetClassOf(const Class* type, std::vector<Class*>* out);

	std::shared_ptr<void> Create(const std::string& name);

	template<class T>
	std::shared_ptr<T> CreateT(const std::string& name)
	{
		std::shared_ptr<void> result = Create(name);
		return std::static_pointer_cast<T>(result);
	}

	void RegisterTypeMap(const std::string& name, Type* type);

	Type* GetType(const std::string& name);
	
private:
	std::unordered_map<std::string, Class*> ClassMap;
	std::unordered_map<std::string, Type*> TypeMap;
};

