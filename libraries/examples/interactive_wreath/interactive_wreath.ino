
#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <Protocentral_VL53L0x.h>

//LDR Pin
#define MIOSTURE_PIN A0    

const char* ssid     = "SSID";  // 
const char* password = "PASSWORD";

const char* host = "184.106.153.149"; //thingspeak server
String GET = "GET /update?key=";//QCB1BEGJSVX0F65W&field1=232\r\n"; // GET /update?key=APIkey&field1=

const char* tokenid = "P9CGBL3X4RY0TRXA";

long lastConnectionTime = 0; 
boolean lastConnected = false;
int failedCounter = 0;

int sensorValue = 0; // variable to store the value coming from the sensor

#define PIN 5

volatile int range=0,previousrange=0,range1,range2=0;
volatile bool trigger = false;
volatile uint8_t  count = 0,count2=0;
unsigned long previousMillis = 0;

WiFiClient client;
VL53L0X sensor;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(120/*no 0f LEDS*/, PIN, NEO_GRB + NEO_KHZ800);

void timer0_ISR (void)
{
  range =sensor.readRangeContinuousMillimeters();

  if(range >= 100 && range<2000) 
  {
    if(range <200)
    range1 = map(range,100,200,10,20);
    else if(range >200 && range<500)
    range1 = map(range,200,500,21,50);
    else if(range >500 && range <1000)
    range1 = map(range,500,1000,51,100);
    else if(range >1000 && range <2000)
    range1 = map(range,1000,2000,101,1000);
  
   
    count++;
    if(count>=2)
    {
      trigger = true;
      count=0;
      
    }
    count2=0;
  }

  else 
  {
      count2++;
    if(count2>=2)
    {
      trigger = false;
      count2=0;
      
    }
    count=0;
   }
  
  timer0_write(ESP.getCycleCount() + 4000000); //80Mhz -> 80*10^6 = 1 second
}
  
void setup() 
{
  
  Serial.begin(9600); //sets serial port for communication
  Serial.println();
  Serial.println();

  Wire.begin();

  //LED Strip 
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  sensor.init();
  sensor.startContinuous();
  
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
  
  timer0_isr_init();
  timer0_attachInterrupt(timer0_ISR);
  timer0_write(ESP.getCycleCount() + 4000000); //80Mhz -> 80*10^6 = 1 second
  interrupts();
  
}
void loop() 
{
    rainbowCycle(3);
     
    if(trigger == true) 
     {
         if(range > 50  && range <2000)
         {
             theaterChase(strip.Color(200, 0, 0), range); // Red
             postThingspeak(range); 
         }
     }   
}

void  postThingspeak(unsigned int temp)
{
    lastConnectionTime = millis()+7000;
    
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= 7000) 
    {
        previousMillis = currentMillis;
    
        Serial.print("connecting to ");
        Serial.println(host);
        
        // Use WiFiClient class to create TCP connections
        
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
        
        while (client.available() == 0) 
        {
          if (timeout - millis() < 0) 
          {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
          }
        }
    }

   
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
     if(trigger == true){    break;}
    for(i=0; i< strip.numPixels(); i++) {
      if(trigger == true){   break;}
      else
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {
  //do 10 cycles of chasing
  if(trigger == false){    break;}
    for (int q=0; q < 3; q++) {
      
  if(trigger == false){    break;}
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
                if(trigger == false){   break;}   
      else
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(range1);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
          if(trigger == false){   break;}
          else
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
