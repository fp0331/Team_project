#include <SPI.h>
#include <WiFiNINA.h>
#include <Arduino_LSM6DSOX.h>
#include "arduino_secrets.h" 

float Ax, Ay, Az;
float Gx, Gy, Gz;
float initial_time, final_time;

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                 // your network key index number (needed only for WEP)                

int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  
  Serial.println("Access Point Web Server");

  pinMode(LED_BUILTIN, OUTPUT);
    
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }
  
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    
    delay(10000);
  }
  server.begin();

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);
  if (status != WiFi.status()) {
    status = WiFi.status();

    if (status == WL_AP_CONNECTED) {
      Serial.println("Device connected to AP");
    } else {
      Serial.println("Device disconnected from AP");
    }
  }
  
  WiFiClient client = server.available();

  if (client) {                            
    Serial.println("new client");
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {
      delayMicroseconds(10);                // This is required for the Arduino Nano RP2040 Connect
      if (client.available()) {            
        char c = client.read();             
        Serial.write(c);                    
        if (c == '\n') {                    
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
/*
            client.print("Click <a href=\"/RH\">here</a> turn the Red LED on<br>");

            client.print("Click <a href=\"/GH\">here</a> turn the Green LED on<br>");
  
            client.print("Click <a href=\"/BH\">here</a> turn the Blue LED on<br>");
*/
            client.print("Click <a href=\"/prova\">here</a> send data<br>"); //parte aggiunta
            client.println();
            break;
          }
          else {
            currentLine = "";
          }
        }
        else if (c != '\r') {    
          currentLine += c;
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /prova")) {
          
        AccGyroAcquisition();
        
        }   
      }
    }
    client.stop();
    Serial.println("client disconnected");
  }
}
//funzione che invia il dato 

//funzione in cui attiva IMU e legge per 30 secondi (possibile fare un return del buffere da inviare)
void AccGyroAcquisition() {

    digitalWrite(LED_BUILTIN, HIGH);
    initial_time = millis();
    final_time = millis();
    
    while(final_time-initial_time <= 30000){
    
        if (IMU.accelerationAvailable()) {
            IMU.readAcceleration(Ax, Ay, Az);
        }
        
        if (IMU.gyroscopeAvailable()) {
            IMU.readGyroscope(Gx, Gy, Gz);
        }
        final_time = millis();
        //store data from Gyro end Acc in buffers
        //funzione che invia il dato
    }
}
