/*
SerialEndpoint.cpp

TODO: add the static array for the Serial endpoints
*/

#include <SerialEndpoint.h>


SerialEndpoint::SerialEndpoint(String deviceName, uint8_t rx, uint8_t tx) : Endpoint(deviceName, rx, tx), serialEndpoint(tx, rx) 
{
	setDeviceName(deviceName);
	setTX(rx);
	setRX(tx);
	// TODO: the rate shuold be a parameter?
	serialEndpoint.begin(4800);
}

void SerialEndpoint::send(String command) {
	serialEndpoint.println(command);
}
 
String SerialEndpoint::receive() {
	if (serialEndpoint.available()) {
		// read data from the buffer 
		int data = serialEndpoint.read();
		return String(data);
	} else {
		return NULL;
	}
}



