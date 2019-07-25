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
**|         Copyright (c) 2016 - 2017 Texas Instruments Incorporated         |**
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

#include "VXLIB_sobel_3x3_i8u_o16s_o16s.h"

CODE_SECTION(VXLIB_sobel_3x3_i8u_o16s_o16s,              ".text:optimized")
CODE_SECTION(VXLIB_sobel_3x3_i8u_o16s_o16s_core,         ".text:optimized")
CODE_SECTION(VXLIB_sobel_3x3_i8u_o16s_o16s_checkParams,  ".text:optimized")

static void VXLIB_sobel_3x3_i8u_o16s_o16s_core(const uint8_t src[restrict],
                                    int16_t dst_x[restrict],
                                    int16_t dst_y[restrict],
                                    int32_t srcStride,
                                    int32_t outWidth);

static void VXLIB_sobel_3x3_i8u_o16s_o16s_core(const uint8_t src[restrict],
                                    int16_t dst_x[restrict],
                                    int16_t dst_y[restrict],
                                    int32_t srcStride,
                                    int32_t outWidth)
{
    int32_t x = 0;
    int64_t mask1_8 = 0x0101010101010101;
    int64_t mask2_8 = 0x0202020202020202;
    uint32_t mask1_4 = _loll(mask1_8);
    uint32_t mask2_4 = _loll(mask2_8);
    int64_t r0, r1, r2;
    __x128_t r0_2, r1_2, r2_2;

    /* Used for x output */
    int64_t r01_lo, r01_hi, r012_lo, r012_hi, r012_lo_d2, r012_hi_d2, c02_lo, c02_hi;

    /* Used for y output */
    int64_t r02_lo, r02_hi, r02_lo_d1, r02_hi_d1, r02_lo_d2, r02_hi_d2, c01_lo, c01_hi, c012_lo, c012_hi;

    /* Since unaligned loads are bottleneck, save cost of data fetch
     * by fetching first 2 columns and adding/subtracting cols.  Update this state
     * for every iteration. */
    uint32_t x0_3210 = _mem4_const(&src[srcStride*0]);
    uint32_t x1_3210 = _mem4_const(&src[srcStride*1]);
    uint32_t x2_3210 = _mem4_const(&src[srcStride*2]);

    /* Convert from 8bpp to 16bpp so we can do SIMD ops on rows */
    int64_t x0_2 = _mpyu4ll(x0_3210, mask1_4);
    int64_t x1_2 = _mpyu4ll(x1_3210, mask2_4);
    int64_t x2_2 = _mpyu4ll(x2_3210, mask1_4);

    /* Add rows 0+1+2, column-wise (for x output) */
    int32_t x01    = _add2((int32_t)_loll(x0_2), (int32_t)_loll(x1_2));
    uint32_t xaxb  = (uint32_t)_add2(x01, (int32_t)_loll(x2_2));

    /* Subtract row 0 from 2, column-wise (for y output) */
    uint32_t yayb  = (uint32_t)_sub2((int32_t)_loll(x2_2), (int32_t)_loll(x0_2));

    /* Case 1: SIMD of 8 output pixels at a time */
    /* 7/8 = 0.875 cycles per pixel */
    /* NOTE: The case for aligned output was tried, but it could only achieve
     * better perfomance (13/16) if the loop was unrolled 2x, and it could not
     * be splooped, so I deemed not worth adding code, but can be added back if
     * needed */
    for(x=0; x < (outWidth/8); x++) {
        /* Read 8 bytes from each of the 3 lines. */
        r0 = _mem8_const(&src[(srcStride*0) + (x*8) + 2]);
        r1 = _mem8_const(&src[(srcStride*1) + (x*8) + 2]);
        r2 = _mem8_const(&src[(srcStride*2) + (x*8) + 2]);

        /* Convert from 8bpp to 16bpp so we can do SIMD of rows */
        r0_2 = _dmpyu4(r0, mask1_8);
        r1_2 = _dmpyu4(r1, mask2_8);
        r2_2 = _dmpyu4(r2, mask1_8);

        /* FOR X OUTPUT */

        /* Add rows 0+1, column-wise */
        r01_lo = _dadd2(_lo128(r0_2), _lo128(r1_2));
        r01_hi = _dadd2(_hi128(r0_2), _hi128(r1_2));

        /* Add previous sum to row 2, column-wise */
        r012_lo = _dadd2(r01_lo, _lo128(r2_2));
        r012_hi = _dadd2(r01_hi, _hi128(r2_2));

        /* Left shift this sum by 2 pixels, using history */
        r012_lo_d2 = _itoll(_loll(r012_lo), xaxb);
        r012_hi_d2 = _itoll(_loll(r012_hi), _hill(r012_lo));

        /* Now subtract right from left columns to find filter result */
        /*    7 6 5 4 3 2 1 0 = r012
         *  - 5 4 3 2 1 0 a b = r012_d2
         * --------------------
         *              c02
         */
        c02_lo = _dsub2(r012_lo, r012_lo_d2);
        c02_hi = _dsub2(r012_hi, r012_hi_d2);

        /* save overlap data for next iteration */
        xaxb = (uint32_t)_mvd((int32_t)_hill(r012_hi));

        /* store 8 sobel x filter values */
        _mem8(&dst_x[x*8])     = c02_lo;
        _mem8(&dst_x[(x*8)+4]) = c02_hi;


        /* FOR Y OUTPUT */

        /* Subtract row 0 from 2, column-wise (for y output) */
        r02_lo = _dsub2(_lo128(r2_2), _lo128(r0_2));
        r02_hi = _dsub2(_hi128(r2_2), _hi128(r0_2));

        /* Left shift this sum by one pixel, and 2 pixels respectively, using history */
        r02_lo_d1 = _itoll(_packlh2(_hill(r02_lo), _loll(r02_lo)),
                           _packlh2(_loll(r02_lo), yayb));
        r02_hi_d1 = _itoll(_packlh2(_hill(r02_hi), _loll(r02_hi)),
                           _packlh2(_loll(r02_hi), _hill(r02_lo)));

        r02_lo_d2 = _itoll(_loll(r02_lo), yayb);
        r02_hi_d2 = _itoll(_loll(r02_hi), _hill(r02_lo));

        /* Now add adjacent columns together to find neigborhood sums */
        /*    7 6 5 4 3 2 1 0 = r02
         *  + 6 5 4 3 2 1 0 a = r02_d1 * 2
         *  + 5 4 3 2 1 0 a b = r02_d2
         * --------------------
         *              c012
         */
        c01_lo  = _dadd2(r02_lo, _dshl2(r02_lo_d1, 1U));
        c012_lo = _dadd2(c01_lo, r02_lo_d2);

        c01_hi  = _dadd2(r02_hi, _dshl2(r02_hi_d1, 1U));
        c012_hi = _dadd2(c01_hi, r02_hi_d2);

        /* save overlap data for next iteration */
        yayb = (uint32_t)_mvd((int32_t)_hill(r02_hi));

        /* store 8 sobel y filter values */
        _mem8(&dst_y[x*8])     = c012_lo;
        _mem8(&dst_y[(x*8)+4]) = c012_hi;
    }

/* Setting macro to 1 for now ... priority is for reads to not read past input buffer vs code size.
 * Perhaps this will change in future depending on usage */
#if 1 || (!defined(VXLIB_ALIGNED8_WIDTHS) && !defined(VXLIB_SOBEL_3X3_I8U_O16S_O16S_ALIGNED8_WIDTHS))
    /* Case 2: Process remaining pixels beyond multiple of 8 */
    /* 6 cycle per pixel */
    for(x*=8; x < outWidth; x++) {
        int16_t sum;
        sum =   ((int16_t)src[(0*srcStride) + (x+0)] * (int16_t)-1 ) + 
                ((int16_t)src[(0*srcStride) + (x+1)] * (int16_t) 0 ) +
                ((int16_t)src[(0*srcStride) + (x+2)] * (int16_t) 1) +
                ((int16_t)src[(1*srcStride) + (x+0)] * (int16_t)-2 ) +
                ((int16_t)src[(1*srcStride) + (x+1)] * (int16_t) 0 ) +
                ((int16_t)src[(1*srcStride) + (x+2)] * (int16_t) 2) +
                ((int16_t)src[(2*srcStride) + (x+0)] * (int16_t)-1 ) +
                ((int16_t)src[(2*srcStride) + (x+1)] * (int16_t) 0 ) +
                ((int16_t)src[(2*srcStride) + (x+2)] * (int16_t) 1);
        
        dst_x[x] = sum;

        sum =   ((int16_t)src[(0*srcStride) + (x+0)] * (int16_t)-1 ) + 
                ((int16_t)src[(0*srcStride) + (x+1)] * (int16_t)-2 ) +
                ((int16_t)src[(0*srcStride) + (x+2)] * (int16_t)-1 ) +
                ((int16_t)src[(1*srcStride) + (x+0)] * (int16_t) 0) +
                ((int16_t)src[(1*srcStride) + (x+1)] * (int16_t) 0) +
                ((int16_t)src[(1*srcStride) + (x+2)] * (int16_t) 0) +
                ((int16_t)src[(2*srcStride) + (x+0)] * (int16_t) 1) +
                ((int16_t)src[(2*srcStride) + (x+1)] * (int16_t) 2) +
                ((int16_t)src[(2*srcStride) + (x+2)] * (int16_t) 1);
        
        dst_y[x] = sum;
    }
#endif
}

