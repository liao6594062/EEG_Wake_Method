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
/* @�������
 * 			absFft: ���飬�ܳ���Ϊ��256*3600��*25Сʱ����FFT�ľ���ֵƽ��
 *			totalSecond: ��ʱ��������λ����
 *			indClear: ���飬�ܳ���Ϊ3000�� ����clearRaw����������������clearRaw
 * *
 * @�������
 * 			score�����飬�ܳ���Ϊ3000. �������������ҹ���ڽ���������ڵ�ÿ��Ԫ�ض�Ӧÿ30��ķ��ڽ����
 *
 * @return
 * 			��
 */

//void clearRaw(float *rawdata, int totalTime, float threshold, char *indClear);
/* @�������
 * 			rawdata: ԭʼʱ���ź�
 *			totalTime: ��ʱ��������λ����
 *			threshold: ����ֵ��������Ϊ500
 * *
 * @�������
 * 			indClear�����飬�ܳ���Ϊ3000
 *
 * @return
 * 			��
 */

int aasm(float* rawdata, float *fftdata, int totalSecond, char *score);
/* @�������
 * 			rawdata: ���飬�ܳ���Ϊ��256*3600��*25Сʱ����ԭʼʱ���źţ�������256Hz
 * 			absFft: ���飬�ܳ���Ϊ��256*3600��*25Сʱ����ԭʼʱ���źŰ������FFT�����ľ���ֵƽ��
 *			totalSecond: ��ʱ��������λ����
 * *
 * @�������
 * 			score�����飬�ܳ���Ϊ3000. �������������ҹ���ڽ���������ڵ�ÿ��Ԫ�ض�Ӧÿ30��ķ��ڽ����
 *
 * @return
 * 			����ֵ��ʾ����score��ǰ�����Ԫ������Ч�ģ�����Ԫ�غ��ԡ�
 */
#endif
