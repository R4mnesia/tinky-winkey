#include "svc.h"

int     is_service_running(char *serviceName)
{
    SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!hSCM)
    {
        printf("OpenSCManager failed: %lu\n", GetLastError());
        return (-1);
    }

    SC_HANDLE hService = OpenService(hSCM, serviceName, SERVICE_QUERY_STATUS);
    if (!hService)
    {
        printf("Open service failed: %lu\n", GetLastError());
        CloseServiceHandle(hSCM);
        return (-1);
    }

    SERVICE_STATUS_PROCESS ssp;
    DWORD bytesNeeded = 0;

    if (!QueryServiceStatusEx(
            hService,                       // service handle
            SC_STATUS_PROCESS_INFO,         // request service status info
            (LPBYTE)&ssp,                   // buffer that receive status
            sizeof(SERVICE_STATUS_PROCESS), // buffer size
            &bytesNeeded                    // number of bytes returned
    ))
    {
        printf("QueryServiceStatusEx failed: %lu\n", GetLastError());
        CloseServiceHandle(hService);
        CloseServiceHandle(hSCM);
        return (-1);
    }

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCM);

    if (ssp.dwCurrentState == SERVICE_RUNNING)
        return (1);
    return (0);
}