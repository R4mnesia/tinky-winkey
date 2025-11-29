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





















































BOOL start_service(const char *serviceName)
{
    SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!hSCM)
    {
        printf("OpenSCManager failed: %lu\n", GetLastError());
        return FALSE;
    }

    SC_HANDLE hService = OpenService(
        hSCM,
        serviceName,
        SERVICE_START | SERVICE_QUERY_STATUS
    );

    if (!hService)
    {
        printf("OpenService failed: %lu\n", GetLastError());
        CloseServiceHandle(hSCM);
        return FALSE;
    }

    SERVICE_STATUS_PROCESS ssp;
    DWORD bytesNeeded;

    // Get the initial status
    if (!QueryServiceStatusEx(
            hService,
            SC_STATUS_PROCESS_INFO,
            (LPBYTE)&ssp,
            sizeof(SERVICE_STATUS_PROCESS),
            &bytesNeeded))
    {
        printf("QueryServiceStatusEx failed: %lu\n", GetLastError());
        CloseServiceHandle(hService);
        CloseServiceHandle(hSCM);
        return FALSE;
    }

    // If already running, just exit
    if (ssp.dwCurrentState == SERVICE_RUNNING)
    {
        printf("Service is already running.\n");
        CloseServiceHandle(hService);
        CloseServiceHandle(hSCM);
        return TRUE;
    }

    // Try to start the service
    if (!StartService(hService, 0, NULL))
    {
        DWORD err = GetLastError();
        if (err == ERROR_SERVICE_ALREADY_RUNNING)
        {
            printf("Service is already running.\n");
            CloseServiceHandle(hService);
            CloseServiceHandle(hSCM);
            return TRUE;
        }

        printf("StartService failed: %lu\n", err);
        CloseServiceHandle(hService);
        CloseServiceHandle(hSCM);
        return FALSE;
    }

    printf("Starting service...\n");

    // Wait until the service is fully running
    do {
        Sleep(500);

        if (!QueryServiceStatusEx(
                hService,
                SC_STATUS_PROCESS_INFO,
                (LPBYTE)&ssp,
                sizeof(SERVICE_STATUS_PROCESS),
                &bytesNeeded))
        {
            printf("QueryServiceStatusEx failed: %lu\n", GetLastError());
            CloseServiceHandle(hService);
            CloseServiceHandle(hSCM);
            return FALSE;
        }

    } while (ssp.dwCurrentState == SERVICE_START_PENDING);

    if (ssp.dwCurrentState == SERVICE_RUNNING)
    {
        printf("Service started successfully.\n");
        CloseServiceHandle(hService);
        CloseServiceHandle(hSCM);
        return TRUE;
    }
    else
    {
        printf("Service failed to start. Current state: %lu\n", ssp.dwCurrentState);
        CloseServiceHandle(hService);
        CloseServiceHandle(hSCM);
        return FALSE;
    }
}
