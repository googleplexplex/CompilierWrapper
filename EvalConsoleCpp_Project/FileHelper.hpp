#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "EvalConsoleErrors.hpp"
using namespace std;

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