#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include "EvalConsoleErrors.hpp"
#include "FileHelper.hpp"
using namespace std;


string appPath;

fileInfo childFile("childCode.cpp");
fileInfo childLibsFile("childCode_Libs.cpp");
fileInfo childOpsFile("childCode_Ops.cpp");
fileInfo childCodeFile("childCode_Code.cpp");

string beforeCode = "int main(int argc, char* argv[])\n{\n";
string afterCode = "return 0;\n}\n";


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
        SetWindowTextA(consoleHWND, "Inputing");
    }
    else if (AppState == saving)
    {
        SetWindowTextA(consoleHWND, "Saving");
    }
    else if (AppState == collecting)
    {
        SetWindowTextA(consoleHWND, "Collecting");
    }
    else if (AppState == compiling)
    {
        SetWindowTextA(consoleHWND, "Compiling");
    }
    else if (AppState == playing)
    {
        SetWindowTextA(consoleHWND, "Playing");
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
    WaitForSingleObject(pi.hThread, INFINITE);
}

void startChild()
{
    STARTUPINFOA cif;
    ZeroMemory(&cif, sizeof(STARTUPINFO));
    PROCESS_INFORMATION pi;

    CreateProcessA((appPath + "\\Compiliers\\x86_64-8.1.0-win32-seh-rt_v6-rev0\\mingw64\\bin\\childCode.exe").c_str(), NULL, NULL, NULL, FALSE, NULL, NULL, NULL, &cif, &pi);
    WaitForSingleObject(pi.hThread, INFINITE);

    /*string gccBinPath = string("") + appPath + "\\Compiliers\\x86_64-8.1.0-win32-seh-rt_v6-rev0\\mingw64\\bin";
    string command = gccBinPath + "\\a.exe " + "\"" + gccBinPath + "\"";
    safetySystem(command);*/
}


string getInsertedZoneFromInput(string& inp)
{
    for (int i = 0; i < inp.length(); i++)
    {
        if (inp[i] == ' ')
        {
            return inp.substr(0, i);
        }
    }

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
        string inp;
        getline(cin, inp);

        try {
            string insertedZone = getInsertedZoneFromInput(inp);
            string inp_WithoutInsertedZone = inp.substr(insertedZone.length() + 1);

            AppState = saving; showAppState();
            insertInZone(insertedZone, inp_WithoutInsertedZone);
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