#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    char psBuffer[128];
    FILE* pPipe;

    /* Run DIR so that it writes its output to a pipe. Open this
     * pipe with read text attribute so that we can read it
     * like a text file.
     */
    char *pwshell = "powershell.exe -NoProfile -NonInteractive -ExecutionPolicy Bypass -Command ";
    char *str = malloc(sizeof(char) * strlen(pwshell) + strlen(argv[1]) + 3);
    strcpy(str, pwshell);
    strcat(str, "\"");
    strcat(str, argv[1]);
    strcat(str, "\"");
    printf("str = %s\n", str);
    if ((pPipe = _popen(str, "r")) == NULL)
        exit(1);
    free(str);
    //if ((pPipe = _popen("powershell.exe -NoProfile -NonInteractive -ExecutionPolicy Bypass -Command \"ls\" 2>&1", "r")) == NULL)
    //    exit(1);

    /* Read pipe until end of file, or an error occurs. */
    char output[4096] = {0};

    while (fgets(psBuffer, sizeof(psBuffer), pPipe)) {
        strcat(output, psBuffer);
    }
    printf("output: %s", output);
    int endOfFileVal = feof(pPipe);
    int closeReturnVal = _pclose(pPipe);

    if (endOfFileVal)
        printf("\nProcess returned %d\n", closeReturnVal);
    else
        printf("Error: Failed to read the pipe to the end.\n");
}