		// SIMPLE WEB SERVER
        //               FINAL FUNCTIONS: 
        // 1) CFG File to specify configuration parameters
        // 2) send only HTML files
		// 3) Handle GET / POST Request

		// Author :- Chaitanya Belwal
        

#include "debug.h"			// Debugging Class
#include "def.h"			// Constant declarations
#include "data.h"			// Variable declarations
#include "UserConnection.h" // Class Definition



		// FUNCTION: main()

void MultiThreadFunc(void *);

		// Shared Class UserConnection Pool;
		// UserConnection *ptrUserConn[1000];

void main()
{
	
	
		// VARIABLE INITIALIZATION
	
	
	
	WSAStartup(0x202,&wsadata);

	
	sockBind=socket(AF_INET,SOCK_STREAM,0);
	
	dsBind.sin_port=htons(80);
	dsBind.sin_addr.S_un.S_addr=inet_addr("\0");
	dsBind.sin_family=AF_INET;
	

	
    		
    const sockaddr *sockptr = (LPSOCKADDR) &dsBind;

		// SECTION: Set socket in listening state
	
	iStatus=bind(sockBind,sockptr,sizeof(dsBind)); 

	printf("Status = %d , Error Code = %d\n",iStatus, WSAGetLastError());
	
	iStatus=listen(sockBind,6);
    printf("Status = %d , Error Code = %d\n",iStatus, WSAGetLastError());

	int p = sizeof(dsSocketConn);
	
	iSwitch=1;	
	
	while (iSwitch)
	
	  {
		srvSocketConn = accept(sockBind,(LPSOCKADDR)&dsSocketConn, &p);
		iSwitch++;	
		_beginthread(MultiThreadFunc,0,(void *)srvSocketConn);
		
	}

}

	
void MultiThreadFunc(void *srvSock)
{
	
	printf("\nCreating New User Object\n");
	UserConnection *ptrConn;
	ptrConn = new UserConnection(srvSock);
		
			
//	delete ptrConn;  // Delete User Connection Object
		_endthread();
}
