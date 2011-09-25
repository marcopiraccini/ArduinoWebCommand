/*
Endpoint.h
TODO
*/

#ifndef Endpoint_h
#define Endpoint_h

#include <inttypes.h>
#include <Stream.h>

/******************************************************************************
* Definitions
******************************************************************************/

class Endpoint
{
private:
  // per object data
  uint8_t _rx;
  uint8_t _tx;

  void end();
  void setTX(uint8_t transmitPin);
  void setRX(uint8_t receivePin);

public:
  // public methods
  Endpoint(uint8_t rx, uint8_t tx);
  ~Endpoint();
  void send();
  void receive();
};

#endif
