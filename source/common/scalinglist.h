/*****************************************************************************
 * Copyright (C) 2014 x265 project
 *
 * Authors: Steve Borho <steve@borho.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111, USA.
 *
 * This program is also available under a commercial proprietary license.
 * For more information, contact us at license @ x265.com.
 *****************************************************************************/

#ifndef X265_SCALINGLIST_H
#define X265_SCALINGLIST_H

#include "common.h"

namespace x265 {
// private namespace

class ScalingList
{
public:

    enum { NUM_SIZES = 4 };            // 4x4, 8x8, 16x16, 32x32
    enum { NUM_LISTS = 6 };            // number of quantization matrix lists (YUV * inter/intra)
    enum { NUM_REM = 6 };              // number of remainders of QP/6
    enum { MAX_MATRIX_COEF_NUM = 64 }; // max coefficient number per quantization matrix
    enum { MAX_MATRIX_SIZE_NUM = 8 };  // max size number for quantization matrix

    static const int     s_numCoefPerSize[NUM_SIZES];
    static const int32_t s_invQuantScales[NUM_REM];
    static const int32_t s_quantScales[NUM_REM];

    int32_t  m_scalingListDC[NUM_SIZES][NUM_LISTS];   // the DC value of the matrix coefficient for 16x16
    int32_t* m_scalingListCoef[NUM_SIZES][NUM_LISTS]; // quantization matrix

    int32_t* m_quantCoef[NUM_SIZES][NUM_LISTS][NUM_REM];   // array of quantization matrix coefficient 4x4
    int32_t* m_dequantCoef[NUM_SIZES][NUM_LISTS][NUM_REM]; // array of dequantization matrix coefficient 4x4

    bool     m_bEnabled;
    bool     m_bDataPresent; // non-default scaling lists must be signaled

    ScalingList();
    ~ScalingList();

    bool     init();
    void     setDefaultScalingList();
    bool     parseScalingList(const char* filename);
    void     setupQuantMatrices();

    /* used during SPS coding */
    int      checkPredMode(int sizeId, int listId);

protected:

    static const int SCALING_LIST_DC = 16;    // default DC value

    int32_t* getScalingListDefaultAddress(int sizeId, int listId);
    void     processDefaultMarix(int sizeId, int listId);
    bool     checkDefaultScalingList();

    void     processScalingListEnc(int32_t *coeff, int32_t *quantcoeff, int32_t quantScales, int height, int width, int ratio, int stride, int32_t dc);
    void     processScalingListDec(int32_t *coeff, int32_t *dequantcoeff, int32_t invQuantScales, int height, int width, int ratio, int stride, int32_t dc);
};

}

#endif // ifndef X265_SCALINGLIST_H
