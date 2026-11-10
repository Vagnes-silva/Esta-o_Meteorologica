#include <Wire.h>
#include "Adafruit_VEML7700.h"
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>

Adafruit_VEML7700 veml = Adafruit_VEML7700();
Adafruit_AHTX0 aht;
Adafruit_BMP280 bmp;

void setup() {
  Serial.begin(115200);
  // Não é necessário esperar o Serial para o site funcionar,
  // mas mantemos uma pequena pausa para estabilização.
  delay(1000);

  if (!veml.begin()) {
    Serial.println("{\"erro\":\"VEML7700\"}");
    while (1);
  }
  if (!aht.begin()) {
    Serial.println("{\"erro\":\"AHT20\"}");
    while (1);
  }
  if (!bmp.begin(0x76)) {
    if (!bmp.begin(0x77)) {
      Serial.println("{\"erro\":\"BMP280\"}");
      while (1);
    }
  }
  // Configuração do BMP280 para leituras estáveis
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                  Adafruit_BMP280::SAMPLING_X2,
                  Adafruit_BMP280::SAMPLING_X16,
                  Adafruit_BMP280::FILTER_X16,
                  Adafruit_BMP280::STANDBY_MS_500);
}

void loop() {
  float lux = veml.readLux();
  
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  
  float pressao = bmp.readPressure() / 100.0; // hPa
  float tempBmp = bmp.readTemperature();

  // Monta a string JSON manualmente para evitar bibliotecas extras
  Serial.print("{\"lux\":");
  Serial.print(lux, 2);
  Serial.print(",\"temp_aht\":");
  Serial.print(temp.temperature, 2);
  Serial.print(",\"umid\":");
  Serial.print(humidity.relative_humidity, 2);
  Serial.print(",\"pressao\":");
  Serial.print(pressao, 2);
  Serial.print(",\"temp_bmp\":");
  Serial.print(tempBmp, 2);
  Serial.println("}");

  delay(2000); // Envia a cada 2 segundos
}