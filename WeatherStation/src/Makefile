CXXFLAGS = -Ofast -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s -std=c++0x -Wall -Wextra
VPATH = RF24:RF24Network:RF24Mesh
LIBS = -lrf24-bcm -lrf24network -lrf24mesh

WeatherStation: WeatherStation_Main.o WeatherStation.o
	g++ WeatherStation_Main.o WeatherStation.o $(CXXFLAGS) -Wall -I./ $(LIBS) -pthread -o WeatherStation_Run

WeatherStation_Main.o: WeatherStation_Main.cpp
	g++ -c WeatherStation_Main.cpp $(CXXFLags) -Wall -I./ $(LIBS) -pthread 

WeatherStation.o: WeatherStation.cpp
	g++ -c WeatherStation.cpp $(CXXFlags) -Wall -I./ -pthread

clean:
	rm ./*.o
	rm WeatherStation_Run
