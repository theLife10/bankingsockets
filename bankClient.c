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
     printf("\n///////////////////////////////////////////////////\n");
    printf("\nHere are the instructions: \n");
     printf("\n///////////////////////////////////////////////////\n");
    printf("\n IPAdress will be 129.108.32.2 26207  \n");
    printf("\n Port number will be 26207  \n");
     printf("\n///////////////////////////////////////////////////\n");
    printf("\nB is for balance\n");
    printf("\nW is for withdraw\n");
    printf("\nD is for deposit\n");
    printf("\n///////////////////////////////////////////////////\n");
    printf("\n Enter the Account number you want to DEPOSIT/WITHDRAW/CHECK BALANCE FROM \n");
    printf("\n Enter the amount you want to deposit/withdraw\n");
   printf("\n///////////////////////////////////////////////////\n");
    
    //initalizing the bank protocol
   sBANK_PROTOCOL bank;
   //checks transaction is a deposit
   if(*argv[3] == 'D'){
       bank.trans = BANK_TRANS_DEPOSIT; //assigns the transaction to be Deposit
       bank.acctnum = atoi(argv[4]); // assigns the account number
       bank.value =  atoi(argv[5]); // assigns the amount of the deposit

       send(mySocket, &bank, sizeof(bank), 0); // sends socket, bank protocol, sizeof bankprotocol,def:0
       recv(mySocket,  &bank, sizeof(bank), 0); // recieves socket, bank protocol, size of bank, def:0

       printf("\nTransaction: Deposit\n");
       printf("\n You are depositing in account number: %d \n",bank.acctnum);
       printf("\n You are depositing: %d cents \n",bank.value);
   }
     //check if arguement is w = withdraw 
     if(*argv[3] == 'W'){
        bank.trans =  BANK_TRANS_WITHDRAW; // assigns transaction to be withdraw
         bank.acctnum = atoi(argv[4]); // assigns the account number 
          bank.value =  atoi(argv[5]); // value you want to withdraw
         send(mySocket, &bank, sizeof(bank), 0); // sends the socket, size, def:0
       recv(mySocket,  &bank, sizeof(bank), 0); // recieves the socket, bank protocol, size, def:0
       printf("\nTransaction:Withdrawing\n");
        printf("\n You are withdrawing in account number: %d \n",bank.acctnum);
         printf("\n You are withdrawing: %d cents \n",bank.value);
      }
        //checks if arguement is balance
       if(*argv[3] == 'B'){
         bank.trans =  BANK_TRANS_INQUIRY; //assigns the transaction to be bank inquiry   
         bank.acctnum = atoi(argv[4]); // argument assigns the account number
         send(mySocket, &bank, sizeof(bank), 0); //sends the socket, bank protocol, size of protocol, def:0
         recv(mySocket,  &bank, sizeof(bank), 0); //recieves socket, bank protocol, size of protocol, def: 0
         printf("\n Transaction:Checking Balance\n");
         printf("\n Your current balance in account number: %d is %d cents \n",bank.acctnum,bank.value);
       }
    close(mySocket);
}
