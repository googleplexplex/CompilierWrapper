#pragma once
#include <string>
#include <vector>

string getWord(string& allString, string separator = " ")
{
    size_t separatorPos = allString.find(separator);

    if (separatorPos != string::npos)
        return allString.substr(0, separatorPos);
    else
        return allString.substr(0, allString.length());
}

unsigned int getCountOf(string& allString, char counted)
{
    unsigned int result = 0;

    for (int i = 0; i < allString.length(); i++)
    {
        if (allString[i] == counted)
        {
            result++;
        }
    }

    return result;
}

string getlineCin()
{
    string input;
    getline(cin, input);
    return input;
}

string vectorToString(vector<string>& stringsVector, string separator = " ")
{
    string result;

    for (int i = 0; i < stringsVector.size(); i++)
    {
        result += stringsVector[i];

        if (i + 1 != stringsVector.size())
            result += separator;
    }

    return result;
}