
#ifndef __EDGE_DRAWING_LINE_H_
#define __EDGE_DRAWING_LINE_H_

#include <vector>
#include<cmath>
#include<stdint.h>
#include<string.h>
#include<float.h>
#include<cstdio>
#include "VXLIB_sobel_3x3_i8u_o16s_o16s_cn.h"
#include "../array/Array.h"
// #include "image_defines.h"
// #ifdef __cplusplus
// extern "C" {
// #endif

#ifndef PI
#define PI (3.1415926535897932384626433832795)
#endif

#ifndef ZERO
#define ZERO (0)
#endif

#ifndef ROUND
#define ROUND (0.5F)
#endif

#ifndef MIN
#define MIN(a,b)  ((a) > (b) ? (b) : (a))
#endif

#ifndef MAX
#define MAX(a,b)  ((a) < (b) ? (b) : (a))
#endif

//if |dx|<|dy|;
#ifndef Horizontal
#define Horizontal (255)
#endif
//if |dy|<=|dx|;
#ifndef Vertical
#define Vertical    0
#endif

#ifndef UpDir
#define UpDir       1
#endif

#ifndef RightDir
#define RightDir    2
#endif

#ifndef DownDir
#define DownDir     3
#endif

#ifndef LeftDir
#define LeftDir     4
#endif

#ifndef TryTime
#define TryTime     6
#endif

#ifndef SkipEdgePoint
#define SkipEdgePoint 2
#endif

#ifndef RELATIVE_ERROR_FACTOR
#define RELATIVE_ERROR_FACTOR   100.0f
#endif

#ifndef M_LN10
#define M_LN10   2.302585093f 
#endif

#ifndef log_gamma
#define log_gamma(x)    ((x)>15.f?log_gamma_windschitl(x):log_gamma_lanczos(x))
#endif

#ifndef SalienceScale
#define SalienceScale 0.9F//0.9
#endif

#ifndef ONE
#define ONE (1)
#endif

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

typedef struct
{
	float u;            //col of pixel
	float v;            //row of pixel
}pixel_float_t;
typedef struct
{
	unsigned int u;            //col of pixel
	unsigned int v;            //row of pixel
}pixel_uint_t;
typedef struct image_int8u_s
{
	unsigned char * data;
	unsigned int xsize, ysize;
} *image_int8u_p;

typedef struct image_int16s_s
{
	short * data;
	unsigned int xsize, ysize;
} *image_int16s_p;

typedef struct image_int32s_s
{
	int * data;
	unsigned int xsize, ysize;
} *image_int32s_p;

typedef struct image_float_s
{
	float * data;
	unsigned int xsize, ysize;
} *image_float_p;

struct SingleLineInfo
{
	/*endPoints, the coordinate origin is the top-left corner of the original image.
	*startPointX = sPointInScaledX * (factor)^scaledCount;	*/
	float startPointX;
	float startPointY;
	float endPointX;
	float endPointY;
	//endPoints, the coordinate origin is the top-left corner of the scaled image.
	float sPointInScaledX;
	float sPointInScaledY;
	float ePointInScaledX;
	float ePointInScaledY;
	//direction of a line, the angle between positive line direction (dark side is in the left) and positive X axis.
	float direction;
	//the summation of gradient magnitudes of pixels on scaled lines
	float salience;
	//the length of line
	float lineLength;
	//number of pixels
	unsigned int numOfPixels;
	//the scaled which this line is detected
	unsigned int scaledCount;
	//the decriptor of line
	std::vector<float> descriptor;
};

// Specifies a vector of lines.

//用结构题数组代替
typedef std::vector<SingleLineInfo> LineSet;

// typedef std::vector<LineSet> ScaleLineSet;//each element in LineSet is a vector of lines which corresponds the same line detected in different scaled images.

 
struct EDLineParam{

	float gradientThreshold;
	float anchorThreshold;
	int scanIntervals;
	int minLineLen;
	float lineFitErrThreshold;
};

