#include "winkey.h"
#include <fileapi.h>

// void    WriteLogs(WCHAR *userInput)
// {
//     // (void*)userInput;
//     wchar_t tempPath[MAX_PATH];

//     GetTempPathW(MAX_PATH, tempPath);
//     wcscat_s(tempPath, MAX_PATH, L"log_tmp.txt");
//     // DBG("PATH TMP: %ls", tempPath);

//     HANDLE  hFile = CreateFileW(
//         tempPath,
//         GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
// 		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL
//     );

//     if (hFile == INVALID_HANDLE_VALUE)
//     {
//         printf("CreateFile error: %lu\n", GetLastError());
//         return ;
//     }
    
//     // set pointer at the end of file for write
//     SetFilePointer(hFile, 0, NULL, FILE_END);

//     char    *log = FormatLogTime();
//     DWORD   written;

//     WriteFile(hFile, log, strlen(log), &written, NULL);
//     // DBG("buffer: %s", userInput);
//     WriteFile(hFile, userInput, wcslen(userInput), &written, NULL);
//     free(log);
//     CloseHandle(hFile);
// }