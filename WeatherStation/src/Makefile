CXXFLAGS = -Ofast -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s -std=c++0x -Wall -Wextra
VPATH = RF24:RF24Network:RF24Mesh
LIBS = -lrf24-bcm -lrf24network -lrf24mesh

test: WeatherStation_Main.o WeatherStation.o UnitTest.o
	g++ WeatherStation_Main.o WeatherStation.o UnitTest.o $(CXXFLAGS) -Wall -I./ $(LIBS) -pthread -o Test_WeatherStation

run: WeatherStation_Main.o WeatherStation.o
	g++ WeatherStation_Main.o WeatherStation.o $(CXXFLAGS) -Wall -I./ $(LIBS) -pthread -o WeatherStation_Run

WeatherStation_Main.o: WeatherStation_Main.cpp
	g++ -c WeatherStation_Main.cpp $(CXXFLags) -Wall -I./ $(LIBS) -pthread 

WeatherStation.o: WeatherStation.cpp
	g++ -c WeatherStation.cpp $(CXXFlags) -Wall -I./ -pthread

UnitTest.o: UnitTests.cpp
	g++ -c UnitTests.cpp $(CXXFLAGS) -Wall -I./ -pthread -o UnitTest.o

clean-test:
	rm Test_WeatherStation
	rm ./*.o

clean-run: 
	rm WeatherStation_Run
	rm ./*.o
