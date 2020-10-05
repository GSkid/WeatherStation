#include <vector>
#include "WeatherStation.h"
#include "UnitTests.h"





/********** D_CLASS METHODS **********/

/*@Function: D_Class::Log
* @Param: const char* outPut - file path to the file where weather data is to be logged
* @Return: int - 1 if the outputFile was successfully opened, 0 if it fails to open
* @Desc: Logs the private data of the D_Class object to an output file. First checks the
* output file, deleting a line if there are at least MAX_LOGS lines (this represents a 
* 24 hour time period). Then the data gets logged in the format - 
* Soil Moisture (f), Light Level (f), Temperature (d), Barometric Pressure (d), Node ID (d), Digital Out (d)
*/
int D_Class::Log(const char* outputFile) {
	std::ifstream dataFile(outputFile);
	// Double check the txt file was able to be opened
	if (dataFile.is_open()) {
		std::string line, data, outData;
		int lineCounter = 0;
		// Now we count the number of data logs to see if we need to delete an entry
		while (std::getline(dataFile, line)) {
			if (!lineCounter) {
				data += line;
			}
			else {
				data += "\n" + line;
			}
			lineCounter++;
		}
		// Here we check to see if there are 24 hours worth of logs
		if (lineCounter >= MAX_LOGS) {
			unsigned int startingIndex = data.find_first_of('\n') + 1;
			// Now we have to copy over all the old data except the first line, which starts after the first '\n' char
			for (unsigned int old_index = startingIndex; old_index < data.length(); old_index++) {
				outData += data[old_index];
			}
		}
		// Now close the input file
		dataFile.close();

		// Here we open the output file
		std::ofstream dataFile("./src/DataFile.txt");
		// Then we add all the contents from the most recent data pull to a temp string
		char strBuffer[30];
		std::snprintf(strBuffer, 30, "\n%f,%f,%d,%d,%d,%d,", m_soilMoisture, m_lightLevel, m_temp, m_baroPressure, m_nodeID, m_digitalOut);
		// And add the temp string to the end of the output string
		outData += strBuffer;
		// Then write the outputData to the dataFile
		dataFile << outData;
		dataFile.close();
	}
	// Else if we can't find the txt file
	else {
		std::cout << "DataFile.txt does not exist or cannot be found" << std::endl;
		return 0;
	}
	return 1;
}


/*@Function: D_Class::Clear
* @Param: void
* @Return: void
* @Desc: Clears all the private data in the D_Class object, setting it to 0
*/
void D_Class::Clear() {
	m_soilMoisture = 0;
	m_lightLevel = 0;
	m_temp = 0;
	m_baroPressure = 0;
	m_nodeID = 0;
	m_digitalOut = 0;
}


/*@Function: D_Class::Print
* @Param: void
* @Return: void
* @Desc: Prints all the private data in the  D_Class object
*/
void D_Class::Print() {
	// Each member must be cast as an int to print them as a number instead of a char
	std::cout << "Soil Moisture: " << (int)m_soilMoisture << std::endl;
	std::cout << "Light Level: " << (int)m_lightLevel << std::endl;
	std::cout << "Temperature: " << (int)m_temp << std::endl;
	std::cout << "Barometric Pressure: " << (int)m_baroPressure << std::endl;
	std::cout << "node ID: " << (int)m_nodeID << std::endl;
	std::cout << "Digital Out: " << (int)m_digitalOut << std::endl;
}


/*@Function: D_Class::Print
* @Param: void
* @Return: void
* @Desc: Prints all the private data in the  D_Class object
*/
int D_Class::Read_LightLevelSensor(int pinID) {
	return 1;
}


/******** P_CLASS METHODS **********/

/*@Function: P_Class::Pull
* @Param: const char* inputFile - file path to the text file where neural network data is stored
* @Return: int - 1 if the outputFile was successfully opened, 0 if it fails to open
* @Desc: Pulls the data from the file path provided by the inputFile string. This data then gets
* read and stored to the P_Class object's private data.
*/
int P_Class::Pull(const char* inputFile) {
	std::ifstream pullFile(inputFile);
	if (pullFile.is_open()) {
		std::string data;
		// Here we pull the first line from the file, which should be the only line
		std::getline(pullFile, data);
		pullFile.close();

		// Now that we have the data, we just need to transfer it to the class attributes
		m_30min = data[0];
		m_1hr = data[1];
		m_3hr = data[2];
		m_6hr = data[3];
		m_12hr = data[4];
		m_24hr = data[5];

	}
	// If the file can't be opened or found
	else {
		std::cout << "File cannot be opened or found" << std::endl;
		return 0;
	}
	return 1;
}


/*@Function: P_Class::Clear
* @Param: void
* @Return: void
* @Desc: Resets all the data in the P_Class object's private data to 0
*/
void P_Class::Clear() {
	m_30min = 0;
	m_1hr = 0;
	m_3hr = 0;
	m_6hr = 0;
	m_12hr = 0;
	m_24hr = 0;
}