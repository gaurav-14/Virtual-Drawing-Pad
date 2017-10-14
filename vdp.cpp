#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
  VideoCapture cap(0); //capture the video from laptop web cam

    if ( !cap.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the web cam" << endl;
         return -1;
    }
	 namedWindow("Display", CV_WINDOW_NORMAL); //create a window called "Control"
	namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"
	 namedWindow("color", CV_WINDOW_AUTOSIZE); //create a window called "Color"

	 int iLowH = 0;
	 int iHighH = 29;

	 int iLowS = 96;
	 int iHighS = 255;

	 int iLowV = 173;
	 int iHighV = 255;


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
         cvCreateTrackbar("size", "color", &size,30); //Value (0 - 1) 
         cvCreateTrackbar("wipeoutscreen", "color", &wipeoutscreen,1); //Value (0 - 1) 
         cvCreateTrackbar("save", "color", &save,1); //Value (0 - 1) 



	 Mat imgOriginal,imgLines;
       
 	 int n=0;
    while (true)
    {
         n++;
        
         bool bSuccess = cap.read(imgOriginal); // read a new frame from video


         if(n==1) ///creates only once imgLines matrix.
	 imgLines = Mat::zeros(imgOriginal.size(), CV_8UC3 );

         if (!bSuccess) //if not success, break loop
          {
             cout << "Cannot read a frame from video stream" << endl;
             break;
          }
	  Mat imgHSV;

	  cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
	 
	  Mat imgThresholded;

	  inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
	      
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
	  
	   //Mat imgLines(posX,posY, CV_8UC3, Scalar( 255,255,255));

	   //Eraser	
           if(Black==1)
	   {  
             Red=0;
             Green=0;
             Blue=0;
             size=40;}	

           
	   //wipeout screen
            if(wipeoutscreen==1)
            {
             imgLines = Mat::zeros(imgOriginal.size(), CV_8UC3 );
            }
           //vector<vector<Point> > contours;
           //vector<Vec4i> hierarchy;
          // findContours(imgThresholded, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
           
        if(dArea>1700000) //Checking for distance of obj from camera
     {
             
          
           cout<<dArea<<endl;

	   line(imgLines,Point(xi,yi),Point(xf,yf),Scalar(Blue,Green,Red),size);
        }
          

	   circle( imgOriginal, Point(posX, posY), 2, Scalar( 255, 255, 255 ), 2, 8 );
           
	     if(save==1)
	   {   
              imwrite("/home/gaurav/Pictures/my_img.jpg",imgLines) ;
           }     

 
	   imshow("Display", imgLines); 
	   imshow("Thresholded Image",imgThresholded );//show the thresholded image
	   imshow("Original", imgOriginal); //show the original image

           if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
           {
            cout << "esc key is pressed by user" << endl;
            break; 
           }
    }

   return 0;

}
