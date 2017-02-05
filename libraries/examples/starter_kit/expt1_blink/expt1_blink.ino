#include <ESP8266WiFi.h>

#define BUTTON_PIN 4   // butt
#define LED_PIN   5  //
int buttonState = 0;         // variable for reading the pushbutton status
bool buttonReleased = false;
unsigned int  pressCount=0;
bool postOnthingspeak =false;

const char* ssid     = "circuitects";  // 
const char* password = "open1234";

const char* host = "184.106.153.149"; //thingspeak server
String GET = "GET /update?key=";//QCB1BEGJSVX0F65W&field1=232\r\n"; // GET /update?key=APIkey&field1=
const char* tokenid = "OFZEOKCL95S8HRNI";

void setup() {
  Serial.begin(115200);
  delay(10);
  
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  
  Serial.begin(9600);
  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  
}

int value = 0;

void loop() {

  buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == LOW && buttonReleased == true)   //check whether button is pressed
  {
    // turn LED on:
     digitalWrite(LED_PIN, HIGH);
     pressCount++;
     postThingspeak(pressCount); 
     buttonReleased = false;
  }
  else if (buttonState == LOW  && buttonReleased == false)
  {
    // turn LED off:
   
    buttonReleased = true;
  }
}




void  postThingspeak(unsigned int count)
{
        Serial.print("connecting to ");
        Serial.println(host);
        
        // Use WiFiClient class to create TCP connections
        WiFiClient client;
        const int httpPort = 80;
        if (!client.connect(host, httpPort)) {
          Serial.println("connection failed");
          return;
        }
        
        // We now create a URI for the request
        String url = tokenid;
        url += "&field1=";
        url += count;
        url += "\r\n";
        
        Serial.print("Requesting URL: ");
        Serial.println(GET+url);
     // This will send the request to the server
        client.print(GET +url);
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
        digitalWrite(LED_PIN, LOW);
        postOnthingspeak = false;

}

