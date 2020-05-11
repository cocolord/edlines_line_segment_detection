#include <fstream>
#include <iostream>
#include <cstdint>
#include<chrono>
#include "edlines/edlines.h"
#define TEST_EDLINE_WITH_OPENCV_LIB 0
#define TEST_OPENCV_EDLINE 0
#define TEST_EDLINE_WITHOUT_ANY_DEPENDENCIES 1 
#if TEST_EDLINE_WITH_OPENCV_LIB
	// include opencv to display pics
	#include<opencv2/core.hpp>
	#include<opencv2/imgproc.hpp>
	#include<opencv2/imgcodecs.hpp>
	#include<opencv2/highgui.hpp>
#endif

#if TEST_OPENCV_EDLINE
	#include <opencv2/line_descriptor.hpp>
#endif

#define MAX_LINE_BUFFER_SIZE 500
// define the resolution of image
const int pWidth = 1024;
const int pHeight = 768;
const int scaleX = 1,scaleY = 1;

std::vector<line_float_t> lines;

unsigned char *pBuf;

using namespace std;

int main()
{
	#if TEST_EDLINE_WITHOUT_ANY_DEPENDENCIES
		pBuf = (unsigned char*) malloc(pWidth*pHeight*sizeof(unsigned char));
		line_float_t *lBuf =  (line_float_t*)malloc(MAX_LINE_BUFFER_SIZE);
		FILE *infile = fopen("pics/2019-05-06_10-55-52_00016509.bmp","rb");
		FILE *outfile = fopen("pics/test.bmp","wb");

		if(infile == NULL){ // Check if file open was successful
			printf("File cannot be opened.\n");
		}
		
		boundingbox_t bbox = {0, 0,pWidth,pHeight};
		int64_t cycle_us = 1e6 / 34;
		auto start = std::chrono::high_resolution_clock::now();
		int flag = EdgeDrawingLineDetector(pBuf,pWidth,pHeight,1.0,1.0,bbox,lBuf);
		auto elapsed_0 = std::chrono::high_resolution_clock::now() - start;
		int64_t microseconds_0 = std::chrono::duration_cast<std::chrono::microseconds>(elapsed_0).count();
		cout<<"line detect runtime:"<<microseconds_0/1000.0<<"ms "<<endl;

		//0:ok; 1:error;
		printf("Succeed: %d\n",!flag);
		free(pBuf);
		fclose(infile);
		//如果不使用opencv库，目前不使用其他库对图像进行修改,outfile为空。
		fclose(outfile);

	#elif TEST_EDLINE_WITH_OPENCV_LIB
		/* 用opencv测试 */
		cv::Mat image;
		image = cv::imread("pics/2019-05-06_10-55-52_00016518.bmp");
		// cv::cvtColor(image, gray_image, CV_BGR2GRAY);
		int W = image.cols *0.5;
		int H = image.rows *0.5;
		int image_size = W * H ;
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
		boundingbox_t Bbox = { 0,100,W,H-100 };
		float scalex = 1.0;
		float scaley = 1.0;
		int Flag = 0;
		//Run Edline

		LineDescriptor * ld  = new LineDescriptor; 
		LineSet Lineset;

		// unsigned char* input = new unsigned char[image_size];
		image_int8u_p int8u_image = new image_int8u_s[1];
		int8u_image->data = input;
		int8u_image->xsize = W;
		int8u_image->ysize = H;
		ld->Run(1,1,Bbox,int8u_image,Lineset);
		for (int i = 0; i < Lineset.size(); i++)
		{
			cv::line(temp, cv::Point(Lineset[i].startPointX, Lineset[i].startPointY), cv::Point(Lineset[i].endPointX, Lineset[i].endPointY), cv::Scalar(0, 0, 255), 2);
		}
		cv::imshow("image1", temp);
		int64_t cycle_us = 1e6 / 34;
		auto start = std::chrono::high_resolution_clock::now();
		line_float_t lines_buf[MAX_LINE_BUFFER_SIZE] = {0,0,0,0};/*[MAX_LINE_BUFFER_SIZE];*/
		Flag = EdgeDrawingLineDetector(input,W,H,scalex,scaley,Bbox,lines_buf);
		auto elapsed_0 = std::chrono::high_resolution_clock::now() - start;
		int64_t microseconds_0 = std::chrono::duration_cast<std::chrono::microseconds>(elapsed_0).count();
		cout<<"detect:"<<microseconds_0/1000.0<<"ms "<<endl;
		auto bd = cv::line_descriptor::BinaryDescriptor::createBinaryDescriptor();
		std::vector<cv::line_descriptor::KeyLine> lines;
		auto roi_vp  = cv::Rect(0, 100, W, H-100);
		cv::Mat mask_vp_ = cv::Mat::zeros(H, W, CV_8UC1);
		mask_vp_(roi_vp).setTo(255);

		cv::Mat mask = /*mask_ar_ &*/ mask_vp_;
		cv::Mat src_roi(img_raw_.size(), CV_8UC1, cv::Scalar(0));

		img_raw_.copyTo(src_roi, mask);
		elapsed_0 = std::chrono::high_resolution_clock::now() - start;
		bd->detect(src_roi, lines, mask);
		microseconds_0 = std::chrono::duration_cast<std::chrono::microseconds>(elapsed_0).count();
		cout<<"OPENCV detect:"<<microseconds_0/1000.0<<"ms "<<endl;
		
		std::cout << Flag << std::endl;

		cv::Mat temp1;
		temp.copyTo(temp1);
		for (int i = 0; i < lines.size(); i++)
		{
			cv::line(temp1, cv::Point(lines[i].startPointX, lines[i].startPointY), cv::Point(lines[i].endPointX, lines[i].endPointY), cv::Scalar(0, 0, 255), 2);
		}
		cv::imshow("image opencv",temp1);

		for (int i = 0; i < sizeof(lines_buf)/sizeof(line_float_t); i++)
		{
			cv::line(temp, cv::Point(lines_buf[i].startx, lines_buf[i].starty), cv::Point(lines_buf[i].endx, lines_buf[i].endy), cv::Scalar(0, 0, 255), 2);
		}
		cv::imshow("image1", temp);
		cv::waitKey(0);
	#endif

	return 0;

}