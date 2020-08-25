#ifndef WEATHER_STATION
#define WEATHER_STATION
class D_Class {
public:
	D_Class() = default;
	D_Class(uint8_t&& sM, uint8_t&& lL, uint8_t&& t, uint8_t&& bP, uint8_t&& ID, uint8_t&& dO) {
		printf("Init!\n");
		m_soilMoisture = sM;
		m_lightLevel = lL;
		m_temp = t;
		m_baroPressure = bP;
		m_nodeID = ID;
		m_digitalOut = dO;
	}

	D_Class(D_Class&& newD) noexcept {
		printf("Move!\n");
		m_soilMoisture = newD.m_soilMoisture;
		m_lightLevel = newD.m_lightLevel;
		m_temp = newD.m_temp;
		m_baroPressure = newD.m_lightLevel;
		m_nodeID = newD.m_nodeID;
		m_digitalOut = newD.m_digitalOut;

		newD.Clear();
	}

	// METHODS

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
		std::cout << "Soil Moisture: " << (int)m_soilMoisture << std::endl;
		std::cout << "Light Level: " << (int)m_lightLevel << std::endl;
		std::cout << "Temperature: " << (int)m_temp << std::endl;
		std::cout << "Barometric Pressure: " << (int)m_baroPressure << std::endl;
		std::cout << "node ID: " << (int)m_nodeID << std::endl;
		std::cout << "Digital Out: " << (int)m_digitalOut << std::endl;
	}

private:
	uint8_t m_soilMoisture;
	uint8_t m_lightLevel;
	uint8_t m_temp;
	uint8_t m_baroPressure;
	uint8_t m_nodeID;
	uint8_t m_digitalOut;
};

#endif
