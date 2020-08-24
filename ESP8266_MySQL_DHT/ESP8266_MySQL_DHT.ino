/*
 *  ==== ESP8266 DHT Sensor Data to MySQL Database ====
 * 
 * MySQL Request: INSERT INTO room_temp_sensor(humidity,temperature,date) VALUES(38,26,'22.08.2020:01:49');
 * 
 * Example of GET method protocol HTTP:server.php?value=foo
 * 
 * 
 *  TODO:
 *  1) Get sensor data with ESP8266 to Serial for testing (DONE!)
 *  2) Get local date and time                            (DONE!)
 *  3) Get connection to MySQL database via WI-FI         (DONE!!)
 *    - Create .php script on the server                  
 *    - Understand how GET method works for HTTP protocol
 *    ...
 *  4) Save data from the sensor to database              
 *  5)
 */

#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include "DHT.h"
#define DHTPIN D1     
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);

const unsigned long SECOND = 1000;
const unsigned long HOUR = 3600*SECOND;

const char* ssid       = "iPhone (Alex)";       // Name of the WI-FI
const char* password   = "alexpassword1";       // Password for WI-FI

String date = "";                               // Current date
float h;
float t;

//String request = "INSERT INTO room_temp_sensor(humidity,temperature,date) VALUES(";

String serverName = "https://sensors.noyanov.ru/getSensor.php";

const long utcOffsetInSeconds = 3600;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

// Get local date and time:
String getDateTime(){
  timeClient.update();
  String currentDate = "";
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  currentDate += daysOfTheWeek[timeClient.getDay()];
  Serial.print(", ");
  currentDate += 
  Serial.print(timeClient.getHours());
  
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
  //Serial.println(timeClient.getFormattedTime());
}

void getHumTemp(){
   h = dht.readHumidity();
   t = dht.readTemperature();
  
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");
  } else {
    Serial.print("Temp = "); 
    Serial.print(t);
    Serial.print(" *C\t");
    Serial.print("Hum = "); 
    Serial.print(h);
    Serial.println("%");
  }
}

// To insert data into database:
void insertToDatabase(String myServerName){
  // https://sensors.noyanov.ru/getSensor.php?temp=28&hum=31&date=27/08/2020
  String tempStr = String(t);
  String humStr = String(h);

if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverName);

      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      // Data to send with HTTP POST
      String httpRequestData = "?temp=";
         // httpRequestData += tempStr;
          httpRequestData.concat(tempStr);
          //httpRequestData += "&hum=";
          httpRequestData.concat("&hum=");
          //httpRequestData += humStr;
          httpRequestData.concat(humStr);
          //httpRequestData += "&date=";
          httpRequestData.concat("&date=");
          //httpRequestData += date;
          httpRequestData.concat(date);
          
             Serial.println(httpRequestData);
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);
      
      // If you need an HTTP request with a content type: application/json, use the following:
      //http.addHeader("Content-Type", "application/json");
      //int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");

      // If you need an HTTP request with a content type: text/plain
      //http.addHeader("Content-Type", "text/plain");
      //int httpResponseCode = http.POST("Hello, World!");
     
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
        
      // Free resources
      http.end();
      
        Serial.println("=== DATA INSERTED SUCCESSFULLY! ==="); 
        
    }

}

void setup() {
  Serial.begin(9600); 
  Serial.println("");
  Serial.println("==== RESTARTED ===="); 
  dht.begin();

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  timeClient.begin();
}

void loop() {  
  date = getDateTime();
  getHumTemp();

  insertToDatabase(serverName);
  
  delay(HOUR);
}
