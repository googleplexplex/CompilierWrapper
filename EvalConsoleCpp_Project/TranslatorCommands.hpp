#pragma once
#include <vector>

bool showDispathedZone = true;

vector<string> getArgs(string allArgs)
{
    if (allArgs == "")
        return { "" };

    vector<string> result;

    while (allArgs != "")
    {
        result.push_back(getWord(allArgs));
        allArgs = allArgs.substr(result[result.size() - 1].length());
    }

    return result;
}


void reset(vector<string>)
{
    clearAllChildCode();

    cout << "<<< All code reseted >>>" << endl;
}

void showAllCode(vector<string>)
{
    outputFile(childFile.absolutePath);
}

void showZone(vector<string> zoneName)
{
    string insertedZoneFileName = appPath + "\\childCode_" + zoneName[0] + ".cpp";

    if (!checkFile(insertedZoneFileName))
        throw EvalConsoleError_WrongZone(zoneName[0]);

    outputFile(insertedZoneFileName);
}

void backupZone(vector<string> zoneName)
{
    string insertedZoneFileName = appPath + "\\childCode_" + zoneName[0] + ".cpp";

    if (!checkFile(insertedZoneFileName))
        throw EvalConsoleError_WrongZone(zoneName[0]);

    clearLastLineFile(insertedZoneFileName);
}

void changeDispathResultShow(vector<string> val)
{
    if (val[0] == "show")
        showDispathedZone = true;
    else if(val[0] == "hide")
        showDispathedZone = false;
    else
        throw EvalConsoleError_WrongZone(val[0]);
}

void setChildCodeOutputColor(vector<string> args)
{
    symbolColor firstArg = stingIsColor(args[0]);
    if(firstArg == null)
        throw EvalConsoleError_WrongZone(args[0]);
    childOutputColor = firstArg;

    if (args.size() > 1)
    {
        symbolColor secondArg = stingIsColor(args[1]);
        if (secondArg == null)
            throw EvalConsoleError_WrongZone(args[1]);
        childOutputColor = collectColor(firstArg, secondArg);
    }
}

void setTranslatorOutputColor(vector<string> args)
{
    symbolColor firstArg = stingIsColor(args[0]);
    if (firstArg == null)
        throw EvalConsoleError_WrongZone(args[0]);
    translatorOutputColor = firstArg;

    if (args.size() > 1)
    {
        symbolColor secondArg = stingIsColor(args[1]);
        if (secondArg == null)
            throw EvalConsoleError_WrongZone(args[1]);
        translatorOutputColor = collectColor(firstArg, secondArg);
    }
}

void setInputColor(vector<string> args)
{
    symbolColor firstArg = stingIsColor(args[0]);
    if (firstArg == null)
        throw EvalConsoleError_WrongZone(args[0]);
    inputColor = firstArg;

    if (args.size() > 1)
    {
        symbolColor secondArg = stingIsColor(args[1]);
        if (secondArg == null)
            throw EvalConsoleError_WrongZone(args[1]);
        inputColor = collectColor(firstArg, secondArg);
    }
}



typedef void(*commandProc)(vector<string>);
vector<pair<string, commandProc>> translatorCommands = {
    { "reset", reset },
    { "show all code", showAllCode },
    { "show zone", showZone },
    { "backup zone", backupZone },
    { "dispathed zone", changeDispathResultShow },
    { "set code output color", setChildCodeOutputColor },
    { "set translator output color", setTranslatorOutputColor },
    { "set input color", setInputColor }
    //...
};

void dispathCommand(string& allCommand)
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
        }
    }
}