// there will be an input gate
// there will be 6 output gates
// each output gate will be a division of the input gate clock
// each output gate clock division rate will be selectable using a encoder/switch
// you turn encoder for selecting the division that will round robin between a set of divisions and hold down the encoder and turn to select the output
// each output will also have a coresponding LED
// those LEDs are independently controlled from the aruino through code and unrelated physically to the output gates
// they will normally follow the output gate on/off
// when encoder switch held down, only the selected output will have the corresponding LED ON


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
		// trigOutput_1();
		// trigOutput_2();
		trigOutput(out_1, divider_1);
		trigOutput(out_2, divider_2);
		encSwitchRead();
		
	
}

void count() {
	counter = counter + 1;
	// 64 is the maximum division allowed.
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
	// for now this will be the clock divided by 4
	// Serial.print("counter % 4: ");
	// Serial.println(counter%4);
	if (counter % divisionNum == 0 && prevClockState == 1 && digitalRead(outTrig) == LOW) {
		digitalWrite(outTrig, HIGH);
		Serial.print("trig output: ");
		Serial.println(outTrig);
		// Serial.println("OUTPUT 2 TRIGGRRED");
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

// void trigOutput_1(){
// 	// for now this will be the clock divided by 4
// 	// Serial.print("counter % 4: ");
// 	// Serial.println(counter%4);
// 	if (counter % 4 == 0 && prevClockState == 1 && digitalRead(out_1) == LOW) {
// 		digitalWrite(out_1, HIGH);
// 		// Serial.println("OUTPUT 1 TRIGGRRED");
// 	} else if (prevClockState == 0) {
// 		digitalWrite(out_1, LOW);
// 	}
// }
// void trigOutput_2(){
// 	// for now this will be the clock divided by 4
// 	// Serial.print("counter % 4: ");
// 	// Serial.println(counter%4);
// 	if (counter % 2 == 0 && prevClockState == 1 && digitalRead(out_2) == LOW) {
// 		digitalWrite(out_2, HIGH);
// 		// Serial.println("OUTPUT 2 TRIGGRRED");
// 	} else if (prevClockState == 0) {
// 		digitalWrite(out_2, LOW);
// 	}
// }


void setDivision(){
	// TODO: read the user input to set the division rate
}