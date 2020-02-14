#pragma once
#include "Modules.h"
#include "Utils.h"

int main()
{
	int i, m = 10;
	int FilterLength = 0, SignalLength = 0, UseFilter = 0;
	float LowPassFrequency = 0.0, HighPassFrequency = 0.0;
	long iii = 0;

	int sampleSize = 1;

	for (i = 0; i < m; i++)
	{
		sampleSize *= 2;
	}
	int samplingRate = 1024;
	SignalLength = sampleSize - (FilterLength - 1);
	HealthMonitoring machine = HealthMonitoring(sampleSize, samplingRate);
	machine.ReadData();

	if (UseFilter == 1)
	{
		// Desiging Band Pass Filter
		FilterKernel = Cal_BandPassFilterKernel(LowPassFrequency, HighPassFrequency, FilterLength);

		for (i = 0; i <= FilterLength; i++)
		{
			FilterFFTRe[i] = FilterKernel[i];
		}

		fp = fopen("C:\\ConditionMonitoringSystem\\HilbertTransformTechnique\\FilterKernel.txt", "w");
		if (!feof(fp))
		{
			for (i = 0; i < v.SampleSize; i++)
			{
				fprintf(fp, "%d\t%f\n", i, FilterFFTRe[i]);
			}
			fclose(fp);
		}

		// Calculating FFT of Filter Kernel
		Cal_FFT(m, FilterFFTRe, FilterFFTIm);

		for (i = 0; i < SignalLength; i++)
		{
			SignalFFTRe[i] = v.RawData[i];
		}

		// Calculating FFT of Real Signal
		Cal_FFT(m, SignalFFTRe, SignalFFTIm);
		// Convolution of Signal and Filter in frequency domain
		Cal_FFTConvolution(SignalFFTRe, SignalFFTIm, FilterFFTRe, FilterFFTIm);
	}
	else
	{
		for (i = 0; i < v.SampleSize; i++)
		{
			SignalFFTRe[i] = v.RawData[i];//*0.5*(1.0-cos(2.0*PI*i/FilterLength));
		}

		fp = fopen("C:\\ConditionMonitoringSystem\\HilbertTransformTechnique\\InputSignal.txt", "w");
		if (!feof(fp))
		{
			for (i = 0; i < v.SampleSize; i++)
			{
				fprintf(fp, "%f\t%f\n", v.time[i], SignalFFTRe[i]);
			}
			fclose(fp);
		}
		// Calculating FFT of Real Signal
		Cal_FFT(m, SignalFFTRe, SignalFFTIm);
		//for(i=0;i<=350;i++)
		//{
		//	SignalFFTRe[i]=0.0;
		//	SignalFFTIm[i]=0.0;
		//}
	}

	/*
	v.RawData = (float*)malloc(v.SampleSize * sizeof(float));
	v.time = (float*)malloc(v.SampleSize * sizeof(float));
	
	// Reading Original Signal Data
	ReadRawData();
	

	delete[]v.RawData;
	delete[]v.time;
	v.RawData = nullptr;
	v.time = nullptr;

	*/

	std::cout << "Success!" << std::endl;

	return 0;
}