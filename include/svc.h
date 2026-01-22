#ifndef SVC_H
#define SVC_H


#include <stdio.h>
#include <string.h>

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

#define NTDDI_WIN11_DT 0x0A000007

#include <windows.h>
#include <tlhelp32.h>
#include <wchar.h>

#define INSTALL "install"
#define START   "start"
#define STOP    "stop"
#define DEL     "delete"

#define SERVICE_NAME "tinky"

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

typedef struct s_svc_ctx
{
    SERVICE_STATUS status;              // struct windows state of service
    SERVICE_STATUS_HANDLE hStatus;      // handle id status

    PROCESS_INFORMATION winkey_proc;    // handle process winkey
    HANDLE hStopEvent;                  // event for stop signal
} t_svc_ctx;


// install.c
void    install_svc(char *pathExe);

// start.c
void    start_svc(void);

// stop.c
void    stop_svc(void);

// delete.c
void    delete_svc(void);

// utils.c
int     is_service_running(void);
void    getFilePathFromExe(char *output, char *filename);
DWORD   GetWinLogonPid(DWORD sessionID);
HANDLE  GetSystemToken(void);
void    KillWinkeyPID(void);

// svc.c
void WINAPI ServiceMain(DWORD argc, LPTSTR *argv);

#endif