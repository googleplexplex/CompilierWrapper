#include <iostream>
#include <string>
#include <fstream>
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

void collectChild()
{
    string line;
    ofstream childCodeFileStream;
    childCodeFileStream.open(childFile.absolutePath);
    if (!childCodeFileStream.is_open())
        throw new EvalConsoleError_CannotOpenFile(childFile.absolutePath);

    copyFileInStream(childLibsFile.absolutePath, childCodeFileStream);
    copyFileInStream(childOpsFile.absolutePath, childCodeFileStream);
    childCodeFileStream << beforeCode;
    copyFileInStream(childCodeFile.absolutePath, childCodeFileStream);
    childCodeFileStream << afterCode;

    childCodeFileStream.close();
}

void compileChild()
{
    string command = "\"" + appPath + "\\Compiliers\\x86_64-7.2.0-posix-seh-rt_v5-rev1\\mingw64\\bin\\g++.exe\" " + childFile.absolutePath;
    safetySystem(command);
}

void startChild()
{
    string gccBinPath = string("") + appPath + "\\Compiliers\\x86_64-7.2.0-posix-seh-rt_v5-rev1\\mingw64\\bin";
    string command = gccBinPath + "\\a.exe " + "\"" + gccBinPath + "\"";
    safetySystem(command);
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

    throw new EvalConsoleError_WrongZone(inp);
}

void insertInZone(string& insertedZone, string& insertedStr)
{
    ofstream insertedZoneFile;
    string insertedZoneFileName = "Debug\\childCode_" + insertedZone + ".cpp";
    insertedZoneFile.open(insertedZoneFileName, ios::app);

    if (!insertedZoneFile.is_open())
        throw new EvalConsoleError_CannotOpenFile(insertedZoneFileName);
    insertedZoneFile << insertedStr;

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
    while (true)
    {
        cout << ">>>";
        string inp;
        getline(cin, inp);

        try {
            string insertedZone = getInsertedZoneFromInput(inp);
            string inp_WithoutInsertedZone = inp.substr(insertedZone.length() + 1);

            insertInZone(insertedZone, inp_WithoutInsertedZone);
            collectChild();
            compileChild();
            startChild();
        }
        catch (EvalConsoleError catchedError)
        {
            cout << "Error " << catchedError.ID << " - " << catchedError.Description << endl;
        }
    }

    return 0;
}