struct ScaledLine{
	unsigned int scaledCount;//the scaled which this line is detected
	unsigned int lineIDInScaled;//the line ID in that scaled image
	unsigned int lineIDInScaleLineVec;//the line ID in Scale line vector
	float lineLength; //the length of line in original image scale
};


struct Pixel{
	unsigned int x;//X coordinate
	unsigned int y;//Y coordinate
};
struct EdgeChains{
	std::vector<unsigned int> xCors;//all the x coordinates of edge points
	std::vector<unsigned int> yCors;//all the y coordinates of edge points
	std::vector<unsigned int> sId;  //the start index of each edge in the coordinate arrays
	unsigned int numOfEdges;//the number of edges whose length are larger than minLineLen; numOfEdges < sId.size;
};
struct LineChains{
	std::vector<unsigned int> xCors;//all the x coordinates of line points
	std::vector<unsigned int> yCors;//all the y coordinates of line points
	std::vector<unsigned int> sId;  //the start index of each line in the coordinate arrays
	unsigned int numOfLines;//the number of lines whose length are larger than minLineLen; numOfLines < sId.size;
};


class EDLineDetector
{
public:
	EDLineDetector();
	EDLineDetector(EDLineParam param);
	~EDLineDetector();

public:

	/*extract edges from image
	*image:    In, gray image;
	*edges:    Out, store the edges, each edge is a pixel chain
	*smoothed: In, flag to mark whether the image has already been smoothed by Gaussian filter.
	*return 1: error happen
	*/
	int EdgeDrawing(image_int8u_p image, EdgeChains &edgeChains, bool smoothed = false);

	/*extract lines from image
	*image:    In, gray image;
	*lines:    Out, store the extracted lines,
	*smoothed: In, flag to mark whether the image has already been smoothed by Gaussian filter.
	*return 1: error happen
	*/
	int EDline(image_int8u_p image, LineChains &lines, bool smoothed = false);

	/*extract line from image, and store them*/
	int EDline(image_int8u_p image, bool smoothed = false);


public:
	image_int16s_p dxImg_;
	image_int16s_p dyImg_;
	//store the gradient image without threshold;
	image_int16s_p gImgWO_;

	LineChains lines_; //store the detected line chains;
	//store the line Equation coefficients, vec3=[w1,w2,w3] for line w1*x + w2*y + w3=0;
	std::vector<Array<float, 3> > lineEquations_;
	//store the line endpoints, [x1,y1,x2,y3]
	std::vector<Array<float, 4> > lineEndpoints_;
	//store the line direction
	std::vector<float>  lineDirection_;
	//store the line salience, which is the summation of gradients of pixels on line
	std::vector<float>  lineSalience_;
	unsigned int imageWidth;
	unsigned int imageHeight;

private:
	void InitEDLine_();
	/*For an input edge chain, find the best fit line, the default chain length is minLineLen_
	*xCors:  In, pointer to the X coordinates of pixel chain;
	*yCors:  In, pointer to the Y coordinates of pixel chain;
	*offsetS:In, start index of this chain in array;
	*lineEquation: Out, [a,b] which are the coefficient of lines y=ax+b(horizontal) or x=ay+b(vertical);
	*return:  line fit error; 1:error happens;
	*/
	float LeastSquaresLineFit_(unsigned int *xCors, unsigned int *yCors,
		unsigned int offsetS, Array<float, 2> &lineEquation);
	/*For an input pixel chain, find the best fit line. Only do the update based on new points.
	*For A*x=v,  Least square estimation of x = Inv(A^T * A) * (A^T * v);
	*If some new observations are added, i.e, [A; A'] * x = [v; v'],
	*then x' = Inv(A^T * A + (A')^T * A') * (A^T * v + (A')^T * v');
	*xCors:  In, pointer to the X coordinates of pixel chain;
	*yCors:  In, pointer to the Y coordinates of pixel chain;
	*offsetS:In, start index of this chain in array;
	*newOffsetS: In, start index of extended part;
	*offsetE:In, end index of this chain in array;
	*lineEquation: Out, [a,b] which are the coefficient of lines y=ax+b(horizontal) or x=ay+b(vertical);
	*return:  line fit error; 1:error happens;
	*/
	float LeastSquaresLineFit_(unsigned int *xCors, unsigned int *yCors,
		unsigned int offsetS, unsigned int newOffsetS,
		unsigned int offsetE, Array<float, 2> &lineEquation);
	/* Validate line based on the Helmholtz principle, which basically states that
	* for a structure to be perceptually meaningful, the expectation of this structure
	* by chance must be very low.
	*/
	bool LineValidation_(unsigned int *xCors, unsigned int *yCors,
		unsigned int offsetS, unsigned int offsetE,
		Array<float, 3> &lineEquation, float &direction);


private:

