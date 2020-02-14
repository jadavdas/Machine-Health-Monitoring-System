#pragma once
#include "Modules.h"
#include "Utils.h"

struct Data
{
	int rows, cols;
	float* data;
};

class HealthMonitoring
{
private:
	int sampleSize, samplingRate;
	float* rawData, * time, *filteredData;
	float* amplitude, * phase, * frequency, * powerSpectrum, * envelopeFrequency, * envelopeSpectrum;

public:
	HealthMonitoring(int sampleSize, int samplingRate); // constructor
	//Data myData;
	void ReadData(); // read data file
	float* HmCalLowPassFilterKernel(float lowPassFrequency, int filterLength, int samplingRate);
	float* HmCalHighPassFilterKernel(float highPassFrequency, int filterLength, int samplingRate);
	float* HmCalBandRejectedFilterKernel(float lowPassFrequency, float highPassFrequency, int filterLength, int samplingRate);
	float* HmCalBandPassFilterKernel(float lowPassFrequency, float highPassFrequency, int filterLength, int samplingRate);
	void HmCalFFT(int m, float* fftRe, float* fftIm, int sampleSize);

	virtual ~HealthMonitoring();
};