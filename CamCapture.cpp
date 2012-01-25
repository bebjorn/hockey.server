#include "CamCapture.h"
#include "D:\CamLicense.h"

//Constructor that initilizes the library. License from camera.
#ifdef COLORCHECKER
float scalingFactor=1;
#endif
CamCapture::CamCapture(){
	if( !DShowLib::InitLibrary( CAM_SERIAL_NUMBER ) )
	{
		fprintf( stderr, "The library could not be initialized ");
		fprintf( stderr, "(invalid license key?).\n");
		exit( 1 );
	}
	
	
	
}
//Intializes the camera and shows the popup window where fps can be changed. 
int CamCapture::initCam(){
	
	if( !setupDeviceFromFile( grabber ,"device.xml") ){
		return -1;
	}
	
	//Grabbing only works for RGB24, if other needed the myQueryFrame-method must be changed.
	try{
	pSink = FrameHandlerSink::create( DShowLib::eRGB24, 1);
	pSink->setSnapMode(true);
	}catch(exception e){
		cerr<<"no configuration available"<<endl;
		exit(1);
	}
	if (!grabber.setSinkType(pSink)){
		return -1;
	}

	 // Start the live video.	
	if( !grabber.startLive( false ) ){
		std::cerr << grabber.getLastError().toString() << std::endl;
		return -1;
	}
	if(pSink.get()==NULL){
		cout<<"could not read from camera"<<endl;
		exit(1);
	}
	return 1;
}

//Method for getting latest frame from camera. Equsivalent method as cvQueryFrame of the OpenCv library.
//IplImage* frame is containing the latest frame after method call. 

void CamCapture::myQueryFrame(IplImage* frame){
	
	//Grab one Image from Camera:
	
	if(!pSink.get()==NULL){
		
	pSink->snapImages(1,100);
	}else{
		cout<<"could not read from camera"<<endl;
		exit(1);
	}
		
	MemBufferCollection::tMemBufferPtr buffer = pSink->getLastAcqMemBuffer();
	
	IplImage* raw=cvCreateImage(NATIVEIMAGESIZE,8,3);
	raw->imageData=(char*)buffer->getPtr();
	
	cvResize(raw,frame);
	cvReleaseImage(&raw);
	
}


