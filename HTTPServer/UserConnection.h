// Class definition of User Connection - Will handle all user requests. 
// every instance will be created in a different thread

class UserConnection
{
private:
	struct List
	{
		char *str;
		List *link_ptr;
	};

	SOCKET srvSocket1;


public:
	UserConnection(void *); // Constructor
	~UserConnection(); // Destructor
	
		// Support Functions

	void ConnectionRole();
	int CheckString(char *, char *);
	char * LoadFile(char *); // Load File into a variable
	char * ExtractAttribute ( char *, int); // Eg. to get XXX in GET XXX \r\n
	char * CStylePath(char *);
	void LogFunc(char *,int); // For Logging in Log File
	long GetSize(char *); // To get size of dynamically allocated strings 
						  // _size gives screwing problems.Checked in MSDN too
};

	// Function Declarations

UserConnection::UserConnection(void *srvSock)
{
	srvSocket1 = (SOCKET) srvSock;
	ConnectionRole(); 
}

UserConnection::~UserConnection()
{
	printf("Object is dead\n");
	closesocket(srvSocket1);
}


//NOTE: Due to debug problems in _msize and malloc() 
// We are going to use arrays for size of path var.

void UserConnection::ConnectionRole()
{
	
	// ENTRY into Class. Called by constructor

	int iBytes=0,iResult,pathpos,iHeaderSize;
	char *send_buffer,*strFileBuffer; //Assuming Max size of path = 100

	//--------Initiate Receive Buffer

	char *buffer,tmpbuffer[2],prevbuffer[2]; //prevbuffer[]'\0;
	char *path;

	send_buffer = (char *)malloc(sizeof(char) * 30);	
				printf("#1 %u\n",_msize(send_buffer)); // TEMP


	buffer=(char *)malloc(MAX_RECEIVE_SIZE); // Max size of main buffer
	if (buffer==NULL)
		{printf("Insufficient memory to create buffer\n");
		exit(0);}
	
	for(iResult=0;iResult<=MAX_RECEIVE_SIZE-1;iResult++)
		buffer[iResult]='\0';

	iResult=1;	
	tmpbuffer[0]='\0';
			// Some changes required in receive buffer for web server
	
	
	do
	{
		iResult = recv(srvSocket1,tmpbuffer,1,0);
		tmpbuffer[1]='\0';
		strcat(buffer,tmpbuffer);

		if(iResult >0)
		{
			iBytes=iBytes + iResult;
			


		// WEB SERVER request end in XXX \\n and \r\n ( again )
		if ( !(strcmp(tmpbuffer,"\r")) && !(strcmp(prevbuffer,"\n")) ) // To identify headers as they are separted lile \r\n \r\n 
		{	
			if(pathpos=CheckString(buffer,"GET"))
			{
				LogFunc("GET Request",sizeof("GET Request"));
					
					// OUTPUT BUFFER FORMATION
				//iHeaderSize = sizeof("HTTP/1.1 200 OK\r\nServer: BeluBhai w3c\r\nDate: Fri, 06 Dec 2000 10:18:48 GMT\r\nContent-Type: text/html\r\nContent-Length: 6\r\n\r\n");
				iHeaderSize = sizeof("HTTP/1.1 200 OK\r\n\r\n"); //TEMP
				
				path=(char *)malloc(GetSize(ExtractAttribute(buffer,pathpos))); // ASSUMPTION: Path size = 100
				path = ExtractAttribute(buffer,pathpos);

				send_buffer = (char *)malloc(sizeof(char) * iHeaderSize);			
				send_buffer[0]='\0';
				
				
				//strcpy(send_buffer,"HTTP/1.1 200 OK\r\nServer: BeluBhai w3c\r\nDate: Fri, 06 Dec 2000 10:18:48 GMT\r\nContent-Type: text/html\r\nContent-Length: 6\r\n\r\n");
				strcpy(send_buffer,"HTTP/1.1 200 OK\r\n\r\n");  //TEMP
				
//				path=ExtractAttribute(buffer,pathpos);
				strFileBuffer = (char *)malloc(_msize(LoadFile(path)));
				strFileBuffer = LoadFile(path);
				
				printf("#before Send_buffer = %u File Buffer = %u Header = %d\n",GetSize(send_buffer),GetSize(strFileBuffer),iHeaderSize);
				send_buffer = (char *)realloc(send_buffer,iHeaderSize + GetSize(strFileBuffer) + 3);
				printf("#After Send_buffer = %u File Buffer = %u Header = %d\n",GetSize(send_buffer),GetSize(strFileBuffer),iHeaderSize);
				
				if(send_buffer !=NULL)
				{
					// Assign each char of strfilebuffer to send_buffer beacuse strcat giving problems
					for(int iLoop = iHeaderSize-1;iLoop< iHeaderSize + GetSize(strFileBuffer)-1;iLoop++)
						send_buffer[iLoop] = strFileBuffer[iLoop - (iHeaderSize-1)];
					
					printf("%d %u",iLoop,_msize(send_buffer));

					send_buffer[++iLoop] = '\r';

					send_buffer[++iLoop] = '\n';
					send_buffer[++iLoop] = '\0';			
							for(int i =0;i<_msize(send_buffer);i++)
								printf("%c  = %d\n",send_buffer[i],i);

						// END TEMP
			
					send(srvSocket1,(LPSTR) send_buffer,_msize(send_buffer),0);  
				}
			
					// Empty all buffers
					free(buffer);
				

					// Close Socket for receive
			//	shutdown(srvSocket1,SD_RECEIVE);
		//		free(send_buffer);
			//	LogFunc("Request ACK Data Transferred",sizeof("Request ACK Data Transferred"));
			}
		}
			strcpy(prevbuffer,tmpbuffer); //TEMP
		}
	}while (iResult > 0);

	free(buffer);
	// Send data after in coming data stops from browser		
}


