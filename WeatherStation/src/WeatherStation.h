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

	// METHODS

	int Log(const char* inputFile) {
		std::ifstream dataFile(inputFile);
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

	void Clear() {
		m_soilMoisture = 0;
		m_lightLevel = 0;
		m_temp = 0;
		m_baroPressure = 0;
		m_nodeID = 0;
		m_digitalOut = 0;
	}

	void Print() {
		// Each member must be cast as an int to print them as a number instead of a char
		std::cout << "Soil Moisture: " << (int)m_soilMoisture << std::endl;
		std::cout << "Light Level: " << (int)m_lightLevel << std::endl;
		std::cout << "Temperature: " << (int)m_temp << std::endl;
		std::cout << "Barometric Pressure: " << (int)m_baroPressure << std::endl;
		std::cout << "node ID: " << (int)m_nodeID << std::endl;
		std::cout << "Digital Out: " << (int)m_digitalOut << std::endl;
	}


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


	// METHODS

	int Pull(const char* inputFile) {
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

	void Clear() {
		m_30min = 0;
		m_1hr = 0;
		m_3hr = 0;
		m_6hr = 0;
		m_12hr = 0;
		m_24hr = 0;
	}

private:
	uint8_t m_30min;
	uint8_t m_1hr;
	uint8_t m_3hr;
	uint8_t m_6hr;
	uint8_t m_12hr;
	uint8_t m_24hr;
};

#endif
