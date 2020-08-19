#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "EvalConsoleErrors.hpp"
using namespace std;

bool checkFile(const string fileName);
void createFile(const string fileName);

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

    void checkCallable()
    {
        if (!checkFile(absolutePath))
        {
            createFile(absolutePath);

            if (!checkFile(absolutePath))
                throw new EvalConsoleError_CannotOpenFile(relativePath);
        }
    }
};

void copyFileInStream(const string& fileName, ostream& stream)
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
    ofstream clearedFile(fileName, ios::out | ios::trunc);
    clearedFile.close();
}

void clearLastLineFile(const string& fileName)
{
    ifstream readedFile;
    string allFile;
    string presentLine;
    readedFile.open(fileName);
    while (true)
    {
        getline(readedFile, presentLine);

        if (readedFile.eof())
            break;

        allFile += presentLine;
    }
    allFile = allFile.substr(allFile.length() - presentLine.length());
    readedFile.close();

    ofstream writedFile(fileName);
    writedFile << allFile;
    writedFile.close();
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

bool checkFile(const string fileName)
{
    ifstream file;
    file.open(fileName);

    bool result = file.is_open();

    file.close();

    return result;
}

void outputFile(const string fileName)
{
    string line;
    ifstream file(fileName);
    while (!file.eof())
    {
        getline(file, line);
        cout << line << endl;
    }
    file.close();
}

void createFile(const string fileName)
{
    ofstream createdFile(fileName);
    createdFile.close();
}