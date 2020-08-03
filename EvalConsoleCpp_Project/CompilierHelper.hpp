#pragma once
#include <string>
#include <vector>

struct compilierInfo
{
    string name;
	string absolutePathToCompilier;
	string absolutePathToCompiliedEXE;
	string attributes;
};

vector<compilierInfo> compiliers;
compilierInfo* usedNowCompilier;

void addGCCCompilierStandartPathes()
{
    compilierInfo gcc = { "gcc",
        appPath + "\\Compiliers\\x86_64-8.1.0-win32-seh-rt_v6-rev0\\mingw64\\bin\\g++.exe",
        appPath + "\\Compiliers\\x86_64-8.1.0-win32-seh-rt_v6-rev0\\mingw64\\bin\\childCode.exe",
        "-o " + appPath + "\\Compiliers\\x86_64-8.1.0-win32-seh-rt_v6-rev0\\mingw64\\bin\\childCode" };

    compiliers.push_back(gcc);
}

void addClangCompilierStandartPathes()
{
    compilierInfo clang = { "Clang",
        appPath + "\\Compiliers\\LLVM\\bin\\clang++.exe",
        appPath + "\\Compiliers\\LLVM\\bin\\childCode.exe",
        "-o " + appPath + "\\Compiliers\\LLVM\\bin\\childCode" };

    compiliers.push_back(clang);
}