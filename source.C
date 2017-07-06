#include <source.h>
#include <image.h>

Image* Source::GetOutput() {
	return &srcImg;
}

Source::Source() {
	srcImg.setSource(this); //passing a pointer to this Source
}