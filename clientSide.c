#include<stdio.h>
#include <sys/types.h> 
#include <string.h> 
#include <stdlib.h> 
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")
//gcc clientSide.c -o client.exe -lws2_32
int main(int argc, char *argv[]) { 
    int sd; 
    struct sockaddr_in serveraddr; 
    char buffer[256];

    // Initialisation de Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        printf("Erreur lors de l'initialisation de Winsock");
        exit(1);
    }

    while (1) {
        // Création de la socket
        if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
            printf("Erreur lors de la création de la socket");
            exit(1);
        }

        // Configuration de l'adresse du serveur
        serveraddr.sin_family = AF_INET; 
        serveraddr.sin_port = htons(1500); 
        serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

        // Connexion au serveur
        if (connect(sd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
            printf("Impossible de se connecter au serveur");
            closesocket(sd);
            continue;
        }

        // Communication avec le serveur
        while (1) {
            printf("Message : ");
            fgets(buffer, sizeof(buffer), stdin);

            // Envoi du message au serveur
            if (send(sd, buffer, strlen(buffer), 0) < 0) {
                printf("Erreur lors de l'envoi du message");
                closesocket(sd);
                break;
            }

            // Réception de la réponse du serveur
            if (recv(sd, buffer, sizeof(buffer), 0) < 0) {
                printf("Erreur lors de la réception de la réponse du serveur");
                closesocket(sd);
                break;
            }

            printf("Server : %s", buffer);
        }
    }

    // Fermeture de la socket
    closesocket(sd); 

    // Libération de Winsock
    WSACleanup();
    return 0;
}


