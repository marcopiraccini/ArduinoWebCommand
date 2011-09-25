/*
TODO
*/

// When set,TODO
#define _DEBUG 0
// 
// Includes
// 

#include <Endpoint.h>
#include <inttypes.h>
#include <Stream.h>


void Endpoint::send() { 
}

void Endpoint::receive() { 
}

//
// Constructor
//
Endpoint::Endpoint(uint8_t rx, uint8_t tx) 
{
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

