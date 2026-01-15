#include "svc.h"

/*
    typedef struct _SERVICE_STATUS_PROCESS {
      DWORD dwServiceType;
      DWORD dwCurrentState;
      DWORD dwControlsAccepted;
      DWORD dwWin32ExitCode;
      DWORD dwServiceSpecificExitCode;
      DWORD dwCheckPoint;
      DWORD dwWaitHint;
      DWORD dwProcessId;
      DWORD dwServiceFlags;
    } SERVICE_STATUS_PROCESS, *LPSERVICE_STATUS_PROCESS;
*/

void    stop_svc(void)
{
    KillWinkeyPID();
    SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!hSCM)
    {
        printf("Unable to open SCM: %lu\n", GetLastError());
        return ;
    }

    SC_HANDLE hService = OpenService(
        hSCM,
        SERVICE_NAME,
        SERVICE_STOP | SERVICE_QUERY_STATUS
    );
    if (!hService)
    {
        printf("Open service failed: %lu\n", GetLastError());
        CloseServiceHandle(hSCM);
        return ;
    }

    if (is_service_running())
    {
        /*
            SERVICE_STATUS_PROCESS -> struct to store state of service
            bytesNeeded -> not useful in the function but mandatory for QueryServiceStatusEx()
        */
        SERVICE_STATUS_PROCESS ssp; 
        DWORD bytesNeeded;

        if (ControlService(hService, SERVICE_CONTROL_STOP, (LPSERVICE_STATUS)&ssp))
        {
            // waiting service
            printf("Stopping service...\n");

            /*
            
                BOOL QueryServiceStatusEx(
                  [in]            SC_HANDLE      hService,
                  [in]            SC_STATUS_TYPE InfoLevel,
                  [out, optional] LPBYTE         lpBuffer,
                  [in]            DWORD          cbBufSize,
                  [out]           LPDWORD        pcbBytesNeeded
                );

            */

            while (QueryServiceStatusEx(
                    hService,
                    SC_STATUS_PROCESS_INFO,
                    (LPBYTE)&ssp,
                    sizeof(SERVICE_STATUS_PROCESS),
                    &bytesNeeded))
            {
                if (ssp.dwCurrentState == SERVICE_STOPPED)
                {
                    printf("Service stopped successfully.\n");
                    break ;
                }
                Sleep(500);
            }
        }
        else
            printf("Failed to stop service: %lu\n", GetLastError());
    }
    else
        printf("Service is not running\n");

    return ;
}