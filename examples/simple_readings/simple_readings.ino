#include "Wire.h"
#include "qmc5883l.h"


QMC5883L magnetometer;
float mag_x, mag_y, mag_z;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  // Default settings
  // Mode: continious
  // Datarate: 200Hz
  // Range: +-8GA
  // Oversampling: 512
  if (magnetometer.begin()){
    Serial.println(F("QMC5883L sensor is found on the bus."));
  }
  else{
    Serial.println(F("Could not find a valid QMC5883L sensor, check wiring."));
    while(1) delay(10);
  }
}

void loop() {
  if (magnetometer.isReady() and (!magnetometer.isOverflowed())){
    magnetometer.get_magneticField_uT(mag_x, mag_y, mag_z);
  }
  
  Serial.print(mag_x); Serial.print(" ");
  Serial.print(mag_y); Serial.print(" ");
  Serial.println(mag_z);

  delay(100);
}
