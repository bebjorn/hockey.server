#include <iostream>
#include "GameState.h"
using namespace std;
void calibrateMicroControllers(){
	initializeMicroControllers();
	homeSerial->write(NULL,0,"a");
	awaySerial->write(NULL,0,"a");
	cout<<"move every player to it's maximum and then minimum positions"<<endl;
	cout<<"turn every player so that they face forward"<<endl;
	cout<<"press any key when done"<<endl;
	system("pause");
	homeSerial->write(NULL,0);//save calibration
}