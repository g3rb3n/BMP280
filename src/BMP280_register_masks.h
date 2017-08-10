// BMP280 registers

#include <BitMasks.h>

//0xF3 “status”
#define BMP280_MASK_MEASURING  BIT_3
#define BMP280_MASK_UPDATING   BIT_0

// 0xF4 “ctrl_meas”
#define BMP280_MASK_OVERSAMPLING_TEMPERATURE   BIT_7_5
#define BMP280_MASK_OVERSAMPLING_PRESSURE      BIT_4_2
#define BMP280_MASK_POWER_MODE                 BIT_1_0
#define BMP280_SHIFT_OVERSAMPLING_TEMPERATURE    5
#define BMP280_SHIFT_OVERSAMPLING_PRESSURE       2
#define BMP280_SHIFT_POWER_MODE                  0

//Register 0xF5 “config”
#define BMP280_MASK_T_SB       BIT_7_5
#define BMP280_MASK_FILTER     BIT_4_2
#define BMP280_MASK_SPI3W_EN   BIT_0
#define BMP280_SHIFT_T_SB        5
#define BMP280_SHIFT_FILTER      2
  
// 0xF7…0xF9 “press” (_msb, _lsb, _xlsb)
#define BMP280_MASK_PRESS_XLSB BIT_7_4

// 0xFA…0xFC “temp” (_msb, _lsb, _xlsb)
#define BMP280_MASK_TEMP_XLSB  BIT_7_4

