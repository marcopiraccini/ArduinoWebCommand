#include <SPI.h>

#include <Ethernet.h>
#include <Udp.h>
#include <Client.h>
#include <Server.h>

#include <Endpoint.h>

/*
  Web Command Serial Library
  Generic Command library used to interact with Arudino + Ethernet shield.
  Listen for generic command requests, applied to Endpoints. 
  Send the request to the "endpoints"and waits 
  for the answer. Uses code from RESTduino project (https://github.com/jjg/RESTduino)
  and the Arduino's Ethernet Shield tutorials. 
 */

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 1, 177 };

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
Server server(80);

//  url buffer size
#define BUFSIZE 255

void setup()
{
  Serial.begin(9600);
  Serial.println("Web Command Server ##################");
  Serial.print("Starting server with IP:");
  Serial.print(ip_to_str(ip));
  Serial.print(" and MAC:");
  Serial.println(ip_to_str(mac));
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.println("...server started");
}

void loop()
{
  // Serve a request 
  serveRequest();
}

/**
 * Serve a request.
 * // TODO: Anaylze the Ethernet shield behaviour under heavy concurrency. 
 */
void serveRequest() {
  
  int index = 0;
  char clientline[BUFSIZE];
  
    // listen for incoming clients
  Client client = server.available();
  if (client) {

    //  reset input buffer
    index = 0;

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        //  fill url the buffer
        if(c != '\n' && c != '\r'){
          clientline[index] = c;
          index++;

          //  if we run out of buffer, overwrite the end
          if(index >= BUFSIZE)
            index = BUFSIZE -1;

          continue;
        } 

        //  convert clientline into a proper
        //  string for further processing
        String urlString = String(clientline);

        //  extract the operation
        String op = urlString.substring(0,urlString.indexOf(' '));

        //  we're only interested in the first part...
        urlString = urlString.substring(urlString.indexOf('/'), urlString.indexOf(' ', urlString.indexOf('/')));

        //  put what's left of the URL back in client line
        urlString.toCharArray(clientline, BUFSIZE);

        //  get the first two parameters
        char *device = strtok(clientline,"/");
        char *command = strtok(NULL,"/");

        //  this is where we actually *do something*!
        String outValue;

        if(device != NULL){
          int selectedDevice = atoi (device);
          if(command != NULL){
            //  set the pin value
            Serial.print("Command for the device:");
            Serial.print(selectedDevice);                  
            Serial.print(" - Command:");
            Serial.println(command);               
                        
            // TODO
            // PUT ACTIONS FOR THE COMMAND
            outValue = processCommand(device, command);
            
            returnAnswer(client, selectedDevice, command, outValue);
            
          } else {
            //  assemble the json output
            returnAnswer(client, selectedDevice, "NULL", outValue);       
          }
        } else {
          
          //  error
          Serial.println("erroring");
          client.println("HTTP/1.1 404 Not Found");
          client.println("Content-Type: text/html");
          client.println();
          
        }
        break;
      }
    }

    // give the web browser time to receive the data
    delay(1);
    
    // close the connection:
    client.stop();  
  }
}

/**
 * Return the JSON answer using the Client. 
 */
void returnAnswer(Client client, String device, String command, String response) {
  String jsonOut = String();
  //  assemble the json output
  jsonOut += "{\"";
  jsonOut += device;
  jsonOut += "\":\"";
  jsonOut += command;
  jsonOut += "\":\"";
  jsonOut += response;
  jsonOut += "\"}";
 
  //  return status
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.println(jsonOut);     
}

/**
 * Just a utility function to nicely format an IP address.
 */
const char* ip_to_str(const uint8_t* ipAddr)
{
  static char buf[16];
  sprintf(buf, "%d.%d.%d.%d\0", ipAddr[0], ipAddr[1], ipAddr[2], ipAddr[3]);
  return buf;
}

/**
 * PRocess the command. 
 */
String processCommand(char *device, char *command) {
    Serial.print("Executing command: ");
    Serial.print(command);
    Serial.print(" on device: ");
    Serial.println(atoi (device));
    // TODO: implement. 
    return "result";
}
