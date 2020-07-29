#pragma once
#include <vector>

bool showDispathedZone = true;


void reset(string&)
{
    clearAllChildCode();

    cout << "<<< All code reseted >>>" << endl;
}

void showAllCode(string&)
{
    outputFile(childFile.absolutePath);
}

void showZone(string& zoneName)
{
    string insertedZoneFileName = appPath + "\\childCode_" + zoneName + ".cpp";

    if (!checkFile(insertedZoneFileName))
        throw EvalConsoleError_WrongZone(zoneName);

    outputFile(insertedZoneFileName);
}

void backupZone(string& zoneName)
{
    string insertedZoneFileName = appPath + "\\childCode_" + zoneName + ".cpp";

    if (!checkFile(insertedZoneFileName))
        throw EvalConsoleError_WrongZone(zoneName);

    clearLastLineFile(insertedZoneFileName);
}

void changeDispathResultShow(string& val)
{
    if (val == "show")
        showDispathedZone = true;
    else if(val == "hide")
        showDispathedZone = false;
    else
        throw EvalConsoleError_WrongZone(val);
}


typedef void(*commandProc)(string&);
vector<pair<string, commandProc>> translatorCommands = {
    { "reset", reset },
    { "show all code", showAllCode },
    { "show zone", showZone },
    { "backup zone", backupZone },
    { "dispathed zone", changeDispathResultShow }
    //...
};

void dispathCommand(string& allCommand)
{
    for (int i = 0; i < translatorCommands.size(); i++)
    {
        if (allCommand.find(translatorCommands[i].first) == 0)
        {
            string commandArgs;

            if (allCommand.length() == translatorCommands[i].first.length())
                commandArgs = "";
            else
                commandArgs = allCommand.substr(translatorCommands[i].first.length() + 1);

            translatorCommands[i].second(commandArgs);
        }
    }
}