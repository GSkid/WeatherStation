#ifndef WEATHER_STATION
#define WEATHER_STATION
#include <fstream>
#include <iostream>
#include <string>
#include <stdio.h>

#define MAX_LOGS 96

class D_Class {
public:
	// Constructors

	D_Class() = default;
	D_Class(double&& wS, int16_t&& lL, uint16_t&& t, int32_t&& bP, double&& wD, double&& pA) {
		m_windSpeed = (double)wS;
		m_lightLevel = (int16_t)lL;
		m_temp = (uint16_t)t;
		m_baroPressure = (int32_t)bP;
		m_windDirection = (double)wD;
		m_precipAmount = (double)pA;
	}

	D_Class(D_Class&& newD) noexcept {
		m_windSpeed = newD.m_windSpeed;
		m_lightLevel = newD.m_lightLevel;
		m_temp = newD.m_temp;
		m_baroPressure = newD.m_baroPressure;
		m_windDirection = newD.m_windDirection;
		m_precipAmount = newD.m_precipAmount;

		newD.Clear();
	}

	// Methods
	int Log(const char*);
	int CSV_Log(const char*, const char*, std::string = "0");
	void Clear();
	void Print();
	void Set_m_precipAmount(double);
	void Set_m_windSpeed(double);
	void Set_m_windDirection(double);
	void Set_m_lightLevel(int16_t);
	void Set_m_temp(uint16_t);
	void Set_m_baroPressure(int32_t);

private:
	double m_precipAmount;
	double m_windSpeed;
	double m_windDirection;
	int16_t m_lightLevel;
	uint16_t m_temp;
	int32_t m_baroPressure;
};



class P_Class {
public:
	// Constructors 

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
