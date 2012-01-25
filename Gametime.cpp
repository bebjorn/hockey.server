#include <time.h>
#include <math.h>
#include <stdio.h>
#include <float.h>

#define TIME clock()
namespace gametime{
	int startTime;
	int totalPausetime=0;
	int pauseStartTime=0;
}
using namespace gametime;
void startGametime(){
	startTime=TIME;
	totalPausetime=0;
}
void pauseGametime(){
	if(pauseStartTime==0){
		pauseStartTime=TIME;
	}
}
void resumeGametime(){
	if(pauseStartTime!=0){
		totalPausetime+=TIME-pauseStartTime;
		pauseStartTime=0;
	}
}
int getGametime(){
	return TIME-startTime-totalPausetime;
}