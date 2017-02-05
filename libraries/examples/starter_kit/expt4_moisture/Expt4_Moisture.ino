
#include <ESP8266WiFi.h>

//LDR Pin
#define MIOSTURE_PIN A0    

const char* ssid     = "circuitects";  // 
const char* password = "open1234";

const char* host = "184.106.153.149"; //thingspeak server
String GET = "GET /update?key=";//QCB1BEGJSVX0F65W&field1=232\r\n"; // GET /update?key=APIkey&field1=
const char* tokenid = "QCB1BEGJSVX0F65W";
int sensorValue = 0; // variable to store the value coming from the sensor

void setup() {
  
  Serial.begin(9600); //sets serial port for communication
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
  pinMode(MIOSTURE_PIN, INPUT); 
  
}
void loop() {
  
     sensorValue = analogRead(MIOSTURE_PIN); // read the value from the sensor
     Serial.println(sensorValue); //prints the values coming from the sensor on the screen
     postThingspeak(sensorValue); 
     delay(1000);                                     //waiting a second
     
}



void  postThingspeak(unsigned int temp)
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
        url += temp;
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

      

}
