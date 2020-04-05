#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include "opencv2/opencv.hpp"
#include "time.h"
#include "CvxText.h"

static int ToWchar(char* &src, wchar_t* &dest, const char *locale = "zh_CN.utf8")
{
    if (src == NULL) {
        dest = NULL;
        return 0;
    }

    // 根据环境变量设置locale
    setlocale(LC_CTYPE, locale);

    // 得到转化为需要的宽字符大小
    int w_size = mbstowcs(NULL, src, 0) + 1;

    // w_size = 0 说明mbstowcs返回值为-1。即在运行过程中遇到了非法字符(很有可能使locale没有设置正确)
    if (w_size == 0) {
        dest = NULL;
        return -1;
    }

    //wcout << "w_size" << w_size << endl;
    dest = new wchar_t[w_size];
    if (!dest) {
        return -1;
    }

    int ret = mbstowcs(dest, src, strlen(src)+1);
    if (ret <= 0) {
        return -1;
    }
    return 0;
}

int main()
{
    cv::Mat img = cv::imread("./src.jpg");
    if (!img.data || img.channels() != 3) {
        fprintf(stderr, "read image failed\n");
        return -1;
    }

    CvxText text("./simhei.ttf"); //指定字体
    cv::Scalar size1{ 50, 0.5, 0.1, 0 }; // (字体大小, 无效的, 字符间距, 无效的 }
    cv::Scalar size2{ 30, 0.5, 0.1, 0 };
    text.setFont(nullptr, &size1, nullptr, 0);
    char* str = (char *)"我爱北京天安门";
    wchar_t *w_str;
    ToWchar(str,w_str);
    text.putText(img, w_str, cv::Point(50,100), cv::Scalar(255, 0, 0));
    text.setFont(nullptr, &size2, nullptr, 0);
    text.putText(img, w_str, cv::Point(50,180), cv::Scalar(70, 90, 255));
    
    cv::resize(img, img, cv::Size(768, 512));
   // cv::imshow("demo", img);
    //cv::waitKey(0);
    cv::imwrite("./zhCN.jpg", img);
    return 0;
}
