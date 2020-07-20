#include <iostream>
#include <string>
#include <fstream>
#include "EvalConsoleErrors.hpp"
#include "FileHelper.hpp"
using namespace std;


string beforeCode = "int main(int argc, char* argv[])\n{\n";
string afterCode = "return 0;\n}\n";

void collectChild()
{
    string line;
    ofstream childCodeFile;
    childCodeFile.open("childCode.cpp");
    if (!childCodeFile.is_open())
        throw new EvalConsoleError_CannotOpenFile("childCode.cpp");

    copyFileInStream("childCode_Libs.cpp", childCodeFile);
    copyFileInStream("childCode_Ops.cpp", childCodeFile);
    childCodeFile << beforeCode;
    copyFileInStream("childCode_Code.cpp", childCodeFile);
    childCodeFile << afterCode;

    childCodeFile.close();
}

void compileChild()
{
    //compile "ChildCode.cpp"
}

void startChild()
{
    //start "ChildCode.exe"
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
    string insertedZoneFileName = "childCode_" + insertedZone + ".cpp";
    insertedZoneFile.open(insertedZoneFileName, ios::app);

    if (!insertedZoneFile.is_open())
        throw new EvalConsoleError_CannotOpenFile(insertedZoneFileName);
    insertedZoneFile << insertedStr;

    insertedZoneFile.close();
}

void clearAllZones()
{
    clearFile("childCode_Libs.cpp");
    clearFile("childCode_Ops.cpp");
    clearFile("childCode_Code.cpp");
}


int main(void)
{
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