	bool bValidate_;//flag to decide whether line will be validated

	/*the threshold of pixel gradient magnitude.
	*Only those pixel whose gradient magnitude are larger than this threshold will be
	*taken as possible edge points. Default value is 36*/
	short gradienThreshold_;
	/*If the pixel's gradient value is bigger than both of its neighbors by a
	*certain threshold (ANCHOR_THRESHOLD), the pixel is marked to be an anchor.
	*Default value is 8*/
	unsigned char anchorThreshold_;
	/*anchor testing can be performed at different scan intervals, i.e.,
	*every row/column, every second row/column etc.
	*Default value is 2*/
	unsigned int scanIntervals_;
	int minLineLen_;//minimal acceptable line length

	/*This type of storage order is because of the order of edge detection process.
	*For each edge, start from one anchor point, first go right, then go left or first go down, then go up*/
	unsigned int *pFirstPartEdgeX_;//store the X coordinates of the first part of the pixels for chains
	unsigned int *pFirstPartEdgeY_;//store the Y coordinates of the first part of the pixels for chains
	unsigned int *pFirstPartEdgeS_;//store the start index of every edge chain in the first part arrays
	unsigned int *pSecondPartEdgeX_;//store the X coordinates of the second part of the pixels for chains
	unsigned int *pSecondPartEdgeY_;//store the Y coordinates of the second part of the pixels for chains
	unsigned int *pSecondPartEdgeS_;//store the start index of every edge chain in the second part arrays
	unsigned int *pAnchorX_;//store the X coordinates of anchors
	unsigned int *pAnchorY_;//store the Y coordinates of anchors

	image_int8u_p edgeImage_;

	float lineFitErrThreshold_;

	//store the gradient image;
	image_int16s_p gImg_;
	//store the direction image
	image_int8u_p dirImg_;

	float logNT_;

	image_float_p ATA;	//the previous matrix of A^T * A;
	image_float_p ATV;	//the previous vector of A^T * V;
	image_float_p fitMatT;	//the matrix used in line fit function;
	image_float_p fitVec;	//the vector used in line fit function;
	image_float_p tempMatLineFit;	//the matrix used in line fit function;
	image_float_p tempVecLineFit;	//the vector used in line fit function;


