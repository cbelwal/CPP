		// Defintion of PING Header

class ping
{
private:

	
	SOCKET sockConn;
	sockaddr_in *dsDest;
	WSAData wsadata;
	ICMP_HEADER FAR *icmp_header;			   // Struct
	u_short usRequestID, usSequenceID;
	char strIOBuf[BUFFER_SIZE];
	clock_t timeSendPacket, timeReceivePacket; // Defined in time.h
	float fTimeOut;							   // Timeout in milliseconds


public:

	char *strDest,*ip_addr;			// Destination IP Address or domain name
	char *strFQDN;                  // Store FQDN for public access
	ping(char *,float);					// Constructor
	~ping();						// Destructor

	int Initialize();
	int SendICMP();
	float GetICMP();
	u_short GenerateCheckSum(u_short FAR*, int);
	long GetSize(char *);
};


ping::ping(char *str,float fTime)
{
	strDest = (char *)calloc(GetSize(str) / sizeof(char) ,sizeof(char));
	strcpy(strDest,str); // str is memory ptr outside object perview
	
			// Initialize Variables
	dsDest = (sockaddr_in *)malloc(sizeof(sockaddr_in));
	icmp_header = (ICMP_HEADER FAR *) malloc (sizeof(ICMP_HEADER));
	usRequestID = 1;
	usSequenceID = 1;

	fTimeOut = fTime;

}


ping::~ping()
{
	
			// FREE ALL RESOURCES
	closesocket(sockConn);
	WSACleanup();
}


int ping::Initialize()
{

	int iStatus;
	iStatus = WSAStartup(0x202,&wsadata);	
	
	if(iStatus != 0)
	{	printf("Error in TCP/IP stack DLL. Reinstall\n");
			return -2; // Error
	}

	sockConn = socket(AF_INET,SOCK_RAW,IPPROTO_ICMP);
		if (sockConn == SOCKET_ERROR)
	{	
			return -3; // Error	
	}


	// GET IP ADDRESS OF HOST NAME
		HOSTENT *hostptr;
		IN_ADDR addr;
		
	// Check is IP Address is given
	 
	if (inet_addr(strDest) == INADDR_NONE) // Host Name Given
	{
		
		hostptr=gethostbyname(strDest);
		if(hostptr == NULL)
			return -1;  //Illegal host name
		memcpy(&addr, hostptr->h_addr_list[0],sizeof(IN_ADDR));
		ip_addr = (char *) malloc (GetSize(inet_ntoa(addr)));
			strcpy(ip_addr,inet_ntoa(addr));
		
	}
	else
	{
		ip_addr = (char *) malloc (GetSize(strDest));
			strcpy(ip_addr,strDest);

		
		addr.S_un.S_addr = inet_addr(strDest);
			
		hostptr=gethostbyaddr((char *)&addr,sizeof(in_addr),AF_INET);
		if(hostptr == NULL)	
			{strFQDN = (char *) malloc(GetSize("Unknown"));
			strcpy(strFQDN,"Unknown");}
		
		
	}
	
	if(hostptr != NULL)
		{GetSize(hostptr->h_name);
		strFQDN = (char *) malloc(GetSize(hostptr->h_name));
		strcpy(strFQDN,hostptr->h_name);}




	dsDest->sin_addr.S_un.S_addr=inet_addr(ip_addr);
	dsDest->sin_family=AF_INET;
	dsDest->sin_port = 0;
	

	return 1;
}

int ping::SendICMP()
{
	
	// Will form ICMP Header first and then print
	int iStatus=0;
	icmp_header->icmp_type=ICMP_ECHO_REQ;
	icmp_header->icmp_code=0;
	icmp_header->icmp_cksum=0;
	icmp_header->icmp_id=usRequestID++;
	icmp_header->icmp_seq = usSequenceID++;
	icmp_header->icmp_data[0]='c';
	
	icmp_header->icmp_cksum = GenerateCheckSum((u_short FAR *) icmp_header, ICMP_HEADER_SIZE);
	timeSendPacket = clock();
	iStatus = sendto(sockConn, (LPSTR)icmp_header,ICMP_HEADER_SIZE ,0,(LPSOCKADDR)dsDest, sizeof(sockaddr_in));
		return iStatus;
}

float ping::GetICMP()
{
	int iStatus=0;
	float iTime;
	u_long lOnOff=true; // for ioctlsocket
	int iSize = sizeof(sockaddr_in);
	
			// Make the socket non blocking

	iStatus = ioctlsocket(sockConn, FIONBIO, (u_long FAR *)&lOnOff);
	if (iStatus == SOCKET_ERROR)
		return -2; //SOCKET ERROR
	
	while( (float)(clock()-timeSendPacket) * (1000/CLOCKS_PER_SEC) < fTimeOut)
	{
		iStatus = recvfrom(sockConn, strIOBuf,ICMP_HEADER_SIZE + IP_HEADER_SIZE,0,(LPSOCKADDR) dsDest, &iSize);
			timeReceivePacket = clock();
		if (iStatus > 0) //Data Received
		{
			if(strIOBuf[0] == 0x45 ) // IP header present
				icmp_header = (ICMP_HEADER FAR *) &strIOBuf[IP_HEADER_SIZE];
			else
				icmp_header = (ICMP_HEADER FAR *) strIOBuf;

			if (icmp_header->icmp_id == usRequestID-1)	// Our Packet
					{if (timeReceivePacket == timeSendPacket ) // Very Fast
						return 1;

					iTime= ((float)(timeReceivePacket - timeSendPacket)) * ( 1000 / CLOCKS_PER_SEC); // Time in Miliseconds
					return iTime;}
		}
		
	}
	return -1;
}

	// SUPPORT ROUTINES

long ping::GetSize(char *str)
{
	long lSize=0;
	while(str[lSize++] != '\0');
		return (lSize-1);
}


u_short ping::GenerateCheckSum(u_short FAR *lpBuf, int iLen)
{
	// Checksum -> Add all the values in buffer as a 16 bit words.
	// Do a ones complment of them. Definiton in RFC 792 mite be wrong
	
	u_short lSum = 0;

	while (iLen > 0)
		{lSum += *(lpBuf++);
		iLen -=2;} // Reduce 2 bytes
	if(iLen) // if ILen odd
		lSum += *lpBuf;
	
	return(~lSum);
}




