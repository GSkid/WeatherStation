#ifndef UNIT_TESTING
#define UNIT_TESTING
#include "WeatherStation.h"



// Function Prototypes

/*@Function: D_Log_Test
* @Param: const char* inputFile - file path to the file where weather data is to be logged
* @Return: int - 1 if data file matched the expected format of the data log file when logging the hardcoded data
* @Desc: Logs a hardcoded data struct to the provided file location of inputFile. This function checks that Log() 
* correctly logs the data in the proper format.
*/
int D_Log_Test(const char*);

/*@Function: D_CSV_Log_Test
* @Param: const char* inputFile - file path to the file where weather data is to be logged
* @Return: int - 1 if data file matched the expected format of the data log file when logging the hardcoded data
* @Desc: Logs a hardcoded data struct to the provided file location of inputFile. This function checks that Log()
* correctly logs the data in the proper format.
*/
int D_CSV_Log_Test(const char*, const char*, const char*);

/*@Function: P_Pull_Test
* @Param: const char* inputFile - file path to the file where weather data is stored
* @Return: int - 1 if pulled data matched what is hardcoded to the test output file (currently hardcoded)
* @Desc: Pulls data from the python TEST file and verifies that the data matches the hardcoded values.
*/
int P_Pull_Test(const char*);

#endif //UNIT_TESTING