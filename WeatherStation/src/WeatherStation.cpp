#include <stdio.h>
#include <iostream>
#include <vector>
#include "WeatherStation.h"





int main() {
	D_Class WeatherData(0, 14, 1000, 27, 10, 0);

	WeatherData.Log();
	WeatherData.Clear();

	std::cin.get();
}