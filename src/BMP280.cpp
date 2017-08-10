
#include "BMP280.h"
#include "BMP280_registers.h"
#include "BMP280_register_masks.h"
#include "BMP280_register_values.h"

#include <HardwareSerial.h>

namespace g3rb3n
{
  
  BMP280::BMP280():
    _address(BMP280_ADDRESS),
    i2c(BMP280_ADDRESS)
  {
  }

  BMP280::BMP280(uint8_t address):
    _address(address),
    i2c(BMP280_ADDRESS)
  {
  }

  BMP280::BMP280(uint8_t sda, uint8_t scl, uint8_t address):
    _address(address),
    i2c(BMP280_ADDRESS, sda, scl)
  {
  }

  BMP280::~BMP280()
  {}

  uint8_t BMP280::address() const
  {
    return i2c.address();
  }

  uint8_t BMP280::identification() const
  {
    return i2c.readByte(BMP280_ID);
  }

  int BMP280::connected() const
  {
    return identification() == BMP280_IDENTIFIER;
  }

  bool BMP280::available() const
  {
    bool measuring = i2c.readMaskBit(BMP280_STATUS, BMP280_MASK_MEASURING);
    bool updating = i2c.readMaskBit(BMP280_STATUS, BMP280_MASK_UPDATING);
    return !measuring && !updating;
  }

  Error BMP280::pressure(uint32_t &destination) const
  {
    uint8_t rawData[3];
    i2c.readBytes(BMP280_PRESS_MSB, 3, &rawData[0]);
    destination = ((int32_t)rawData[0] << 12) | ((int32_t)rawData[1] << 4) | rawData[2] >> 4;
    return _OK;
  }

  Error BMP280::temperature(uint32_t &destination) const
  {
    uint8_t rawData[3];
    i2c.readBytes(BMP280_TEMP_MSB, 3, &rawData[0]);
    destination = ((int32_t)rawData[0] << 12) | ((int32_t)rawData[1] << 4) | rawData[2] >> 4;
    return _OK;
  }

  double BMP280::var1var2() const
  {
    uint32_t adc_T;
    temperature(adc_T);
    double var1 = (((double)adc_T)/16384.0 - ((double)calibration.T1)/1024.0) * ((double)calibration.T2);
    double var2 = ((((double)adc_T)/131072.0 - ((double)calibration.T1)/8192.0) * (((double)adc_T)/131072.0 - ((double) calibration.T1)/8192.0)) * ((double)calibration.T3);
    return var1 + var2;
  }

  int32_t BMP280::temperatureFine() const
  {
    return (int32_t)(var1var2());
  }

  Error BMP280::temperature(double& destination) const
  {
    destination = var1var2() / 5120.0;
    return _OK;
  }

  // Returns pressure in Pa as double. Output value of “96386.2” equals 96386.2 Pa = 963.862 hPa
  Error BMP280::pressure(double& destination) const
  {
    uint32_t t_fine = temperatureFine();
    uint32_t adc_P;
    pressure(adc_P);
    double var1, var2, p;
    var1 = ((double)t_fine/2.0) - 64000.0;
    var2 = var1 * var1 * ((double)calibration.P6) / 32768.0;
    var2 = var2 + var1 * ((double)calibration.P5) * 2.0;
    var2 = (var2/4.0)+(((double)calibration.P4) * 65536.0);
    var1 = (((double)calibration.P3) * var1 * var1 / 524288.0 + ((double)calibration.P2) * var1) / 524288.0;
    var1 = (1.0 + var1 / 32768.0)*((double)calibration.P1);
    if (var1 == 0.0)
    {
      return DIVISION_BY_ZERO; // avoid exception caused by division by zero
    }
    p = 1048576.0 - (double)adc_P;
    p = (p - (var2 / 4096.0)) * 6250.0 / var1;
    var1 = ((double)calibration.P9) * p * p / 2147483648.0;
    var2 = p * ((double)calibration.P8) / 32768.0;
    destination = p + (var1 + var2 + ((double)calibration.P7)) / 16.0;
    return _OK;
  }

  PowerMode BMP280::powerMode() const
  {
    i2c.readMaskShift(BMP280_CTRL_MEAS, BMP280_MASK_POWER_MODE, BMP280_SHIFT_POWER_MODE);
  }

  void BMP280::powerMode(PowerMode mode)
  {
    i2c.writeMaskShiftValue(BMP280_CTRL_MEAS, BMP280_MASK_POWER_MODE, BMP280_SHIFT_POWER_MODE, mode);
  }

  SamplingMode BMP280::pressureSamplingMode() const
  {
    i2c.readMaskShift(BMP280_CTRL_MEAS, BMP280_MASK_OVERSAMPLING_PRESSURE, BMP280_SHIFT_OVERSAMPLING_PRESSURE);  
  }

  void BMP280::pressureSamplingMode(SamplingMode mode)
  {
    i2c.writeMaskShiftValue(BMP280_CTRL_MEAS, BMP280_MASK_OVERSAMPLING_PRESSURE, BMP280_SHIFT_OVERSAMPLING_PRESSURE, mode);  
  }

