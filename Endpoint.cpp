/*
Endpoint.cpp
*/

// When set,TODO
#define _DEBUG 0
// 
// Includes
// 

#include <Endpoint.h>
#include <inttypes.h>
#include <Stream.h>

//
// Constructor
//
Endpoint::Endpoint(String deviceName, uint8_t rx, uint8_t tx) 
{
  setDeviceName(deviceName);
  setTX(rx);
  setRX(tx);
}

//
// Destructor
//
Endpoint::~Endpoint()
{
  end();
}

void Endpoint::setDeviceName(String deviceName)
{
  _deviceName = deviceName;
}

void Endpoint::setTX(uint8_t tx)
{
  _tx = tx;
}

void Endpoint::setRX(uint8_t rx)
{
  _rx = rx;
}

//
// Public methods
//

void Endpoint::end()
{
	// TODO
}

//
// Public methods
//

String Endpoint::getDeviceName()
{
	return _deviceName;
}



