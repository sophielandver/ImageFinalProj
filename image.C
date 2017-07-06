#include <image.h>
#include <stdlib.h>
#include <string.h>
#include <source.h>

Image::Image() { 
	width = 0;
	height = 0;
	maxval = 255;
	strcpy(magicNum, "P6");
	data = NULL;
	source = NULL;
}

Image::~Image() {
	free(data);
}

Image::Image(int w, int h, int max_color, char *mag_num) { //USE: this constructor seems most useful for reading an image in becuase now ready to populate pixels i.e. read bytes in image.data now
	width = w;
	height = h;
	maxval = max_color;
	strcpy(magicNum, mag_num);
	data = (unsigned char *) malloc(width*height*3*sizeof(unsigned char));
}

Image::Image(Image &img) {
	width = img.width; 
	height = img.height;
	maxval = img.maxval;
	strcpy(magicNum, img.magicNum);
	data = (unsigned char *) malloc(width*height*3*sizeof(unsigned char));
	memcpy(data, img.data, width*height*3); //deep copy. Could have done a shallow copy, i.e. data  = img.data but then if change img's data change this image's data too and we choose not to have this behavior
}

void Image::ResetSize(int w, int h) { //after we call this method we are now ready to populate the pixels
	width = w;
	height = h;
	if (data == NULL) {
		data = (unsigned char *) malloc(width*height*3*sizeof(unsigned char)); //putting new chunk of data in
	} else {
		free(data); //throwing old data away 
		data = (unsigned char *) malloc(width*height*3*sizeof(unsigned char)); //putting new chunk of data in
	}
}

void Image::setSource(Source *s) {
	source = s;
}

void Image::Update() const { 
	if (source != NULL) { //if someone just makes an Image and then calls this Update method, we dont want to get a null pointer dereference exception 
		source->Update();
	}
}

int Image::GetWidth() const {
	return width;
}

void Image::SetWidth(int w) {
	width = w;
}

int Image::GetHeight() const {
	return height;
}

void Image::SetHeight(int h) {
	height = h;
}

int Image::GetMaxVal() const {
	return maxval;
}

void Image::SetMaxVal(int max) {
	maxval = max;
}

const char* Image::GetMagicNum() const {
	return magicNum;
}

void Image::SetMagicNum(char *mag_num) {
	strcpy(magicNum, mag_num);
}

unsigned char* Image::GetData() const {
	return data;
}	

void Image::SetData(unsigned char* d) { 
//I chose to use shallow copies. 
//Don't really need this method but for consistency I wanted a SetData
	if (data == NULL) {
		data = d; //shallow copy
	} else {
		free(data); //throwing old data away 
		data = d; //shallow copy
	}
}

void Image::SetPixelColor(int col, int row, unsigned char red, unsigned char green, unsigned char blue) {
	data[3*(row*width + col) + 0] = red;
	data[3*(row*width + col) + 1] = green;
	data[3*(row*width + col) + 2] = blue;
}

unsigned char Image::GetPixelRed(int col, int row) const {
	return data[3*(row*width + col) + 0];
}

unsigned char Image::GetPixelGreen(int col, int row) const {
	return data[3*(row*width + col) + 1];
}

unsigned char Image::GetPixelBlue(int col, int row) const {
	return data[3*(row*width + col) + 2];
}

