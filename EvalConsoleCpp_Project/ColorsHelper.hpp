#pragma once
#include <windows.h>
#define setCursorBegin() setTo(0, 0)
#define enumInc(enumName, valName) valName = enumName(int(valName) + 1)

HANDLE stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);

typedef enum symbolColor {
	null = -1,
	colorsEnumBegin = 0,
	black = 0,
	blue,
	green,
	aqua,
	red,
	violet,
	yellow,
	white,
	gray,
	brightBlue,
	brightGreen,
	brightAqua,
	brightRed,
	brightViolet,
	brightYellow,
	brightWhite,
	colorsEnumEnd
};
#define symbolColorEnumInc(val) enumInc(symbolColor, val)

#define ifret(str, val) if (str == #val) return val
symbolColor stingIsColor(string& str)
{
	ifret(str, black); else
	ifret(str, blue); else
	ifret(str, green); else
	ifret(str, aqua); else
	ifret(str, red); else
	ifret(str, violet); else
	ifret(str, yellow); else
	ifret(str, white); else
	ifret(str, brightBlue); else
	ifret(str, brightGreen); else
	ifret(str, brightAqua); else
	ifret(str, brightRed); else
	ifret(str, brightViolet); else
	ifret(str, brightYellow); else
	ifret(str, brightWhite); else
	return null;
}

string toString(symbolColor color)
{
	switch (color)
	{
	case black: return "black";
	case blue: return "blue";
	case green: return "green";
	case aqua: return "aqua";
	case red: return "red";
	case violet: return "violet";
	case yellow: return "yellow";
	case white: return "white";
	case brightBlue: return "brightBlue";
	case brightGreen: return "brightGreen";
	case brightAqua: return "brightAqua";
	case brightRed: return "brightRed";
	case brightViolet: return "brightViolet";
	case brightYellow: return "brightYellow";
	case brightWhite: return "brightWhite";
	default: return "";
	}
}


symbolColor presentTextAttribute = white;
symbolColor inline collectColor(symbolColor textColor, symbolColor backgroundColor)
{
	return symbolColor(textColor + (backgroundColor * 16));
}
void inline setSymbolFullColor(symbolColor color)
{
	presentTextAttribute = collectColor(color, color);
	SetConsoleTextAttribute(stdHandle, presentTextAttribute);
}
void inline setSymbolColor(symbolColor textColor, symbolColor backgroundColor)
{
	presentTextAttribute = collectColor(textColor, backgroundColor);
	SetConsoleTextAttribute(stdHandle, presentTextAttribute);
}
void inline setStandartSymbolsColor()
{
	presentTextAttribute = white;
	SetConsoleTextAttribute(stdHandle, presentTextAttribute);
}


symbolColor childOutputColor = white;
symbolColor translatorOutputColor = aqua;
symbolColor inputColor = white;

void setChildOutputColor()
{
	presentTextAttribute = childOutputColor;
	SetConsoleTextAttribute(stdHandle, presentTextAttribute);
}

void setTranslatorOutputColor()
{
	presentTextAttribute = translatorOutputColor;
	SetConsoleTextAttribute(stdHandle, presentTextAttribute);
}

void setInputColor()
{
	presentTextAttribute = inputColor;
	SetConsoleTextAttribute(stdHandle, presentTextAttribute);
}


void outputColorWithName(symbolColor color)
{
	setSymbolColor(color, black);
	cout << toString(color) << endl;
}

void outputAllColorsWithName()
{
	symbolColor startSymbolColor = presentTextAttribute;

	for (symbolColor i = colorsEnumBegin; i < colorsEnumEnd; symbolColorEnumInc(i))
	{
		outputColorWithName(i);
	}

	setSymbolColor(startSymbolColor, black);
}