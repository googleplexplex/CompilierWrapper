#pragma once
#include <vector>
#include "VariablesSaver.hpp"
#include "TranslatorCommands.hpp"

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

    if(appMode == translator)
        childCodeFileStream << beforeLibs;
    copyFileInStream(childLibsFile.absolutePath, childCodeFileStream);
    copyFileInStream(childOpsFile.absolutePath, childCodeFileStream);
    childCodeFileStream << beforeCode;
    if (appMode == translator)
        childCodeFileStream << getCodeVariablesInitilizer();
    copyFileInStream(childCodeFile.absolutePath, childCodeFileStream);
    if (appMode == translator)
        insertCodeVariablesSaverInStream(childCodeFileStream);
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
    if (vectorHas(zones, firstWord))
        return { firstWord, inp.substr(firstWord.length() + 1) };

    if (firstWord == "class" || firstWord == "struct" || firstWord == "namespace" || firstWord == "typedef")
    {
        string rawTypeName = inp.substr(firstWord.length() + 1);
        string typeName = getWord(rawTypeName, " {");
        typesList.push_back(typeName);
        return { "Ops", inp };
    }
    if (listHas(typesList, firstWord))
    {
        if(inp[inp.length() - 1] == ')')
            return { "Ops", inp };

        if (appMode == translator)
            return { "Vars", inp };
        else
            return { "Code", inp };
    }

    if (firstWord == "using")
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
    if (appMode == translator)
        updateCodeVariables();
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