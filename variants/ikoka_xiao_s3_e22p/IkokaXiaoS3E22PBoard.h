#pragma once

#include <Arduino.h>
#include <helpers/ESP32Board.h>

class IkokaXiaoS3E22PBoard : public ESP32Board {
public:
  const char* getManufacturerName() const override {
    return "Ikoka XIAO S3 E22P";
  }
};
