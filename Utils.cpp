#pragma once
#include "Modules.h"
#include "Utils.h"

namespace utils
{
	float RandomNumber(long* idum)
	{
		int j;
		long k;
		static long iy = 0;
		static long iv[NTAB];
		float temp;

		if (*idum <= 0 || !iy)
		{
			if (-(*idum) < 1)
				*idum = 1;
			else
				*idum = -(*idum);
			for (j = NTAB + 7; j >= 0; j--)
			{
				k = (*idum) / IQ;
				*idum = IA * (*idum - k * IQ) - IR * k;
				if (*idum < 0)
					*idum += IM;
				if (j < NTAB)
					iv[j] = *idum;
			}
			iy = iv[0];
		}
		k = (*idum) / IQ;
		*idum = IA * (*idum - k * IQ) - IR * k;
		if (*idum < 0)
			*idum += IM;
		j = iy / NDIV;
		iy = iv[j];
		iv[j] = *idum;
		if ((temp = AM * iy) > RNMX)
			return RNMX;
		else
			return temp;
	}

	float* CalLowPassFilterKernel(float lowPassFrequency, int filterLength, int samplingRate)
	{
		int i;
		float* lowPassFilter;
		float cutOffFrequency = 0.0, sum = 0.0;

		if ((int)lowPassFrequency > samplingRate / 2)
		{
			printf("\nerror...\n");
			printf("Cut off frequency can not be greater than half of the sampling rate\n");
			exit(1);
		}

		if (filterLength % 2 != 0)
		{
			printf("\nerror...\n");
			printf("Filter kernel length should be even integer\n");
			exit(1);
		}

		lowPassFilter = new float[filterLength + 1];

		for (i = 0; i <= filterLength; i++)
		{
			lowPassFilter[i] = 0.0;
		}

		cutOffFrequency = lowPassFrequency / (float)samplingRate;

		for (i = 0; i <= filterLength; i++)
		{
			if ((i - filterLength / 2) == 0)
			{
				lowPassFilter[i] = 2.0 * PI * cutOffFrequency;
			}
			else
			{
				lowPassFilter[i] = sin(2.0 * PI * cutOffFrequency * (i - filterLength / 2.0)) / (i - filterLength / 2.0);
				// Blackman Window
				//lowPassFilter[i] = lowPassFilter[i]*(0.42-0.5*cos(2.0*PI*i/filterLength)+0.08*cos(4.0*PI*i/filterLength));
				// Hanning Window
				lowPassFilter[i] = lowPassFilter[i] * 0.5 * (1.0 - cos(2.0 * PI * i / filterLength));
			}
		}

		sum = 0.0;
		for (i = 0; i <= filterLength; i++)
		{
			sum += lowPassFilter[i];
		}

		for (i = 0; i <= filterLength; i++)
		{
			lowPassFilter[i] = lowPassFilter[i] / sum;
		}

		return lowPassFilter;
	}

	float* CalHighPassFilterKernel(float highPassFrequency, int filterLength, int samplingRate)
	{
		int i;
		float* highPassFilter;

		if ((int)highPassFrequency > samplingRate / 2)
		{
			printf("\nerror...\n");
			printf("Cut off frequency can not be greater than half of the sampling rate\n");
			exit(1);
		}

		if (filterLength % 2 != 0)
		{
			printf("\nerror...\n");
			printf("Filter kernel length should be even integer\n");
			exit(1);
		}

		highPassFilter = new float[filterLength + 1];

		for (i = 0; i <= filterLength; i++)
		{
			highPassFilter[i] = 0.0;
		}

		highPassFilter = CalLowPassFilterKernel(highPassFrequency, filterLength, samplingRate);

		for (i = 0; i <= filterLength; i++)
		{
			highPassFilter[i] = -highPassFilter[i];
		}

		highPassFilter[filterLength / 2] += 1.0;

		return highPassFilter;
	}

	float* CalBandRejectedFilterKernel(float lowPassFrequency, float highPassFrequency, int filterLength, int samplingRate)
	{
		int i;
		float* lowPassFilter, * highPassFilter, * bandRejectedFilter;

		lowPassFilter = new float[filterLength + 1];
		highPassFilter = new float[filterLength + 1];
		bandRejectedFilter = new float[filterLength + 1];

		lowPassFilter = CalLowPassFilterKernel(lowPassFrequency, filterLength, samplingRate);
		highPassFilter = CalHighPassFilterKernel(highPassFrequency, filterLength, samplingRate);

		for (i = 0; i <= filterLength; i++)
		{
			bandRejectedFilter[i] = lowPassFilter[i] + highPassFilter[i];
		}
		return bandRejectedFilter;
	}

	float* CalBandPassFilterKernel(float lowPassFrequency, float highPassFrequency, int filterLength, int sampligRate)
	{
		int i;
		float* bandPassFilter;

		bandPassFilter = new float[filterLength + 1];
		for (i = 0; i <= filterLength; i++)
		{
			bandPassFilter[i] = 0.0;
		}

		bandPassFilter = CalBandRejectedFilterKernel(lowPassFrequency, highPassFrequency, filterLength, sampligRate);

		for (i = 0; i <= filterLength; i++)
		{
			bandPassFilter[i] = -bandPassFilter[i];
		}
		bandPassFilter[filterLength / 2] += 1.0;

		return bandPassFilter;
	}