int UserConnection::CheckString(char *strMain, char *strSearch)
{
		//Define string
   
	unsigned int iLoop,iCount = 0;
	char *temp,tempno=0,iAbspos=0,*tempcheck; // temp is used to store words before \r\n

		
		// Initialize

	temp = (char *)malloc(sizeof(char) * 1000);
	for(iLoop=0;iLoop<=_msize(temp)-1;iLoop++)
		temp[iLoop]='\0';

		//Define Link List

	
	List *mainptr,*tmpptr;
    

	mainptr=(List *)malloc(sizeof(List));	
	tmpptr = mainptr;
	
		// We are going to store every string in an array
		// Store in File
	
	for(iLoop=0;iLoop <= (strlen(strMain)-1);iLoop++)
	{
		while (strMain[iLoop] != '\r' &&  strMain[iLoop+1] != '\n')
		{
	
			if(islower(strMain[iLoop])) //_toupper is an asshole
				temp[tempno++] = _toupper(strMain[iLoop++]);
			else
				temp[tempno++] = strMain[iLoop++];

		}
	
		iLoop++;

		temp[tempno] = '\0';
			//Store in Linked List
		tmpptr->str = (char *) malloc(_msize(temp));	
		strcpy(tmpptr->str,temp);

		tmpptr->link_ptr=(List *) malloc(sizeof(List));
		tmpptr = tmpptr->link_ptr;

		tempno = 0;
		iCount++;
	}

		// Print the list --> Temp
		//	tmpptr = mainptr;
		//	for(iLoop=0;iLoop<=iCount-1;iLoop++)
		//	{
		//		printf("Word is %s\n",tmpptr->str);
		//		tmpptr=tmpptr->link_ptr;
		//	}
		//-- END

	// Now do search
	
	tmpptr = mainptr;
	printf("-----------\n");

	tempcheck=(char *)malloc(sizeof(strSearch));


	//LogFunc("CheckString Called ",sizeof("CheckString Called "));

	for(iLoop=0;iLoop<=iCount-1;iLoop++)
	{
	
		strncpy(tempcheck,tmpptr->str,sizeof(strSearch)-1);
	
		if(!strcmp(tempcheck,strSearch)) // Return value in relation to main buffer
		{free(tempcheck);return iAbspos+sizeof(strSearch);} // Return value from where attribute string starts
		tmpptr=tmpptr->link_ptr;
		iAbspos += sizeof(tmpptr->str) + 2; // +2 to account for /r/n
	}
	free(tempcheck);
	return 0;
}

		
		// FUNCTION: LoadFile
