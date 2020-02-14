#pragma once
#include "Modules.h"
#include "Utils.h"

HealthMonitoring::HealthMonitoring(int sampleSize, int samplingRate)
{
	//constructor
	this->sampleSize = sampleSize;
	this->samplingRate = samplingRate;
}

HealthMonitoring::~HealthMonitoring()
{
	//destructor
	delete[]rawData;
	rawData = nullptr;
}

void HealthMonitoring::ReadData()
{
	int i = 0;
	long ii = 0;
	float delta_t = 0.0;

	delta_t = 1.0 / (float)samplingRate;
	rawData = new float[sampleSize];
	time = new float[sampleSize];

	std::ofstream outfile;
	outfile.open("rawData.txt");

	for (i = 0; i < sampleSize; i++)
	{
		// type-1: Raw data read from a file
		//fscanf(fp,"%f\n",&v.RawData[i]);

		if (i == 0)
		{
			time[0] = 0.0;
		}
		else
		{
			time[i] = time[i - 1] + delta_t;
		}

		// type-2: Raw data generated from equation
		//v.RawData[i] = 2.0*cos(2.0*PI*10.0*v.time[i])+1.5*cos(2.0*PI*100.0*v.time[i]);
		//v.RawData[i] += 3.0*cos(2.0*PI*200.0*v.time[i])+2.5*cos(2.0*PI*400.0*v.time[i]);

		// Signal type - 3
		ii = (long)-i;
		rawData[i] = (0.1 * utils::RandomNumber(&ii) + 7.5 * cos(2.0 * PI * 10.0 * time[i])) * sin(2.0 * PI * 100.0 * time[i]);

		//	v.RawData[i] = (7.5*cos(2.0*PI*10.0*v.time[i]))*(7.5*sin(2.0*PI*100.0*v.time[i]));

		//	v.RawData[i] = (7.5+7.5*cos(2.0*PI*10.0*v.time[i]))*sin(2.0*PI*100.0*v.time[i]);

		//	v.RawData[i] = 7.5*cos(2.0*PI*10.0*v.time[i]))*sin(2.0*PI*100.0*v.time[i]) + 7.5*sin(2.0*PI*100.0*v.time[i]);

		//v.RawData[i] += (0.0+4.5*sin(2*PI*20*v.time[i]))*cos(2.0*PI*200.0*v.time[i]);
		//v.RawData[i] += (0.0+1.5*sin(2*PI*30*v.time[i]))*cos(2.0*PI*300.0*v.time[i]);
		//v.RawData[i] += (0.0+5.5*sin(2*PI*40*v.time[i]))*cos(2.0*PI*400.0*v.time[i]);

		// type-4: Raw data generated from equation
		//v.RawData[i] = 10.0*cos(2.0*PI*100.0*v.time[i]);
		//v.RawData[i]=(4.0*cos(2.0*PI*2.0*v.time[i])-6.0*sin(2.0*PI*3.0*v.time[i]))*cos(2.0*PI*100.0*v.time[i]);
		//v.RawData[i]=(4.0*cos(2.0*PI*2.0*v.time[i])-6.0*sin(2.0*PI*3.0*v.time[i]));
		outfile << time[i] << " " << rawData[i] << std::endl;
	}

	outfile.close();
}

