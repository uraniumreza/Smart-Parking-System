/*
    This sketch sends data via HTTP GET requests to data.sparkfun.com service.

    You need to get streamId and privateKey at data.sparkfun.com and paste them
    below. Or just customize this script to talk to other HTTP servers.

*/

#include <ESP8266WiFi.h>


// Use WiFiClient class to create TCP connections
WiFiClient client;

const char* ssid     = "CSE-107";
const char* password = "1stCSE@BUET";

const char* host = "172.20.22.96";
char* parkingLotId = "4";
int floorId = 1;
int freeSpaceSmall1 = 0;
int freeSpaceMedium1 = 10;
int freeSpaceSmall2 = 10;
int freeSpaceMedium2 = 0;

const int ledPin = D0; // the pin that the LED is attached to

void setup() {
  Serial.begin(9600);
  delay(10);

  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  Serial.print("connecting to ");
  Serial.println(host);
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  Incoming();
  Incoming();
}

void loop() {
  if (Serial.available() > 0) {
    char incomingByte;      // a variable to read incoming serial data into
    // read the oldest byte in the serial buffer:
    incomingByte = Serial.read();

    // if it's a capital H (ASCII 72), turn on the LED:
    if (incomingByte == 'A' || incomingByte == 'B' || incomingByte == 'C' || incomingByte == 'D' || incomingByte == 'E') {
      if (incomingByte == 'A' || incomingByte == 'B' || incomingByte == 'C') {
        freeSpaceMedium1--;
        floorId = 1;
      }
      else if (incomingByte == 'D' || incomingByte == 'E') {
        freeSpaceSmall2--;
        floorId = 2;
      }

      Incoming();
    }
    // if it's an L (ASCII 76) turn off the LED:
    else if (incomingByte == 'P' || incomingByte == 'Q' || incomingByte == 'R' || incomingByte == 'S' || incomingByte == 'T') {
      if (incomingByte == 'P' || incomingByte == 'Q' || incomingByte == 'R') {
        freeSpaceMedium1++;
        floorId = 1;
      }
      else if (incomingByte == 'S' || incomingByte == 'T') {
        freeSpaceSmall2++;
        floorId = 2;
      }

      Outgoing();
    }

  }

  String url;
  url += "http://";
  url += host;
  url += "/myweb/public/index.php/getdata/";
  url += parkingLotId;
  url += "/";
  url += floorId;
  url += "/";
  if(floorId == 1) url += freeSpaceSmall1;
  else url += freeSpaceSmall2;
  url += "/";
  if(floorId == 1) url += freeSpaceMedium1;
  else url += freeSpaceMedium2;
  //url += "/";

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: open\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");
}

void Incoming()
{
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
}

void Outgoing()
{
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
}
