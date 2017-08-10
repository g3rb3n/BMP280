#include <Arduino.h>
#include <unity.h>

#include "../src/BMP280.h"

#ifdef UNIT_TEST

using namespace g3rb3n;

BMP280 bmp;

void test_bmp_connected(void) {
  TEST_ASSERT_EQUAL(bmp.connected(), true);
}

void powerModeIs3(void) {
  TEST_ASSERT_EQUAL(3, bmp.powerMode());
}

void powerModeIs0(void) {
  TEST_ASSERT_EQUAL(0, bmp.powerMode());
}

void pressureSamplingIsOff(void) {
  TEST_ASSERT_EQUAL(OFF, bmp.pressureSamplingMode());
}

void pressureSamplingIsX16(void) {
  TEST_ASSERT_EQUAL(X16, bmp.pressureSamplingMode());
}

void temperatureSamplingIsOff(void) {
  TEST_ASSERT_EQUAL(OFF, bmp.temperatureSamplingMode());
}
void temperatureSamplingIsX2(void) {
  TEST_ASSERT_EQUAL(X2, bmp.temperatureSamplingMode());
}

void getIRRFilterCoeffiecientIs0(void) {
  TEST_ASSERT_EQUAL(0, bmp.filterCoefficientMode());
}

void getIRRFilterCoeffiecientIs4(void) {
  TEST_ASSERT_EQUAL(4, bmp.filterCoefficientMode());
}

void getStandByTimeIs0(void) {
  TEST_ASSERT_EQUAL(0, bmp.standByTimeMode());
}

void temperatureIsRoom(void) {
  double t = 0;
  bmp.temperature(t);
  TEST_ASSERT(t > 15 && t < 35);
}

void pressureIsRoom(void) {
  double t = 0;
  bmp.pressure(t);
  TEST_ASSERT(t > 90000 && t < 110000);
}

void setup() {
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);

  UNITY_BEGIN();

  RUN_TEST(test_bmp_connected);

  bmp.initialize();
  bmp.efficiencyMode(ULTRA_HIGH_RESOLUTION);
  
  RUN_TEST(powerModeIs3);
  RUN_TEST(pressureSamplingIsX16);
  RUN_TEST(temperatureSamplingIsX2);
  RUN_TEST(getIRRFilterCoeffiecientIs4);
  RUN_TEST(temperatureIsRoom);
  RUN_TEST(pressureIsRoom);
  
  bmp.reset();
  RUN_TEST(powerModeIs0);
  RUN_TEST(pressureSamplingIsOff);
  RUN_TEST(temperatureSamplingIsOff);
  RUN_TEST(getIRRFilterCoeffiecientIs0);
  
  UNITY_END();
}

void loop() {
}

#endif
