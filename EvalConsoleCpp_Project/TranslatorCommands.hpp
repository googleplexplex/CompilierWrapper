#pragma once
#include <vector>
#include "VariablesSaver.hpp"

vector<string> getArgs(string allArgs)
{
    if (allArgs == "")
        return { "" };

    vector<string> result;

    while (allArgs != "")
    {
        result.push_back(getWord(allArgs));

        if (result[result.size() - 1] == allArgs)
            return result;
        
        allArgs = allArgs.substr(result[result.size() - 1].length() + 1);
    }

    return result;
}

string collectArgs(vector<string> allArgs)
{
    string result;

    for (string i : allArgs)
    {
        result += i + ' ';
    }

    return result;
}


void reset(vector<string>)
{
    clearAllChildCode();

    if(showTranslatorMessages)
        cout << "<<< All code reseted >>>" << endl;
}

void showAllCode(vector<string>)
{
    if (showTranslatorMessages)
        cout << "<<< All code >>>" << endl;

    outputFile(childFile.absolutePath);
}

void showZone(vector<string> zoneName)
{
    string insertedZoneFileName = appPath + "\\childCode_" + zoneName[0] + ".cpp";

    if (!checkFile(insertedZoneFileName))
        throw EvalConsoleError_CannotOpenFile(zoneName[0]);

    if (showTranslatorMessages)
        cout << "<<< Zone" << zoneName[0] << " code >>>" << endl;

    outputFile(insertedZoneFileName);
}

void backupZone(vector<string> zoneName)
{
    string insertedZoneFileName = appPath + "\\childCode_" + zoneName[0] + ".cpp";

    if (!checkFile(insertedZoneFileName))
        throw EvalConsoleError_CannotOpenFile(zoneName[0]);

    clearLastLineFile(insertedZoneFileName);

    if (showTranslatorMessages)
        cout << "<<< Zone" << zoneName[0] << " backuped >>>" << endl;
}

void changeDispathResultShow(vector<string> val)
{
    if (val[0] == "show")
        showDispathedZone = true;
    else if(val[0] == "hide")
        showDispathedZone = false;
    else
        throw EvalConsoleError_WrongTranslatorCommand(val[0]);

    if (showTranslatorMessages)
        cout << "<<< Dispath Result Show changed >>>" << endl;
}

void setChildCodeOutputColor(vector<string> args)
{
    symbolColor firstArg = stingIsColor(args[0]);
    if(firstArg == null)
        throw EvalConsoleError_WrongTranslatorCommand(args[0]);
    childOutputColor = firstArg;

    if (args.size() > 1)
    {
        symbolColor secondArg = stingIsColor(args[1]);
        if (secondArg == null)
            throw EvalConsoleError_WrongTranslatorCommand(args[1]);
        childOutputColor = collectColor(firstArg, secondArg);
    }

    if (showTranslatorMessages)
        cout << "<<< Child code output color changed >>>" << endl;
}

void setTranslatorOutputColor(vector<string> args)
{
    symbolColor firstArg = stingIsColor(args[0]);
    if (firstArg == null)
        throw EvalConsoleError_WrongTranslatorCommand(args[0]);
    translatorOutputColor = firstArg;

    if (args.size() > 1)
    {
        symbolColor secondArg = stingIsColor(args[1]);
        if (secondArg == null)
            throw EvalConsoleError_WrongTranslatorCommand(args[1]);
        translatorOutputColor = collectColor(firstArg, secondArg);
    }

    if (showTranslatorMessages)
        cout << "<<< Translator output color changed >>>" << endl;
}

void setInputColor(vector<string> args)
{
    symbolColor firstArg = stingIsColor(args[0]);
    if (firstArg == null)
        throw EvalConsoleError_WrongTranslatorCommand(args[0]);
    inputColor = firstArg;

    if (args.size() > 1)
    {
        symbolColor secondArg = stingIsColor(args[1]);
        if (secondArg == null)
            throw EvalConsoleError_WrongTranslatorCommand(args[1]);
        inputColor = collectColor(firstArg, secondArg);
    }

    if (showTranslatorMessages)
        cout << "<<< Input color changed >>>" << endl;
}

void showStartScript(vector<string>)
{
    if (!checkFile(startScript.absolutePath))
        throw EvalConsoleError_CannotOpenFile(startScript.name);

    outputFile(startScript.absolutePath);

    if (showTranslatorMessages)
        cout << "<<< Start script >>>" << endl;
}

void backupStartScript(vector<string>)
{
    if (!checkFile(startScript.absolutePath))
        throw EvalConsoleError_CannotOpenFile(startScript.name);

    clearLastLineFile(startScript.absolutePath);

    if (showTranslatorMessages)
        cout << "<<< Start script backuped >>>" << endl;
}

