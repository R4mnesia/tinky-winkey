#ifndef SVC_H
#define SVC_H


#include <stdio.h>
#include <string.h>

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

void    install_svc(char *serviceName, char *pathExe);
void    start_svc(void);
void    stop_svc(void);
void    delete_svc(char *serviceName);

#endif