//
// Created by 35207 on 11/3/2022.
//
#include <iostream>
#include "TestClass.h"

int main()
{
	std::cout << "hello " << TestClass::StaticClass()->Name() << std::endl;
	return 0;
}