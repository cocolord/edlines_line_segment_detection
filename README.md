__edge drawing line detector (edlines)__

__All original dependencies have been removed. Code could be run  independently.__

edge drawing line detector(EDLines) with a scale in vertical and horizonal direction in boundingbox, respectively

__EDLines__ Simple Interface with Scale in Boundingbox

    @param src         				image,single channel.

    @param w           				width of image.

    @param h           				height of image.

    @param scaleX      				downscale factor in X-axis.

    @param scaleY      				downscale factor in Y-axis.

    @param bbox        				boundingbox to detect.

    @param lines      				result.

    @return            				0:ok; 1:error
                       
int __EdgeDrawingLineDetector__(unsigned char *src, int w, int h,float scaleX, scaleY, boundingbox_t bbox, std::vector<line_float_t> &lines);



#
g++ -O3 -std=c++11 test.cpp array/Array/Array.cpp edlines/edlines.cpp c66/VXLIB_sobel_3x3_i8u_o16s_o16s_cn.c -o test -Iedlines/ -Ic66/ -I/array \`pkg-config --cflags --libs opencv\`
#
To do list:
1. C++ STL 用标准C替换

2. 在ccs上测试



Finished List:
1. 图像流已经换成了一维数组

2. DSP中的sobel函数替换源代码中的sobel函数

3. 替换了STL的array

4. OPENCV edline函数的效果对比

5. gaussian_bbox 的参数需要调