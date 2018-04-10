/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic 
 * Semiconductor ASA.Terms and conditions of usage are described in detail 
 * in NORDIC SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT. 
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRENTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *              
 * $LastChangedRevision: 5756 $
 */

/** @file
 * @brief Common defines and macros for firmware developed by Nordic Semiconductor.
 *
 */

#ifndef _COMMON_H__
#define _COMMON_H__

/** Swaps the upper byte with the lower byte in a 16 bit variable */
//lint -emacro((572),SWAP) // Suppress warning 572 "Excessive shift value"
#define SWAP(x) ((((x)&0xFF)<<8)|(((x)>>8)&0xFF))

/** The upper 8 bits of a 16 bit value */
#define MSB_H(a) ((a & 0xFF00) >> 8)
/** The lower 8 bits (of a 16 bit value) */
#define LSB_L(a) ((a & 0xFF))

/** Leaves the minimum of the two arguments */
#define MIN(a, b) ((a) < (b) ? (a) : (b))
/** Leaves the maximum of the two arguments */
#define MAX(a, b) ((a) < (b) ? (b) : (a))

#define BIT_0 0x01 /**< The value of bit 0 */
#define BIT_1 0x02 /**< The value of bit 1 */
#define BIT_2 0x04 /**< The value of bit 2 */
#define BIT_3 0x08 /**< The value of bit 3 */
#define BIT_4 0x10 /**< The value of bit 4 */
#define BIT_5 0x20 /**< The value of bit 5 */
#define BIT_6 0x40 /**< The value of bit 6 */
#define BIT_7 0x80 /**< The value of bit 7 */
#define BIT_8 0x0100 /**< The value of bit 8 */
#define BIT_9 0x0200 /**< The value of bit 9 */
#define BIT_10 0x0400 /**< The value of bit 10 */
#define BIT_11 0x0800 /**< The value of bit 11 */
#define BIT_12 0x1000 /**< The value of bit 12 */
#define BIT_13 0x2000 /**< The value of bit 13 */
#define BIT_14 0x4000 /**< The value of bit 14 */
#define BIT_15 0x8000 /**< The value of bit 15 */


#endif // _COMMON_H__
