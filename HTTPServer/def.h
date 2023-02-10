		//CONSTANT DECLARATIONS


#define MAX_SEND_BUFFER 100000
#define MEM_ERROR "Insufficient Memory for Application\n\0";
#define MAX_RECEIVE_SIZE 10000

		// HTTP Error Codes

#define HTTP_404 "File not found on this server\r\n\0";



		//GENERIC

#define WEB_SERVER_PRODUCT_NAME "BeluBhai W3SVR" 

	// CFG File Specific - only tmp

char PATH[] =  "c:\\perdata\\site"; // Not using #define due to error in UserConnection Line 197 and 201
char LOG_PATH[] = "c:\\perdata\\belubhai.log";
#define DEFAULT "index.html";
