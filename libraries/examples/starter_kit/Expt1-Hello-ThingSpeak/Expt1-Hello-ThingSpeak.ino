/*
  Button
 This example posts a no. 
 Turns on and off a light emitting diode(LED) connected to digital
 pin 4, when pressing a pushbutton attached to pin 2.


 The circuit:
 * LED attached from pin 5 to ground with a 180 resistor
 * pushbutton one end attached to pin 4 with  pullup of 10K , other end to the ground
 
 */


#include <ESP8266WiFi.h>

#define BUTTON_PIN 4             // button pin
#define LED_PIN    5              // led pin
int buttonState = 0;             // variable for reading the pushbutton status
bool buttonReleased = false;   
unsigned int  pressCount=0;
bool postOnthingspeak =false;

const char* ssid     = "xxxxxxxxxx";  //network id
const char* password = "xxxxxxxxx";     // password

const char* host = "184.106.153.149";   //thingspeak server
String GET = "GET /update?key=";         //
const char* tokenid = "H3FYQMIPNJFANH5O"; // unique id of the channel 

void setup() {
  
  Serial.begin(115200);        // define baudrate
  delay(10);
  
  pinMode(BUTTON_PIN, INPUT);  // set the button pin direction as input
  pinMode(LED_PIN, OUTPUT);    // set the LED direction as output

   Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);   
  WiFi.begin(ssid, password);   // start by connecting to Wifi network
  
  while (WiFi.status() != WL_CONNECTED) {   // retry if connection failed
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());   // print the IP address

  
}

int value = 0;

void loop() {

  buttonState = digitalRead(BUTTON_PIN);              // read the button status
  if (buttonState == LOW && buttonReleased == true)   //check whether button is pressed
  {
     Serial.println("button is press detected");
     digitalWrite(LED_PIN, HIGH);    // turn LED on:
     pressCount++;                   // increment the count
     Serial.println("Posting the count on the Thingspeak channel.....");
     postThingspeak(pressCount);     //post the count on thingspeak
     digitalWrite(LED_PIN, LOW);     // turn off the led after posting
     buttonReleased = false;         //reset the flag 
  }
  else if (buttonState == HIGH  && buttonReleased == false)  // check for the button release
  {
       buttonReleased = true;   
  }
}


void  postThingspeak(unsigned int temp)
{
        Serial.print("connecting to ");
        Serial.println(host);       
        
        // Use WiFiClient class to create TCP connections
        WiFiClient client;
        const int httpPort = 80;
        if (!client.connect(host, httpPort)) { // connect to host ip address
          Serial.println("connection failed");
          return;
        }
        
        // We now create a URI for the request
        String url = tokenid;
        url += "&field1=";
        url += pressCount;
        url += "\r\n";
        
        Serial.print("Requesting URL: ");
        Serial.println(GET+url);
  
        client.print(GET +url);   //send the request to the server
        int timeout = millis() + 5000;
         while (client.available() == 0) {
          if (timeout - millis() < 0) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
          }
        }
        
        // Read all the lines of the reply from server and print them to Serial
        while(client.available()){
          String line = client.readStringUntil('\r');
          Serial.print(line);
        }
        
        Serial.println();
        Serial.println("closing connection");
        
        postOnthingspeak = false;

}

