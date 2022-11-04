#pragma once
#include <string>
#include "Class.h"
#include "EnumType.h"
#include "MethodInfo.h"
#include "FieldInfo.h"
#include "MemberInfo.h"
#include "Namespace.h"
#include "BaseTypes.h"
#include "ClassStorage.h"


//------------------------------------------------------------------------------

#define RE_CLASS(...)
#define RE_FIELD(...)
#define RE_ENUM(...)
#define RE_FUNCTION(...)

//------------------------------------------------------------------------------
#define SET_DEFAULT_CLASS_CTOR(className) \
	self->SetCtor([](){ return std::make_shared<className>(); });

#define SET_CLASS_CTOR(ctorFunc) \
	self->SetCtor(ctorFunc);


//------------------------------------------------------------------------------


#define DEFINE_CLASS(className) \
public:\
	typedef className ClassType; \
	typedef void Super; \
	static std::string StaticClassName() { return #className;}\
	static const Class* StaticClass() { return &selfClass; }\
	virtual const Class* GetClass() const { return className::StaticClass(); }\
	virtual std::string ClassName() const { return #className; }\
private:\
	static Class selfClass;

#define DEFINE_CLASS_IMP_BEGIN(className, flag) \
	Class className::selfClass(sizeof(className), \
		nullptr, \
		#className, \
		[](){ return GetClassStorage<className>(); }, \
		flag, \
		[](Class* self){

#define DEFINE_CLASS_IMP_END() \
	});

#define DEFINE_CLASS_IMP(className) \
	DEFINE_CLASS_IMP_BEGIN(className, ClassFlag::None)\
	SET_DEFAULT_CLASS_CTOR(className);\
	DEFINE_CLASS_IMP_END()


#define DEFINE_CLASS_IMP_WITH_FLAG(className, flag) \
	DEFINE_CLASS_IMP_BEGIN(className, flag)\
	SET_DEFAULT_CLASS_CTOR(className);\
	DEFINE_CLASS_IMP_END()


#define DEFINE_DERIVED_CLASS(className, baseClassName) \
public:\
	typedef className ClassType; \
	typedef baseClassName Super; \
	static std::string StaticClassName() { return #className;} \
	static const Class* StaticClass() { return &selfClass; } \
	virtual const Class* GetClass() const override { return className::StaticClass(); } \
	virtual std::string ClassName() const override { return #className; } \
private:\
	static Class selfClass;



#define DEFINE_DERIVED_CLASS_IMP_BEGIN(className, baseClassName, flag) \
	Class className::selfClass(sizeof(className), \
		baseClassName::StaticClass(), \
		#className, \
		[](){ return GetClassStorage<className>(); }, \
		flag, \
		[](Class* self){ \
			

#define DEFINE_DERIVED_CLASS_IMP_END() \
	});


#define DEFINE_DERIVED_CLASS_IMP(className, baseClassName) \
	DEFINE_DERIVED_CLASS_IMP_BEGIN(className, baseClassName, ClassFlag::None)\
	SET_DEFAULT_CLASS_CTOR(className);\
	DEFINE_DERIVED_CLASS_IMP_END()


#define DEFINE_DERIVED_CLASS_IMP_WITH_FLAG(className, baseClassName, flag) \
	DEFINE_DERIVED_CLASS_IMP_BEGIN(className, baseClassName, flag)\
	SET_DEFAULT_CLASS_CTOR(className);\
	DEFINE_DERIVED_CLASS_IMP_END()


//------------------------------------------------------------------------------
#define DEFINE_TRANSFER(className) \
public:\
	template<class TransferFunction>\
	void TransferClass(TransferFunction& transferFunc);\
	virtual void TransferJsonWrite(class JsonWrite& transfer);\
	virtual void TransferJsonRead(class JsonRead& transfer);\
	virtual void TransferImGui(class ImGuiTransfer& transfer);


//------------------------------------------------------------------------------
#define DEFINE_GETTER(typeName, varName) \
	const typeName& get_##varName() const { return varName; }

#define DEFINE_SETTER(typeName, varName) \
	void set_##varName(const typeName& varName) { this->varName = varName; }

#define DEFINE_GETTER_SETTER(typeName, varName) \
	DEFINE_GETTER(typeName, varName) \
	DEFINE_SETTER(typeName, varName)

#define DEFINE_GETTER_DECLTYPE(varName) \
	auto get_##varName() const -> const decltype(varName)& { return varName; }

#define DEFINE_SETTER_DECLTYPE(varName) \
	void set_##varName(const decltype(this->varName)& varName) { this->varName = varName; }

#define DEFINE_GETTER_SETTER_DECLTYPE(typeName, varName) \
	DEFINE_GETTER_DECLTYPE(typeName, varName) \
	DEFINE_SETTER_DECLTYPE(typeName, varName)
