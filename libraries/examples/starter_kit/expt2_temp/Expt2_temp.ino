#include <ESP8266WiFi.h>

//TMP36 Pin Variables
#define SENSOR_PIN A0 //the analog pin the TMP36's Vout (sense) pin is connected to
                        //the resolution is 10 mV / degree centigrade with a
                        //500 mV offset to allow for negative temperatures

 const char* ssid     = "circuitects";
const char* password = "open1234";

const char* host = "184.106.153.149"; //thingspeak server
String GET = "GET /update?key=";//QCB1BEGJSVX0F65W&field1=232\r\n"; // GET /update?key=APIkey&field1=
const char* tokenid = "VZBSRO95ALGP4XWH";


/*
 * setup() - this function runs once when you turn your Arduino on
 */
void setup()
{
  Serial.begin(9600);  //Start the serial connection with the computer
                       //to view the result open the serial monitor 
  pinMode(SENSOR_PIN, INPUT);                     

  
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
 
void loop()                     // run over and over again
{
 //getting the voltage reading from the temperature sensor
 int reading = analogRead(SENSOR_PIN);  
 
 // converting that reading to voltage, for 3.3v arduino use 3.3
 float voltage = reading * 3.3	;
 voltage /= 1024.0; 
 
 // print out the voltage
 Serial.print(voltage); Serial.println(" volts");
 
 // now print out the temperature
 float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
                                               //to degrees ((voltage - 500mV) times 100)
 Serial.print(temperatureC); Serial.println(" degrees C");
 temperatureC = 26.7;
 postThingspeak(temperatureC); 
 delay(1000);                                     //waiting a second
}


void  postThingspeak(float temp)
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

        //client.print(String("GET") +url);
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

