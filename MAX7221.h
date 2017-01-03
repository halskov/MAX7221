/*
 *    MAX7221.h - A library for controling the MAX7221 over SPI
 *    Copyright (c) 2016 Jakob Halskov
 * 
 *    Permission is hereby granted, free of charge, to any person
 *    obtaining a copy of this software and associated documentation
 *    files (the "Software"), to deal in the Software without
 *    restriction, including without limitation the rights to use,
 *    copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following
 *    conditions:
 * 
 *    This permission notice shall be included in all copies or 
 *    substantial portions of the Software.
 * 
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *    OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef MAX7221_h
#define MAX7221_h

#include <avr/pgmspace.h>
#include <Arduino.h>
#include <SPI.h>

 /*
 * Lookup table for digits 
 */
 const static byte digitTable [] PROGMEM  = {
 	B01111110,B00110000,B01101101,B01111001,B00110011,B01011011,B01011111,B01110000, // Hexidecimal 0 to 7
 	B01111111,B01111011,B01110111,B00011111,B00001101,B00111101,B01001111,B01000111, // Hexidecimal 8 to F
 };

  /*
 * Lookup table for chars 
 */
 const static byte charTable [] PROGMEM  = {
 	B01111110,B00110000,B01101101,B01111001,B00110011,B01011011,B01011111,B01110000, // 48:  Integer 0 to 7
 	B01111111,B01111011,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000, // 56:  Integer 8 to 9
 	B00000000,B01110111,B01111111,B01001110,B01111110,B01001111,B01000111,B00000000, // 64:    A B C D E F 
 	B00110111,B00110000,B00000000,B00000000,B00001110,B00000000,B01110110,B01111110, // 72:  H I     L   N O
 	B01100111,B01111110,B01110111,B01011011,B00000000,B00111110,B00000000,B00000000, // 80:  P Q S   U     
 	B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00001000, // 88:                _
 	B00000000,B00011101,B00011111,B00001101,B00111101,B00000000,B00000000,B00000000, // 96:    a b c d    
 	B00000000,B00010000,B00000000,B00000000,B00000000,B00000000,B00010101,B00011101, // 102:   i    
 	B01100111,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000, // 110:
 	B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000	 // 118:
 };

 class MAX7221
 {
 public:
 	MAX7221(int chipSelect, int numDigits);

 	void setIntensity(int intensity);
 	void setScanLimit(int limit);
 	void displayTest(bool b);
 	void shutdown(bool b);

 	void setDigit(int digit, int value, bool dp);
 	void setDigit(int digit, int value);
 	void setChar(int digit, byte value, bool dp);
 	void clearDigit(int digit);

 	void displayNumber(int number, bool leadinZeros);
 	void displayTime(int high, int low);
 	void displayMinutes(int minutes);

 private:

 	int chipSelect_;
 	int numDigits_;

 	void sendCommand(int command, int value);
 };

#endif