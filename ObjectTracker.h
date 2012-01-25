#ifndef OBJECTTRACKER_H
#define OBJECTTRACKER_H

#include <iostream>
#include "cv.h"
#include "highgui.h"
#include <ctype.h>
#include <string>
#include <math.h>
#include <vector>
#include <cmath>

using namespace std; 
#define PI 3.14159256

//The class that tracks a specific object on the board.
class ObjectTracker {
   public:
	   ObjectTracker(){};
	  ObjectTracker(IplImage* _searchRegion,string color="",bool _calibrationMode=false);
      void trackObject(IplImage* image, CvPoint2D32f* objPos,bool erode=true);
	  void setColor(string color);
	  void setColor(CvScalar _minColor, CvScalar _maxColor);
	  void saveColor();
	  void setCalibrationMode(bool cal);
	private:
		string colorName;
		CvScalar minColor;
		CvScalar maxColor;
		IplImage* searchRegion;
		bool calibrationMode;
		//vector<CvPoint> recentPositions;
		int colorAtPixel(IplImage* mask,int x, int y);
		IplImage* hsv;
		IplImage* maskedImage;
		IplImage* tresholdImage;
		CvMoments *moments;
		IplImage* erodeImg;
};
IplImage* Sub_Image(IplImage *image, CvRect roi);


void resizeToImage(IplImage** toResize,IplImage* rightSize);
//Help functions for checking if the tracking is being successfully done.
bool isAreaWithinLimit(CvRect area);
bool hasLostTracking(double area);
bool trackerOutOfSight(double area, double lastArea);
bool isTrackerConvergingToPosition(CvBox2D box, CvBox2D theLastBox);
void lighten(IplImage* img, int p);
void contrast(IplImage* img, int p);


#endif
