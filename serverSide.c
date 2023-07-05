#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")
//gcc serverSide.c -o server.exe -lws2_32
int main(int argc, char *argv[]) {
    WSADATA wsaData;
    SOCKET sd, ns;
    struct sockaddr_in serveraddr, clientaddr;
    char request[256];
    char response[256];
    

    // Initialize Winsock
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    // Create a TCP socket
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == INVALID_SOCKET) {
        printf("socket failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Bind the socket to a specific address and port
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(1500);
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        closesocket(sd);
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    if (listen(sd, SOMAXCONN) == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(sd);
        WSACleanup();
        return 1;
    }

    // Accept a new client connection
    int clientaddrlen = sizeof(clientaddr);
    ns = accept(sd, (struct sockaddr *)&clientaddr, &clientaddrlen);
    if (ns == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(sd);
        WSACleanup();
        return 1;
    }

    printf("Connected to client\n");

    while (true) {
        // Receive data from the client
        memset(request, 0, sizeof(request));
        int nb = recv(ns, request, sizeof(request), 0);
        if (nb <= 0) {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(ns);
            closesocket(sd);
            WSACleanup();
            return 1;
        }

        printf("Received message from client: %s\n", request);

        // Send a response to the client
        printf("server : ");
        fgets(response, sizeof(response), stdin);
        if (send(ns, response, sizeof(response), 0) == SOCKET_ERROR) {
            printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(ns);
            closesocket(sd);
            WSACleanup();
            return 1;
        }
    }

    // Cleanup Winsock
    closesocket(ns);
    closesocket(sd);
    WSACleanup();

    return 0;
}




