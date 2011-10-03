/*
SerialEndpoint.cpp

TODO: add the static array for the Serial endpoints
*/

#include <SerialEndpoint.h>

SerialEndpoint::SerialEndpoint(String deviceName, uint8_t rx, uint8_t tx) :Endpoint(deviceName, rx, tx) {
	setDeviceName(deviceName);
	setTX(rx);
	setRX(tx);
}

void SerialEndpoint::send(String command) {
	// TODO
}
 
String SerialEndpoint::receive() {
	// TODO
}



