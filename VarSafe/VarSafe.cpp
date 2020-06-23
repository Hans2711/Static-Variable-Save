// VarSafe.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "Var.h"

std::string getDecVar(std::string name);

int main()
{
	Start("dowhateveryyouwantinhere12");
	AddVar("Hello1", "12345abc1");
	std::string end = getDecVar("Hello1");
	std::cout << end << std::endl;
	std::cin.get();
}

std::string getDecVar(std::string name)
{
	std::string down =  GetVar(name);
	return  decrypt(down);
}