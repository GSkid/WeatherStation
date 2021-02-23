// ********** INCLUDES **********
#include <SPI.h>
#include <EEPROM.h>
#include <Wire.h>
#include "RF24.h"
#include "RF24Network.h"
#include "RF24Mesh.h"
#include "Adafruit_BMP085.h"
#include <printf.h>
#include <avr/sleep.h>
#include <avr/power.h>

/**** Configure the Radio ****/
RF24 radio(7, 8);
RF24Network network(radio);
RF24Mesh mesh(radio, network);

/**** #Defines ****/
#define time_Thresh Timer(C_Thresh.time_thresh, 10)//D_Struct.timeStamp)

/**** GLOBALS ****/
#define nodeID 1 // Set this to a different number for each node in the mesh network
#define MOISTURE_PIN A0
#define LIGHT_PIN A1
#define BATTERY A2
#define LIQUID_SENSE 10000
#define INTERRUPT_MASK 0b01000000
#define VOLTAGE_DIVIDER 10

#define MINS_15 900000

// D_Struct stores the relevant sensor data
typedef struct {
  int32_t pressure;
  float temp_C;
//  double windSpeed;
//  double windDirection;
  int16_t lightLevel;
//  double precipAmount;
} D_Struct;

// Timers
static uint32_t sleepTimer = 0;
static uint32_t messageTimer = 0;
static uint32_t witchTimer = 60000;
static uint32_t batteryTimer = 0;

// Timer Support
static uint8_t timerFlag = 0;
static uint8_t message_Flag = 0;

// Sensor Vars
static Adafruit_BMP085 bmp;
static uint8_t bmpFlag = 0;

// RF24 Vars
static uint8_t sleepFlag = 0;

// Use these vars to store the header data
static uint8_t M_Dat = 0;

// C and D type structs
static D_Struct Data_Struct;

/**** Function Prototypes ****/
void D_Struct_Serial_print(D_Struct);
float pullMoistureSensor(void);
float getMoistureReading(void);
int16_t pullLightSensor(void);
float getLightReading(void);
uint8_t pullBatteryLevel(void);
int Timer(uint32_t, uint32_t);


void setup() {
  Serial.begin(115200);
  printf_begin();

  // Set the IO
  pinMode(MOISTURE_PIN, INPUT);
  pinMode(LIGHT_PIN, INPUT);
  pinMode(BATTERY, INPUT);

  // Begin the Barometric Pressure Sensor
  // Pin out: Vin->5V, SCL->A5, SDA->A4
  //BMP not working on sensor nodes
  if (bmp.begin()) {
    bmpFlag = 1;
  } else {
    Serial.println(F("BMP Failed to init"));
  }

  // Set this node as the master node
  mesh.setNodeID(nodeID);

  // Connect to the mesh
  Serial.println(F("Connecting to the mesh..."));
  mesh.begin();

  // Print out the mesh addr
  Serial.print(F("Mesh Network ID: "));
  Serial.println(mesh.getNodeID());
  Serial.print(F("Mesh Address: ")); Serial.println(mesh.getAddress(nodeID));
  radio.setPALevel(RF24_PA_MAX);

  //  radio.printDetails();
  Serial.println(F("**********************************\r\n"));

  // Setting the watchdog timer
  set_sleep_mode(SLEEP_MODE_IDLE);
  network.setup_watchdog(9);
  sleepFlag = 1;
}

void loop() {

  // Keep the network updated
  mesh.update();


  /**** Battery Level Check ****/
/*
  if (Timer(MINS_15, batteryTimer)) {
    batteryTimer = millis();
    uint8_t batteryVoltage = pullBatteryLevel();
    if (batteryVoltage <= 35) {
      batteryVoltage = pullBatteryLevel();
      if (batteryVoltage <= 35) {
        printf("Battery Level Low: %d\n\n------- Reset Device To Continue -------",\
          batteryVoltage);
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        while (1) {
          sleep_enable();
          sleep_cpu();
        }
      }
    }
  }

*/



  /**** Read Sensors ****/

  if (sleepFlag) {
    sleepFlag = 0; // Ensures that we only read and send a message once after waking up

    // Read all sensors
    Data_Struct.lightLevel = pullLightSensor();
    if (bmpFlag) {
      Data_Struct.temp_C = bmp.readTemperature();
      Data_Struct.pressure = bmp.readPressure();
    }
    //Data_Struct.windSpeed = 0;
    //Data_Struct.windDirection = 0;
    //Data_Struct.precipAmount = 0;


    /**** Data Transmission ****/

    if (mesh.checkConnection()) {
      // Send the D_Struct to Master
      // Sends the data up through the mesh to the master node to be evaluated
      if (!mesh.write(&Data_Struct, 'D', sizeof(Data_Struct), 0)) {
        Serial.println(F("Send failed; checking network connection."));
        // Check if still connected
        if (!mesh.checkConnection()) {
          // Reconnect to the network if disconnected and no send
          Serial.println(F("Re-initializing the Network Address..."));
          mesh.renewAddress();
          Serial.print(F("New Network Addr: ")); Serial.println(mesh.getAddress(nodeID));
        } else {
          Serial.println(F("Network connection good."));
          Serial.println(F("**********************************\r\n"));
          sleepFlag = 1;
        }
      } else {
        Serial.println(F("**********************************"));
        Serial.println(F("Sending Data to Master")); D_Struct_Serial_print(Data_Struct);
        // Set the flag to check for a failed message response
        message_Flag = 1; messageTimer = millis();
        delay(10);
      }
    } else {
      // Reconnect to the mesh if disconnected
      Serial.println(F("Re-initializing the Network Address..."));
      mesh.renewAddress();
      Serial.print(F("New Network Addr: ")); Serial.println(mesh.getAddress(nodeID));
    }

    /**** Now go to sleep for 15 minutes ****/
    uint8_t sleeping = 0;
    while (sleeping < 15) {
      network.sleepNode(8, 255); // Node goes to sleep here
      sleepFlag = 1; // Tell the node it's time to read sensors and send a message
      sleeping++;
    }
    sleeping = 0;
  }

} // Loop


