#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include "EvalConsoleErrors.hpp"
#include "FileHelper.hpp"
#include "FooHelper.hpp"
using namespace std;

string appPath;
const string appName = "Eval Console C++";

fileInfo childFile("childCode.cpp");
fileInfo childLibsFile("childCode_Libs.cpp");
fileInfo childOpsFile("childCode_Ops.cpp");
fileInfo childCodeFile("childCode_Code.cpp");
void clearAllChildCode();
fileInfo startScript("startScript.txt");
string pathToCompiliedFile;

string beforeCode = "int main(int argc, char* argv[])\n{\n";
string afterCode = "return 0;\n}\n";
string nameStateSeparator = " - ";

HWND consoleHWND = GetConsoleWindow();

#include "ColorsHelper.hpp"
#include "CompilierHelper.hpp"
#include "AppState.hpp"
#include "TranslatorCommands.hpp"


void showStartMessage()
{
    if (showTranslatorMessages)
    {
        setTranslatorOutputColor();

        cout << "Eval Console C++, Version 1.0." << endl
            << "(Type @help to get help)" << endl;
    }
}

void clearAllChildCode()
{
    clearFile(childLibsFile.absolutePath);
    clearFile(childOpsFile.absolutePath);
    clearFile(childCodeFile.absolutePath);
    clearFile(childFile.absolutePath);
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

    CreateProcessA(usedNowCompilier->absolutePathToCompilier.c_str(), (char*)((usedNowCompilier->absolutePathToCompilier + " " + pathToCompiliedFile + " " + usedNowCompilier->attributes).c_str()), NULL, NULL, FALSE, NULL, NULL, NULL, &cif, &pi);
    WaitForSingleObject(pi.hProcess, INFINITE);
}

void startChild()
{
    STARTUPINFOA cif;
    ZeroMemory(&cif, sizeof(STARTUPINFO));
    PROCESS_INFORMATION pi;

    CreateProcessA(usedNowCompilier->absolutePathToCompiliedEXE.c_str(), NULL, NULL, NULL, FALSE, NULL, NULL, NULL, &cif, &pi);
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

    string firstWord = getWord(inp);
    if (checkFile(appPath + "\\childCode_" + firstWord + ".cpp"))
        return { firstWord, inp.substr(firstWord.length() + 1) };

    if (firstWord == "class" || firstWord == "struct" || firstWord == "namespace" || firstWord == "typedef"
        || (inp[inp.length() - 1] == ')' && (firstWord == "void" || firstWord == "int" || firstWord == "long" || firstWord == "double" || firstWord == "float" || firstWord == "char"))) //TOFIX
        return { "Ops", inp };

    if(firstWord == "using")
        return { "Libs", inp };

    if (inp[inp.length() - 1] == ';')
        return { "Code", inp };

    throw EvalConsoleError_WrongZone(inp);
}

void eval(string& codeString)
{
    InputStruct input = dispathInput(codeString);
    if (showDispathedZone)
        cout << "(Dispathed to Zone " << input.zone << ")" << endl;

    AppState = saving; showAppState();
    vector<string> addToZoneArgs = { input.zone, input.content };
    addToZone(addToZoneArgs);

    AppState = collecting; showAppState();
    collectChild();
    AppState = compiling; showAppState();
    compileChild();
    setChildOutputColor();
    AppState = playing; showAppState();
    startChild();
}

void metaEval(string& dispathed)
{
    setTranslatorOutputColor();

    if (dispathed[0] == '@')
        dispathCommand(dispathed.substr(1));
    else
        eval(dispathed);

    setStandartSymbolsColor();
}

void launchStartScript()
{
    if (!checkFile(startScript.absolutePath))
    {
        cout << "(Start Script not found)" << endl;
        return;
    }

    string line;
    ifstream startScriptStream(startScript.absolutePath);
    while (!startScriptStream.eof())
    {
        getline(startScriptStream, line);

        if (line == "")
            continue;

        try {
            if(line[0] != '@')
                eval(line);
            else
                dispathCommand(line.substr(1));
        }
        catch (EvalConsoleError catchedError)
        {
            if (showErrors)
            {
                setTranslatorOutputColor();
                cout << "Error " << catchedError.ID << " - " << catchedError.Description << endl;
                setTranslatorOutputColor();
            }
        }
    }
    startScriptStream.close();
}


int main(int argc, char** argv)
{
    appPath = getAppPath(argv[0]);
    childFile.fill(appPath);
    childLibsFile.fill(appPath);
    childOpsFile.fill(appPath);
    childCodeFile.fill(appPath);
    startScript.fill(appPath);
    pathToCompiliedFile = appPath + "\\childCode.cpp";
    
    addGCCCompilierStandartPathes();
    addClangCompilierStandartPathes();
    usedNowCompilier = &(compiliers[0]);

    setTranslatorOutputColor();
    clearAllChildCode();
    launchStartScript();

    showStartMessage();

    string inputCache;
    while (true)
    {
        setTranslatorOutputColor();
        AppState = inputing; showAppState();
        cout << ">>>";
        setInputColor();
        string rawInput = getlineCin();

        try {
            inputCache += rawInput;
            int openedBlocks = getCountOf(inputCache, '{') - getCountOf(inputCache, '}');
            if (openedBlocks > 0)
                continue;

            metaEval(inputCache);
        }
        catch (EvalConsoleError catchedError)
        {
            if (showErrors)
            {
                setTranslatorOutputColor();
                cout << "Error " << catchedError.ID << " - " << catchedError.Description;
            }
        }
        inputCache = "";

        setTranslatorOutputColor();
        cout << endl;
    }

    return 0;
}