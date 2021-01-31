#include <vector>
#include "WeatherStation_Config.h"
#include "WeatherStation.h"
#include "WeatherStation_Main.h"
#ifdef TEST_SUITE
#include "UnitTests.h"
#endif //TEST_SUITE
#ifdef RF24
#include "RF24/RF24.h"
#include "RF24Network/RF24Network.h"
#include "RF24Mesh/RF24Mesh.h"
#endif //RF24
#ifdef RPi
#include "RF24/utility/RPi/bcm2835.h"
#endif //RPi

// Defines
#define DATA_FLAG           (flags)
#define TIMER_15_MINS_FLAG  (flags << 1)

#define MINUTES_15 (900000)

/* Helper function declarations */
#ifdef RF24
uint8_t rf24_setup();
#endif //RF24
#ifdef RPi
int Timer(const uint32_t& delayThresh, uint32_t& prevDelay);
#endif //RPi

/* Static module level variables */
static uint16_t flags = 0;
static uint32_t CSV_Log_Timer = 0;



int main() {
#ifdef TEST_SUITE
	// Logging Test
	std::cout << "D_Class Test Suite - Method=Log() Result: " << (D_Log_Test("./DataFile.csv") ? "SUCCESS" : "FAIL") << std::endl;
	std::cout << "                     Method=CSV_Log() Result: " << (D_CSV_Log_Test("../../NN_Guess_TEST.txt", "../../Website/WeatherData_TEST.csv", "-1") ? "SUCCESS" : "FAIL") << std::endl;

	// Pulling Test
	std::cout << "\nP_Class Test Suite - Method=Pull() Result: " << (P_Pull_Test("../../NN_Guess_TEST.txt") ? "SUCCESS" : "FAIL") << std::endl;
    std::cin.get();
#endif //TEST_SUITE

	/********* SETUP *********/
#ifdef DEBUG
	std::cout << "Starting Set-up..." << std::endl;
#endif //DEBUG
#ifdef RF24
	// Here we initialize any variables and call rf24_setup to setup the wireless network
	rf24_setup();
    
    	// Sensor_Data_Struct for retrieving info from sensor node
    	Sensor_Data_Struct Sensor_Data;
#endif //RF24

	// Weather data object
	D_Class WeatherData;
	WeatherData.Clear(); // Clear it to init all member data to 0

	// Forecast object
	P_Class ForecastPredictions;
	ForecastPredictions.Clear(); // Clear it to init all member data to 0

#ifdef RF24
	RF24 radio(RPI_BPLUS_GPIO_J8_22, RPI_BPLUS_GPIO_J8_24, BCM2835_SPI_SPEED_8MHZ);
	RF24Network network(radio);
	RF24Mesh mesh(radio, network);
#endif //RF24


    /********* Main Control Loop *********/
	/**** 
	* The main control loop follows these steps:
	* 1) Wait for RF24 messages from the sensor node
	* 2) Update the D_Class object members
	* 3) Log the data to the output data stream file
	* 4) Call the python neural network forecast file
	* 5) Log the data to the Weather Data's csv file for output (to be used by website)
	*****/
#ifdef DEBUG
	std::cout << "Starting maing loop..." << std::endl;
#endif //DEBUG

    while (1) {
        /********* STEP 1 *********/
        /* Wait for RF24 Messages from the sensor node*/
#ifdef RF24
        // Keep the network updated
        mesh.update();

        // Since this is the master node, we always want to be dynamically assigning addresses the new nodes
        mesh.DHCP();

        // Check for incoming data from other nodes
        if (network.available()) {
            // Create a header var to store incoming network header
            RF24NetworkHeader header;
            // Get the data from the current header
            network.peek(header);

            // First ensure the message is actually addressed to the master
            if (header.to_node == 0) {
                // Switch on the header type to sort out different message types
                switch (header.type) {
                    // Retrieve the data struct for D class messages
                case 'D':
#ifdef DEBUG
		    std::cout << "Message Received From Sensor Node." << std::endl;
#endif //DEBUG
                    // Use the data object to store data messages
                    network.read(header, &Sensor_Data, sizeof(Sensor_Data));
                    // Set the flag that indicates we need to respond to a new message
                    DATA_FLAG = 1;
                    break;

                    // Do not read the header data if incorrect message type/corrupt data
                default:
                    break;
                }
            }
            // Generally will never get here as network.available is only high when a message is directed to this specific node
            // Regardless, reading it will just remove the message from the input buffer
            else network.read(header, 0, 0);
        }

        /********* STEP 2 *********/
        /* Update the D Class object members */

        if (DATA_FLAG) {
#ifdef DEBUG
	    std::cout << "Updating Weather Object" << std::endl;
#endif //DEBUG
            WeatherData.Set_m_baroPressure(Sensor_Data.baroPressure);
            WeatherData.Set_m_temp(Sensor_Data.temp);
            WeatherData.Set_m_windSpeed(Sensor_Data.windSpeed);
            WeatherData.Set_m_windDirection(Sensor_Data.windDirection);
            WeatherData.Set_m_lightLevel(Sensor_Data.lightLevel);
            WeatherData.Set_m_precipAmount(Sensor_Data.precipAmount);
        }

        /********* STEP 3 *********/
        /* Log the data to the output data stream file */
        if (DATA_FLAG) {
#ifdef DEBUG
	    std::cout << "Logging to Output Data Stream File." << std::endl;
#endif //DEBUG
	    WeatherData.Log("/home/pi/Desktop/WeatherStation/WeatherStation/src/DataFile.csv");
	}

        /********* STEP 4 *********/
        /* Call the python neural network forecast file */

#endif //RF24

        /********* STEP 5 *********/
        /* Log the data to the Weather Data's csv file for output (to be used by website) */
#ifdef RPi
        if (DATA_FLAG || Timer(MINUTES_15, CSV_Log_Timer)) {
#ifdef DEBUG
	    std::cout << "Logging to Website CSV File." << std::endl;
#endif //DEBUG
            // Log the data to the Website output file
            WeatherData.CSV_Log("/home/pi/Desktop/WeatherStation/NN_Guess_TEST.txt", "/home/pi/Desktop/WeatherStation/Website/WeatherData.csv", "0");
            DATA_FLAG = 0;
	}
#endif //RPi
    }

	return 0;
}


