#ifndef _BMP280_H
#define _BMP280_H

#include <I2C.h>

#define BMP280_ADDRESS    0x76

namespace g3rb3n{

enum PowerMode
{
  SLEEP = 0,
  FORCED = 1,
  NORMAL = 3
};

enum SamplingMode
{
  OFF = 0,
  X1,
  X2,
  X4,
  X8,
  X16
};

enum StandByTimeMode
{
  _0_5_MS = 0,
  _62_5_MS,
  _125_MS,
  _250_MS,
  _500_MS,
  _1000_MS,
  _2000_MS,
  _4000_MS  
};

enum FilterCoefficientMode
{
  _0 = 0,
  _2,
  _4,
  _8,
  _16
};

struct Calibration
{
  uint16_t T1;
  int16_t T2;
  int16_t T3;
  uint16_t P1;
  int16_t P2;
  int16_t P3;
  int16_t P4;
  int16_t P5;
  int16_t P6;
  int16_t P7;
  int16_t P8;
  int16_t P9;
};

enum Error
{
  _OK = 0,
  DIVISION_BY_ZERO,
  UNDEFINED
};

enum EfficiencyMode
{
  ULTRA_LOW_POWER,
  LOW_POWER,
  STANDARD_RESOLUTION,
  HIGH_RESOLUTION,
  ULTRA_HIGH_RESOLUTION
};

class BMP280
{
  private:
    uint8_t _address;
    I2C i2c;
    Calibration calibration;

  public:
    BMP280();
    BMP280(uint8_t address);
    BMP280(uint8_t address, uint8_t sda, uint8_t cls);
    ~BMP280();

    uint8_t address() const;
    uint8_t identification() const;
    int connected() const;

    bool available() const;

    Error temperature(uint32_t &destination) const;
    Error pressure(uint32_t &destination) const;
    Error temperature(double &destination) const;
    Error pressure(double &destination) const;

    
    PowerMode powerMode() const;
    void powerMode(PowerMode mode);
    
    SamplingMode pressureSamplingMode() const;
    void pressureSamplingMode(SamplingMode mode);
    
    SamplingMode temperatureSamplingMode() const;
    void temperatureSamplingMode(SamplingMode mode);
    
    FilterCoefficientMode filterCoefficientMode() const;
    void filterCoefficientMode(FilterCoefficientMode coefficient);
    
    StandByTimeMode standByTimeMode() const;
    void standByTimeMode(StandByTimeMode mode);
    
    void initialize();
    void reset();
    void efficiencyMode(EfficiencyMode mode);
    
    void printRegisters(uint8_t start, uint8_t end) const;
    void dumpRegisters() const;

  private:
    double var1var2() const;
    int32_t temperatureFine() const;

    void readCalibration();
  };
}
#endif
