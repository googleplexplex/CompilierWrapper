#pragma once
#include <string>

struct compilierInfo
{
	string absolutePathToCompilier;
	string absolutePathToCompiliedEXE;
	string attributes;
};

compilierInfo gcc;
compilierInfo clang;

compilierInfo& usedCompilier = gcc;