#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include "EvalConsoleErrors.hpp"
#include "FileHelper.hpp"
using namespace std;


string appPath;
const string appName = "Eval Console C++";

fileInfo childFile("childCode.cpp");
fileInfo childLibsFile("childCode_Libs.cpp");
fileInfo childOpsFile("childCode_Ops.cpp");
fileInfo childCodeFile("childCode_Code.cpp");

string beforeCode = "int main(int argc, char* argv[])\n{\n";
string afterCode = "return 0;\n}\n";
string nameStateSeparator = " - ";


HWND consoleHWND = GetConsoleWindow();

typedef enum AppStatesEnum
{
    inputing = 0,
    saving,
    collecting,
    compiling,
    playing
};
AppStatesEnum AppState;

void showAppState()
{
    if (AppState == inputing)
    {
        SetWindowTextA(consoleHWND, (appName + nameStateSeparator + "Inputing").c_str());
    }
    else if (AppState == saving)
    {
        SetWindowTextA(consoleHWND, (appName + nameStateSeparator + "Saving").c_str());
    }
    else if (AppState == collecting)
    {
        SetWindowTextA(consoleHWND, (appName + nameStateSeparator + "Collecting").c_str());
    }
    else if (AppState == compiling)
    {
        SetWindowTextA(consoleHWND, (appName + nameStateSeparator + "Compiling").c_str());
    }
    else if (AppState == playing)
    {
        SetWindowTextA(consoleHWND, (appName + nameStateSeparator + "Playing").c_str());
    }
}


void collectChild()
{
    string line;
    ofstream childCodeFileStream;
    childCodeFileStream.open(childFile.absolutePath);
    if (!childCodeFileStream.is_open())
        throw EvalConsoleError_CannotOpenFile(childFile.absolutePath);

    copyFileInStream(childLibsFile.absolutePath, childCodeFileStream);
    copyFileInStream(childOpsFile.absolutePath, childCodeFileStream);
    childCodeFileStream << beforeCode;
    copyFileInStream(childCodeFile.absolutePath, childCodeFileStream);
    childCodeFileStream << afterCode;

    childCodeFileStream.close();
}

void compileChild()
{
    STARTUPINFOA cif;
    ZeroMemory(&cif, sizeof(STARTUPINFO));
    PROCESS_INFORMATION pi;

    string compilierPath = appPath + "\\Compiliers\\x86_64-8.1.0-win32-seh-rt_v6-rev0\\mingw64\\bin\\g++.exe";
    string pathToCompiliedFile = appPath + "\\childCode.cpp";
    string compilierAttributes = "-o " + appPath + "\\Compiliers\\x86_64-8.1.0-win32-seh-rt_v6-rev0\\mingw64\\bin\\childCode";
    CreateProcessA(compilierPath.c_str(), (char*)((compilierPath + " " + pathToCompiliedFile + " " + compilierAttributes).c_str()), NULL, NULL, FALSE, NULL, NULL, NULL, &cif, &pi);
    WaitForSingleObject(pi.hProcess, INFINITE);
}

void startChild()
{
    STARTUPINFOA cif;
    ZeroMemory(&cif, sizeof(STARTUPINFO));
    PROCESS_INFORMATION pi;

    CreateProcessA((appPath + "\\Compiliers\\x86_64-8.1.0-win32-seh-rt_v6-rev0\\mingw64\\bin\\childCode.exe").c_str(), NULL, NULL, NULL, FALSE, NULL, NULL, NULL, &cif, &pi);
    WaitForSingleObject(pi.hProcess, INFINITE);
}


struct InputStruct
{
    string zone;
    string content;
};
InputStruct dispathInput(string& inp)
{
    if (inp[0] == '#')
        return { "Libs", inp };

    string firstWord;
    for (int i = 0; i < inp.length(); i++)
    {
        if (inp[i] == ' ')
        {
            firstWord = inp.substr(0, i);
            break;
        }
    }
    if (checkFile(appPath + "\\childCode_" + firstWord + ".cpp"))
        return { firstWord, inp.substr(firstWord.length() + 1) };

    if (firstWord == "class" || firstWord == "struct" || firstWord == "namespace" || firstWord == "typedef"
        || (inp[inp.length() - 1] == ')' && (firstWord == "void" || firstWord == "int" || firstWord == "long" || firstWord == "double" || firstWord == "float" || firstWord == "char"))) //TOFIX
        return { "Ops", inp };

    if (inp[inp.length() - 1] == ';')
        return { "Code", inp };

    throw EvalConsoleError_WrongZone(inp);
}

void insertInZone(string& insertedZone, string& insertedStr)
{
    string insertedZoneFileName = appPath + "\\childCode_" + insertedZone + ".cpp";

    if(!checkFile(insertedZoneFileName))
        throw EvalConsoleError_WrongZone(insertedZone);

    ofstream insertedZoneFile;
    insertedZoneFile.open(insertedZoneFileName, ios::app);
    insertedZoneFile << insertedStr << endl;
    insertedZoneFile.close();
}

void clearAllZones()
{
    clearFile(childLibsFile.absolutePath);
    clearFile(childOpsFile.absolutePath);
    clearFile(childCodeFile.absolutePath);
}


int main(int argc, char** argv)
{
    appPath = getAppPath(argv[0]);
    childFile.fill(appPath);
    childLibsFile.fill(appPath);
    childOpsFile.fill(appPath);
    childCodeFile.fill(appPath);

    clearAllZones();
    clearFile(childFile.absolutePath);

    while (true)
    {
        AppState = inputing; showAppState();
        cout << ">>>";
        string rawInput;
        getline(cin, rawInput);

        try {
            InputStruct input = dispathInput(rawInput);

            AppState = saving; showAppState();
            insertInZone(input.zone, input.content);
            AppState = collecting; showAppState();
            collectChild();
            AppState = compiling; showAppState();
            compileChild();
            AppState = playing; showAppState();
            startChild();
        }
        catch (EvalConsoleError catchedError)
        {
            cout << "Error " << catchedError.ID << " - " << catchedError.Description;
        }

        cout << endl;
    }

    return 0;
}