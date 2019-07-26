
#ifndef __EDGE_DRAWING_LINE_H_
#define __EDGE_DRAWING_LINE_H_

#include <vector>
#include<cmath>
#include<stdint.h>
#include<string.h>
#include<float.h>
#include "/home/dong/PROCESSOR_SDK_VISION_03_06_00_00/ti_components/algorithms/vxlib_c66x_1_1_3_0/packages/ti/vxlib/src/vx/VXLIB_sobel_3x3_i8u_o16s_o16s/VXLIB_sobel_3x3_i8u_o16s_o16s_cn.h"

// #include "image_defines.h"
// #ifdef __cplusplus
// extern "C" {
// #endif


typedef struct
{
	int x;
	int y;
	int width;
	int height;
}boundingbox_t;


typedef struct
{
	float startx;
	float starty;
	float endx;
	float endy;
}line_float_t;


/*
@function    EdgeDrawingLineDetector
@param       [in]      src:						  image,single channel
@param       [in]      w:                         width of image
@param       [in]      h:                         height of image
@param       [in]      scaleX:                    downscale factor in X-axis
@param       [in]      scaleY:                    downscale factor in Y-axis
@param       [in]      bbox:                      boundingbox to detect
@param       [in/out]  lines:                     result
@return��										  0:ok; 1:error
@brief��

*/
int EdgeDrawingLineDetector(unsigned char *src, int w, int h,
	float scaleX, float scaleY, boundingbox_t bbox, std::vector<line_float_t> &lines);


//
//#ifdef __cplusplus
//}
//#endif

#endif