void addToStartScript(vector<string> args)
{
    ofstream insertedstartScriptStream;
    insertedstartScriptStream.open(startScript.absolutePath, ios::app);
    for (int i = 0; i < args.size(); i++)
    {
        insertedstartScriptStream << args[i] << endl;
    }
    insertedstartScriptStream.close();

    if (showTranslatorMessages)
        cout << "<<< Start script edited >>>" << endl;
}

void setEchoMode(vector<string> args)
{
    if (args[0] == "translator")
    {
        if (args[1] == "on")
        {
            showTranslatorMessages = true;
        }
        else if (args[1] == "off")
        {
            showTranslatorMessages = false;
        }
        else
            throw EvalConsoleError_CannotOpenFile(args[1]);
    }
    else if (args[0] == "errors")
    {
        if (args[1] == "on")
        {
            showErrors = true;
        }
        else if (args[1] == "off")
        {
            showErrors = false;
        }
        else
            throw EvalConsoleError_WrongTranslatorCommand(args[1]);
    }
    else
        throw EvalConsoleError_WrongTranslatorCommand(args[0]);

    if (showTranslatorMessages)
        cout << "<<< Echo mode edited >>>" << endl;
}


void startChild();
void play(vector<string>)
{
    startChild();
}

void deleteAllCode(vector<string>)
{
    clearAllChildCode();

    if (showTranslatorMessages)
        cout << "<<< All code deleted >>>" << endl;
}

void deleteZone(vector<string> arg)
{
    if(arg[0] == "Libs")
        clearFile(childLibsFile.absolutePath);
    else if (arg[0] == "Ops")
        clearFile(childOpsFile.absolutePath);
    else if (arg[0] == "Code")
        clearFile(childCodeFile.absolutePath);
    else
        throw EvalConsoleError_WrongTranslatorCommand(arg[0]);

    if (showTranslatorMessages)
        cout << "<<< Zone" << arg[0] << " deleted >>>" << endl;
}

void deleteStartScript(vector<string> args)
{
    clearFile(startScript.absolutePath);

    if (showTranslatorMessages)
        cout << "<<< Start script deleted >>>" << endl;
}

void help(vector<string>)
{
    cout << "<<< Help list >>>" << endl;

    cout << "---Zones:" << endl;

    cout << "Libs - This zone stores work with import of libraries and preprocessor" << endl;
    cout << "Ops - This zone stores definitions (classes, functions and structures, etc)" << endl;
    cout << "Code - This zone stores function calls, working with variables and other executable code" << endl;
    cout << endl;


    cout << "---Input structure example:" << endl;

    cout << "--Explicit zone indication" << endl;
    cout << "Libs #include <iostream>" << endl;
    cout << "^    ^" << endl;
    cout << "Zone Code" << endl;
    cout << endl;

    cout << "--Implicit zone specification example:" << endl;
    cout << "#include <iostream>" << endl;
    cout << "^" << endl;
    cout << "Found # at the beginning, which means working with a preprocessor or importing a library. Distributed in Libs" << endl;
    cout << endl;


    cout << "---All translator commands:" << endl;

    cout << "reset - Reset all the code written into the translator before" << endl;
    cout << "   (Has no arguments)" << endl;
    cout << "play - Play written code" << endl;
    cout << "   (Has no arguments)" << endl;
    cout << "echo - Configures echo of translators" << endl;
    cout << "   (First argument - \"translator\" or \"errors\", Second - \"on\" or \"off\")" << endl;
    cout << "dispatched zone - Hides / displays the output of the zone where the code is placed" << endl;
    cout << "   (\"show\" or \"hide\")" << endl;
    cout << "set app mode - Set app mode" << endl;
    cout << "   (\"collector\" or \"translator\")" << endl;

    cout << "show all code - Show all code" << endl;
    cout << "   (Has no arguments)" << endl;
    cout << "show zone - Show zone" << endl;
    cout << "   (List of zones above)" << endl;
    cout << "backup zone - Delete last line in zone" << endl;
    cout << "   (List of zones above)" << endl;
    cout << "delete all code - Reset all the code written into the translator before" << endl;
    cout << "   (Has no arguments)" << endl;
    cout << "delete zone - Reset all the code written into the zone before" << endl;
    cout << "   (List of zones above)" << endl;

    cout << "show start script - Show start script" << endl;
    cout << "   (Has no arguments)" << endl;
    cout << "backup start script - Delete last line in start script" << endl;
    cout << "   (Has no arguments)" << endl;
    cout << "add to start script - Add string in start script" << endl;
    cout << "   (Added string)" << endl;
    cout << "delete start script - Delete start script" << endl;
    cout << "   (Has no arguments)" << endl;

    cout << "set code output color - Set code output color" << endl;
    cout << "   (Color text and color fone)" << endl;
    cout << "set translator output color - Set translator output color" << endl;
    cout << "   (Color text and color fone)" << endl;
    cout << "set input color - Set input color" << endl;
    cout << "   (Color text and color fone)" << endl;
    cout << endl;


    cout << "---Colors names:" << endl;
    outputAllColorsWithName();
    cout << endl;


    cout << "Official App GitHub link - https://github.com/googleplexplex/EvalConsoleCpp.git" << endl;
}

