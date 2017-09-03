#include <Arduino.h>

#include "../../../src/BMP280.h"

using namespace g3rb3n;

BMP280 bmp;

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("BMP280 online");

  Serial.println("Connecting to BMP280");
  while(!bmp.connected())
  {
    Serial.print("BMP responded with : ");
    Serial.print(bmp.identification());
    Serial.println();
  }
  bmp.initialize();
  bmp.efficiencyMode(ULTRA_HIGH_RESOLUTION);
  Serial.println("BMP280 initialized");
  delay(1000);
}

void loop() {
  double p;
  bmp.pressure(p);
  Serial.println(p);
  //Serial.printf("%5.4f,%5.4f,%5.4f %5.4f\n", a.x, a.y, a.z, squareLength);
  //Serial.printf("%f,%f,%f %f\n", a.x, a.y, a.z, squareLength);
}
