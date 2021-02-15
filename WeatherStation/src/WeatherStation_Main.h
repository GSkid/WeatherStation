#ifndef WEATHER_STATION_MAIN
#define WEATHER_STATION_MAIN

/* Sensor Data Struct (used for transmitting data between devices) */
typedef struct {
	int32_t baroPressure;
	float temp;
	double windSpeed;
	double windDirection;
	double lightLevel;
	double precipAmount;
}Sensor_Data_Struct;

#endif //WEATHER_STATION_MAIN