#include "WeatherStation.h"
#include "UnitTests.h"

// Logging
int D_Log_Test(const char* inputFile) {
	// Initial Hardcoded class
	D_Class WeatherData(0, 14, 1000, 27, 10, 0);

	// Calling the log method
	WeatherData.Log(std::move(inputFile));

	std::ifstream dataFile(inputFile);
	// Now we just read the file
	std::string line, data, lastLine;
	int lineCounter = 0;
	while (std::getline(dataFile, line)) {
		if (!lineCounter) data += line;
		else data += "\n" + line;
		lineCounter++;
	}
	// Now that we have the full file in data, we find the last '\n' char and read everything after it
	unsigned int startingIndex = data.find_last_of('\n') + 1;
	// With this, we are looking to place everything in the last line into lastLine
	for (unsigned int oldIndex = startingIndex; oldIndex < data.length(); oldIndex++) {
		lastLine += data[oldIndex];
	}

	// Finally we compare with what was read in vs our hardcoded value
	return((lastLine.compare("0.000000,14.000000,1000,27,1") == 0) ? 1 : 0);
}

// CSV Logging
int D_CSV_Log_Test(const char* forecastFile, const char* outputFile, const char* dateTime) {
	// Initial Hardcoded class
	D_Class WeatherData(0, 14, 1000, 27, 10, 0);

	// Calling the CSV Log method
	WeatherData.CSV_Log(std::move(forecastFile), std::move(outputFile), std::move(dateTime));

	std::ifstream csvFile(outputFile);
	// Now we just read the file
	std::string fileContents, line;
	// Get the file contents
	std::getline(csvFile, line);
	fileContents += line + '\n';
	std::getline(csvFile, line);
	fileContents += line;
	
	// Now check the file contents
	return (!fileContents.compare("Temp,Humidity,Light,WindSpeed,WindDirection,Precip,ForecastPrecip,TimeStamp\n1000,27,14,0,0,0,0,-1")) ? 1 : 0;
}


// Pulling
int P_Pull_Test(const char* inputFile) {
	P_Class PullData;
	PullData.Clear();

	PullData.Pull(std::move(inputFile));

	std::ifstream pullFile(inputFile);
	std::string data;
	if (pullFile.is_open()) {
		// Here we pull the first line from the file, which should be the only line
		std::getline(pullFile, data);
		pullFile.close();
	}

	// Finally we compare with what was read in vs the hardcoded value found in "NN_Guess_TEST.txt"
	return((data.compare("001110") == 0) ? 1 : 0);
}
	