VXLIB_STATUS VXLIB_sobel_3x3_i8u_o16s_o16s(const uint8_t src[restrict],
                               const VXLIB_bufParams2D_t *src_addr,
                               int16_t dst_x[restrict],
                               const VXLIB_bufParams2D_t *dst_x_addr,
                               int16_t dst_y[restrict],
                               const VXLIB_bufParams2D_t *dst_y_addr)
{
    VXLIB_STATUS    status = VXLIB_SUCCESS;

#if defined(VXLIB_CHECK_PARAMS) || defined(VXLIB_SOBEL_3X3_I8U_O16S_O16S_CHECK_PARAMS)
    status = VXLIB_sobel_3x3_i8u_o16s_o16s_checkParams(src, src_addr, dst_x, dst_x_addr, dst_y, dst_y_addr);
    if( status == VXLIB_SUCCESS )
#endif
    {

#if !defined(VXLIB_EQUAL_WIDTH_STRIDE) && !defined(VXLIB_SOBEL_3X3_I8U_O16S_O16S_EQUAL_WIDTH_STRIDE)
        if( (src_addr->dim_x      == src_addr->stride_y) &&
           ((dst_x_addr->dim_x*2) == dst_x_addr->stride_y) &&
           ((dst_y_addr->dim_x*2) == dst_y_addr->stride_y) &&
            (dst_x_addr->dim_x    == src_addr->dim_x) &&
            (dst_y_addr->dim_x    == src_addr->dim_x))
#endif
        {
            VXLIB_sobel_3x3_i8u_o16s_o16s_core(src, dst_x, dst_y, src_addr->stride_y, ((dst_x_addr->stride_y/2) * dst_x_addr->dim_y) - 2);
        }
#if !defined(VXLIB_EQUAL_WIDTH_STRIDE) && !defined(VXLIB_SOBEL_3X3_I8U_O16S_O16S_EQUAL_WIDTH_STRIDE)
        else {

            int32_t    y;
            int32_t    width = VXLIB_min(dst_x_addr->dim_x, (src_addr->dim_x-2));

            for( y=0; y < dst_x_addr->dim_y; y++ ) {

                const uint8_t  *src_t = (const uint8_t *)&src[ y * src_addr->stride_y];
                int16_t         *dstx_t  = (int16_t *)&dst_x[ y * (dst_x_addr->stride_y/2)];
                int16_t         *dsty_t  = (int16_t *)&dst_y[ y * (dst_y_addr->stride_y/2)];

                VXLIB_sobel_3x3_i8u_o16s_o16s_core(src_t, dstx_t, dsty_t, src_addr->stride_y, width);

            }
        }
#endif
    }
    return (status);
}

