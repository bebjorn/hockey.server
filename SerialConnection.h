#include "SerialClass.h"
#include <process.h>
#include <windows.h>
#include <tchar.h>

#ifndef SERIALCONNECTION
#define SERIALCONNECTION
#define MAXBUF 24

class SerialConnection:Serial{
	
public:
	
	void write(char* com,int length,char* _mode="c");
	int read(char* com);
	bool gotMode();
	bool isAvailable();
	SerialConnection(char* arg);
	~SerialConnection();
private:
	HANDLE readMutex;
	HANDLE writeMutex;
	HANDLE serialThreadHandle;
	int inBufLength;
	int outBufLength;
	char inBuf[MAXBUF];
	char* outBuf;
	char* mode;
	void communicate();
	static unsigned __stdcall serialThread(void*);
};
#endif