/**** Helper Functions ****/

void D_Struct_Serial_print(D_Struct sct) {
  Serial.print(F("Ambient Lux Level   (lx): ")); Serial.println(sct.lightLevel);
  Serial.print(F("Ambient Temperature (C ): ")); Serial.println(sct.temp_C);
  Serial.print(F("Barometric Pressure (Pa): ")); Serial.println(sct.pressure);
  return;
}


/* @name: getLightReading
   @param: none
   @return: value of the mapped sensor value
*/
float getLightReading(void) {
  float b = -0.94;
  float c = 38.9;
  float a = 0.014;
  // First map the voltage reading
  float lightV = map(analogRead(LIGHT_PIN), 0, 1023, 0, 500);
  float mr_Lumen = lightV - b * c * 100;
  mr_Lumen /= c * 100;
  mr_Lumen = pow(mr_Lumen, 1 / a);
  // Returns the mapped analog value
  return (mr_Lumen);
}


/* @name: pullLightSensor
   @param: none
   @return: averaged value of the mapped sensor value
*/
int16_t pullLightSensor(void) {
  float read1 = getLightReading();
  delayMicroseconds(10);
  float read2 = getLightReading();
  delayMicroseconds(10);
  float read3 = getLightReading();
  delayMicroseconds(10);
  float read4 = getLightReading();
  delayMicroseconds(10);
  float read5 = getLightReading();
  return ((read1 + read2 + read3 + read4 + read5) / 5);
}

/* @name: getBatteryReading
   @param: none
   @return: mapped battery voltage in dV
*/
uint8_t getBatteryReading(void) {
  float rawVoltageDivider1 = ((float)analogRead(BATTERY) * 50.5) / 1023.0;
  delayMicroseconds(10);
  float rawVoltageDivider2 = ((float)analogRead(BATTERY) * 50.5) / 1023.0;
  delayMicroseconds(10);
  float rawVoltageDivider3 = ((float)analogRead(BATTERY) * 50.5) / 1023.0;
  delayMicroseconds(10);
  float rawVoltageDivider4 = ((float)analogRead(BATTERY) * 50.5) / 1023.0;
  delayMicroseconds(10);
  float rawVoltageDivider5 = ((float)analogRead(BATTERY) * 50.5) / 1023.0;
  delayMicroseconds(10);
  float rawVoltageDivider6 = ((float)analogRead(BATTERY) * 50.5) / 1023.0;
  delayMicroseconds(10);
  float rawVoltageDivider7 = ((float)analogRead(BATTERY) * 50.5) / 1023.0;
  delayMicroseconds(10);
  float rawVoltageDivider8 = ((float)analogRead(BATTERY) * 50.5) / 1023.0;
  delayMicroseconds(10);
  float rawVoltageDivider9 = ((float)analogRead(BATTERY) * 50.5) / 1023.0;
  delayMicroseconds(10);
  float rawVoltageDivider10 = ((float)analogRead(BATTERY) * 50.5) / 1023.0;
  delayMicroseconds(10);
  float rawVoltageDivider11 = ((float)analogRead(BATTERY) * 50.5) / 1023.0;
  float battAvg = rawVoltageDivider2 + rawVoltageDivider3 + rawVoltageDivider4 +\
    rawVoltageDivider5 + rawVoltageDivider6;
  battAvg =  battAvg + rawVoltageDivider7 + rawVoltageDivider8 + rawVoltageDivider9 +\
    rawVoltageDivider10 + rawVoltageDivider11;
  uint8_t bat_soup = (uint8_t)battAvg;
  return bat_soup;
}


/* @name: getBatteryReading
   @param: none
   @return: mapped battery voltage in dV
*/
uint8_t pullBatteryLevel(void) {
  uint8_t mr_avg = getBatteryReading() + getBatteryReading() + getBatteryReading() +\
    getBatteryReading() + getBatteryReading();
  return (mr_avg / 5);
}


/* @name: Timer
   @param: delayThresh - timer duration
   @param: prevDelay - time in millis() when the timer started
   @return: digital high/low depending if timer elapsed or not
   This is a non-blocking timer that handles uint32_t overflow,
   it works off the internal function millis() as reference
*/
int Timer(uint32_t delayThresh, uint32_t prevDelay) {
  // Checks if the current time is at or beyond the set timer
  if ((millis() - prevDelay) >= delayThresh) {
    return 1;
  } else if (millis() < prevDelay) {
    //Checks and responds to overflow of the millis() timer
    if (((4294967296 - prevDelay) + millis()) >= delayThresh) {
      return 1;
    }
  }
  return 0;
}
