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

 #include "MAX7221.h"

// Operation codes for the MAX7221 and MAX7219
#define OP_DIGIT0 		0x01
#define OP_DIGIT1 		0x02
#define OP_DIGIT2 		0x03
#define OP_DIGIT3 		0x04
#define OP_DIGIT4 		0x05
#define OP_DIGIT5 		0x06
#define OP_DIGIT6 		0x07
#define OP_DIGIT7 		0x08
#define OP_DECODEMODE  	0x09
#define OP_INTENSITY   	0x0A
#define OP_SCANLIMIT   	0x0B
#define OP_SHUTDOWN    	0x0C
#define OP_DISPLAYTEST 	0x0F

MAX7221::MAX7221(int chipSelect, int numDigits)
{
	chipSelect_ = chipSelect;
	numDigits_ = numDigits;

 	SPI.begin(); // initialize SPI

 	// Chip enable output
 	pinMode(chipSelect_, OUTPUT);

 	// Setup display
 	shutdown(false);
 	displayTest(false);
 	setIntensity(8);
 	setScanLimit(numDigits_);

	// Disables decode mode for alle digits
	sendCommand(OP_DECODEMODE, 0x00);

}

void MAX7221::setIntensity(int intensity)
{
	if(intensity >= 0 && intensity<16)
	sendCommand(OP_INTENSITY, intensity);
}

void MAX7221::setScanLimit(int limit)
{
	if(limit>=0 && limit<8)
	sendCommand(OP_SCANLIMIT, limit);
}

void MAX7221::shutdown(bool b)
{
	sendCommand(OP_SHUTDOWN, (b ? 0x00 : 0x01));
}

void MAX7221::displayTest(bool b)
{
	sendCommand(OP_DISPLAYTEST, (b ? 0x01 : 0x00));
}

void MAX7221::clearDigit(int digit) {
	sendCommand(digit+1, 0x00);
}

void MAX7221::setChar(int digit, byte value, boolean dp) {
	byte output = 0;

	if (value > 32)
	{
		output = pgm_read_byte_near(charTable + value - 48); 
	}

	// Dot point
	if(dp)
	output |= B10000000;

	sendCommand(digit+1, output);
}

void MAX7221::setDigit(int digit, int value, boolean dp) {
	byte output;

	if(digit<0 || digit>7 || value>15)
	return;

	output = pgm_read_byte_near(charTable + value); 
	
	// Dot point
	if(dp)
	output |= B10000000;

	sendCommand(digit+1, output);
}

void MAX7221::setDigit(int digit, int value) {
	setDigit(digit,value,false);
}

void MAX7221::displayNumber(int number, bool leadingZeros) {

	// Check if number is negative or too big to display
	if(number < 0 || number >= pow(10, numDigits_)){
		displayNumber(0, leadingZeros);
	}else{
		for(int i = 0; i < numDigits_; i++){
			int digitToDisplay = (int)(number / pow(10, i)) % 10;

			if(i+1 > log10(number) && digitToDisplay == 0 && !leadingZeros){
				clearDigit(i);
			}else{
				setDigit(i, digitToDisplay, false);
			}

			
		}
	}

}

void MAX7221::displayMinutes(int minutes) {
	int minutesCorrected = minutes % (60*24);
	displayTime(minutesCorrected / 60, minutesCorrected % 60);
}

void MAX7221::displayTime(int high, int low) {

	// Only works with four digit displays
	if(numDigits_ == 4){

		if (high >= 0) {
			setDigit(3, high / 10 % 10, false);
			setDigit(2, high % 10, false);
		} else {
			//Clear
			setChar(3, ' ', false);
			setChar(2, ' ', false);
		}

		setDigit(1, low / 10 % 10, false);
		setDigit(0, low % 10, false);
	}
}

void MAX7221::sendCommand(int command, int value){
 	digitalWrite(chipSelect_,LOW); // chip select is active low
 	SPI.transfer(command);
 	SPI.transfer(value);
 	digitalWrite(chipSelect_,HIGH);
 }
