#include <stdio.h>
#include <iostream>
#include <vector>
#include "WeatherStation.h"
#include "UnitTests.h"





int main() {
	// Logging Test
	//std::cout << "Result of Log Test: " << D_Log_Test("./src/DataFile.txt) << std::endl;

	// Pulling Test
	std::cout << "Result of Pull Test: " << P_Pull_Test("../NN_Guess.txt") << std::endl;
	std::cin.get();
}