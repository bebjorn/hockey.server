
#ifndef CAMCAPTURE_H
#define CAMCAPTURE_H

#include <conio.h>
#include <tisudshl.h>

#include "cmdhelper.h"
#include <iostream>
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <ctype.h>

#define NATIVEIMAGESIZE cvSize(744, 480) //Should be changed if other camera resolution than 744x480 is to be used.
using namespace std;
using namespace _DSHOWLIB_NAMESPACE;

//Class to capture from the DFK 21AUC03 camera.
//Written as cvCaptureFromCAM did not work for this camera and gave only black & white picture.
class CamCapture {
	public:
		CamCapture();
		~CamCapture(){}
		int initCam();
		void myQueryFrame(IplImage* image);
		CvSize getFrameCvSize(){return cvSize(width,height);}
	private:
		Grabber grabber;
		tFrameHandlerSinkPtr pSink;
		int width;
		int height;
};


#endif