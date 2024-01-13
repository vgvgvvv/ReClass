#include "ReClassContext.h"
#include "ReClassPrefix.h"

#include "ReClassMisc.h"

namespace ReClassSystem
{
	class ClassContext : public IClassContext
	{
	public:
		void RegisterClassMap(const String& name, Class* type) override;
		void RegisterTypeMap(const String& name, Type* type) override;

		Class* GetClass(const String& name) override;
		Class* GetClassByHash(uint64 hash) override;
		void GetClassOf(const Class* type, Vector<Class*>* out) override;
		Type* GetType(const String& name) override;

		SharedPtr<void> Create(const String& name) override;
	private:
		UnorderedMap<uint64, Class*> ClassMap;
		UnorderedMap<uint64, Type*> TypeMap;
	};

	void ClassContext::RegisterClassMap(const String& name, Class* type)
	{
		auto hash = ClassDetail::Hash(name.c_str());
		ClassMap.insert(RECLASS_MAKE_PAIR(hash, type));
	}

	Class* ClassContext::GetClass(const String& name)
	{
		const auto hash = ClassDetail::Hash(name.c_str());
		if(ClassMap.contains(hash))
		{
			return ClassMap.at(hash);
		}
		return nullptr;
	}

	Class* ClassContext::GetClassByHash(uint64 hash)
	{
		if(ClassMap.contains(hash))
		{
			return ClassMap.at(hash);
		}
		return nullptr;
	}

	void ClassContext::GetClassOf(const Class* type, Vector<Class*>* out)
	{
		for (auto& pair : ClassMap)
		{
			if(IsA(pair.second, type))
			{
				out->push_back(pair.second);
			}
		}
	}

	SharedPtr<void> ClassContext::Create(const String& name)
	{
		auto type = GetClass(name);
		if(type)
		{
			return type->Create();
		}
		return  nullptr;
	}

	void ClassContext::RegisterTypeMap(const String& name, Type* type)
	{
		auto hash = ClassDetail::Hash(name.c_str());
		TypeMap.insert(RECLASS_MAKE_PAIR(hash, type));
	}

	Type* ClassContext::GetType(const String& name)
	{
		auto hash = ClassDetail::Hash(name.c_str());
		if (TypeMap.contains(hash))
		{
			return TypeMap.at(hash);
		}
		return GetClass(name);
	}

	IClassContext& IClassContext::Get()
	{
		static ClassContext Instance;
		return Instance;
	}

}