	/** Compare floats by relative error.*/
	static int float_equal(float a, float b)
	{
		float abs_diff, aa, bb, abs_max;
		/* trivial case */
		if (a == b) return true;
		abs_diff = fabs(a - b);
		aa = fabs(a);
		bb = fabs(b);
		abs_max = aa > bb ? aa : bb;

		if (abs_max < FLT_MIN) abs_max = FLT_MIN;
		/* equal if relative error <= factor x eps */
		return (abs_diff / abs_max) <= (RELATIVE_ERROR_FACTOR * DBL_EPSILON);
	}
	/** Computes the natural logarithm of the absolute value of
	the gamma function of x using the Lanczos approximation.*/
	static float log_gamma_lanczos(float x)
	{
		static float q[7] = { 75122.6331530f, 80916.6278952f, 36308.2951477f,
			8687.24529705f, 1168.92649479f, 83.8676043424f,
			2.50662827511f };
		float a = (x + 0.5f) * log(x + 5.5f) - (x + 5.5f);
		float b = 0.f;
		int n;
		for (n = 0; n<7; n++){
			a -= log(x + (float)n);
			b += q[n] * pow(x, (float)n);
		}
		return a + log(b);
	}
	/** Computes the natural logarithm of the absolute value of
	the gamma function of x using Windschitl method.*/
	static float log_gamma_windschitl(float x)
	{
		return 0.918938533204673f + (x - 0.5f)*log(x) - x
			+ 0.5f*x*log(x*sinh(1 / x) + 1 / (810.f*pow(x, 6.f)));
	}
	/** Computes -log10(NFA).*/
	static float nfa(int n, int 	k, float p, float  logNT)
	{
		float tolerance = 0.1f;       /* an error of 10% in the result is accepted */
		float log1term, term, bin_term, mult_term, bin_tail, err, p_term;
		int i;

		/* check parameters */
		if (n<0 || k<0 || k>n || p <= 0.f || p >= 1.f){
			printf("nfa: wrong n, k or p values.\n");
			exit(1);
		}
		/* trivial cases */
		if (n == 0 || k == 0) return -logNT;
		if (n == k) return -logNT - (float)n * log10(p);

		/* probability term */
		p_term = p / (1.f - p);

		/* compute the first term of the series */
		log1term = log_gamma((float)n + 1.f) - log_gamma((float)k + 1.f)
			- log_gamma((float)(n - k) + 1.f)
			+ (float)k * log(p) + (float)(n - k) * log(1.f - p);
		term = exp(log1term);

		/* in some cases no more computations are needed */
		if (float_equal(term, 0.f)){  /* the first term is almost zero */
			if ((float)k > (float)n * p)     /* at begin or end of the tail?  */
				return -log1term / M_LN10 - logNT;  /* end: use just the first term  */
			else
				return -logNT;                      /* begin: the tail is roughly 1  */
		}

		/* compute more terms if needed */
		bin_tail = term;
		for (i = k + 1; i <= n; i++){

			bin_term = (float)(n - i + 1) / (float)i;
			mult_term = bin_term * p_term;
			term *= mult_term;
			bin_tail += term;
			if (bin_term<1.f){

				err = term * ((1.f - pow(mult_term, (float)(n - i + 1)))
					/ (1.f - mult_term) - 1.f);

				if (err < tolerance * fabs(-log10(bin_tail) - logNT) * bin_tail) break;
			}
		}
		return -log10(bin_tail) - logNT;
	}
};


/* This class is used to generate the line descriptors from multi-scale images  */
class LineDescriptor
{
public:
	LineDescriptor();
	LineDescriptor(unsigned int numOfBand, unsigned int widthOfBand);
	~LineDescriptor();

public:

	/* Interface.*/
	int Run(float scaleX, float scaleY, boundingbox_t bbox,
		image_int8u_p image, LineSet & keyLines);

private:

	/*This function is used to detect lines from multi-scale images.*/
	int ScaledKeyLines(image_int8u_p image, LineSet &keyLines);

	/*This function is used to get numbers of pixels in a line from image.*/
	int GetLinePixelsNums(float startX, float startY, float endX, float endY);

	/*This function is used to get information of lines from downsampled image.*/
	void InverseGaussianSamplerLines(pixel_float_t gs_scale, LineSet &keyLines);



private:

	/*For each scaled of image, we define an EDLineDetector, because we can get gradient images (dxImg, dyImg, gImg)
	*from the EDLineDetector class without extra computation cost. Another reason is that, if we use
	*a single EDLineDetector to detect lines in different scaled of images, then we need to allocate and release
	*memory for gradient images (dxImg, dyImg, gImg) repeatedly for their varying size*/
	std::vector<EDLineDetector*> edLineVec_;

	//int ksize_; //the size of Gaussian kernel: ksize X ksize, default value is 5.

	unsigned int  numOfBand_;//the number of band used to compute line descriptor
	unsigned int  widthOfBand_;//the width of band;
	std::vector<float> gaussCoefL_;//the local gaussian coefficient apply to the orthogonal line direction within each band;
	std::vector<float> gaussCoefG_;//the global gaussian coefficient apply to each Row within line support region



};
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
	float scaleX, float scaleY, boundingbox_t bbox, line_float_t* lines_buf);


//
//#ifdef __cplusplus
//}
//#endif

#endif