void setCompilier(vector<string> arg)
{
    if (arg.size() == 0)
        throw EvalConsoleError_WrongTranslatorCommand();

    string settedCompilierName = arg[0];

    for (int i = 0; i < compiliers.size(); i++)
    {
        if (compiliers[i].name == settedCompilierName)
        {
            usedNowCompilier = &(compiliers[i]);

            if (showTranslatorMessages)
                cout << "<<< Compilier " << settedCompilierName << " setted >>>" << endl;

            return;
        }
    }

    if (showTranslatorMessages)
        cout << "<<< Compilier " << settedCompilierName << " not found >>>" << endl;
}

void setAttributes(vector<string> arg)
{
    usedNowCompilier->attributes = arg[0];

    if (showTranslatorMessages)
        cout << "<<< Compilier attributes changed >>>" << endl;
}

void addCompilier(vector<string> args)
{
    if(args.size() != 4)
        throw EvalConsoleError_WrongTranslatorCommand();

    compilierInfo addedCompilier = { args[0], args[1], args[2], args[3] };

    compiliers.push_back(addedCompilier);

    if (showTranslatorMessages)
        cout << "<<< Compilier " << addedCompilier.name << " added >>>" << endl;
}

void deleteCompilier(vector<string> arg)
{
    if(arg.size() == 0)
        throw EvalConsoleError_WrongTranslatorCommand();
    
    string deletedCompilierName = arg[0];

    for (int i = 0; i < compiliers.size(); i++)
    {
        if (compiliers[i].name == deletedCompilierName)
        {
            compiliers.erase(compiliers.begin() + i);

            if (showTranslatorMessages)
                cout << "<<< Compilier " << deletedCompilierName << " deleted >>>" << endl;

            return;
        }
    }

    if (showTranslatorMessages)
        cout << "<<< Compilier " << deletedCompilierName << " not found >>>" << endl;
}

void showCompiliers(vector<string>)
{
    for (int i = 0; i < compiliers.size(); i++)
    {
        cout << compiliers[i].name << endl;
    }
}

void addToZone(vector<string> args)
{
    string insertedZone = args[0];
    args.erase(args.begin());
    string insertedContent = vectorToString(args);

    if (appMode == translator && insertedZone == "Vars")
    {
        varStruct serializedVar = Deserialize(collectArgs(args));
        varsList.push_back(serializedVar);
        return;
    }

    if (appMode == translator && insertedZone == "Code")
        clearFile(appPath + "\\childCode_Code.cpp");

    if (vectorHas(zones, insertedZone))
    {
        string insertedZoneFileName = appPath + "\\childCode_" + insertedZone + ".cpp";

        if (!checkFile(insertedZoneFileName))
            throw EvalConsoleError_WrongZone(insertedZone);

        ofstream insertedZoneFile;
        insertedZoneFile.open(insertedZoneFileName, ios::app);
        insertedZoneFile << insertedContent << endl;
        insertedZoneFile.close();
    }
}

void setAppMode(vector<string> arg)
{
    if (arg[0] == "collector")
    {
        appMode = collector;
    }
    else if (arg[0] == "translator")
    {
        appMode = translator;
    }
    else
        throw EvalConsoleError_CannotOpenFile(arg[0]);
}


typedef void(*commandProc)(vector<string>);
vector<pair<string, commandProc>> translatorCommands = {
    { "reset", reset },
    { "play", play },
    { "echo", setEchoMode },
    { "dispatched zone", changeDispathResultShow },
    { "help", help },
    { "set app mode", setAppMode },

    { "set compilier", setCompilier},
    { "set attributes", setAttributes},
    { "add compilier", addCompilier},
    { "delete compilier", deleteCompilier},
    { "show compiliers", showCompiliers},

    { "show all code", showAllCode },
    { "show zone", showZone },
    { "backup zone", backupZone },
    { "delete all code", deleteAllCode },
    { "delete zone", deleteZone },
    { "add to zone", addToZone },

    { "show start script", showStartScript },
    { "backup start script", backupStartScript },
    { "add to start script", addToStartScript },
    { "delete start script", deleteStartScript },

    { "set code output color", setChildCodeOutputColor },
    { "set translator output color", setTranslatorOutputColor },
    { "set input color", setInputColor }
};

void dispathCommand(string allCommand)
{
    for (int i = 0; i < translatorCommands.size(); i++)
    {
        if (allCommand.find(translatorCommands[i].first) == 0)
        {
            vector<string> commandArgs;

            if (allCommand.length() == translatorCommands[i].first.length())
                commandArgs.push_back("");
            else
            {
                string undispatcheredArgs = allCommand.substr(translatorCommands[i].first.length() + 1);
                commandArgs = getArgs(undispatcheredArgs);
            }

            translatorCommands[i].second(commandArgs);

            return;
        }
    }

    throw EvalConsoleError_WrongTranslatorCommand(allCommand);
}