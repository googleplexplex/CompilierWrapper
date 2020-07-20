#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class EvalConsoleError : exception
{
public:
    unsigned int ID = 0;
    string Description = NULL;

    EvalConsoleError(unsigned int _ID, string _Description)
    {
        ID = _ID;
        Description = _Description;
    }
};

class EvalConsoleError_WrongZone : public EvalConsoleError
{
public:
    EvalConsoleError_WrongZone(string wrongZoneName)
        : EvalConsoleError(1, string("Zone ") + wrongZoneName + " does not exist") {};
    EvalConsoleError_WrongZone()
        : EvalConsoleError(1, string("Zone does not exist")) {};
};

class EvalConsoleError_CannotOpenFile : public EvalConsoleError
{
public:
    EvalConsoleError_CannotOpenFile(string fileName)
        : EvalConsoleError(2, string("Cannot open ") + fileName) {};
    EvalConsoleError_CannotOpenFile()
        : EvalConsoleError(2, string("Cannot open file")) {};
};



void copyFileInStream(const string& fileName, ofstream& stream)
{
    string line;
    ifstream file(fileName);
    if (!file.is_open())
        throw new EvalConsoleError_CannotOpenFile(fileName);
    while (!file.eof())
    {
        getline(file, line);
        stream << line << endl;
    }
    file.close();
}

void collectChild()
{
    string line;
    ofstream childCodeFile;
    childCodeFile.open("childCode.cpp");
    if (!childCodeFile.is_open())
        throw new EvalConsoleError_CannotOpenFile("childCode.cpp");

    copyFileInStream("childCode_Libs.cpp", childCodeFile);
    copyFileInStream("childCode_Ops.cpp", childCodeFile);
    copyFileInStream("childCode_BeforeCode.cpp", childCodeFile);
    copyFileInStream("childCode_Code.cpp", childCodeFile);
    copyFileInStream("childCode_AfterCode.cpp", childCodeFile);

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
    ofstream childCode_Libs("childCode_Libs.cpp", ios::out | ios::trunc);
    childCode_Libs.close();
    ofstream childCode_Ops("childCode_Ops.cpp", ios::out | ios::trunc);
    childCode_Ops.close();
    ofstream childCode_BeforeCode("childCode_BeforeCode.cpp", ios::out | ios::trunc);
    childCode_BeforeCode.close();
    ofstream childCode_Code("childCode_Code.cpp", ios::out | ios::trunc);
    childCode_Code.close();
    ofstream childCode_AfterCode("childCode_AfterCode.cpp", ios::out | ios::trunc);
    childCode_AfterCode.close();
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