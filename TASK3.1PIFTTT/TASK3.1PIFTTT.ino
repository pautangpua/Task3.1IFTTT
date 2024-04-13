#include "arduino_secrets.h"
#include <WiFiNINA.h>

//1.STORE CREDENTIAL FOR WIFI NETWORK TO CONNECT TO
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

//2. INITIALISE VARIABLE FOR SYSTEM
String value1; 
String value2;
const int sensorpin = A0; 
int sensor; 
const int threshold = 50; // Adjust threshold value based on your environment and LDR sensitivity
bool sunlightPreviouslyDetected = false;

//3.CONNECT TO SPECIFIC IP ADDRESS AND PORT 
WiFiClient client; 

//4.INITIALIZE THREE NEW VARIABLES TO STORE URL, HTTP REQUEST, TRIGGER ACTION
char   HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME   = "/trigger/terrarium_notification/with/key/ZvAxt_sFPoA_E-YXgLoRQ?value1=+"; // change your EVENT-NAME and YOUR-KEY
//String queryString = "?value1=" + value1;

//5. SETUP
void setup() {
  // initialize WiFi connection
  WiFi.begin(ssid, pass);

  Serial.begin(9600);
  while (!Serial); //DOESN'T RUN UNLESS SERIAL MONITOR OPEN

  // connect to web server on port 80:
  if (client.connect(HOST_NAME, 80)) {
    // if connected:
    Serial.println("Connected to server");
  }
  else {// if not connected:
    Serial.println("connection failed");
  }
}

//6.LOOP FOR READING SENSOR
void loop() {
  const int sensorpin = A0; 
  int sensor; 
  sensor = analogRead(sensorpin);
  String sensorValue = String(sensor);

  if (sensor > threshold) {
    if (!sunlightPreviouslyDetected) {
      Serial.print("Light Detected: ");
      Serial.println(sensor);
      sendNotification("Sunlight_Started: ", sensorValue);
      sunlightPreviouslyDetected = true;
    }
  } else {
    if (sunlightPreviouslyDetected) {
      Serial.print("Light Not Detected: ");
      Serial.println(sensor);
      sendNotification("Sunlight_Stopped: ", sensorValue);
      sunlightPreviouslyDetected = false;
    }
  }
  delay(10000); // Check every 10 seconds
}

// METHOD FOR SENDING CUSTOM NOTIFICATION
void sendNotification(String value1, String value2) {
  if (client.connect(HOST_NAME, 80)) {
    client.println("GET " + PATH_NAME + value1 + value2 + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println();
    delay(10);
  }
}


