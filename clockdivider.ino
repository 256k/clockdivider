#include <EEPROM.h>

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
const int numberOfOutputs = 		4;
const int maximumDivisionAmount = 	32;
//=======================

// define hardware pins:
#define clk_in  	9

#define out_1  		5
#define out_2  		6
#define out_3  		7
#define enc_sw  	2
#define enc_A 		3
#define enc_B 		4

//=======================

// define states and variables:
int prevClockState;
int enc_sw_state;
int prevSwitchState;
int enc_a_state;
int enc_a_prevState;
int enc_stepCount =		0;
int counter = 			0;
int outputSelector = 	1;
int divisions[6]; 
int outputs[] = {5,6,7,8};

//=======================




void setup() {
  //  Serial.begin(9600);

  initEEPROM();

	prevClockState = digitalRead(clk_in);
	prevSwitchState = digitalRead(enc_sw);
	pinMode(clk_in, INPUT);
	pinMode(out_1, OUTPUT);
	pinMode(out_2, OUTPUT);
	pinMode(out_3, OUTPUT);
	pinMode(enc_sw, INPUT_PULLUP);
	pinMode(enc_A, INPUT_PULLUP);
	pinMode(enc_B, INPUT_PULLUP);
}

void loop() {
		readClock();
		trigOutput(out_1, divisions[0]);
		trigOutput(out_2, divisions[1]);
		encSwitchRead();
		encRotationRead();
		
	
}

void count() {
	counter = counter + 1;
	if (counter > maximumDivisionAmount) { counter = 1;}
}

void readClock() {
	const int inputState = digitalRead(clk_in);
	if ( inputState == 1 && inputState != prevClockState) {
		prevClockState = inputState;
		count();
	} else if (inputState == 0 && inputState != prevClockState) {
		prevClockState = inputState;
	}
}


// universal trigger function:
void trigOutput(int outTrig, int divisionNum){
	if (counter % divisionNum == 0 && prevClockState == 1 && digitalRead(outTrig) == LOW) {
		digitalWrite(outTrig, HIGH);
	} else if (prevClockState == 0) {
		digitalWrite(outTrig, LOW);
	}
}

void encSwitchRead() {
	enc_sw_state = digitalRead(enc_sw);
	if (enc_sw_state == 0 && enc_sw_state != prevSwitchState ) {
//		Serial.println("enc switch clicked");
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
//				Serial.println("Encoder increment");
				setDivision(outputSelector, 1);
			}
		} else {
			enc_stepCount -= 1;
			if (enc_stepCount % 2 == 0) {
//				Serial.println("Encoder decrement");
				setDivision(outputSelector, -1);
			}
		}
		enc_a_prevState = enc_a_state;
	}
}

void setDivision(int index, int value){
	// TODO: read the user input to set the division rate
	divisions[index - 1] += value;
  EEPROM.write(index - 1, divisions[index - 1]);
}

void initEEPROM() {
  for (int i = 0; i <  numberOfOutputs; i++) {
    if (EEPROM.read(i) <= maximumDivisionAmount) {
      divisions[i] = EEPROM.read(i);
    } else {
      EEPROM.write(i,i+1);
      divisions[i] = EEPROM.read(i);
    }
  }
}
