#include "SerialConnection.h"
#include <time.h>
#include <iostream>
//klass f�rr seriell kommunikation med mikrokontroller
using namespace std;
SerialConnection::SerialConnection(char* arg):Serial(arg){

	readMutex=CreateMutex(NULL,false,_T("readMutex"));
	writeMutex=CreateMutex(NULL,false,_T("readMutex"));
	inBufLength=0;
	outBufLength=0;
	inBuf;//=new char[256];
	outBuf=new char[256];
	mode="c";
	serialThreadHandle=(HANDLE)_beginthreadex(NULL,0,serialThread,this,0,NULL);// skapar tr�den som driver detta objekt och startar den
}
SerialConnection::~SerialConnection(){
	Serial::~Serial();
	TerminateThread(serialThreadHandle,0);

}
void SerialConnection::communicate(){
	//WaitForSingleObject(readWriteMutex,INFINITE);
	char* defaultMode="c";//command
	while(true){
		/*mode="d";*/
		
		
		WaitForSingleObject(writeMutex,INFINITE);	//Sychronized section
		
		
		WriteData(mode,1);//skriv vilken typ av meddelande som skickas
		//cout<<mode<<endl;
		//mode=defaultMode;
		WriteData(inBuf,inBufLength);//skriv meddelande	som f�s av recieverThread
		//inBuf;									
		inBufLength=0;	
		ReleaseMutex(readMutex);//utanf�r syncning
						

		Sleep(10);//v�ntar p� att mikrokontrollern ska f�rdigst�lla sin cykel, se Rapport, mikrokontroller
		
		WaitForSingleObject(readMutex,INFINITE);	//Sychronized section
		outBufLength=ReadData(outBuf,MAXBUF);//l�ser in meddelande fr�n mikrokontroller
		if(outBufLength<0){
			outBufLength=0;
		}
		/*if(outBufLength<0){
			cout<<"could not read Serial"<<endl;
		}*/
		
		
		ReleaseMutex(readMutex);					//
	}
}
void SerialConnection::write(char* com,int length,char* _mode){//s�tter vd som skall skickas till mikrokontrollern
	for(int i=0;i<length;i++){
		cout<<(int)com[i]<<"\t";
	}
	cout<<endl;
	WaitForSingleObject(writeMutex,INFINITE);	//Sychronized section
	mode=_mode;									//
	memcpy(inBuf,com,length);					//
	inBufLength=length;							//
	ReleaseMutex(writeMutex);					//
}
int SerialConnection::read(char* com){//l�ser den senste informationen fr�n mikrokontrollern
	WaitForSingleObject(readMutex,INFINITE);
	memcpy(com, outBuf,outBufLength);
	
	
	
	int temp=outBufLength;
	ReleaseMutex(readMutex);
	return temp;
}
bool SerialConnection::isAvailable(){
	return IsConnected();
}
unsigned _stdcall SerialConnection::serialThread(void* param){
	SerialConnection* serial=(SerialConnection*)param;

	serial->communicate();

	return NULL;
}