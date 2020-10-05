#ifndef WEATHER_STATION
#define WEATHER_STATION
#include <fstream>
#include <iostream>
#include <string>
#include <stdio.h>

#define MAX_LOGS 96

class D_Class {
public:
	D_Class() = default;
	D_Class(int&& sM, int&& lL, int&& t, int&& bP, int&& ID, int&& dO) {
		m_soilMoisture = (double)sM;
		m_lightLevel = (double)lL;
		m_temp = (uint16_t)t;
		m_baroPressure = (uint16_t)bP;
		m_nodeID = (uint16_t)ID;
		m_digitalOut = (uint16_t)dO;
	}

	D_Class(D_Class&& newD) noexcept {
		m_soilMoisture = newD.m_soilMoisture;
		m_lightLevel = newD.m_lightLevel;
		m_temp = newD.m_temp;
		m_baroPressure = newD.m_baroPressure;
		m_nodeID = newD.m_nodeID;
		m_digitalOut = newD.m_digitalOut;

		newD.Clear();
	}

	// Methods
	int Log(const char*);
	void Clear();
	void Print();
	int Read_LightLevelSensor(int);
	int Read_TemperatureSensor(int);
	int Read_BarometricSensor(int);

private:
	double m_soilMoisture;
	double m_lightLevel;
	uint16_t m_temp;
	uint16_t m_baroPressure;
	uint16_t m_nodeID;
	uint16_t m_digitalOut;
};



class P_Class {
public:
	P_Class() = default;
	P_Class(P_Class&& newPull) noexcept {
		m_30min = newPull.m_30min;
		m_1hr = newPull.m_1hr;
		m_3hr = newPull.m_3hr;
		m_6hr = newPull.m_6hr;
		m_12hr = newPull.m_12hr;
		m_24hr = newPull.m_24hr;

		newPull.Clear();
	}

	// Methods
	int Pull(const char*);
	void Clear();

private:
	uint8_t m_30min;
	uint8_t m_1hr;
	uint8_t m_3hr;
	uint8_t m_6hr;
	uint8_t m_12hr;
	uint8_t m_24hr;
};



#endif
