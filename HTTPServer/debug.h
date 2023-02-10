// CLASS NAME : debug
// AUTHOR     : Chaitanya Belwal cbelwal@hotmail.com
// -------------------------------------------------->
// Include header file in your program 
// 1) Add Line : #include "debug.h"
// 2) Create Debug Object e.g. debug D;
// 3) Log on line e.g. D.log("Debug Line #1\n");
// 4) View Log file debug.log using Notepad


#include<stdio.h>


class debug
{
private:
	FILE *fp;

public:
	debug();
	~debug();
	void log(char *);
};


 debug::debug()
{
	fp=fopen("debug.log","w");
	fprintf(fp,"Log Starts --->");
}

 debug::~debug()
{
	fprintf(fp,"Log Ends --->");
	fclose(fp);
}

void  debug::log(char *str)
{

	fprintf(fp,str);
}

// END
		