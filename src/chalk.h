#include <stdio.h>
#include <windows.h>

void printc(char *text, WORD color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;

    /* Save current attributes */
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;

    SetConsoleTextAttribute(hConsole, color);
    printf(text);

    /* Restore original attributes */
    SetConsoleTextAttribute(hConsole, saved_attributes);
    // printf("Back to normal");
}