#pragma once
#include <string>
#include <list>

struct varStruct
{
	string type;
	string name;
	string value;

	string Serialize()
	{
		if (value != "")
			return type + ' ' + name + " = " + value + ';';
		else
			return type + ' ' + name + ';';
	}

	void Deserialize(string data)
	{
		string presentDataLine = data;

		type = getWord(presentDataLine);
		presentDataLine = (char*)(presentDataLine.c_str() + type.size() + 1);

		name = getWord(presentDataLine, "=");
		if (name == presentDataLine)
		{
			name = getWord(presentDataLine, ";");
			value = "";
			return;
		}
		presentDataLine = (char*)(presentDataLine.c_str() + name.size() + 1);

		value = getWord(presentDataLine, ";");
	}
};
varStruct Deserialize(string data)
{
	varStruct result;
	result.Deserialize(data);
	return result;
}


list<varStruct> varsList;
list<string> typesList = { "void", "int", "long", "double", "float", "char" };


void updateCodeVariables()
{
	if(!checkFile(varsValuesFile.absolutePath))
		throw new EvalConsoleError_CannotOpenFile(varsValuesFile.absolutePath);
	ifstream file(varsValuesFile.absolutePath);

	string line;
	for (int i = 0; i < varsList.size(); i++)
	{
		getline(file, line);
		getListElementRef(varsList, i).value = line;
	}

	file.close();
}

string getCodeVariablesInitilizer()
{
	string result = "";

	for (varStruct i : varsList)
	{
		result += i.Serialize() + '\n';
	}

	return result;
}

void insertCodeVariablesSaverInStream(ostream& stream)
{
	stream << "std::ofstream clearedFile(\"";
	stream << duplicateSlashes(varsValuesFile.absolutePath);
	stream << "\", std::ios::out | std::ios::trunc);" << endl;
	stream << "clearedFile.close();" << endl;

	stream << "std::ofstream valuesSave(\"";
	stream << duplicateSlashes(varsValuesFile.absolutePath);
	stream << "\");" << endl;
	
	for (varStruct i : varsList)
	{
		stream << "valuesSave << ";
		stream << i.name;
		stream << " << std::endl;" << endl;
	}

	stream << "valuesSave.close();" << endl;
}