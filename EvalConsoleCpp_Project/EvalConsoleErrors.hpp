#pragma once
#include <string>
using namespace std;


class EvalConsoleError
{
public:
    unsigned int ID = 0;
    string Description = "";

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

class EvalConsoleError_CannotStartChild : public EvalConsoleError
{
public:
    EvalConsoleError_CannotStartChild()
        : EvalConsoleError(3, string("Cannot start child")) {};
};

class EvalConsoleError_CannotCompileChild : public EvalConsoleError
{
public:
    EvalConsoleError_CannotCompileChild()
        : EvalConsoleError(4, string("Cannot compile child")) {};
};

class EvalConsoleError_SystemError : public EvalConsoleError
{
public:
    EvalConsoleError_SystemError()
        : EvalConsoleError(5, string("system() doesnot work")) {};
};