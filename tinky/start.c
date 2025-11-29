#include "svc.h"

void    start_svc(char *serviceName)
{
    SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!hSCM)
    {
        printf("Unable to open SCM: %lu\n", GetLastError());
        return ;
    }

    SC_HANDLE hService = OpenService(
        hSCM,
        serviceName,
        SERVICE_START
    );
    if (!hService)
    {
        printf("Open service failed: %lu\n", GetLastError());
        CloseServiceHandle(hSCM);
        return ;
    }

    if (StartService(hService, 0, NULL) == 0)
        printf("Start service failed: %lu\n", GetLastError());
    else
        printf("Service started successfully\n");

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCM);

    return ;
}