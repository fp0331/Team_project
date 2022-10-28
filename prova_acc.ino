#include <Arduino_LSM6DSOX.h>

float Ax, Ay, Az;
float Gx, Gy, Gz;
float initial_time, final_time;

void setup() {
  Serial.begin(9600);

  while(!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  Serial.println("Start");
  pinMode(LED_BUILTIN, OUTPUT);

}
void loop() {
  
    initial_time = millis();
    final_time = millis();
    
    while(final_time-initial_time <= 30000){
    
        if (IMU.accelerationAvailable()) {
            IMU.readAcceleration(Ax, Ay, Az);
        }
        
        if (IMU.gyroscopeAvailable()) {
            IMU.readGyroscope(Gx, Gy, Gz);
        }
        //delay(10);
        final_time = millis();
        //store data from Gyro end Acc in buffers
    }
    
    Serial.println("End");
    digitalWrite(LED_BUILTIN, HIGH);
    // send buffers of data through WIFI
    delay(1000);
    Serial.println("Start");
    digitalWrite(LED_BUILTIN, LOW);
    
}
