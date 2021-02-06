# WeatherStation
This weather station project allows developers to get access to real-time sensor data through a JSON file that includes information like wind speed, wind direction, precipitation, light level, temperature, and barometric pressure. To get access to the JSON file, visit http://weatherdataapi.ddns.net and click on the button in the middle of the screen. 

## Motivation
When I was doing my thesis, which was a smarter irrigation system that aimed at reducing waste water irrigation (https://github.com/GSkid/SkidLess), I found it difficult to get accurate, real-time weather data. While I had already planned to use my own sensors, I was hoping to augment my system with a more reliable source of weather data to alleviate strain on the sensor system; however, I realized that getting reliably accurate weather information in a form that was easily usable by my system was hard to find. We ended up using the Dark Sky API, which was an open source project that allowed our system to draw weather data from the closest national weather station. But since then, it has since been acquired by Apple who subsequently locked it behind a pay wall, rendering it effectively useless for small projects like this. So I wanted to create a system that allows users to quickly and easily draw up-to-date weather data from a reliable source.

## Build Status
The code base is built, the website is actively hosted, but the sensors still need to be built. This means that the project is up, but not working. The sensors will still take time to get the parts and to build.

## Technologies / Frameworks
Hosted with:
- Flask
- Gunicorn
- NGINX
- NoIP

Libraries used:
- RF24
- RF24 Network
- RF24 Mesh

Built with:
- Microsoft Visual Studio
- GNU Make

## Installation
First, clone the repository: most necessary files are included in the respective directories.
Then you need to install flask (on Linux use the command "pip install flask").
Then install Gunicorn (on Linux use the command "pip install gunicorn").
Then install nginx (on Debian Linux use the command "sudo apt-get install nginx" after updating).

## Tests
To run the tests, navigate to /WeatherStation/WeatherStation/src and open the WeatherStation_Config.h file. In this file, uncomment the line with #define TEST_SUITE. Then back in the terminal in the same /src directory, run the commands "make clean" followed by "make test". Now run the command "./Test_WeatherStation". This should run the test suite and show you which tests from the WeatherStation.cpp file passed or failed.

## How to Use?
Simply go to http://weatherdataapi.ddns.net and click the button in the middle. This opens the JSON file with the most recent weather data; if you want to save the file, then you can right click and select "save as" which will allow you to save the JSON file. Or on Linux, you can download the file via the terminal using curl coupled with the url: http://weatherdataapi.ddns.net/api_get