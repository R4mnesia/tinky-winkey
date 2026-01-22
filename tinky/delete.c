#include <svc.h>

void    delete_svc(void)
{
    SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    //SERVICE_STATUS  ssStatus; 

    if (!hSCM)
    {
        printf("Unable to open SCM: %lu\n", GetLastError());
        return ;
    }

    SC_HANDLE hService = OpenService(
        hSCM,               // SCM database
        SERVICE_NAME,
        DELETE              // need delete access
    );
    if (!hService)
    {
        printf("Open service failed: %lu\n", GetLastError());
        CloseServiceHandle(hSCM);
        return ;
    }

    if (is_service_running() == 0)
    {
        if (!DeleteService(hService))
        {
            printf("Delete service failed: %lu\n", GetLastError());
        }
        else
            printf("Service deleted successfully\n");
    }
    
    CloseServiceHandle(hService);
    CloseServiceHandle(hSCM);

    return ;
}