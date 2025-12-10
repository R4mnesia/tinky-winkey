#include "winkey.h"

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

char    *GetActiveWindowTitle(void)
{
    HWND    hwnd = NULL;
    int     size = 0;
    char    *title = NULL;


    hwnd = GetForegroundWindow();
    if (!hwnd)
    {
        printf("No active window\n");
        return (NULL);
    }

    size = GetWindowTextLengthA(hwnd) + 1;
    title = malloc(size);
    if (!title)
        return (NULL);

    GetWindowTextA(hwnd, title, size);

    return (title);
}

char    *FormatLogTime(void)
{
    char    *time = GetMyLocalTime();
    char    *windowTitle = GetActiveWindowTitle();

    size_t  size = strlen((const char *)time) + strlen((const char *)windowTitle) + 6;
    char    *log = malloc(size + 1);
    
    snprintf(log, size + 1, "%s - '%s'\n", time, windowTitle);

    return (log);
}