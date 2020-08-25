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
		printf("Init!\n");
		m_soilMoisture = (double)sM;
		m_lightLevel = (double)lL;
		m_temp = (uint16_t)t;
		m_baroPressure = (uint16_t)bP;
		m_nodeID = (uint16_t)ID;
		m_digitalOut = (uint16_t)dO;
	}

	D_Class(D_Class&& newD) noexcept {
		printf("Move!\n");
		m_soilMoisture = newD.m_soilMoisture;
		m_lightLevel = newD.m_lightLevel;
		m_temp = newD.m_temp;
		m_baroPressure = newD.m_baroPressure;
		m_nodeID = newD.m_nodeID;
		m_digitalOut = newD.m_digitalOut;

		newD.Clear();
	}

	// METHODS

	int Log() {
		std::ifstream dataFile("./src/DataFile.txt");
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
			std::cout << lineCounter << std::endl;
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
			//std::cout << outData << std::endl;
			// Then write the outputData to the dataFile
			dataFile << outData;
			dataFile.close();
		}
		// Else if we can't find the txt file
		else {
			std::cout << "DataFile.txt does not exist or cannot be found" << std::endl;
		}
		return 0;
	}

	void Clear() {
		printf("Clear!\n");
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

#endif
