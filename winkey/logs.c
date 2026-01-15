#include "winkey.h"

void    WriteLogs(DWORD vkCode)
{
    UNREFERENCED_PARAMETER(vkCode);

    HANDLE  hFile = CreateFile(
        "C:\\Users\\larri\\Desktop\\log.txt",
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