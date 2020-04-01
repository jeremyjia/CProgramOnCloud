#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <ctype.h>
#include <iostream>
using namespace cv;
using namespace std;   
CvFont font;
 
int  main(int argc, char** argv)
{
	Mat src = imread("src.jpg");
	// Mat -> IplImage 类型转换
	IplImage tmp = (IplImage)src;
	IplImage* image = &tmp;
	
	/*-------------------opencv写字程序 8种字体类型------------------*/	
	cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX,1.0,1.0,0,2,8);
	cvPutText(image,"1Hello World !",cvPoint(80,40),&font,cvScalar(255,0,0,1));
	//cout<<image->height <<"\n "<< image->width ;
	printf("%d\n%d",image->height,image->width);
	cvInitFont(&font,CV_FONT_HERSHEY_PLAIN,1.0,1.0,0,2,8);
	cvPutText(image,"2Hello World !",cvPoint(80, 80),&font,cvScalar(0,255,0,1));
 
	cvInitFont(&font,CV_FONT_HERSHEY_DUPLEX,1.0,1.0,0,2,8);
	cvPutText(image,"3Hello World !",cvPoint(80, 140),&font,cvScalar(0,255,255,1));
 
	cvInitFont(&font,CV_FONT_HERSHEY_COMPLEX,1.0,1.0,0,2,8);
	cvPutText(image,"4Hello World !",cvPoint(80, 200),&font,cvScalar(255,255,255,1));
 
	cvInitFont(&font,CV_FONT_HERSHEY_TRIPLEX,1.0,1.0,0,2,8);
	cvPutText(image,"5Hello World !",cvPoint(80, 260),&font,cvScalar(255,255,0,1));
 
	cvInitFont(&font,CV_FONT_HERSHEY_COMPLEX_SMALL,1.0,1.0,0,2,8);
	cvPutText(image,"6Hello World !",cvPoint(80, 300),&font,cvScalar(0,0,0,1));
 
	cvInitFont(&font,CV_FONT_HERSHEY_SCRIPT_SIMPLEX,1.0,1.0,0,2,8);
	cvPutText(image,"7Hello World !",cvPoint(80, 360),&font,cvScalar(255,255,255,1));
 
	cvInitFont(&font,CV_FONT_HERSHEY_SCRIPT_COMPLEX,1.0,1.0,0,2,8);
	cvPutText(image,"8Hello World !",cvPoint(80, 420),&font,cvScalar(0,255,255,1));
	/*-------------------opencv写字程序 8种字体类型------------------*/	
 
	// IplImageMat -> Mat 类型转换
	cv::Mat img = cv::cvarrToMat(image);

        Size size(800,800);
        Size size2(300, 200);
        //Mat img = Mat::zeros(size, CV_8UC3);
        Point p1(100, 200);
        Point p2(200, 400);
        Point p3(300, 300);
        Point p4(500, 200);
        line(img,p1,p2,Scalar(80,100,90));//绘制直线,p1为起点，p2为终点，线的颜色为
        ellipse(img, RotatedRect(p3,size2,45), Scalar(0, 0, 255));//绘制椭圆，旋转中心为p3，大小为size2，旋转角度45，颜色红
        rectangle(img, Rect(50,50,100,100), Scalar(0, 255, 0)); // 绘制矩形，起点坐标（50，50），大小（100，100），颜色绿色
        circle(img, p4, 100, Scalar(255, 0, 0));//绘制圆，中心坐标p4，半径100

	//Save to file.
	imwrite("target.jpg", img);
	//imshow("opencv writing",img);
 	//waitKey();
	return 0;
}