  SamplingMode BMP280::temperatureSamplingMode() const
  {
    i2c.readMaskShift(BMP280_CTRL_MEAS, BMP280_MASK_OVERSAMPLING_TEMPERATURE, BMP280_SHIFT_OVERSAMPLING_TEMPERATURE);  
  }

  void BMP280::temperatureSamplingMode(SamplingMode mode)
  {
    i2c.writeMaskShiftValue(BMP280_CTRL_MEAS, BMP280_MASK_OVERSAMPLING_TEMPERATURE, BMP280_SHIFT_OVERSAMPLING_TEMPERATURE, mode);
  }

  StandByTimeMode BMP280::standByTimeMode() const
  {
    i2c.readMaskShift(BMP280_CONFIG, BMP280_MASK_T_SB, BMP280_SHIFT_T_SB);
  }

  void BMP280::standByTimeMode(StandByTimeMode mode)
  {
    i2c.writeMaskShiftValue(BMP280_CONFIG, BMP280_MASK_T_SB, BMP280_SHIFT_T_SB, mode);
  }

  FilterCoefficientMode BMP280::filterCoefficientMode() const
  {
    i2c.readMaskShift(BMP280_CONFIG, BMP280_MASK_FILTER, BMP280_SHIFT_FILTER);
  }

  void BMP280::filterCoefficientMode(FilterCoefficientMode mode)
  {
    i2c.writeMaskShiftValue(BMP280_CONFIG, BMP280_MASK_FILTER, BMP280_SHIFT_FILTER, mode);
  }

  void BMP280::readCalibration()
  {
    uint8_t data[24];
    i2c.readBytes(BMP280_CALIB00, 24, data);
    calibration.T1 = (uint16_t)data[0] | (uint16_t)(data[1]) << 8;
    calibration.T2 = (uint16_t)data[2] | (uint16_t)(data[3]) << 8;
    calibration.T3 = (uint16_t)data[4] | (uint16_t)(data[5]) << 8;
    calibration.P1 = (uint16_t)data[6] | (uint16_t)(data[7]) << 8;
    calibration.P2 = (uint16_t)data[8] | (uint16_t)(data[9]) << 8;
    calibration.P3 = (uint16_t)data[10] | (uint16_t)(data[11]) << 8;
    calibration.P4 = (uint16_t)data[12] | (uint16_t)(data[13]) << 8;
    calibration.P5 = (uint16_t)data[14] | (uint16_t)(data[15]) << 8;
    calibration.P6 = (uint16_t)data[16] | (uint16_t)(data[17]) << 8;
    calibration.P7 = (uint16_t)data[18] | (uint16_t)(data[19]) << 8;
    calibration.P8 = (uint16_t)data[20] | (uint16_t)(data[21]) << 8;
    calibration.P9 = (uint16_t)(data[22]) | (uint16_t)(data[23]) << 8;
  }

  void BMP280::initialize()
  {
  //  writeMaskSet(BMP280_CTRL_MEAS, BMP280_NORMAL_MODE);
    readCalibration();
  }

  void BMP280::efficiencyMode(EfficiencyMode mode)
  {
    PowerMode power = NORMAL;
    SamplingMode pressureMode = X1;
    SamplingMode temperatureMode = X1;
    FilterCoefficientMode filterMode = _0;
    
    if (mode == ULTRA_LOW_POWER) power = FORCED;

    switch(mode)
    {
      case ULTRA_LOW_POWER:       pressureMode = X1;  temperatureMode = X1; filterMode = _0;  break;
      case LOW_POWER:             pressureMode = X2;  temperatureMode = X1; filterMode = _0;  break;
      case STANDARD_RESOLUTION:   pressureMode = X4;  temperatureMode = X1; filterMode = _4;  break;
      case HIGH_RESOLUTION:       pressureMode = X16; temperatureMode = X2; filterMode = _4;  break;
      case ULTRA_HIGH_RESOLUTION: pressureMode = X16; temperatureMode = X2; filterMode = _16; break;
    }
    powerMode(power);
    pressureSamplingMode(pressureMode);
    temperatureSamplingMode(temperatureMode);
    filterCoefficientMode(filterMode);
  }


  void BMP280::printRegisters(uint8_t start, uint8_t end) const
  {
    Serial.printf("@0x%02X: ", start);
    for (uint8_t i = start ; i <= end ; ++i)
    {
      Serial.printf("%02X ", i2c.readByte(i));
    }
    Serial.println();
  }

  void BMP280::dumpRegisters() const
  {
    printRegisters(0x88,0xA1);
    printRegisters(0xD0,0xD0);
    printRegisters(0xE0,0xE0);
    printRegisters(0xF3,0xF5);
    printRegisters(0xF7,0xF9);
    printRegisters(0xFA,0xFC);
  }

  void BMP280::reset()
  {
    i2c.writeByte(BMP280_RESET, BMP280_RESET_CODE);
  }
  
}
