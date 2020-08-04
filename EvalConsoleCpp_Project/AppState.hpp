#pragma once

typedef enum AppStatesEnum
{
    inputing = 0,
    saving,
    collecting,
    compiling,
    playing
};
AppStatesEnum AppState;

void showAppState()
{
    if (AppState == inputing)
    {
        SetWindowTextA(consoleHWND, (appName + nameStateSeparator + "Inputing").c_str());
    }
    else if (AppState == saving)
    {
        SetWindowTextA(consoleHWND, (appName + nameStateSeparator + "Saving").c_str());
    }
    else if (AppState == collecting)
    {
        SetWindowTextA(consoleHWND, (appName + nameStateSeparator + "Collecting").c_str());
    }
    else if (AppState == compiling)
    {
        SetWindowTextA(consoleHWND, (appName + nameStateSeparator + "Compiling with " + usedNowCompilier->name).c_str());
    }
    else if (AppState == playing)
    {
        SetWindowTextA(consoleHWND, (appName + nameStateSeparator + "Playing").c_str());
    }
}