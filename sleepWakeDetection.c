#include "sleepWakeDetection.h"

FILE* fp;

static void judge(float *epochFilted2, float *th, int *epochValid, int numEpoch, char *score)
{
	int i;

	for(i=0; i<numEpoch; i++)
	{
		if(epochValid[i]==NaN)
		{
			score[i] = NaN;
		}
		else
		{
			if(epochFilted2[i]<th[i])
			{
				score[i] = WAKE;
			}
			else
			{
				score[i] = SLEEP;
			}
		}
	}

	return;
}

static int arrage(float *fftdata, int totalTime, int *indClear)
{
	int i;
	int j;
	int second;

	second = 0;
	for(i=0; i<totalTime; i++)
	{
		if(indClear[i]!=NaN)
		{
			for(j=0; j<NFFT; j++)
			{
				fftdata[NFFT*second+j] = fftdata[NFFT*i+j];
			}
			second++;
		}
	}

	return second;
}

void clearRaw(float *rawdata, int totalTime, float threshold, char *indClear)
{
	int i;
	int j;
	int offset;
	int index;

	index = 0;
	for(i=0; i<totalTime; i++)
	{
		indClear[index] = 1;
		for(j=0; j<NFFT; j++)
		{
			offset = i*NFFT + j;
			if((rawdata[offset]>threshold)||(rawdata[offset]<-1*threshold))
			{
				indClear[index] = NaN;
				break;
			}
		}
		index++;
	}

	return;
}

static float mean(float *data, int num)
{
	int i;
	float sum;
	float result;

	sum = 0;
	for(i=0; i<num; i++)
	{
		sum += data[i];
	}
	result = sum/num;

	return result;
}


static float ratioFreqBand(float *absFft)
{
	int numAlpha;
	int numBeta;
	const int alphaLow = 9;
	const int alphaHigh = 14;
	const int betaLow = 17;
	const int betaHigh = 22;
	float meanAlpha;
	float meanBeta;
	float ratioBand;

	numAlpha = alphaHigh - alphaLow + 1;
	numBeta = betaHigh - betaLow + 1;
	meanAlpha = mean(&absFft[alphaLow],numAlpha);
	meanBeta = mean(&absFft[betaLow],numBeta);
	ratioBand = meanAlpha/meanBeta;

	return ratioBand;
}

static void averagingFilter(float *in, int length, int window, float *out)
{
	int i;
	int j;
	int midpoint;
	float tmp;

	midpoint = window/2 + 1;

	for(i=0; i<midpoint-1; i++)
	{
		out[i] = in[i];
	}

	for(i=midpoint-1; i<length-midpoint+1; i++)
	{
		tmp = 0;
		for(j=i-midpoint; j<i-midpoint+window; j++)
		{
			tmp += in[j];
		}
		out[i] = tmp/window;
	}

	for(i=length-midpoint+1; i<length; i++)
	{
		out[i] = in[i];
	}

	return;
}


static void thSleepWake(float *ratio, int length, float *th)
{
	int i;
	const int window = 5;
	const int startPoint = 30;
	int node[2] = {0,startPoint-1};

	const float gain = ORIGINAL_THRESHOLD;
	const float coef1 = 0.5;
	const float coef2 = 0.5;

	for(i=0; i<startPoint; i++)
	{
		th[i] = gain;
	}

	for(i=startPoint; i<length; i++)
	{
		if(((th[i-1]>ratio[i-1])&&(th[i-1]<ratio[i]))||((th[i-1]<ratio[i-1])&&(th[i-1]>ratio[i])))
		{
			node[0] = node[1];
			node[1] = i;

			if(node[1]-node[0]>=window)
			{
				th[i] = coef1*mean(&ratio[node[0]],node[1]-node[0]) + coef2*ratio[i];
			}
			else
			{
				th[i] = th[i-1];
			}
		}
		else
		{
			th[i] = coef1*mean(&ratio[node[0]],node[1]-node[0]) + coef2*mean(&ratio[node[1]],i-node[1]+1);
		}
	}

	return;
}

static int clearSignal(float *absFft, int totalEpoch, float threshold)
{
	int i;
	int j;
	int k;
	int offset;
	int numEpoch;
	int flag;
	float sum;

	numEpoch = 0;
	for(i=0; i<totalEpoch; i++)
	{
		flag = 0;
		for(j=0; j<SECONDS_OF_EPOCH; j++)
		{
			offset = (i*SECONDS_OF_EPOCH+j)*NFFT;
			sum = 0;
			for(k=0; k<NFFT; k++)
			{
				sum += absFft[offset+k]/NFFT;
			}
			if(sum>threshold)
			{
				flag = 1;
				break;
			}
		}
		if(flag==0)
		{
			absFft[numEpoch] = absFft[i];
			numEpoch++;
		}
	}

	return numEpoch;
}


void detect(float *absFft, int totalSecond, char *indClear, char *score)
{
	int i;
	int j;
	int offset;
	int totalEpoch;
	int numValid;
	int numEpoch;
	int epochValid[MAXENPOCH];
	const int window = 5;
	float th[MAXENPOCH];
	float epoch[MAXENPOCH] = {0};
	float epochFilted[MAXENPOCH] = {0};
	float epochFilted2[MAXENPOCH] = {0};

	fp = fopen("j.txt","w+");

	totalEpoch = totalSecond/SECONDS_OF_EPOCH;

	numEpoch = totalEpoch;

	for(i=0; i<totalEpoch; i++)
	{
		
		numValid = 0;
		epoch[0] = ORIGINAL_THRESHOLD;
		
		for(j=0; j<SECONDS_OF_EPOCH; j++)
		{
			offset = (i*SECONDS_OF_EPOCH+j);
		    if(indClear[offset]!=NaN)
			{
				epoch[i] += ratioFreqBand(absFft+offset*NFFT);
				numValid++;
			}
		}
		
		if(numValid==0)
		{
			if(i>0)
			{
				epoch[i] = epoch[i-1];
			}
			epochValid[i] = NaN;
		}
		else
		{
			epoch[i] /= numValid;
			epochValid[i] = 1;
		}
	}

	averagingFilter(epoch, numEpoch, window, epochFilted);
	averagingFilter(epochFilted, numEpoch, window, epochFilted2);
	thSleepWake(epochFilted2, numEpoch, th);
	judge(epochFilted2, th, epochValid, numEpoch, score);

	for(i=0; i<numEpoch; i++)
	{
		fprintf(fp,"%f ",th[i]);
	}

	return;
}

int aasm(float* rawdata, float *fftdata, int totalSecond, char *score)
{
	char 		*indClear;
	int			numEpoch;
	const int 	rawTh = 500;
	indClear = (char*)malloc(totalSecond*sizeof(char));

	clearRaw(rawdata, totalSecond, rawTh, indClear);
	detect(fftdata, totalSecond, indClear, score);
	numEpoch = totalSecond/SECONDS_OF_EPOCH;

	free(indClear);

	return numEpoch;
}
