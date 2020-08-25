#include <vector>
#include "WeatherStation.h"
#include "UnitTests.h"





int main() {
	// Logging Test
	std::cout << "D_Class Test Suite - Method=Log() Result: " << (D_Log_Test("./src/DataFile.txt") ? "SUCCESS" : "FAIL") << std::endl;

	// Pulling Test
	std::cout << "P_Class Test Suite - Method=Pull() Result: " << (P_Pull_Test("../NN_Guess_TEST.txt") ? "SUCCESS" : "FAIL") << std::endl;
	std::cin.get();
}