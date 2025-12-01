#include <Arduino.h>
#include "target.h"
#include <cstdint>
#include <helpers/ESP32Board.h>

HeltecV4Board board;

#if defined(P_LORA_SCLK)
  static SPIClass spi;
  RADIO_CLASS radio = new Module(P_LORA_NSS, P_LORA_DIO_1, P_LORA_RESET, P_LORA_BUSY, spi);
#else
  RADIO_CLASS radio = new Module(P_LORA_NSS, P_LORA_DIO_1, P_LORA_RESET, P_LORA_BUSY);
#endif

WRAPPER_CLASS radio_driver(radio, board);

ESP32RTCClock fallback_clock;
AutoDiscoverRTCClock rtc_clock(fallback_clock);

#if ENV_INCLUDE_GPS
  #include <helpers/sensors/MicroNMEALocationProvider.h>
  MicroNMEALocationProvider nmea = MicroNMEALocationProvider(Serial1);
  MPSensorManager sensors;
#else
  MPSensorManager sensors;
#endif

#ifdef DISPLAY_CLASS
  DISPLAY_CLASS display;
  MomentaryButton user_btn(PIN_USER_BTN, 1000, true);
#endif

bool radio_init() {
  fallback_clock.begin();
  rtc_clock.begin(Wire);
  
#if defined(P_LORA_SCLK)
  return radio.std_init(&spi);
#else
  return radio.std_init();
#endif
}

uint32_t radio_get_rng_seed() {
  return radio.random(0x7FFFFFFF);
}

void radio_set_params(float freq, float bw, uint8_t sf, uint8_t cr) {
  radio.setFrequency(freq);
  radio.setSpreadingFactor(sf);
  radio.setBandwidth(bw);
  radio.setCodingRate(cr);
}

void radio_set_tx_power(uint8_t dbm) {
  radio.setOutputPower(dbm);
}

mesh::LocalIdentity radio_new_identity() {
  RadioNoiseListener rng(radio);
  return mesh::LocalIdentity(&rng);  // create new random identity
}


bool MPSensorManager::querySensors(uint8_t requester_permissions, CayenneLPP& telemetry) { 

  // Serial.print("Getting solar volts... raw: ");

  pinMode(PIN_VSOL_READ, INPUT);
  adcAttachPin(PIN_VSOL_READ);

  // Read the solar sensor from ADC channel 2 (GPIO Pin 3)
  analogReadResolution(10);
  uint32_t raw = 0;

  for (int i = 0; i < 8; i++) {
    raw += analogRead(PIN_VSOL_READ);
  }

  raw = raw / 8;


  // Serial.print(raw, DEC);
  // Serial.print(", V: ");

  // At this point, raw is the raw ADC counts with 8x oversampling/averaging.
  // But, the voltage on the pin is 1/10 of the voltage of the solar panels.
  float ratio_full_scale = (float)raw / 1024.0; // 0-1024 input count range
  float input_volts = ratio_full_scale * 3.3;   // 3.3v full-scale voltage
  float solar_volts = input_volts * 10.5;       // 10x voltage divider for solar (plus cal)

  // Serial.println(solar_volts, 2);

  telemetry.addVoltage(2, solar_volts);

  return true; 
}

bool MPSensorManager::begin() {
    pinMode(3, INPUT);
    adcAttachPin(3);

    return true;
}