		// Include all Support libraries here

#include <winsock2.h>
#include <process.h>    
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <io.h>


		// All data variables used by Mainfile.cpp


SOCKET srvSocketConn, sockBind;

sockaddr_in dsSocketConn,dsBind;
WSAData wsadata;

int iStatus,iSwitch;