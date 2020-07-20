#pragma once
#include <string>
using namespace std;

class EvalConsoleError : exception
{
public:
    unsigned int ID = 0;
    string Description = NULL;

    EvalConsoleError(unsigned int _ID, string _Description)
    {
        ID = _ID;
        Description = _Description;
    }
};

class EvalConsoleError_WrongZone : public EvalConsoleError
{
public:
    EvalConsoleError_WrongZone(string wrongZoneName)
        : EvalConsoleError(1, string("Zone ") + wrongZoneName + " does not exist") {};
    EvalConsoleError_WrongZone()
        : EvalConsoleError(1, string("Zone does not exist")) {};
};

class EvalConsoleError_CannotOpenFile : public EvalConsoleError
{
public:
    EvalConsoleError_CannotOpenFile(string fileName)
        : EvalConsoleError(2, string("Cannot open ") + fileName) {};
    EvalConsoleError_CannotOpenFile()
        : EvalConsoleError(2, string("Cannot open file")) {};
};