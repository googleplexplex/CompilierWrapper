#pragma once
#include <string>
#include <vector>
#include <list>
#ifndef MIN
#define MIN(f, s) (((f) >= (s))?(s):(f))
#endif

string getWord(string& allString, const char* separators = " ")
{
    unsigned int separatorsCount = strlen(separators);
    size_t minimalSeparatorsPos = string::npos;

    for (int i = 0; i < separatorsCount; i++)
    {
        minimalSeparatorsPos = MIN(allString.find(separators[i]), minimalSeparatorsPos);
    }

    if (minimalSeparatorsPos != string::npos)
        return allString.substr(0, minimalSeparatorsPos);
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

template <typename T>
bool listHas(list<T> list, T element)
{
    for (T i : list)
    {
        if (i == element)
            return true;
    }

    return false;
}

template <typename T>
bool vectorHas(vector<T> vector, T element)
{
    for (T i : vector)
    {
        if (i == element)
            return true;
    }

    return false;
}

bool replace(string& str, const string& from, const string& to)
{
    size_t start_pos = str.find(from);
    if (start_pos == string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

string replaceAll(string str, const string& from, const string& to)
{
    string result = str;
    string present = result;

    int offset = 0;
    while (true)
    {
        size_t find_pos = present.find(from);
        if (find_pos == string::npos)
            break;
        offset += find_pos;
        result.replace(offset, from.length(), to);
        present = (char*)(present.c_str() + offset + from.size());
    }

    return result;
}

string duplicateSlashes(string str)
{
    string result = str;

    for (int i = 0; i < result.size(); i++)
    {
        if (result[i] == '\\')
        {
            result.replace(i, 1, "\\\\");
            i++;
        }
    }

    return result;
}

template <typename T>
T& getListElementRef(list<T>& gettedList, unsigned int index)
{
    auto iterToElement = gettedList.begin();
    std::advance(iterToElement, index);
    return (*iterToElement);
}