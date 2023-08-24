/*
*
* DIO
* ARDUINO SCRIPT FOE EURORACK MODULE 
* By 256k
*
*/

#include <Wire.h>

// #include <EEPROM.h>

// there will be an input gate
// there will be 6 output gates
// each output will have 2 corresponding LEDs: 1 to indicate pulse (red) and 1 to indicate selected state (yellow).
// each output gate will be a division of the input gate clock
// each output gate clock division rate will be selectable using a encoder/switch
// to select a output, click on encoder and the selected LED will round robin betwen all 6 outputs.
// to change rate, while desired output to change is selected, turn encoder to increase or decreate rate 
// encoder rotation will round robin between a fixed range of divisions.

// GENERAL TODOS:
// save divisions and selected output between reboots.
// add i2c functionality to change division rate for each output
// define a division range of vlaues that would work well as a clock divider ( i want to have odd divisions too) (DONE)
// finish encoder division selector function (DONE)


// configuration of functionality:
const int maximumDivisionAmount = 	32;

// define output pins in outputs array:
int outputs[] = {2,4,6,8,10,12};
int numberOfOutputs = 6;
int ledOutputs[] = {3,5,7,9,11,13};
//=======================

// define hardware pins:
#define clk_in  	A0

#define enc_sw  	A1
#define enc_A 		A2
#define enc_B 		A3

//=======================

// define states and variables:

bool outputSelectorMode = false;
int prevClockState;
int enc_sw_state;
int prevSwitchState;
int enc_a_state;
int enc_a_prevState;
int enc_stepCount =		0;
int counter = 			0;
int outputSelector = 	1;
int i2c_data = 0;

// generate an array for how many outputs we have:
int divisions[] = {1,2,3,4,5,7};

// actual outputs right now:
// {4th?, 1st, 3rd, 5th, ?, 6th}
// test interval
int i = 0;

//=======================




void setup() {
  Wire.begin(0xFA);
  Serial.begin(9600);
  Wire.onReceive(handle_i2c);
// initEEPROM();

	prevClockState = digitalRead(clk_in);
	prevSwitchState = digitalRead(enc_sw);
	pinMode(clk_in, INPUT);
	pinMode(enc_sw, INPUT_PULLUP);
	pinMode(enc_A, INPUT_PULLUP);
	pinMode(enc_B, INPUT_PULLUP);
	for (int i = 0; i < numberOfOutputs; i++) {
		pinMode(outputs[i], OUTPUT);
		pinMode(ledOutputs[i], OUTPUT);
	}

	
}

void loop() {

	readClock();
//Serial.print("1");
	// this loop code is still untested:
	for (int i = 0; i < numberOfOutputs; i++) {
		trigOutput(outputs[i], ledOutputs[i], divisions[i]);
	}

	// ====================================
	
	// test outputs LEDs:

	
		// digitalWrite(ledOutputs[0], HIGH);
		// digitalWrite(ledOutputs[1], HIGH);
		// digitalWrite(ledOutputs[2], HIGH);
		// digitalWrite(ledOutputs[3], HIGH);
		// digitalWrite(ledOutputs[4], HIGH);
		// digitalWrite(ledOutputs[5], HIGH);
		
	

	// =================

	showSelectedOutput();
	encSwitchRead();
	// Serial.println(counter);
	encRotationRead();
}



void handle_i2c(int howMany) {
//  i2c_data = Wire.read();
  while(0 < Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
//    Serial.print("i2c C: ");
    Serial.print(c);         // print the character
    
    
  }
  Serial.println("");
//  Serial.print("i2c received: ");
//  Serial.println(i2c_data);
}

void showSelectedOutput() {
	if (outputSelectorMode) {
		for (int i = 0; i < numberOfOutputs; i++) {
			digitalWrite(ledOutputs[i], LOW);
	}
		digitalWrite(ledOutputs[outputSelector], HIGH);
	}
}

void count() {
	counter = counter + 1;
	if (counter > maximumDivisionAmount) { counter = 1;}
	// Serial.print("counter: ");
	// Serial.println(counter);
}

void readClock() {
	const int inputState = digitalRead(clk_in);
	if ( inputState == 1 && inputState != prevClockState) {
		// Serial.println("clickIN");
		prevClockState = inputState;
		count();
	} else if (inputState == 0 && inputState != prevClockState) {
		prevClockState = inputState;
	}
}


// universal trigger function:
void trigOutput(int outTrig, int outLed, int divisionNum){
	if (!outputSelectorMode && counter % divisionNum == 0 && prevClockState == 1 && digitalRead(outTrig) == LOW) {
		digitalWrite(outTrig, HIGH);
		digitalWrite(outLed, HIGH);
	} else if (prevClockState == 0) {
		digitalWrite(outTrig, LOW);
		digitalWrite(outLed, LOW);
	}
}

void encSwitchRead() {
	enc_sw_state = digitalRead(enc_sw);
	if (enc_sw_state == 0 && enc_sw_state != prevSwitchState ) {
		// encoder switch clicked
		outputSelectorMode = !outputSelectorMode;
		Serial.println("enc switch clicked");
		if (outputSelector < numberOfOutputs) {
			outputSelector = outputSelector + 1;
		} else {
			outputSelector = 1;
		}
		prevSwitchState = enc_sw_state;
//		Serial.println(outputSelector);
	} else if (enc_sw_state == 1 && enc_sw_state != prevSwitchState ) {
		prevSwitchState = enc_sw_state;
	}
	
}

void encRotationRead() {
	enc_a_state = digitalRead(enc_A);
	if (enc_a_state != enc_a_prevState) {
		if (digitalRead(enc_B) != enc_a_state) {
			enc_stepCount += 1;
			if (enc_stepCount % 2 == 0) {
				Serial.println("Encoder increment");
				
				if (outputSelectorMode) {
					outputSelector += 1;
					Serial.print("outputSelector increase to: ");
					Serial.println(outputSelector);
					
				} else {
					// setDivision(outputSelector, 1);
				}
			}
		} else {
			enc_stepCount -= 1;
			if (enc_stepCount % 2 == 0) {
//				Serial.println("Encoder decrement");
				
				if (outputSelectorMode) {
					outputSelector -= 1;
				} else {
					// setDivision(outputSelector, -1);
				}
			}
		}
		enc_a_prevState = enc_a_state;
	}
}

void setDivision(int index, int value){
	// TODO: read the user input to set the division rate
	divisions[index - 1] += value;
//   EEPROM.write(index - 1, divisions[index - 1]);
}

// void initEEPROM() {
//   for (int i = 0; i <  numberOfOutputs; i++) {
//     if (EEPROM.read(i) <= maximumDivisionAmount) {
//       divisions[i] = EEPROM.read(i);
//     } else {
//       EEPROM.write(i,i+1);
//       divisions[i] = EEPROM.read(i);
//     }
//   }
// }
