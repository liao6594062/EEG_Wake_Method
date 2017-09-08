#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "readcsv.h"
#include "sleepWakeDetection.h"

int main()
{
	int 		m;
	int 		n;
	int 		totalTime;
	int 		totalSecond;
	int 		numEpoch;
	char 		score[MAXENPOCH];
	char 		epochValid[MAXENPOCH];
	float* 		rawdata;
	float*		fftdata;
	char*		rawfile = "C:\\Users\\EEG\\Desktop\\脑电算法设计\\脑电数据\\android_trackrawdata_songyu_9_20170826_235212.csv";
	char*		fftfile = "C:\\Users\\EEG\\Desktop\\脑电算法设计\\脑电数据\\fftdata.csv";
	int 		offset = 0;

	n = get_col_of_raw(rawfile);
	m = get_row_of_raw(rawfile,n);
	rawdata = (float*)malloc(n*(m-offset)*sizeof(float));
	get_raw_from_csv(rawfile,rawdata,m-offset,n);

	totalTime = m-offset;

	n = get_col_of_raw(fftfile);
	m = get_row_of_raw(fftfile,n);
	fftdata = (float*)malloc(n*(m-offset)*sizeof(float));
	get_raw_from_csv(fftfile,fftdata,m-offset,n);

	totalSecond = m-offset;

	numEpoch = aasm(rawdata, fftdata, totalSecond, score);

	free(rawdata);
	free(fftdata);

	printf("\nBingo.");

	return 0;
}

