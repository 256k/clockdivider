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
int prevClockState = 0;
int counter = 0;


void setup() {
	Serial.begin(9600);
	prevClockState = digitalRead(clk_in);
	pinMode(clk_in, INPUT);
	pinMode(out_1, OUTPUT);
	pinMode(out_2, OUTPUT);
	pinMode(out_3, OUTPUT);
}

void loop() {
		readClock();
	
}

void count() {
	counter = counter + 1;
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