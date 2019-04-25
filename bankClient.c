#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "banking.h"
#include <stdlib.h>
#include <unistd.h>
int setupTCPClient(char *servIPAddr, unsigned int portNum)
{
    int clientSocket;
    struct sockaddr_in servAddr;

    /* Setup address of server */
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(servIPAddr);
    servAddr.sin_port = htons(portNum);

    /* Create socket */
    if ((clientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        printf("Failed to create socket; Error %d: %s\n", errno, strerror(errno));
        return -1;
    }
    /* Connect socket to server */
    if (connect(clientSocket, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
        printf("Failed to connect socket to %s:%d; Error %d: %s\n", servIPAddr, portNum, errno, strerror(errno));
        return -1;
    }

    return clientSocket;
}

int main(int argc, char **argv)
{
    int mySocket;
    char serverIP[15];
    unsigned int portNum;
    char recvBuff[1024];

    if (argc != 6)
    {
        printf("Usage: bankClient servIPAddr servPortNum command acctNum value\n");
        return -1;
    }

    /* Setup the IP address */
    strcpy(serverIP, argv[1]);

    /* Setup TCP port number */
    portNum = atoi(argv[2]);
    /* Setup the client socket */
    if ((mySocket = setupTCPClient(serverIP, portNum)) < 0)
    {
        return -1;
    }
    sBANK_PROTOCOL bank;
    bank.trans =  atoi(argv[3]);
    printf("trans %d\n",bank.trans);
    bank.acctnum = atoi(argv[4]);
     printf("acct %d\n",bank.acctnum);
    bank.value = atoi(argv[5]);
     printf("value %d\n",bank.value);
    send(mySocket, &bank, sizeof(bank), 0);
   recv(mySocket,  &bank, sizeof(bank), 0);
    printf("Received trans:\n%d\n",bank.trans);
    printf("recieved acct: \n%d\n", bank.acctnum);
    printf("recieved value: \n%d\n", bank.value);
     printf("Received buff len:\n%ld\n",sizeof(recvBuff));

    close(mySocket);
}
