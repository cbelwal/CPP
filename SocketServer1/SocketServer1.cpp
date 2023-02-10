// SocketServer1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <malloc.h>
#include <winsock.h>
#include <stdlib.h>


void DisplayMessage(void);

void main(int argc, char* argv[])
{
	
	// Check Params
  
	
   if (argc != 3) // No param specified or too many
   { printf("Wrong number of arguements\n");
     DisplayMessage();
     exit(0);
   }

   if (atoi(argv[1]) <= 0 || atoi(argv[2]) <= 0)
   { printf("Incorrect arguement values\n");
     DisplayMessage();
     exit(0);
   }

	
   // Inititalize Sockets

	WSAData wsadata;
	WSAStartup(0x101,&wsadata);
   
   
   //Initial Declarations

	FILE *fp;
	
    int iResult,iPort,iBytes,iBuffersize;
	

    char send_buffer[100]="Welcome to Generic Server Version 1.2 on ";
	char *buffer;
	
	iBytes=0;
	
	iPort=atoi(argv[1]);
    iBuffersize = atoi(argv[2]);
	HOSTENT *hostptr;

	//Get Host Information

	char strVal[256]="";
	
	gethostname(strVal,sizeof(strVal));
	hostptr=gethostbyname(strVal);
	
	if (hostptr==NULL)
	{printf(". Re install winsock components\n");
	exit(0);}
	
	// Add messages

	strcat(send_buffer,hostptr->h_name);
	strcat(send_buffer,"\n\0");

	
	// File Handling

	
	fp=fopen("gserver.log","w");

	

    buffer=(char *)malloc(iBuffersize);
	if (buffer==NULL)
	{printf("Insufficient memory to create buffer\n");
	exit(0);}
	
	//Empty Buffer
	for(iResult=0;iResult<=iBuffersize-1;iResult++)
		buffer[iResult]='\0';

	

	// Declare structures
	
	
	SOCKET srvSocket;
	SOCKET srvSocket1;
    sockaddr_in sockAddr;
    sockaddr_in sockAddr1;
	
	



	 // Create Socket
	printf("Welcome to Generic Server Version 1.2 ( Created 22/10/00 ) on %s\n", hostptr->h_name);
		fprintf(fp,"Welcome to Generic Server Version 1.2 ( Created 22/10/00 ) on %s\n", hostptr->h_name);
	srvSocket = socket(AF_INET,SOCK_STREAM,0);
    if (srvSocket == INVALID_SOCKET)
	{printf("Error = %d ! You might not have winsock installed\n", WSAGetLastError());
	exit(0);}

	else
		printf("Socket Created\n");

	  // Define structure
    
	

	sockAddr.sin_family=AF_INET;
    sockAddr.sin_addr.S_un.S_addr=inet_addr("\0");
    sockAddr.sin_port=htons(iPort);
	
	    // Initiate Server
   
//	printf("sin_port =  %d\n", sockAddr.sin_port ); 
// Dont specify IP Addr in Server


	const sockaddr *sockptr = (LPSOCKADDR) &sockAddr;

	iResult = bind(srvSocket,sockptr,sizeof(sockAddr));
	//printf("Last Error after bind %d\n", WSAGetLastError());


	if (iResult != 0) 
	{printf("Error ! Port might be in use\n");
	exit(0);}     

	listen(srvSocket,2);
	printf("%s ready to listen on port %d maximum byte count = %d\n",hostptr->h_name,iPort,iBuffersize);
	fprintf(fp,"%s ready to listen on port %d maximum byte count = %d\n",hostptr->h_name,iPort,iBuffersize);
	
	//printf("Last Error after listen %d\n", WSAGetLastError());

    int p = sizeof(sockAddr1);

	
                  
	srvSocket1 = accept(srvSocket,(LPSOCKADDR)&sockAddr1, &p);
	printf("Connection established\n");
	fprintf(fp,"Connection established\n");
	
  send(srvSocket1,(LPSTR) &send_buffer,sizeof(send_buffer),1);
   
	iResult=1;
	while (iResult >0)

{

	iResult = recv(srvSocket1,buffer,iBuffersize,0);
	if (iResult  >0)
		iBytes=iBytes+iResult;
	printf("%s",buffer);
	fprintf(fp,"%s",buffer);
	//printf("iReult = %d",iResult); 
	strcpy(buffer,"");
}

	// Print on file and Disl

    printf("\nConnection closed\n");
	fprintf(fp,"\nConnection closed\n");

	printf("--------------\n%d bytes received from client\n-------------\n",iBytes);
    fprintf(fp,"--------------\n%d bytes received from client\n-------------\n",iBytes);
	printf("%s\n",buffer);
    fclose(fp);    
}



void DisplayMessage(void)
{
	printf("\nUsage:\nGSERVER <PORT> <BUFFER_SIZE>\n\n");
	printf("Eg. gserver 30 100\n");
	printf("-------------------------------------\n\n");
	printf("Output will be saved in gserver.log in directory where you run gserver\n\n");
	printf("COMMENTS et al: Chaitanya Belwal, cbelwal@hotmail.com\n");
}
