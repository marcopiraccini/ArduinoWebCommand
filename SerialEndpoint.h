#ifndef serialendpoint_h
#define serialendpoint_h

#include "Endpoint.h"

class SerialEndpoint : public Endpoint {

public:
  SerialEndpoint(String deviceName, uint8_t rx, uint8_t tx);

  void send(String command);
  String receive();
};

#endif
