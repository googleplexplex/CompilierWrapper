#pragma once

string getWord(string& allString, string separator = " ")
{
    size_t separatorPos = allString.find(separator);

    if (separatorPos != string::npos)
        return allString.substr(0, separatorPos);
    else
        return allString.substr(0, allString.length());
}

string getlineCin()
{
    string input;
    getline(cin, input);
    return input;
}