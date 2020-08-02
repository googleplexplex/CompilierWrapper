#pragma once
#include <vector>

bool showDispathedZone = true;
bool showErrors = true;
bool showTranslatorMessages = true;

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



typedef void(*commandProc)(vector<string>);
vector<pair<string, commandProc>> translatorCommands = {
    { "reset", reset },
    { "play", play },
    { "echo", setEchoMode },
    { "dispatched zone", changeDispathResultShow },

    { "show all code", showAllCode },
    { "show zone", showZone },
    { "backup zone", backupZone },
    { "delete all code", deleteAllCode },
    { "delete zone", deleteZone },

    { "show start script", showStartScript },
    { "backup start script", backupStartScript },
    { "add to start script", addToStartScript },
    { "delete start script", deleteStartScript },

    { "set code output color", setChildCodeOutputColor },
    { "set translator output color", setTranslatorOutputColor },
    { "set input color", setInputColor }

    //...
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
                string undisppatcheredArgs = allCommand.substr(translatorCommands[i].first.length() + 1);
                commandArgs = getArgs(undisppatcheredArgs);
            }

            translatorCommands[i].second(commandArgs);

            return;
        }
    }

    throw EvalConsoleError_WrongTranslatorCommand(allCommand);
}