	void CalFFT(int m, float* fftRe, float* fftIm, int sampleSize)
	{
		int i, i1, j, k, k1 = 0, k2 = 0, l1, n = 0;
		float tRe, tIm;
		float* dummyRe, * dummyIm, * AmpRe, * AmpIm;

		n = sampleSize;
		dummyRe = new float[n];
		dummyIm = new float[n];
		AmpRe = new float[n];
		AmpIm = new float[n];

		// Do the bit reversal 

		i1 = n >> 1;
		j = 0;
		for (i = 0; i < n - 1; i++)
		{
			if (i < j)
			{
				tRe = fftRe[i];
				tIm = fftIm[i];
				fftRe[i] = fftRe[j];
				fftIm[i] = fftIm[j];
				fftRe[j] = tRe;
				fftIm[j] = tIm;
			}
			k = i1;
			while (k <= j)
			{
				j -= k;
				k >>= 1;
			}
			j += k;
		}

		//Calculate Twiddle factor

		float** wRe, ** wIm;
		wRe = (float**)malloc(sampleSize * sizeof(float*));
		wIm = (float**)malloc(sampleSize * sizeof(float*));

		for (i = 0; i < sampleSize; i++)
		{
			wRe[i] = (float*)malloc((m - 2) * sizeof(float));
			wIm[i] = (float*)malloc((m - 2) * sizeof(float));
		}

		for (k = 0; k < sampleSize; k++)
		{
			l1 = 1;
			for (j = 0; j < m - 2; j++)
			{
				wRe[k][j] = cos(2.0 * PI * k * l1 / sampleSize);
				wIm[k][j] = -sin(2.0 * PI * k * l1 / sampleSize);
				l1 <<= 1;
				//printf("w[%d][%d]=%lf\t",k,j,wRe[k][j]);
			}
			//printf("\n");
		}

		// Compute FFT

		for (k = 0; k < n; k++)
		{
			l1 = n;
			for (j = 0; j < n; j++)
			{
				dummyRe[j] = fftRe[j];
				dummyIm[j] = fftIm[j];
			}
			for (j = m - 1; j >= 0; j--)
			{
				if (j == m - 1)
				{
					if (k1 == 0)
					{
						i1 = 0;
						for (i = 0; i < l1; i = i + 2)
						{
							dummyRe[i1] = dummyRe[i] + dummyRe[i + 1];
							dummyIm[i1] = dummyIm[i] + dummyIm[i + 1];
							i1++;
						}
						l1 >>= 1;
					}
					else if (k1 == 2)
					{
						i1 = 0;
						for (i = 0; i < l1; i = i + 2)
						{
							dummyRe[i1] = dummyRe[i] - dummyRe[i + 1];
							dummyIm[i1] = dummyIm[i] - dummyIm[i + 1];
							i1++;
						}
						l1 >>= 1;
					}

					k1 += 2;
					if (k1 == 4)
						k1 = 0;
				}
				else if (j == m - 2)
				{
					if (k2 == 0)
					{
						i1 = 0;
						for (i = 0; i < l1; i = i + 2)
						{
							dummyRe[i1] = dummyRe[i] + dummyRe[i + 1];
							dummyIm[i1] = dummyIm[i] + dummyIm[i + 1];
							i1++;
						}
						l1 >>= 1;
					}
					else if (k2 == 1)
					{
						i1 = 0;
						for (i = 0; i < l1; i = i + 2)
						{
							dummyRe[i1] = dummyRe[i] + dummyIm[i + 1];
							dummyIm[i1] = dummyIm[i] - dummyRe[i + 1];
							i1++;
						}
						l1 >>= 1;
					}
					else if (k2 == 2)
					{
						i1 = 0;
						for (i = 0; i < l1; i = i + 2)
						{
							dummyRe[i1] = dummyRe[i] - dummyRe[i + 1];
							dummyIm[i1] = dummyIm[i] - dummyIm[i + 1];
							i1++;
						}
						l1 >>= 1;
					}
					else if (k2 == 3)
					{
						i1 = 0;
						for (i = 0; i < l1; i = i + 2)
						{
							dummyRe[i1] = dummyRe[i] - dummyIm[i + 1];
							dummyIm[i1] = dummyIm[i] + dummyRe[i + 1];
							i1++;
						}
						l1 >>= 1;
					}

					k2++;
					if (k2 == 4)
						k2 = 0;
				}
				else
				{
					i1 = 0;
					for (i = 0; i < l1; i = i + 2)
					{
						dummyRe[i1] = dummyRe[i] + wRe[k][j] * dummyRe[i + 1] - wIm[k][j] * dummyIm[i + 1];
						dummyIm[i1] = dummyIm[i] + wIm[k][j] * dummyRe[i + 1] + wRe[k][j] * dummyIm[i + 1];
						i1++;
					}
					l1 >>= 1;
				}
			}
			AmpRe[k] = dummyRe[0];
			AmpIm[k] = dummyIm[0];
		}

		for (i = 0; i < n; i++)
		{
			fftRe[i] = AmpRe[i];
			fftIm[i] = AmpIm[i];
		}

		delete []dummyRe;
		delete []dummyIm;
		delete []AmpRe;
		delete []AmpIm;
		delete []wRe;
		delete []wIm;

		dummyRe = nullptr;
		dummyIm = nullptr;
		AmpRe = nullptr;
		AmpIm = nullptr;
		wRe = nullptr;
		wIm = nullptr;
	}
}


