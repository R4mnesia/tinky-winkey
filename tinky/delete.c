#include "svc.h"

void    delete_svc(char *serviceName)
{
    SC_HANDLE       hService = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    SC_HANDLE       schService = NULL;
    //SERVICE_STATUS  ssStatus; 

    if (!hService)
    {
        printf("Unable to open SCM: %lu\n", GetLastError());
        return ;
    }

    schService = OpenService(
        hService,           // SCM database
        serviceName,
        DELETE              // need delete access
    );
    if (!schService)
    {
        printf("Open service failed: %lu\n", GetLastError());
        CloseServiceHandle(hService);
        return ;
    }

    if (!DeleteService(schService))
    {
        printf("Delete service failed: %lu\n", GetLastError());
    }
    else
        printf("Service deleted successfully\n");

    CloseServiceHandle(schService);
    CloseServiceHandle(hService);

    return ;
}