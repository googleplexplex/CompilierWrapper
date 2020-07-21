#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "EvalConsoleErrors.hpp"
using namespace std;

struct fileInfo
{
    string name;
    string absolutePath;
    string relativePath;

    fileInfo(string name)
    {
        this->name = name;
    }

    void fill(string pathToFile)
    {
        absolutePath = pathToFile + "\\" + name;
        relativePath = name;
    }
};

void copyFileInStream(const string& fileName, ofstream& stream)
{
    string line;
    ifstream file(fileName);
    if (!file.is_open())
        throw new EvalConsoleError_CannotOpenFile(fileName);
    while (!file.eof())
    {
        getline(file, line);
        stream << line << endl;
    }
    file.close();
}

void clearFile(const string& fileName)
{
    ofstream childCode_Libs(fileName, ios::out | ios::trunc);
    childCode_Libs.close();
}

string getAppPath(const string pathToOpenedExes)
{
    for (int i = pathToOpenedExes.length(); ; i--)
    {
        if (pathToOpenedExes[i] == '\\')
        {
            char* resCharPtr = new char[i];
            memcpy(resCharPtr, pathToOpenedExes.c_str(), i);
            resCharPtr[i] = NULL;
            return string(resCharPtr);
        }
    }

    return "";
}

void safetySystem(string command)
{
    if (system(command.c_str()) != NULL)
        throw new EvalConsoleError_SystemError();
}