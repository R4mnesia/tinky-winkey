#pragma once

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
#define MAX_CLIENTS 64

DWORD WINAPI remote_shell(void* args);