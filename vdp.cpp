#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
  //capture the video from camera. If laptop webcam is used, keep index = 0 else 1.
  VideoCapture cap(0);

  // if not success, exit program
  if ( !cap.isOpened() )
  {
         cout << "Cannot open the web cam" << endl;
         return -1;
  }
	namedWindow("Display", CV_WINDOW_NORMAL); //create a window called "Display". It displays color marker drawing
	namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control". Used for  thresholding
	namedWindow("color", CV_WINDOW_AUTOSIZE); //create a window called "Color". Used for selecting colors

  // Set of Values thresholded for particular conditions changes for different ligthing conditions.
  // H - Hue, S - Saturation, V -Value of a color
	int iLowH = 0;
	int iHighH = 29;

	int iLowS = 96;
	int iHighS = 255;

	int iLowV = 173;
	int iHighV = 255;

  //Initialize colors,size of marker
	int Red=0;
	int Green=0;
	int Blue=0;
	int Black;
  int size=1;
	int wipeoutscreen=0;
  int save=0;

	//Create trackbars in "Control" window
	 cvCreateTrackbar("LowH", "Control", &iLowH, 255); //Hue (0 - 179)
	 cvCreateTrackbar("HighH", "Control", &iHighH, 255);

	 cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	 cvCreateTrackbar("HighS", "Control", &iHighS, 255);

	 cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
	 cvCreateTrackbar("HighV", "Control", &iHighV, 255);

	////Create trackbars in "color" window
	 cvCreateTrackbar("Blue", "color", &Blue, 255); //Value (0 - 255)
	 cvCreateTrackbar("Green", "color", &Green, 255); //Value (0 - 255)
	 cvCreateTrackbar("Red", "color", &Red, 255); //Value (0 - 255)
	 cvCreateTrackbar("Eraser", "color", &Black,1); //Value (0 - 255)
   cvCreateTrackbar("size", "color", &size,30); //Value (0 - 30)
   cvCreateTrackbar("wipeoutscreen", "color", &wipeoutscreen,1); //Value (0 and 1)
   cvCreateTrackbar("save", "color", &save,1); //Value (0 and 1)

   //Matrices for storing images
	 Mat imgOriginal,imgLines,imgHSV,imgThresholded;

 	 int n=0;
   while (true)
   {
      n++;
      // read a new frame from video
      bool bSuccess = cap.read(imgOriginal);

      if(n==1) ///creates only once imgLines matrix.
	       imgLines = Mat::zeros(imgOriginal.size(), CV_8UC3 );
      //if not success, break loop
      if (!bSuccess){
        cout << "Cannot read a frame from video stream" << endl;
        break;
      }

      //Convert the captured frame from BGR to HSV
  	  cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV);
      //Threshold the image
  	  inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded);

  	  //morphological opening (remove small objects from the foreground)
  	  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  	  dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

  	  //morphological closing (fill small holes in the foreground)
  	  dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  	  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

  	  //Calculate the moments of the thresholded image
  	  Moments oMoments = moments(imgThresholded);

  	  double dM01 = oMoments.m01;
  	  double dM10 = oMoments.m10;
  	  double dArea = oMoments.m00;

  	  int posX = dM10 / dArea;
  	  int posY = dM01 / dArea;

  	  int fposX=640-posX;

  	  int xi=fposX,yi=posY;
  	  int xf=fposX,yf=posY;

      //Varioius Operations
  	  //Eraser
      if(Black==1)
  	   {
         Red=0;
         Green=0;
         Blue=0;
         size=40;
       }
  	  //wipeout screen
      if(wipeoutscreen==1)
        imgLines = Mat::zeros(imgOriginal.size(), CV_8UC3 );

      //Checking for distance of obj from camera
      if(dArea>1700000){
        cout<<dArea<<endl;
  	    line(imgLines,Point(xi,yi),Point(xf,yf),Scalar(Blue,Green,Red),size);
      }
      //creates circle  based on Centre and Radius.
      //circle(image,centre,radius,color,thickness,lineType,shift)
  	  circle( imgOriginal, Point(posX, posY), 2, Scalar( 255, 255, 255 ), 2, 8 );

      //Save image if save is 1
  	  if(save==1)
        imwrite("/home/gaurav/Pictures/my_img.jpg",imgLines) ;


  	  imshow("Display", imgLines); //show the drawn lines
  	  imshow("Thresholded Image",imgThresholded );//show the thresholded image
  	  imshow("Original", imgOriginal); //show the original image

      //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
      if (waitKey(30) == 27){
        cout << "esc key is pressed by user" << endl;
        break;
      }
    }
  return 0;
}
