#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

char trame[5];
SOCKET ss;

void CALLBACK handler(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{ 
    printf("Acquittement perdu... ");
    printf("Rettransmission de la trame: ( %s ) \n", trame);
    send(ss, trame, sizeof(trame), 0);
    printf("Attente d'un nouvel acquittement... \n");
    SetTimer(hwnd, idEvent, 20000, (TIMERPROC)handler); // relance le Handler apres 20 secondes
}

int main(int argc, char *argv[])
{
    WSADATA wsa;
    SOCKET sd, nb;
    struct sockaddr_in serveraddr;

    WSAStartup(MAKEWORD(2, 2), &wsa);

    sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd == INVALID_SOCKET)
        perror("erreur de creation du socket");

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(1500);
    serveraddr.sin_addr.s_addr = inet_addr("192.168.203.10");

    nb = bind(sd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (nb == SOCKET_ERROR)
        perror("erreur d'attachement d'adresse");

    listen(sd, 10);

    ss = accept(sd, 0, 0);

    printf("Saisir la trame a envoyer\n");
    scanf("%s", trame);

    UINT_PTR timerId = SetTimer(NULL, 0, 20000, (TIMERPROC)handler); //Demarrer le timer

    char ack[5];
    do
    {
        printf("***************************Debut de la transmission*****************************\n");
        send(ss, trame, sizeof(trame), 0);
        printf("Attente ACK ou NACK... \n");
        if (recv(ss, ack, sizeof(ack), 0) == SOCKET_ERROR)
            perror("Erreur de reception");

        KillTimer(NULL, timerId); //Annuler le timer si la recuperation de l'ACK est dans le delai

        printf("Reception d'un ack :%s\n", ack);
        if (strcmp(ack, "0000") == 0)
        {
            printf("\n acquittement negatif rettransmission de la trame : %s\n", trame);
        }

        timerId = SetTimer(NULL, 0, 20000, (TIMERPROC)handler); //Redemarrer le timer
    } while (strcmp(ack, "0000") == 0);
    printf("***************************Fin de la transmission*******************************");

    closesocket(ss);
    closesocket(sd);
    WSACleanup();

    return 0;

}
