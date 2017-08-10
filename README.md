A C++ driver for the BMP280.

# Example #
```

BMP280 bmp;

void setup()
{
  Serial.begin(9600);
  bmp.initialize();
  bmp.efficiencyMode(ULTRA_HIGH_RESOLUTION);
}

void loop()
{
  double pressure;
  double temperature;
  bmp.temperature(temperature);
  bmp.pressure(pressure);
  Serial.println("temperature = " + temperature);
  Serial.println("pressure    = " + pressure);
}
```
