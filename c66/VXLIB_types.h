/*******************************************************************************
**+--------------------------------------------------------------------------+**
**|                            ****                                          |**
**|                            ****                                          |**
**|                            ******o***                                    |**
**|                      ********_///_****                                   |**
**|                      ***** /_//_/ ****                                   |**
**|                       ** ** (__/ ****                                    |**
**|                           *********                                      |**
**|                            ****                                          |**
**|                            ***                                           |**
**|                                                                          |**
**|        Copyright (c) 2016 - 2017 Texas Instruments Incorporated          |**
**|                                                                          |**
**|              All rights reserved not granted herein.                     |**
**|                                                                          |**
**|                         Limited License.                                 |**
**|                                                                          |**
**|  Texas Instruments Incorporated grants a world-wide, royalty-free,       |**
**|  non-exclusive license under copyrights and patents it now or            |**
**|  hereafter owns or controls to make, have made, use, import, offer to    |**
**|  sell and sell ("Utilize") this software subject to the terms herein.    |**
**|  With respect to the foregoing patent license, such license is granted   |**
**|  solely to the extent that any such patent is necessary to Utilize the   |**
**|  software alone.  The patent license shall not apply to any              |**
**|  combinations which include this software, other than combinations       |**
**|  with devices manufactured by or for TI ("TI Devices").  No hardware     |**
**|  patent is licensed hereunder.                                           |**
**|                                                                          |**
**|  Redistributions must preserve existing copyright notices and            |**
**|  reproduce this license (including the above copyright notice and the    |**
**|  disclaimer and (if applicable) source code license limitations below)   |**
**|  in the documentation and/or other materials provided with the           |**
**|  distribution                                                            |**
**|                                                                          |**
**|  Redistribution and use in binary form, without modification, are        |**
**|  permitted provided that the following conditions are met:               |**
**|                                                                          |**
**|    *  No reverse engineering, decompilation, or disassembly of this      |**
**|  software is permitted with respect to any software provided in binary   |**
**|  form.                                                                   |**
**|                                                                          |**
**|    *  any redistribution and use are licensed by TI for use only with    |**
**|  TI Devices.                                                             |**
**|                                                                          |**
**|    *  Nothing shall obligate TI to provide you with source code for      |**
**|  the software licensed and provided to you in object code.               |**
**|                                                                          |**
**|  If software source code is provided to you, modification and            |**
**|  redistribution of the source code are permitted provided that the       |**
**|  following conditions are met:                                           |**
**|                                                                          |**
**|    *  any redistribution and use of the source code, including any       |**
**|  resulting derivative works, are licensed by TI for use only with TI     |**
**|  Devices.                                                                |**
**|                                                                          |**
**|    *  any redistribution and use of any object code compiled from the    |**
**|  source code and any resulting derivative works, are licensed by TI      |**
**|  for use only with TI Devices.                                           |**
**|                                                                          |**
**|  Neither the name of Texas Instruments Incorporated nor the names of     |**
**|  its suppliers may be used to endorse or promote products derived from   |**
**|  this software without specific prior written permission.                |**
**|                                                                          |**
**|  DISCLAIMER.                                                             |**
**|                                                                          |**
**|  THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY      |**
**|  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE       |**
**|  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR      |**
**|  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL TI AND TI'S LICENSORS BE      |**
**|  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR     |**
**|  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF    |**
**|  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR         |**
**|  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,   |**
**|  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE    |**
**|  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,       |**
**|  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                      |**
**+--------------------------------------------------------------------------+**
*******************************************************************************/

#ifndef _COMMON_VXLIB_TYPES_H_
#define _COMMON_VXLIB_TYPES_H_ 1

/* This header is needed to be included in OpenCL programs which link
 * against VXLIB, but OpenCL doesn't need the following headers */
#ifndef __OPENCL_VERSION__
#include <stdlib.h>
#include <stdint.h>
#include "TI_platforms.h"
#include "VXLIB_bufParams.h"
#endif

typedef double VXLIB_D64;          /*!< Double precision floating point */
typedef float VXLIB_F32;           /*!< Single precision floating point */

/*! \brief The enumeration of all status codes. */
typedef enum {
    VXLIB_SUCCESS = 0,                          /*!< No Error */
    VXLIB_ERR_FAILURE = 1,                      /*!< Unspecified Error */
    VXLIB_ERR_INVALID_VALUE = 2,                /*!< Invalid parameter value */
    VXLIB_ERR_INVALID_TYPE = 3,                 /*!< Invalid parameter type (VXLIB_data_type_e data_type) */
    VXLIB_ERR_INVALID_DIMENSION = 4,            /*!< Dimension parameter (width/height) is too big/small */
    VXLIB_ERR_NULL_POINTER = 5,                 /*!< Unsupported null pointer condition */
    VXLIB_ERR_NOT_IMPLEMENTED = 6,              /*!< Parameter configuration is not supported/implemented */
    VXLIB_ERR_NOT_EQUAL_WIDTH_STRIDE = 7,       /*!< Stride should be equal to width * element size */
    VXLIB_ERR_NOT_ALIGNED_PTRS_STRIDES = 8,     /*!< Pointers and stride values are not aligned to documented value  */
    VXLIB_ERR_NOT_ALIGNED_WIDTHS = 9,           /*!< Width values are not aligned to documented value */
    VXLIB_ERR_BUFFER_TOO_SMALL = 10,            /*!< Buffer size not large enough */
    VXLIB_ERROR_MAX = 11
} VXLIB_STATUS_NAME;

typedef VXLIB_STATUS_NAME VXLIB_STATUS;    /*!< return value for all VXLIB functions */

#define VXLIB_min(x, y) (((x) < (y)) ? (x) : (y)) /*!< A macro to return the minimum of 2 values. */
#define VXLIB_max(x, y) (((x) < (y)) ? (y) : (x)) /*!< A macro to return the maximum of 2 values. */

#define VXLIB_CONVERT_POLICY_WRAP 0U        /*!< Defines overflow policy to wrap */
#define VXLIB_CONVERT_POLICY_SATURATE 1U    /*!< Defines overflow policy to saturate */

#define VXLIB_INTERPOLATION_TYPE_NEAREST_NEIGHBOR 0U    /*!< Defines interpolation type to nearest neighbor interpolation */
#define VXLIB_INTERPOLATION_TYPE_BILINEAR 1U            /*!< Defines interpolation type to bilinear interpolation */
#define VXLIB_INTERPOLATION_TYPE_AREA 2U                /*!< Defines interpolation type to area interpolation */


#define VXLIB_COLOR_SPACE_NONE 0x0          /*!< Defines no specific color space */
#define VXLIB_COLOR_SPACE_BT601_525 0x1     /*!< Defines BT601_525 color space */
#define VXLIB_COLOR_SPACE_BT601_625 0x2     /*!< Defines BT601_625 color space */
#define VXLIB_COLOR_SPACE_BT709 0x3         /*!< Defines BT709 color space */
#define VXLIB_COLOR_SPACE_DEFAULT 0x3       /*!< Defines default color space */

#define VXLIB_TERM_ITERATIONS 0U    /*!< Defines termination criteria as number of iterations */
#define VXLIB_TERM_EPSILON 1U       /*!< Defines termination criteria as epsilon */
#define VXLIB_TERM_BOTH 2U          /*!< Defines termination criteria as the earlier of number of iterations or epsilon */

#endif /* _COMMON_VXLIB_TYPES_H_ */

/* ======================================================================== */
/*  End of file:  VXLIB_types.h                                             */
/* ======================================================================== */

