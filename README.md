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
To do list:
1. DSP中的sobel函数替换源代码中的sobel函数
2. 车道线检测中所有opencv函数替换
3. 输入图像已经改成了一维数组
