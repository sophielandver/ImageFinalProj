#include <sink.h>
#include <image.h>
#include <stdlib.h>

Sink::Sink() {
	Input = NULL;
	Input2 = NULL;
	//now we have a way of asking whether this sink has just an Input or also an Input2
}

void Sink::SetInput(Image *img) {
	Input = img; //shallow copy
	
}

void Sink::SetInput2(Image *img) {
	Input2 = img; //shallow copy
}

const Image* Sink::GetInput() {
	return Input;
}

const Image* Sink::GetInput2() {
	return Input2;
}