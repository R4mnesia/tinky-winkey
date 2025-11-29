#ifndef SVC_H
#define SVC_H


#include <stdio.h>
#include <string.h>


/* 
    minimal version for compilator 
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

#define INSTALL "install"
#define START   "start"
#define STOP    "stop"
#define DEL     "delete"

// install.c
void    install_svc(char *serviceName, char *pathExe);

// start.c
void    start_svc(char *serviceName);

// stop.c
void    stop_svc(void);

// delete.c
void    delete_svc(char *serviceName);

// utils.c
int     is_service_running(char *serviceName);

#endif