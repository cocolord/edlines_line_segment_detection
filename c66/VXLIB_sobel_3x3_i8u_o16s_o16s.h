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

#ifndef VXLIB_SOBEL_3X3_I8U_O16S_O16S_H_
#define VXLIB_SOBEL_3X3_I8U_O16S_O16S_H_ 1

#include "VXLIB_types.h"

/** @defgroup VXLIB_sobel_3x3_i8u_o16s_o16s */
/** @ingroup  VXLIB_sobel_3x3_i8u_o16s_o16s */
/* @{ */
/**
 * @par Description:
 *  Computes the sobel image filter kernel over a 3x3 window of imput image, for each pixel.
 *
 * @par Method:
 *  Produces 2 separate output planes in a single pass through the image; one plane for X
 *  and one plane for Y. The sobel operators are defined as:
 *
 * @verbatim

          | -1 0 +1 |              | -1 -2 -1 |
    Gx =  | -2 0 +2 |        Gy =  |  0  0  0 |
          | -1 0 +1 |              | +1 +2 +1 |

   @endverbatim
 *
 * @par
 *   @param [in]  src[]             Pointer to array containing input image (UQ8.0)
 *   @param [in]  src_addr[]        Pointer to structure containing dimensional information of src
 *   @param [out] dst_x[]           Pointer to array containing X output image (SQ15.0)
 *   @param [in]  dst_x_addr[]      Pointer to structure containing dimensional information of dst_x
 *   @param [out] dst_y[]           Pointer to array containing Y output image (SQ15.0)
 *   @param [in]  dst_y_addr[]      Pointer to structure containing dimensional information of dst_y
 *
 * @par Assumptions:
 *    - I/O buffer pointers are assumed to be not aliased.
 *    - Output height should be == (Input height - 2)
 *    - Output width should be == (Input width - 2) OR (Input width)
 *
 * @par Performance Considerations:
 *    - For best performance, the following parameter settings are recommended:
 *      - Set output width values to a multiple of 8
 *      - Set widths equal to each other and equal to strides (allows processing over whole image in single loop)
 *      - If only x or y gradient is needed instead of both, one should use \ref VXLIB_sobelX_3x3_i8u_o16s or
 *        \ref VXLIB_sobelY_3x3_i8u_o16s, respecively.
 */
VXLIB_STATUS    VXLIB_sobel_3x3_i8u_o16s_o16s(const uint8_t src[restrict],
                                  const VXLIB_bufParams2D_t *src_addr,
                                  int16_t dst_x[restrict],
                                  const VXLIB_bufParams2D_t *dst_x_addr,
                                  int16_t dst_y[restrict],
                                  const VXLIB_bufParams2D_t *dst_y_addr);

/**
 * @par Description:
 *  Checks the parameters for programming errors for the VXLIB_sobel_3x3_i8u_o16s_o16s function.
 *
 * @par Method:
 *  The following checks are made:
 *    - There shall be no NULL pointers
 *    - Output height should be == (Input height - 2)
 *    - Output width should be <= Input width
 *    - The strides of each image shall be equal to or greater than the x dimension
 *
 * @par
 *   @param [in]  src[]             Pointer to array containing input image (UQ8.0)
 *   @param [in]  src_addr[]        Pointer to structure containing dimensional information of src
 *   @param [out] dst_x[]           Pointer to array containing X output image (SQ15.0)
 *   @param [in]  dst_x_addr[]      Pointer to structure containing dimensional information of dst_x
 *   @param [out] dst_y[]           Pointer to array containing Y output image (SQ15.0)
 *   @param [in]  dst_y_addr[]      Pointer to structure containing dimensional information of dst_y
 *
 */
VXLIB_STATUS    VXLIB_sobel_3x3_i8u_o16s_o16s_checkParams(const uint8_t src[restrict],
                                              const VXLIB_bufParams2D_t *src_addr,
                                              const int16_t dst_x[restrict],
                                              const VXLIB_bufParams2D_t *dst_x_addr,
                                              const int16_t dst_y[restrict],
                                              const VXLIB_bufParams2D_t *dst_y_addr);
/** @} */
#endif

