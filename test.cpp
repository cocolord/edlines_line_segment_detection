#include <fstream>
#include <iostream>
#include <cstdint>
#include<chrono>
#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include <opencv2/line_descriptor.hpp>
#include "edlines.h"


int pWidth = 1024;
int pHeight = 768;
int scaleX = 1,scaleY = 1;
std::vector<line_float_t> lines;
unsigned char *pBuf;
using namespace std;
typedef struct
{
    unsigned char b;
    unsigned char g;
    unsigned char r;
}RGB;

int main()
{
	// unsigned char header[54];
    // unsigned char color_table[1024];
	// pBuf = (unsigned char*) malloc(pWidth*pHeight*sizeof(unsigned char));
	// FILE *infile = fopen("/home/dong/Downloads/line_detector-master/2019-05-06_10-55-52_00016509.bmp","rb");
	// FILE *outfile = fopen("/home/dong/Downloads/line_detector-master/testg.bmp","wb");

	// if(infile == NULL){ // Check if file open was successful
    //     printf("File cannot be opened.\n");
    // }
	// for(int i = 0; i<54; i++){
    //     header[i] = getc(infile); // getc is get single character
    // }
	// // Store width, height, and bit depth from the header array.
    // int width = *(int *)&header[18]; // The header array is an array of chars, so we need to cast the values and store them as ints.
    // int height = *(int *)&header[22];
	// int size = height * width;
	// int bit_depth = *(int *)&header[28];
	// std::cout<<"bit_depth "<<bit_depth<<std::endl;

    // // Not all bitmaps have a color table, only indexed color images.  We need to check before we read in the color table.
    // if (bit_depth <= 8){ // If the image is either grayscale or indexed color.
    //     fread(color_table, sizeof(unsigned char), 1024, infile); // Read in color table into our color_table array.
    // }

	// // fread(pBuf, sizeof(unsigned char), size,infile);
	// //转灰度图
	// if(bit_depth==24){
	// 	RGB *img = new RGB[size];
	// 	// RGB img[size];
	// 	fread(img, sizeof(RGB), size,infile);
	// 	cout<<"size of raw img"<<sizeof(img)/sizeof(RGB)<<endl;
	// 	for(int i = 0;i<size;i++)
	// 		pBuf[i]=img[i].r;
	// 	cout<<"size "<< size<<endl;
	// 	cout<<"size of pBuf"<<strlen(pBuf)<<endl;
	// 	// header[28] = 8;
	// 	fwrite(&header,sizeof(header),1,outfile);
	// 	//转灰度图之后缺少一个调色板文件
	// 	fwrite(pBuf, sizeof(unsigned char), size, outfile);
	// }
	
	// cout<<"size of uchar"<<sizeof(unsigned char)<<endl;
    // boundingbox_t bbox = {0, 0,width,height};
	// int64_t cycle_us = 1e6 / 34;
	// auto start = std::chrono::high_resolution_clock::now();
	// int flag = EdgeDrawingLineDetector(pBuf,width,height,1.0,1.0,bbox,lines);
	// auto elapsed_0 = std::chrono::high_resolution_clock::now() - start;
	// int64_t microseconds_0 = std::chrono::duration_cast<std::chrono::microseconds>(elapsed_0).count();
	// cout<<"detect:"<<microseconds_0/1000.0<<"ms "<<endl;

	// std::cout<<lines.size()<<std::endl;
	// //0:ok; 1:error;
	// std::cout << flag << std::endl;
    // free(pBuf);
	// fclose(infile);
	// fclose(outfile);
	// return 0;


	/* 用opencv测试 */
    cv::Mat image;
	image = cv::imread("/home/dong/Downloads/line_detector-master/2019-05-06_10-55-52_00016509.bmp");
	// cv::cvtColor(image, gray_image, CV_BGR2GRAY);
	int W = image.cols;
	int H = image.rows;
	int image_size = W * H  * 0.5 * 0.5;
	//resize 
	unsigned char* input = new unsigned char[image_size];
	cv::Mat temp;
	cv::resize(image, temp, cv::Size(image.cols*0.5,image.rows*0.5));
	//3->1 channel
	cv::Mat img_hsv[3];
	cv::split(temp, img_hsv);
	cv::Mat img_raw_ = img_hsv[2]/* - img_hsv[1] + img_hsv[0]*/;
	//init params
	memcpy(input,img_raw_.data, image_size);
	std::vector<line_float_t> Lines;
	//左上角是坐标原点
	//用法和cv::rect一样
	boundingbox_t Bbox = { 0,100,W*0.5,H*0.5-100 };
	float scalex =1.0;
	float scaley =1.0;
	int Flag = 0;
	//Run Edline
	int64_t cycle_us = 1e6 / 34;
	auto start = std::chrono::high_resolution_clock::now();
    Flag = EdgeDrawingLineDetector(input,W*0.5,H*0.5,scalex,scaley,Bbox,Lines);
	auto elapsed_0 = std::chrono::high_resolution_clock::now() - start;
	int64_t microseconds_0 = std::chrono::duration_cast<std::chrono::microseconds>(elapsed_0).count();
	cout<<"detect:"<<microseconds_0/1000.0<<"ms "<<endl;
	std::cout << Flag << std::endl;
	for (int i = 0; i < Lines.size(); i++)
	{
		cv::line(temp, cv::Point(Lines[i].startx, Lines[i].starty), cv::Point(Lines[i].endx, Lines[i].endy), cv::Scalar(0, 0, 255), 2);
	}
	cv::imshow("image1", temp);
	std::cout<<Lines.size()<<std::endl;
	cv::waitKey(0);
	return 0;
	/* opencv 测试*/
}