#if (!defined(VXLIB_REMOVE_CHECK_PARAMS) && !defined(VXLIB_SOBEL_3X3_I8U_O16S_O16S_REMOVE_CHECK_PARAMS)) || (defined(VXLIB_CHECK_PARAMS)) || (defined(VXLIB_SOBEL_3X3_I8U_O16S_O16S_CHECK_PARAMS))

VXLIB_STATUS VXLIB_sobel_3x3_i8u_o16s_o16s_checkParams(const uint8_t src[restrict],
                                           const VXLIB_bufParams2D_t *src_addr,
                                           const int16_t dst_x[restrict],
                                           const VXLIB_bufParams2D_t *dst_x_addr,
                                           const int16_t dst_y[restrict],
                                           const VXLIB_bufParams2D_t *dst_y_addr)
{
    VXLIB_STATUS    status = VXLIB_SUCCESS;

    if( (src == NULL) || (dst_x == NULL) || (dst_y == NULL) ) {
        status = VXLIB_ERR_NULL_POINTER;
    } else if( (src_addr->dim_x < dst_x_addr->dim_x) ||
                (src_addr->dim_x < dst_y_addr->dim_x) ||
               ((src_addr->dim_y-2) !=  dst_x_addr->dim_y) ||
               ((src_addr->dim_y-2) !=  dst_y_addr->dim_y) ||
               (src_addr->stride_y < src_addr->dim_x) ||
               (dst_x_addr->stride_y < (dst_x_addr->dim_x*2)) ||
               (dst_y_addr->stride_y < (dst_y_addr->dim_x*2)) ) {
        status = VXLIB_ERR_INVALID_DIMENSION;
    } else if((src_addr->data_type != VXLIB_UINT8) ||
               (dst_x_addr->data_type != VXLIB_INT16) ||
               (dst_y_addr->data_type != VXLIB_INT16)) {
        status = VXLIB_ERR_INVALID_TYPE;
    }
#if defined(VXLIB_EQUAL_WIDTH_STRIDE) || defined(VXLIB_SOBEL_3X3_I8U_O16S_O16S_EQUAL_WIDTH_STRIDE)
    else if( (src_addr->dim_x != src_addr->stride_y) ||
             ((dst_x_addr->dim_x*2) != dst_x_addr->stride_y) ||
             ((dst_y_addr->dim_x*2) != dst_y_addr->stride_y) ||
              (src_addr->dim_x != dst_x_addr->dim_x) ||
              (src_addr->dim_x != dst_y_addr->dim_x)) {
        status = VXLIB_ERR_NOT_EQUAL_WIDTH_STRIDE;
    }
#endif
/* Setting macro to 0 for now ... priority is for reads to not read past input buffer vs code size.
 * Perhaps this will change in future depending on usage */
#if 0 && (defined(VXLIB_ALIGNED8_WIDTHS) || defined(VXLIB_SOBEL_3X3_I8U_O16S_O16S_ALIGNED8_WIDTHS))
    else if(((dst_x_addr->dim_x % 8U) != 0) ||
             ((dst_y_addr->dim_x % 8U) != 0)) {
        status = VXLIB_ERR_NOT_ALIGNED_WIDTHS;
    }
#endif
    else {
        status = VXLIB_SUCCESS;
    }
    return (status);
}

#endif
