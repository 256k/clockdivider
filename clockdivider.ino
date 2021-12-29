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
// add a reset input (will reset the counter value to 0)
// define a division range of vlaues that would work well as a clock divider ( i want to have odd divisions too)
// finish encoder division selector function

// define hardware pins:
const int clk_in = 9;
const int out_1 = 5;
const int out_2 = 6;
const int out_3 = 7;
const int enc_sw = 2;

int prevClockState = 0;
int prevSwitchState = 0;
int counter = 0;
int outputSelector = 1;
int divider_1 = 2;
int divider_2 = 4;
int divider_3 = 1;
int divider_4 = 1;


void setup() {
	Serial.begin(9600);
	prevClockState = digitalRead(clk_in);
	prevSwitchState = digitalRead(enc_sw);
	pinMode(clk_in, INPUT);
	pinMode(out_1, OUTPUT);
	pinMode(out_2, OUTPUT);
	pinMode(out_3, OUTPUT);
	pinMode(enc_sw, INPUT_PULLUP);
}

void loop() {
		readClock();
		trigOutput(out_1, divider_1);
		trigOutput(out_2, divider_2);
		encSwitchRead();
		
	
}

void count() {
	counter = counter + 1;
	// 64 is the maximum division allowed. this will change later.
	if (counter > 64) { counter = 1;}
	Serial.println(counter);
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
		Serial.print("trig output: ");
		Serial.println(outTrig);
	} else if (prevClockState == 0) {
		digitalWrite(outTrig, LOW);
	}
}

void encSwitchRead() {
	int enc_sw_read = digitalRead(enc_sw);
	if (enc_sw_read == 0 && enc_sw_read != prevSwitchState ) {
		Serial.println("enc switch clicked");
		if (outputSelector < 4) {
			outputSelector = outputSelector + 1;
		} else {
			outputSelector = 1;
		}
		prevSwitchState = enc_sw_read;
		Serial.print("output selector: ");
	Serial.println(outputSelector);
	} else if (enc_sw_read == 1 && enc_sw_read != prevSwitchState ) {
		prevSwitchState = enc_sw_read;
	}
	
}

void setDivision(){
	// TODO: read the user input to set the division rate
}