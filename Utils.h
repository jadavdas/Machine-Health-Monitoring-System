#pragma once
#include "Modules.h"
#include "Utils.h"

#include <iostream>
#include <fstream>

#define IA		16807
#define	IM		2147483647
#define	AM		(1.0/IM)
#define	IQ		127773
#define	IR		2836
#define	NTAB	32
#define	NDIV	(1+(IM-1)/NTAB)
#define	EPS		1.2e-7
#define	RNMX	(1.0-EPS)
#define PI		3.1415926535897932384626433832795


namespace utils
{
	float RandomNumber(long* idum);
	float* CalLowPassFilterKernel(float lowPassFrequency, int filterLength, int samplingRate);
	float* CalHighPassFilterKernel(float highPassFrequency, int filterLength, int samplingRate);
	float* CalBandRejectedFilterKernel(float lowPassFrequency, float highPassFrequency, int filterLength, int samplingRate);
	float* CalBandPassFilterKernel(float lowPassFrequency, float highPassFrequency, int filterLength, int sampligRate);
	void CalFFT(int m, float* fft_Re, float* fft_Im, int sampleSize);
}

