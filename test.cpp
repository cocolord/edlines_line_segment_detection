#include <fstream>
#include<iostream>
#include <cstdint>
#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include <opencv2/line_descriptor.hpp>

#include "edlines.h"
// #include "../lsdlines/lsdlines.h"

unsigned char *pBuf;
int pWidth = 1024;
int pHeight = 768;
int scaleX = 1,scaleY = 1;
std::vector<line_float_t> lines;

int main()
{
    // boundingbox_t bbox = {0, 0,pWidth,pHeight};
    // pBuf = (unsigned char*) malloc(pWidth*pHeight*sizeof(unsigned char));
    // ifstream src("1.bmp", ifstream::binary);
    // src.seekg(0, src.end);
	// int length = src.tellg();
	// src.seekg(0, src.beg);
    // src.read(reinterpret_cast<char*>(pBuf),length);
    // EdgeDrawingLineDetector(pBuf,pWidth,pHeight,1.0,1.0,bbox,lines);
    // free(pBuf);
    cv::Mat image;
    // double 
	image = cv::imread("pic/3.bmp");
    // cv::imshow("img",image);
	cv::Mat gray_image = cv::imread("pic/2.png");
    cv::Mat temp;
	cv::cvtColor(image, gray_image, CV_BGR2GRAY);
    // cv::resize(gray_image,temp,cv::Size(512,384));
	int W = image.cols;
	int H = image.rows;
	int image_size = W * H;
	unsigned char* input = new unsigned char[image_size];
	memcpy(input, gray_image.data, image_size);
    // memcpy(input, temp.data, image_size);
	std::vector<line_float_t> Lines;
	boundingbox_t Bbox = { 0,0,W,H };
	float scalex =1.0;
	float scaley =1.0;
	int Flag = 0;
	// Flag = LsdLineDetector(input, W, H, scalex, scaley, Bbox, Lines);
    int64_t start_time = cv::getTickCount();
    Flag = EdgeDrawingLineDetector(input,W,H,scalex,scaley,Bbox,Lines);
	std::cout << Flag << std::endl;
	for (int i = 0; i < Lines.size(); i++)
	{
		cv::line(image, cv::Point(Lines[i].startx, Lines[i].starty), cv::Point(Lines[i].endx, Lines[i].endy), cv::Scalar(0, 0, 255), 2);
	}
    std::cout<< (cv::getTickCount() - start_time)*1000/cv::getTickFrequency()<<std::endl;
	cv::imshow("image1", image);
	cv::waitKey(0);
}