char * UserConnection::LoadFile(char *strPath)
{
	int fh; // File Handle
	long iFileSize=0;
	
	char *strFileBuffer,*strFinalPath;
	

	strFinalPath=(char *)malloc( _msize(strPath) + sizeof(PATH) + sizeof(char) );
	

	strcpy(strFinalPath,PATH);
	strcat(strFinalPath,strPath); //_msize is used for 

	// Delete 1
	printf("File Request = %s\n",strFinalPath);
	// Delete 0
	
	LogFunc("File Requested ",sizeof("File Requested "));
	LogFunc(strFinalPath,GetSize(strFinalPath));
	
	fh=_open(strFinalPath,_O_BINARY,_S_IREAD); // We will use Low level IO routines
	if(fh == -1)
	{
			return HTTP_404;
	}
	
	iFileSize=_filelength(fh);

		strFileBuffer = (char *)malloc((sizeof(char) * iFileSize)+1); //calloc sets it to NULL
		if (strFileBuffer == NULL)
			printf("Critical Memory Error\n");
		_read(fh,strFileBuffer,iFileSize);
	_close(fh);

	strFileBuffer[iFileSize] = '\0';
	LogFunc("File Load Complete",sizeof("File Load Complete"));
	
	return strFileBuffer;
}



char * UserConnection::ExtractAttribute ( char *str, int iPos)
{
	int iLoop=0;
	char *attr;	

	attr = (char *)malloc(sizeof(char));
	do
	{
		attr[iLoop]=str[iPos + iLoop];
		attr = (char *)realloc(attr,sizeof(char)+_msize(attr));
	} while(attr[iLoop] != '\r' && attr[iLoop++] != ' '); // Check for space also
	attr[iLoop] = '\0';
	return attr;	// Return values
}


void UserConnection::LogFunc(char *str,int iSize)
{
	char *strptr;
	FILE *logfp=NULL;
	logfp=fopen(LOG_PATH,"a");
	if( logfp != NULL)
		if((strptr = (char *)malloc(iSize+sizeof(char)*2)) != NULL) // Realloc is giving a problem here
			{
			strcpy(strptr,str);
			strcat(strptr,"\n");
			fprintf(logfp,strptr);
			fclose(logfp);
			free(strptr);}
		else
			printf("Insufficient Memory for buffer");
	logfp=NULL; // Make file pointer void
	

}


long UserConnection::GetSize(char *str)
{
	long lSize=0;
	while(str[lSize++] != '\0');
	//	printf("Size = %d\n",lSize-1); // TEMP
	return (lSize-1);
}


		// FUNCTION NOT BEING USED
char * UserConnection::CStylePath(char *str)
{
	int iLoop1,iLoop2 = 0;
	char  *strFinalPath; // We will use malloc to allocate space

	strFinalPath = (char *)malloc(sizeof(str) + 2);
	strFinalPath[0]='\0';

	for(iLoop1=0;iLoop1<=(sizeof(str) - 1);iLoop1++)
	{
		if(str[iLoop1] == '\\')
		{
			strFinalPath[iLoop2] = '\\';
			strFinalPath =(char *)realloc(strFinalPath,1);	
			strFinalPath[++iLoop2] ='\\';
		}
		strFinalPath[iLoop2++] = str[iLoop1];
		strFinalPath =(char *) realloc(strFinalPath,1);	
	}
	return strFinalPath;
}