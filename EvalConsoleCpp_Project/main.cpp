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

const vector<string> zones = { "Libs", "Ops", "Code" };
fileInfo childFile("childCode.cpp");
fileInfo childLibsFile("childCode_Libs.cpp");
fileInfo childOpsFile("childCode_Ops.cpp");
fileInfo childCodeFile("childCode_Code.cpp");
fileInfo startScript("startScript.txt");
fileInfo varsValuesFile("Values.txt");
string pathToCompiliedFile;

string beforeLibs = "#include <fstream>\n";
string beforeCode = "int main(int argc, char* argv[])\n{\n";
string afterCode = "return 0;\n}\n";
string nameStateSeparator = " - ";

bool showDispathedZone = true;
bool showErrors = true;
bool showTranslatorMessages = true;

HWND consoleHWND = GetConsoleWindow();

void clearAllChildCode();

#include "ColorsHelper.hpp"
#include "CompilierHelper.hpp"
#include "AppState.hpp"
#include "TranslatorCommands.hpp"

#include "VariablesSaver.hpp"
#include "Core.hpp"

void showStartMessage()
{
    if (showTranslatorMessages)
    {
        setTranslatorOutputColor();

        cout << "Eval Console C++, Version 1.0." << endl
            << "(Type @help to get help)" << endl;
    }
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
    varsValuesFile.fill(appPath);
    childFile.checkCallable();
    childLibsFile.checkCallable();
    childOpsFile.checkCallable();
    childCodeFile.checkCallable();
    startScript.checkCallable();
    varsValuesFile.checkCallable();
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