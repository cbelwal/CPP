#include "data.h"
#include "headers.h"
#include "ping.h"


		// SUPPORT FUNCTIONS
void ParseData(int, char *[]);
void CheckInputValues();
void DisplayMessage();
void DisplayError(char *);
long GetSize(char *);
void wait(int); // wait for seconds




void main(int argc,char * argv[])
{

		// INIT VARIABLES
	
	flagPing=0;
	flagLogFile=0;
	flagSpecial=0;
	flagTimeout=0;


	int fh,iStatus;           // Log file
	long lBytes;
	float fTime;
	unsigned int iLoop;
	char strMessage[60],strDate[128], strTime[128];


		
		// INSERT DEFAULT VALUES
	
	_strtime(strTime);  // Copy Time
	_strdate(strDate);  // Copy Date

		// --------------------->


	ParseData(argc, argv);

	if(flagPing == 0)
		iPing = DEFAULT_PING;
	if(flagTimeout == 0)
		fTimeOut = DEFAULT_TIMEOUT;
	if(flagWait == 0)
		{iWait = DEFAULT_WAIT;
		 if (flagSpecial == 1)
			 iWait = DEFAULT_BREAK_WAIT;}
	if(flagLogFile == 1)
		{
			fh=_open(strLogFile, _O_WRONLY | _O_CREAT |_O_TRUNC ,_S_IWRITE); // 0x8000 = _O_BINARY defined in fnctl.h
			if(fh == -1)
				DisplayError("Could not create log file. Path name should be valid");
		}

	sprintf(strMessage,"Starting 'E'cho  %s %s\n\n", strTime, strDate);
	printf("%s",strMessage);

	if (flagLogFile == 1)
		_write(fh,strMessage,GetSize(strMessage));
		
		// Create Object

	
	
	if(flagSpecial != 1) // If not Special Case
	{
		ping *objPing; // Get Object
		objPing = new ping(strHostname,fTimeOut);
		iStatus = objPing->Initialize();
		if (iStatus == -1)
			DisplayError("Invalid host! Cannot resolve IP address");
		if (iStatus < -1)
			DisplayError("TCP/IP Stack Error");
		printf("ICMP echo request to %s\n\n",objPing->strFQDN);
		for(iLoop=0;iLoop<iPing;iLoop++)
		{   lBytes = objPing->SendICMP();
		    if (lBytes > 0)   // Send Packet Successful
			{		
				fTime = objPing->GetICMP();
				if ( fTime > -1)
					sprintf(strMessage,"Echo reply from %s in %0.0f ms \n",objPing->ip_addr,fTime);
				else
					sprintf(strMessage,"Echo receive timed out in %0.0f ms\n",fTimeOut);
			}
			else
				sprintf(strMessage,"Cannot reach destination host\n");
			
			printf("%s",strMessage);
				if(flagLogFile == 1)
					_write(fh,strMessage,GetSize(strMessage));
				wait(iWait);
		}

		 delete objPing; // Close everything
	}
	
	
	else // Check only Network Breaks
	{
		iLoop=1;
		int downFlag = 0;
		clock_t diffTime=clock();
		
		while(iLoop)
		{
		
			ping *objPing;
			objPing = new ping(strHostname,fTimeOut);
			iStatus = objPing->Initialize();

			if (iStatus == -1)
				DisplayError("Invalid host! Cannot resolve IP address");
			if (iStatus < -1)
				DisplayError("TCP/IP Stack Error");

			lBytes = objPing->SendICMP();
		    
			if (lBytes > 0) // Packet Sent
				fTime = objPing->GetICMP();

			if ( (fTime < 0 && downFlag == 0) || (lBytes < 0 && downFlag ==0)) // Network Break
			{	_strtime(strTime);  // Copy Time
				_strdate(strDate);  // Copy Date
				 
				 sprintf(strMessage,"Network to %s went down at %s %s after %0.0f seconds\n",objPing->ip_addr,strTime,strDate,(float)(clock()-diffTime)/CLOCKS_PER_SEC);
				 downFlag = 1;
				 diffTime = clock();
				 printf("%s",strMessage);
					if(flagLogFile == 1)
						_write(fh,strMessage,GetSize(strMessage));
			}
			else
				if (fTime > 0 && downFlag == 1)
				{_strtime(strTime);  // Copy Time
				 _strdate(strDate);  // Copy Date
				sprintf(strMessage,"Network to %s came up at %s %s after %0.0f seconds\n",objPing->ip_addr,strTime,strDate,(float)(clock()-diffTime)/CLOCKS_PER_SEC);
				downFlag = 0;
				diffTime = clock();
				printf("%s",strMessage);
					if(flagLogFile == 1)
						_write(fh,strMessage,GetSize(strMessage));
				}
			
			delete objPing;  // Free the Resources
			wait(iWait);  //Check Every Minute
		}		

	}
		if(flagLogFile == 1)
			_close(fh);  // Close File
}

		// SUPPORT FUNCTIONS

