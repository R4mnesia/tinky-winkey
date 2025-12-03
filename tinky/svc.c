#include "svc.h"

/*

typedef struct _SERVICE_STATUS {
  DWORD dwServiceType;  The type of service. This member can be one of the following values.
  DWORD dwCurrentState; The current state of the service. This member can be one of the following values.
  DWORD dwControlsAccepted; The control codes the service accepts and processes in its handler function (see Handler and HandlerEx). 
  DWORD dwWin32ExitCode; he error code the service uses to report an error that occurs when it is starting or stopping. 
  DWORD dwServiceSpecificExitCode; A service-specific error code that the service returns when an error occurs while the service is starting or stopping.
  DWORD dwCheckPoint; The check-point value the service increments periodically to report its progress during a lengthy start, stop, pause, or continue operation. 
  DWORD dwWaitHint; The estimated time required for a pending start, stop, pause, or continue operation, in milliseconds.
} SERVICE_STATUS, *LPSERVICE_STATUS;

*/

static DWORD WINAPI ServiceCtrlHandlerEx(DWORD ctrl, DWORD eventType, LPVOID eventData, LPVOID context)
{
    UNREFERENCED_PARAMETER(eventType);
    UNREFERENCED_PARAMETER(eventData);
    
    t_svc_ctx *ctx = (t_svc_ctx*)context;

    if (ctrl == SERVICE_CONTROL_STOP)
    {
        ctx->status.dwCurrentState = SERVICE_STOP_PENDING;
        if (!SetServiceStatus(ctx->hStatus, &ctx->status))
            printf("Set service status failed: %lu", GetLastError());
        if (!SetEvent(ctx->hStopEvent)) // sig stop
            printf("Set event failed: %lu", GetLastError());
    }
    return (0);
}

static DWORD   GetWinLogonPid(DWORD sessionID)
{
    UNREFERENCED_PARAMETER(sessionID);


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
            if (_stricmp(pe.szExeFile, "winlogon.exe") == 0)
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

// session 0 -> not interactive
// session 1 -> interactive
// start service SYSTEM privilege (session 0)
void WINAPI ServiceMain(DWORD argc, LPTSTR *argv)
{
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    t_svc_ctx   ctx;
    ZeroMemory(&ctx, sizeof(ctx));

    ctx.status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    ctx.status.dwCurrentState = SERVICE_START_PENDING;

    ctx.hStatus = RegisterServiceCtrlHandlerExA(
        SERVICE_NAME,
        ServiceCtrlHandlerEx,
        &ctx
    );
    if (!ctx.hStatus)
    {
        printf("Register Service handler failed: %lu\n", GetLastError());
        return ;
    }

    ctx.status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    ctx.status.dwCurrentState = SERVICE_RUNNING;

    // notifies the SCM of the current status of service
    if (!SetServiceStatus(ctx.hStatus, &ctx.status))
        printf("Set service status failed: %lu", GetLastError());
    
    // duplicate SYSTEM token (session 1)
    
    DWORD   sessionID = WTSGetActiveConsoleSessionId(); // session 0
    DWORD   pid = GetWinLogonPid(sessionID);    
    
    HANDLE hProcess = OpenProcess(
        PROCESS_QUERY_INFORMATION, 
        FALSE, 
        pid);

    if (!hProcess)
    {
        printf("OpenProcess failed: %lu\n", GetLastError());
        return ;
    }

    // open token winlogon
    HANDLE hToken = NULL;
    if (!OpenProcessToken(
        hProcess, 
        TOKEN_DUPLICATE | TOKEN_QUERY, 
        &hToken))
    {
        printf("OpenProcessToken failed: %lu\n", GetLastError());
        return ;
    }

    HANDLE hNewToken = NULL;
    if (!DuplicateTokenEx(
        hToken,
        TOKEN_ALL_ACCESS,
        NULL,
        SecurityImpersonation,
        TokenPrimary,
        &hNewToken
    ))
    {
        printf("DuplicateTokenEx failed: %lu\n", GetLastError());
        return ;
    }

    // for test
    /*STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    if (CreateProcessAsUser(
            hNewToken,
            "C:\\Windows\\System32\\notepad.exe",
            NULL,
            NULL,
            NULL,
            FALSE,
            0,
            NULL,
            NULL,
            &si,
            &pi))
    {
        printf("Process created!\n");
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }*/

    // function for start programme winkey
}
