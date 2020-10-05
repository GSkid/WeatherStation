#include <vector>
#include "WeatherStation.h"
#include "UnitTests.h"

#define TEST 1 // 1 for test, 0 for no test



int main() {
#ifdef TEST
	// Logging Test
	std::cout << "D_Class Test Suite - Method=Log() Result: " << (D_Log_Test("./src/DataFile.txt") ? "SUCCESS" : "FAIL") << std::endl;

	// Pulling Test
	std::cout << "P_Class Test Suite - Method=Pull() Result: " << (P_Pull_Test("../NN_Guess_TEST.txt") ? "SUCCESS" : "FAIL") << std::endl;
#endif //TEST
	std::cin.get();
}