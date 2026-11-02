#include <xtl.h>
#include <winsockx.h>

#define SERVER_IP "192.168.100.106" 
#define SERVER_PORT 7331

void ConnectToMobileServer() {
    XNetStartupParams xnsp;
    memset(&xnsp, 0, sizeof(xnsp));
    xnsp.cfgSizeOfStruct = sizeof(XNetStartupParams);
    xnsp.cfgFlagsXNet = XNET_STARTUP_BYPASS_SECURITY;
    XNetStartup(&xnsp);

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) return;

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != SOCKET_ERROR) {
        char* msg = "Hello from Xbox 360 RGH!";
        send(sock, msg, strlen(msg), 0);
    }

    closesocket(sock);
    WSACleanup();
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        ExCreateThread(NULL, 0, NULL, NULL, (PTHREAD_START_ROUTINE)ConnectToMobileServer, NULL, 0);
    }
    return TRUE;
}
