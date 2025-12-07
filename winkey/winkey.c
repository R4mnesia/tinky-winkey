#include <stdio.h>
#include <windows.h>

// add this line or add user32.lib on Makefile
//#pragma comment(lib, "user32.lib")

LRESULT CALLBACK hook_proc(int code, WPARAM wParam, LPARAM lParam)
{   
    printf("key wa pressed!\n");
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