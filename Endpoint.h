/*
Endpoint.h
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
  String _deviceName;
  uint8_t _rx;
  uint8_t _tx;

protected:
  void end();
  void setDeviceName(String deviceName);
  void setTX(uint8_t transmitPin);
  void setRX(uint8_t receivePin);

public:
  // public methods
  Endpoint(String deviceName, uint8_t rx, uint8_t tx);
  ~Endpoint();

  String getDeviceName();
  // pure virtual methods...
  virtual void send(String command) = 0;
  virtual String receive() = 0;
};

#endif
