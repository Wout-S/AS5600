#include "Arduino.h"
#include "AS5600.h"

AS5600::AS5600() {
  Wire.begin();
  // Wire.setClock(100000);
}

uint16_t AS5600::getPosition() {
  return _getRegisters2(_RAWANGLEAddressMSB, _RAWANGLEAddressLSB);
}

uint16_t AS5600::getAngle() {
  return _getRegisters2(_ANGLEAddressMSB, _ANGLEAddressLSB);
}

uint16_t AS5600::getRawAngle() {
  return _getRegisters2(_RAWANGLEAddressMSB, _RAWANGLEAddressLSB);
}

float AS5600::getScaledAngle() {
  float pos = (float)_getRegisters2(_RAWANGLEAddressMSB,_RAWANGLEAddressLSB)*360/4095;
  return pos;
}

uint8_t AS5600::getStatus() {
  return _getRegister(_STATUSAddress) & 0b00111000;
}

bool AS5600::isMagnetTooStrong() {
  uint8_t _b=0;
  _b = getStatus();
  if (_b & (1<<5)) { return true; }
  return false;
}

bool AS5600::isMagnetTooWeak() {
  uint8_t _b=0;
  _b = getStatus();
  if (_b & (1<<4)) { return true; }
  return false;
}

bool AS5600::isMagnetDetected() {
  uint8_t _b=0;
  _b = getStatus();
  if (_b & (1<<3)) { return true; }
  return false;
}

uint8_t AS5600::getGain() {
  return _getRegister(_AGCAddress);
}

uint8_t AS5600::getMagnet() {
  return _getRegister(_AGCAddress);
}

uint16_t AS5600::getMagnitude() {
  return _getRegisters2(_MAGNITUDEAddressMSB, _MAGNITUDEAddressLSB);
}

uint8_t AS5600::_getRegister(byte register1) {
  uint8_t _b=0;
  byte requestResult;

  Wire.beginTransmission(_AS5600Address);
  Wire.write(register1);
  requestResult =Wire.endTransmission(false);
  if (requestResult){
		Serial.print("I2C error: ");
		Serial.println(requestResult);
	}
  Wire.requestFrom(_AS5600Address, 1, true);

  while (Wire.available() == 0) { }
  _b = Wire.read();

  return _b;
}

uint16_t AS5600::_getRegisters2(byte registerMSB, byte registerLSB) {
  uint16_t _hi=0, _lo=0;
  byte requestResult;
  Wire.beginTransmission(_AS5600Address);
  Wire.write(registerMSB);
  requestResult = Wire.endTransmission(false);
   
  // if (requestResult){
	// 	Serial.print("I2C error: ");
	// 	Serial.println(requestResult);
	// }
  Wire.requestFrom(_AS5600Address,2,true);
  while(Wire.available() <= 1);
    _hi = Wire.read(); // Reads the data from the register
    _lo = Wire.read();   
  return (_hi<<8) | (_lo);
}