void ParseData(int argc, char *argv[])
{
	int iLoop;
	if (argc < 2)
		DisplayMessage();
	
	if (argv[1][0] == '-' || argv[1][0] == '?') // If not hostname or ?
		DisplayMessage();

	strHostname=(char *) malloc(GetSize(argv[1])); // Assign Hostname
	strcpy(strHostname,argv[1]);


// Parameter C:\> E <HOSTNAME> -n <NO of PINGS> -l <LOG FILE> -b  -t timeout -w <WAIT TIME IN SECONDS>

	
	for (iLoop = 2;iLoop < argc ;iLoop++)
	{
		if (!(strcmp(argv[iLoop],"-n")))
		{	iPing = atoi(argv[++iLoop]);
			flagPing=1;}

		if (!(strcmp(argv[iLoop],"-w")))
		{	iWait = atoi(argv[++iLoop]);
			flagPing=1;}


		if (!(strcmp(argv[iLoop],"-l")))
		{strLogFile = (char *) malloc(GetSize(argv[++iLoop]));
		strcpy(strLogFile, argv[iLoop]);
		flagLogFile = 1;}

		if (!(strcmp(argv[iLoop],"-b")))
		{flagSpecial = 1;}

		
		if (!(strcmp(argv[iLoop],"-t")))
		{fTimeOut= (float) atof(argv[++iLoop]);
		flagTimeout = 1;}							
	}		
}


void CheckInputValues()
{
	// Check the values given at command line
	
	// 0 < iWait < 9600	
	if (iWait < 0 || iWait > 9600)
		DisplayError("Invalid wait time! Should be between 0 and 9600 ");
	//fTimeOut 
	if (fTimeOut < 1000 || fTimeOut > 60000)
		DisplayError("Invalid timeout! Should be between 1000 and 60000 ");
	// iPing
	if (iPing < 1 || iPing > 64000)
		DisplayError("Invalid ping number! Should be between 1 and 64000 ");

}


void DisplayMessage()
{
		// Display Help Message
	printf("'E' cho Usage:\n\n");
	
	
	
	printf("E <HOSTNAME> -n xxx -l xxx -t xxx -w xxx -b\n\n");

	printf(" <HOSTNAME> Valid hostname or IP adress\n");
	printf(" -n Number of times to send ping. Default is 2\n");
	printf(" -l Valid log file path\n");
	printf(" -t Timeout values in milliseconds (between 1000 and 60000).Default is 2000 \n");
	printf(" -w Time in seconds to wait before sending next echo. Default is 1 second\n");
	printf(" -b Check for network breaks.\n\n");
	printf("Use file logging for debuggin your network\n\n");

	printf("Questions/Bugs/Suggestions: Chaitanya Belwal , cbelwal@hotmail.com\n");
	printf("                            http://cbelwal.tripod.com\n\n");
	exit(0);
}

void DisplayError(char *str)
{
	char *strMessage;
	strMessage = (char *)malloc(GetSize(str));
	strcpy(strMessage, str);
	printf("%s",strMessage);
	printf("\n\n");
		free(strMessage);
	exit(1);

}



void wait(int iSeconds)
{
	clock_t start;
	start = clock();
	while  ( ((int) (clock() - start)  / CLOCKS_PER_SEC ) < iSeconds );
}



long GetSize(char *str)
{
	long lSize=0;
	while(str[lSize++] != '\0');
	return (lSize-1);
}

	
