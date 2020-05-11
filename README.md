# Edge drawing line detector (edlines)

All OpenCV dependencies have been removed. Code could be run  independently.  
Edge drawing line detector(EDLines) with a scale in vertical and horizonal direction in boundingbox, respectively
# params 
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



## Usage
### if test with opencv lib
g++ -O3 -std=c++11 test.cpp array/Array/Array.cpp edlines/edlines.cpp c66/VXLIB_sobel_3x3_i8u_o16s_o16s_cn.c -o test -Iedlines/ -Ic66/ -I/array \`pkg-config --cflags --libs opencv\`
### if test without opencv dependencies
g++ -O3 -std=c++11 test.cpp array/Array/Array.cpp edlines/edlines.cpp c66/VXLIB_sobel_3x3_i8u_o16s_o16s_cn.c -o test -Iedlines/ -Ic66/ -I/array