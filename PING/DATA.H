		// Include all Support libraries here
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <io.h>
#include <sys/stat.h>
#include <fcntl.h>






#define ICMP_HEADER_SIZE sizeof(ICMP_HEADER)
#define IP_HEADER_SIZE 20
#define BUFFER_SIZE ICMP_HEADER_SIZE + IP_HEADER_SIZE

#define ICMP_ECHO_REPLY  0// ICMP Type: 0 defined in RFC 792
#define ICMP_ECHO_REQ    8// ICMP Type: 8
		
#define DEFAULT_TIMEOUT 2000  //  ms
#define DEFAULT_PING 2
#define DEFAULT_WAIT 1
#define DEFAULT_BREAK_WAIT 60
	
	//Data Variable
//Parameter C:\> E <HOSTNAME> -n <NO of PINGS> -l <LOG FILE> -s -t timeout

char *strHostname, *strLogFile;
unsigned int iPing,iFlagSpecialFile,iWait;
float fTimeOut;
char *logfile;
int flagPing, flagLogFile, flagSpecial, flagTimeout,flagWait;
