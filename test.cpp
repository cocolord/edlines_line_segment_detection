#include <fstream>
#include <iostream>
#include <cstdint>
// #include<opencv2/core.hpp>
// #include<opencv2/imgproc.hpp>
// #include<opencv2/imgcodecs.hpp>
// #include<opencv2/highgui.hpp>
// #include <opencv2/line_descriptor.hpp>
#include "edlines.h"

int pWidth = 1024;
int pHeight = 768;
int scaleX = 1,scaleY = 1;
std::vector<line_float_t> lines;

unsigned char *pBuf;
int main()
{
    boundingbox_t bbox = {0, 0,pWidth,pHeight};
    pBuf = (unsigned char*) malloc(pWidth*pHeight*sizeof(unsigned char));
    std::ifstream src("1.bmp", std::ifstream::binary);
    src.seekg(0, src.end);
	int length = src.tellg();
	src.seekg(0, src.beg);
    src.read((char *)(pBuf),pWidth*pHeight);   
	// for(int i = 0;i<pWidth*pHeight;i++) std::cout<<pBuf[i]<<" ";
	int flag = EdgeDrawingLineDetector(pBuf,pWidth,pHeight,1.0,1.0,bbox,lines);
	std::cout<<lines.size()<<std::endl;
	//0:ok; 1:error;
	std::cout << flag << std::endl;
    free(pBuf);
	return 0;


	// /* 用opencv测试 */
    // cv::Mat image;
	// image = cv::imread("pic/3.bmp");
    // // cv::imshow("img",image);
	// cv::Mat gray_image ;
	// cv::cvtColor(image, gray_image, CV_BGR2GRAY);
	// int W = image.cols;
	// int H = image.rows;
	// int image_size = W * H;
	// unsigned char* input = new unsigned char[image_size];
	// memcpy(input, gray_image.data, image_size);
	// std::vector<line_float_t> Lines;
	// boundingbox_t Bbox = { 0,0,W,H };
	// float scalex =1.0;
	// float scaley =1.0;
	// int Flag = 0;
	// // Flag = LsdLineDetector(input, W, H, scalex, scaley, Bbox, Lines);
    // int64_t start_time = cv::getTickCount();
    // Flag = EdgeDrawingLineDetector(input,W,H,scalex,scaley,Bbox,Lines);
	// std::cout << Flag << std::endl;
	// for (int i = 0; i < Lines.size(); i++)
	// {
	// 	cv::line(image, cv::Point(Lines[i].startx, Lines[i].starty), cv::Point(Lines[i].endx, Lines[i].endy), cv::Scalar(0, 0, 255), 2);
	// }
    // std::cout<< (cv::getTickCount() - start_time)*1000/cv::getTickFrequency()<<std::endl;
	// cv::imshow("image1", image);
	// std::cout<<Lines.size()<<std::endl;
	// cv::waitKey(0);
}