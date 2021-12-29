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


void setup() {
	pinMode(clk_in, INPUT);
	pinMode(out_1, OUTPUT);
	pinMode(out_2, OUTPUT);
	pinMode(out_3, OUTPUT);
}

void loop() {
	if (digitalRead(clk_in) == HIGH) {
		digitalWrite(out_1, HIGH);
	} else {
		digitalWrite(out_1, LOW);
	}
}