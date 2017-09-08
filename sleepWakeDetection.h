#ifndef __SLEEPWAKEDETECTION_H__
#define __SLEEPWAKEDETECTION_H__

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "readcsv.h"

#define LEN 				(1500)
#define MAXENPOCH 			(1500)
#define SECONDS_OF_EPOCH 	(30)
#define NFFT				(256)
#define NaN					(-1)
#define ORIGINAL_THRESHOLD	(2)
#define WAKE				(1)
#define SLEEP				(2)
#define REM					(5)

//void detect(float *absFft, int totalSecond, char *indClear, char *score);
/* @输入参数
 * 			absFft: 数组，总长度为（256*3600秒*25小时），FFT的绝对值平方
 *			totalSecond: 总时间量，单位：秒
 *			indClear: 数组，总长度为3000。 函数clearRaw的输出结果，见函数clearRaw
 * *
 * @输出参数
 * 			score：数组，总长度为3000. 整个数组标书整夜分期结果，数组内的每个元素对应每30秒的分期结果。
 *
 * @return
 * 			无
 */

//void clearRaw(float *rawdata, int totalTime, float threshold, char *indClear);
/* @输入参数
 * 			rawdata: 原始时域信号
 *			totalTime: 总时间量，单位：秒
 *			threshold: 门限值，建议设为500
 * *
 * @输出参数
 * 			indClear：数组，总长度为3000
 *
 * @return
 * 			无
 */

int aasm(float* rawdata, float *fftdata, int totalSecond, char *score);
/* @输入参数
 * 			rawdata: 数组，总长度为（256*3600秒*25小时），原始时域信号，采样率256Hz
 * 			absFft: 数组，总长度为（256*3600秒*25小时），原始时域信号按秒计算FFT后结果的绝对值平方
 *			totalSecond: 总时间量，单位：秒
 * *
 * @输出参数
 * 			score：数组，总长度为3000. 整个数组标书整夜分期结果，数组内的每个元素对应每30秒的分期结果。
 *
 * @return
 * 			返回值表示数组score内前面多少元素是有效的，其余元素忽略。
 */
#endif
