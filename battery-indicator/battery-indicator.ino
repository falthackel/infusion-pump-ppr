#include <Arduino.h>
#include <esp_adc_cal.h>

// Pin connected to the junction of R1 and R2 in the voltage divider
const int batteryPin = 34; // ADC1 channel 6

// Constants for voltage divider
const float R1 = 100000.0; // 100k ohms
const float R2 = 39000.0;  // 39k ohms

// Maximum voltage of your battery
const float maxVoltage = 12;

// Constants for ADC calibration
esp_adc_cal_characteristics_t adc_cal;

// Number of readings to average
const int numReadings = 10;
int adcReadings[numReadings];

float batteryPercentage;

void setup() {
  Serial.begin(115200);

  // Configure ADC
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11); // Use ADC1 channel 6

  // Characterize ADC
  esp_adc_cal_value_t adc_type = esp_adc_cal_characterize(
      ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, maxVoltage * 1000, &adc_cal);

  if (adc_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
    Serial.println("eFuse Vref");
  } else if (adc_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
    Serial.println("Two Point");
  } else {
    Serial.println("Default");
  }
}

void loop() {
  // Read battery voltage
  float batteryVoltage = readBatteryVoltage();

  // Print raw ADC value
  uint32_t adcValue = adc1_get_raw(ADC1_CHANNEL_6);
  Serial.print("Raw ADC Value: ");
  Serial.println(adcValue);

  // Print the voltage
  Serial.print("Battery Voltage: ");
  Serial.print(batteryVoltage, 2);
  Serial.print("V\t");

  batteryPercentage = batteryVoltage*100/11.1;

  Serial.print(batteryPercentage, 2);
  Serial.println("%");

  delay(500); // Adjust the delay as needed
}

float readBatteryVoltage() {
  // Read ADC value
  uint32_t adcValue = adc1_get_raw(ADC1_CHANNEL_6);

  // Shift the old readings
  for (int i = numReadings - 1; i > 0; i--) {
    adcReadings[i] = adcReadings[i - 1];
  }

  // Add the new reading
  adcReadings[0] = adcValue;

  // Calculate the average
  int adcSum = 0;
  for (int i = 0; i < numReadings; i++) {
    adcSum += adcReadings[i];
  }

  // Convert ADC value to voltage
  float voltage = esp_adc_cal_raw_to_voltage(adcSum / numReadings, &adc_cal);
  
  voltage = voltage * (R1 + R2) / R2; // Account for the voltage divider

  return voltage / 1000.0; // Convert millivolts to volts
}
