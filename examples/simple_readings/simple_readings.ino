#include "Wire.h"
#include "qmc5883l.h"


QMC5883L magnetometer;
float magnetic_field[3];

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
    magnetometer.get_magneticField_uT(magnetic_field);
  }
  
  Serial.print(magnetic_field[0]); Serial.print(" ");
  Serial.print(magnetic_field[1]); Serial.print(" ");
  Serial.println(magnetic_field[2]);

  delay(100);
}
