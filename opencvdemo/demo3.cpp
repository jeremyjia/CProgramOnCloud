#include<opencv2/opencv.hpp>
#include<cv.h>
#include<vector>
#include<stdio.h>

using namespace cv;
using namespace std;

cv::Mat LeftRightMerge(cv::Mat A, cv::Mat B)
{
    int totalCols = A.cols + B.cols;
    cv::Mat mergedDescriptors(A.rows, totalCols, A.type());
    cv::Mat submat = mergedDescriptors.colRange(0, A.cols);
    A.copyTo(submat);
    submat = mergedDescriptors.colRange(A.cols, totalCols);
    B.copyTo(submat);
    return mergedDescriptors;
}

int main(int argc,char *argv[])
{
    Mat img1 = imread(argv[1]);
    Mat img2 = imread(argv[2]);
	if(!img1.data || !img2.data)
	{
		std::cout<<"Open image failed，Please check the file path!"<<std::endl;
		return 0;
	}
    
    int h = img1.rows;
    int w = img1.cols;
    if(img2.rows!=h || img2.cols!=w){
       std::cout<<"Image size is diff!"<<std::endl;
       return 0;
    }

    Mat leftMat1 = img1(cvRect(0, 0, w/2, h));
    Mat rightMat2 = img2(cvRect(w/2, 0, w/2, h));
    Mat outputImage = LeftRightMerge(leftMat1, rightMat2);

    imwrite("stitch_all_image.jpg", outputImage);

}
