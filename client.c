#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <time.h>

#pragma comment(lib, "ws2_32.lib")

int main(int argc, char *argv[]) {
    WSADATA wsa;
    SOCKET sd;
    struct sockaddr_in serveraddr;

    int random1;
    int random2;
    char trame[5];
    char ack[5];

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Erreur d'initialisation de WinSock\n");
        return 1;
    }

    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Erreur de creation du socket\n");
        return 1;
    }

    serveraddr.sin_family = AF_INET;
serveraddr.sin_port = htons(1500);
serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR) {
        printf("Erreur de connexion\n");
        closesocket(sd);
        WSACleanup();
        return 1;
    }

    srand(time(0));

    do {
        printf("En Attente d'une Trame...  \n");
        recv(sd, trame, sizeof(trame), 0);
        printf("Nouvelle trame recue : %s\n", trame);
        random1 = rand() % 2;
        if (random1 == 1) {
            printf("Acquittement a envoyer ACK est 1111.....\n");
            strcpy(ack, "1111");
        }
        if (random1 == 0) {
            printf("Acquittement a envoyer NACK est 0000....\n");
            strcpy(ack, "0000");
        }
        random2 = rand() % 2;
        if (random2 == 1) {
            printf("Acquittement envoye avec succes\n");
            send(sd, ack, sizeof(ack), 0);
        } else
            printf("Erreur de transmission d'ack...Attente d'une retransmission de la trame\n");
    } while (random2 == 0);

    closesocket(sd);
    WSACleanup();
    return 0;
}
