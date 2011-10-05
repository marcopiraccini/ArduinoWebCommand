#include <SPI.h>
#include <Ethernet.h>
#include <Endpoint.h>
#include <SerialEndpoint.h>



/*
  Web Command Serial Library
  Generic Command library used to interact with Arudino + Ethernet shield.
  Listen for geenrci command requests, applied to Endpoints. 
  Send the request to the "endpoints"and waits 
  for the answer.
  Uses code from RESTduion project (https://github.com/jjg/RESTduino)
  and the Arduino's Ethernet Shield tutorials. 
  NOTE: The ethernet shield should use pins: 10, 11, 12, 13 (to be verified).
 */

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 1, 177 };

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

//  url buffer size
#define BUFSIZE 255

// The used Endpoints
static SerialEndpoint endpoints[4] = {
    SerialEndpoint("1", 2,3),
    SerialEndpoint("2", 4,5),
    SerialEndpoint("3", 6,7),
    SerialEndpoint("4", 8,9)
};


void setup()
{
  Serial.begin(9600);
  Serial.println("Web Command Server ##############################");
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
  EthernetClient client = server.available();
  
  if (client) {

    //  reset input buffer
    index = 0;

    while (client.connected()) {
      if (client.available()) {
        boolean currentLineIsBlank = true;
         
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
        char *data = strtok(NULL,"/");

        //  this is where we actually *do something*!
        String outValue;

        if(device != NULL){

          if(data != NULL){
            // Some data to send....
            //  set the pin value
            Serial.print("Data for the device:");
            Serial.print(device);                  
            Serial.print(" - Data:");
            Serial.println(data);               
                        
            // Sends the data
            outValue = sendData(device, data);
            
            returnAnswer(client, device, data, outValue);
            
          } else {
            
            // Receive the data            
            outValue = receiveData(device);
            
            //  assemble the json output
            returnAnswer(client, device, "NULL", outValue);       
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
void returnAnswer(EthernetClient client, String device, String command, String response) {
  String jsonOut = String();
  //  assemble the json output
  jsonOut += "{\"";
  jsonOut += device;
  jsonOut += "\":\"";
  jsonOut += command;
  jsonOut += "\":\"";
  jsonOut += response;
  jsonOut += "\"}";
 
  //  return value with wildcarded Cross-origin policy
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Access-Control-Allow-Origin: *");
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
 * Sends the data
 */
String sendData(char *device, char *data) {
    Serial.print("Sending data: ");
    Serial.print(data);
    Serial.print(" on device: ");
    Serial.println(device);
    String deviceString = String(device);
    int i = 0;
    for (int i=0; i < sizeof(endpoints)/sizeof(*endpoints); i++)
    {
        Serial.println(endpoints[i].getDeviceName());
        if (deviceString == endpoints[i].getDeviceName()) 
        {
            Serial.println("Found device:" + endpoints[i].getDeviceName());
            endpoints[i].send(data);
        }        
    }
    return "response";        
}

/**
 * Receive the data
 */
String receiveData(char *device) {
    Serial.print("Receiveing data from device: ");
    Serial.println(device);
    String deviceString = String(device);
    int i = 0;
    String response;
    for (int i=0; i < sizeof(endpoints)/sizeof(*endpoints); i++)
    {
        Serial.println(endpoints[i].getDeviceName());
        if (deviceString == endpoints[i].getDeviceName()) 
        {
            Serial.println("Found device:" + endpoints[i].getDeviceName());
            response = endpoints[i].receive();
        }        
    }
    return response;        
}