#ifdef RF24
/**** RF24_Setup ****/
/* @Param: none
*  @Return: whether network set-up was successful or not
*  @Desc: Sets up this device as the master node in charge of the network.
*         Also sets the power level of the antenna and prints out debugging info to the terminal if in testing mode
*/
uint8_t rf24_setup() {
	// Set this node as the master node
	mesh.setNodeID(nodeID);
	printf("Node ID: %d\n", nodeID);
	radio.setPALevel(RF24_PA_MAX);

	// Initialize the mesh and check for proper chip connection
	if (mesh.begin()) printf("\nInitialized: %d\n", radio.isChipConnected();

	// Print out debugging information
	radio.printDetails();
	return;
}
#endif //RF24

#ifdef RPi
/*@name: Timer
   @param: delayThresh - timer duration
   @param: prevDelay - time in millis() when the timer started
   @return: digital high/low depending if timer elapsed or not
   This is a non-blocking timer that handles uint32_t overflow,
   it works off the internal bcm2835 function millis() as reference
*/
int Timer(const uint32_t& delayThresh, uint32_t& prevDelay)
{
    // Checks if the current time is at or beyond the set timer
    if ((bcm2835_millis() - prevDelay) >= delayThresh)
    {
        prevDelay = millis();
        return 1;
    }
    // Now we check if millis() (the current time) is less than the previous time
    // This is a unique check bc the only time we should get this is if we overflowed
    else if (millis() < prevDelay)
    {
        // We still have to check if the total time elapsed is greater than the timer delay
        if (((4294967296 - prevDelay) + bcm2835_millis()) >= delayThresh)
        {
            prevDelay = millis();
            return 1;
        }
    }
    return 0;
}
#endif //RPi
