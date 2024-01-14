//
// Created by chen.dong on 11/4/2022.
//

#pragma once
#include "ReClassInfo.h"

class TestClass
{
	DECLARE_CLASS(TestClass)
};

class TestClass2
{
	DEFINE_CLASS_DYNAMIC(TestClass2)
};

class TestClass3 : public TestClass
{
	DEFINE_DERIVED_CLASS_DYNAMIC(TestClass3, TestClass)
};

class TestClass4 : public TestClass2
{
	DECLARE_DERIVED_CLASS(TestClass4, TestClass2)
};