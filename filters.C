#include <filters.h>
#include <image.h>
#include <stdio.h>
#include <exception>
#include <string.h>
#include <logging.h>
#include <set>

std::set<Image*> myset;

const char *Filter::SourceName() {
	return FilterName();
}

const char *Filter::SinkName() {
	return FilterName();
}

void Filter::Update() {
	//filters are recurssive case, i.e. becase they have an Input or an Input2 we must first call update on these images before we can execute this filter

	//is &srcImg in the set? if it is exception, if not put it in the set
	if (myset.find(&srcImg) != myset.end()) { //if srcImg is in the set
		char msg[1024];
		sprintf(msg, "%s: Somewhere you have set filter's input to be filter's output, resulting in non-terminating loop.", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;

	} else { //srcImg is not in the set
		myset.insert(&srcImg);
	}

	if (Input != NULL) {
		char msg[128];
		sprintf(msg, "%s: about to update input1", SourceName());
		Logger::LogEvent(msg);

		Input->Update(); //calling Image class's Update method

		sprintf(msg, "%s: done updating input1", SourceName());
		Logger::LogEvent(msg);
	} 

	if (Input2 != NULL) {
		char msg[128];
		sprintf(msg, "%s: about to update input2", SourceName());
		Logger::LogEvent(msg);

		Input2->Update(); //calling Image class's Update method

		sprintf(msg, "%s: done updating input2", SourceName());
		Logger::LogEvent(msg);
	}

	char msg[128];
	sprintf(msg, "%s: about to execute", SourceName());
	Logger::LogEvent(msg);

	//now that we updated the input(s) we are ready to call execute
	Execute();

	sprintf(msg, "%s: done executing", SourceName());
	Logger::LogEvent(msg);

	//remove &srcImg from the set
	myset.erase(&srcImg);
}

const char *Shrinker::FilterName() {
	return "Shrinker";
}

void Shrinker::Execute() {
	//only has one input
	if (Input == NULL) {
		char msg[1024];
		sprintf(msg, "%s: no input1!", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	srcImg.ResetSize((Input->GetWidth())/2, (Input->GetHeight()/2));
	//now populate output image data
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	int width = srcImg.GetWidth();
	int height = srcImg.GetHeight();
	for(int col=0; col<width; col++) {
		for(int row=0; row<height; row++) {
			red = Input->GetPixelRed(2*col, 2*row);
			green = Input->GetPixelGreen(2*col, 2*row);
			blue = Input->GetPixelBlue(2*col, 2*row);
			srcImg.SetPixelColor(col, row, red, green, blue);
		}
	}
}

const char *LRConcat::FilterName() {
	return "LRConcat";
}

void LRConcat::Execute() { //Input = leftInput   Input2 = rightInput
	//takes 2 inputs that must have the same height
	//before ask about inputs heights need to make sure inputs aren't null or will get null pointer exception
	if (Input == NULL) {
		char msg[1024];
		sprintf(msg, "%s: no input1!", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	if (Input2 == NULL) {
		char msg[1024];
		sprintf(msg, "%s: no input2!", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	if (Input->GetHeight() != Input2->GetHeight()) {
		char msg[1024];
		sprintf(msg, "%s: heights must match: %d, %d", SinkName(), Input->GetHeight(), Input2->GetHeight());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	int new_width = Input->GetWidth() + Input2->GetWidth();
	srcImg.ResetSize(new_width, Input->GetHeight());
	//now populate output image data 
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	int srcWidth = srcImg.GetWidth();
	int srcHeight = srcImg.GetHeight();
	int inputWidth = Input->GetWidth();

	for(int col=0; col<srcWidth; col++) {
		for(int row=0; row<srcHeight; row++) {
			if (col < inputWidth) {
				//get pixel from leftinput
				red = Input->GetPixelRed(col, row);
				green = Input->GetPixelGreen(col, row);
				blue = Input->GetPixelBlue(col, row);
				srcImg.SetPixelColor(col, row, red, green, blue);
			} else {
				//get pixel from rightInput
				red = Input2->GetPixelRed(col - inputWidth, row);
				green = Input2->GetPixelGreen(col - inputWidth, row);
				blue = Input2->GetPixelBlue(col - inputWidth, row);
				srcImg.SetPixelColor(col, row, red, green, blue);
			}
			
		}
	}
}

const char *TBConcat::FilterName() {
	return "TBConcat";
}

void TBConcat::Execute() { //Input = topInput  Input2 = bottomInput
	//takes 2 inputs that must have the same width
	if (Input == NULL) {
		char msg[1024];
		sprintf(msg, "%s: no input1!", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	if (Input2 == NULL) {
		char msg[1024];
		sprintf(msg, "%s: no input2!", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	if (Input->GetWidth() != Input2->GetWidth()) {
		char msg[1024];
		sprintf(msg, "%s: widths must match: %d, %d", SinkName(), Input->GetWidth(), Input2->GetWidth());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	int new_height = Input->GetHeight() + Input2->GetHeight();
	srcImg.ResetSize(Input->GetWidth(), new_height);
	//now populate output image data 
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	int srcWidth = srcImg.GetWidth();
	int srcHeight = srcImg.GetHeight();
	int inputHeight = Input->GetHeight();
	for(int col=0; col<srcWidth; col++) {
		for(int row=0; row<srcHeight; row++) {
			if (row < inputHeight) {
				//get pixel from topInput
				red = Input->GetPixelRed(col, row);
				green = Input->GetPixelGreen(col, row);
				blue = Input->GetPixelBlue(col, row);
				srcImg.SetPixelColor(col, row, red, green, blue);
			} else {
				//get pixel from bottomInput
				red = Input2->GetPixelRed(col, row - inputHeight);
				green = Input2->GetPixelGreen(col, row - inputHeight);
				blue = Input2->GetPixelBlue(col, row - inputHeight);
				srcImg.SetPixelColor(col, row, red, green, blue);
			}
			
		}
	}
}

const char *Blender::FilterName() {
	return "Blender";
}

void Blender::SetFactor(float fa) {
	factor = fa;
}

void Blender::Execute() {
	//takes 2 inputs that must have the same width and the same height and factor must be between 0 and 1
	if (Input == NULL) {
		char msg[1024];
		sprintf(msg, "%s: no input1!", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	if (Input2 == NULL) {
		char msg[1024];
		sprintf(msg, "%s: no input2!", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	if (Input->GetWidth() != Input2->GetWidth()) {
		char msg[1024];
		sprintf(msg, "%s: widths must match: %d, %d", SinkName(), Input->GetWidth(), Input2->GetWidth());
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	if (Input->GetHeight() != Input2->GetHeight()) {
		char msg[1024];
		sprintf(msg, "%s: heights must match: %d, %d", SinkName(), Input->GetHeight(), Input2->GetHeight());
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	if (factor < 0 || factor > 1) {
		char msg[1024];
		sprintf(msg, "Invalid factor for Blender: %f", factor);
		DataFlowException e(SinkName(), msg);
		throw e;
	}


	srcImg.ResetSize(Input->GetWidth(), Input->GetHeight());
	//now populate output image data
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	int srcWidth = srcImg.GetWidth();
	int srcHeight = srcImg.GetHeight();
	for(int col=0; col<srcWidth; col++) {
		for(int row=0; row<srcHeight; row++) {
			red = (factor * Input->GetPixelRed(col, row)) + ((1-factor) * Input2->GetPixelRed(col, row));
			green = (factor * Input->GetPixelGreen(col, row)) + ((1-factor) * Input2->GetPixelGreen(col, row));
			blue = (factor * Input->GetPixelBlue(col, row)) + ((1-factor) * Input2->GetPixelBlue(col, row));
			srcImg.SetPixelColor(col, row, red, green, blue);
		}
	}
}

const char *Mirror::FilterName() {
	return "Mirror";
}

void Mirror::Execute() {
	//only has one input
	if (Input == NULL) {
		char msg[1024];
		sprintf(msg, "%s: no input1!", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	srcImg.ResetSize(Input->GetWidth(), Input->GetHeight());
	//now populate output image data
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	int width = srcImg.GetWidth();
	int height = srcImg.GetHeight();
	for(int col=0; col<width; col++) {
		for(int row=0; row<height; row++) {
			red = Input->GetPixelRed(col, row);
			green = Input->GetPixelGreen(col, row);
			blue = Input->GetPixelBlue(col, row);
			srcImg.SetPixelColor(width - 1 - col, row, red, green, blue);
		}
	}
}

const char *Rotate::FilterName() {
	return "Rotate";
}

void Rotate::Execute() {
	//only has one input
	if (Input == NULL) {
		char msg[1024];
		sprintf(msg, "%s: no input1!", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	srcImg.ResetSize(Input->GetHeight(), Input->GetWidth());
	//now populate output image data
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	int width = Input->GetWidth();
	int height = Input->GetHeight();
	for(int row=0; row<height; row++) {
		for(int col=0; col<width; col++) {
			red = Input->GetPixelRed(col, row);
			green = Input->GetPixelGreen(col, row);
			blue = Input->GetPixelBlue(col, row);
			srcImg.SetPixelColor(height - 1 - row, col, red, green, blue);
		}
	}
}

const char *Subtract::FilterName() {
	return "Subtract";
}

void Subtract::Execute() {
	//takes 2 inputs that must have the same width and height
	if (Input == NULL) {
		char msg[1024];
		sprintf(msg, "%s: no input1!", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	if (Input2 == NULL) {
		char msg[1024];
		sprintf(msg, "%s: no input2!", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	if (Input->GetWidth() != Input2->GetWidth()) {
		char msg[1024];
		sprintf(msg, "%s: widths must match: %d, %d", SinkName(), Input->GetWidth(), Input2->GetWidth());
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	if (Input->GetHeight() != Input2->GetHeight()) {
		char msg[1024];
		sprintf(msg, "%s: heights must match: %d, %d", SinkName(), Input->GetHeight(), Input2->GetHeight());
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	srcImg.ResetSize(Input->GetWidth(), Input->GetHeight());
	//now populate output image data 
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	int width = srcImg.GetWidth();
	int height = srcImg.GetHeight();
	for(int col=0; col<width; col++) {
		for(int row=0; row<height; row++) {
			//get red
			if (Input->GetPixelRed(col, row) > Input2->GetPixelRed(col, row)) {
				red = Input->GetPixelRed(col, row) - Input2->GetPixelRed(col, row);
			} else {
				red = 0;
			}
			//get green
			if (Input->GetPixelGreen(col, row) > Input2->GetPixelGreen(col, row)) {
				green = Input->GetPixelGreen(col, row) - Input2->GetPixelGreen(col, row);
			} else {
				green = 0;
			}
			//get blue
			if (Input->GetPixelBlue(col, row) > Input2->GetPixelBlue(col, row)) {
				blue = Input->GetPixelBlue(col, row) - Input2->GetPixelBlue(col, row);
			} else {
				blue = 0;
			}
			srcImg.SetPixelColor(col, row, red, green, blue);
		}
	}
}

const char *Grayscale::FilterName() {
	return "Grayscale";
}

void Grayscale::Execute() {
	//only has one input
	if (Input == NULL) {
		char msg[1024];
		sprintf(msg, "%s: no input1!", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	srcImg.ResetSize(Input->GetWidth(), Input->GetHeight());
	//now populate output image data
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char newcolor;
	int width = srcImg.GetWidth();
	int height = srcImg.GetHeight();
	for(int col=0; col<width; col++) {
		for(int row=0; row<height; row++) {
			red = Input->GetPixelRed(col, row);
			green = Input->GetPixelGreen(col, row);
			blue = Input->GetPixelBlue(col, row);
			newcolor = (red/5) + (green/2) + (blue/4);
			srcImg.SetPixelColor(col, row, newcolor, newcolor, newcolor);
		}
	}
}

const char *Blur::FilterName() {
	return "Blur";
}

void Blur::Execute() {
	//only has one input
	if (Input == NULL) {
		char msg[1024];
		sprintf(msg, "%s: no input1!", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	srcImg.ResetSize(Input->GetWidth(), Input->GetHeight());
	//now populate output image data
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	int width = srcImg.GetWidth();
	int height = srcImg.GetHeight();
	for(int col=0; col<width; col++) {
		for(int row=0; row<height; row++) {
			//if pixed is on the top row, or the bottom row, or the leftmost column or the rightmost column, copy it over
			if (row == 0 || row == (height - 1) || col == 0 || col == (width - 1)) {
				red = Input->GetPixelRed(col, row);
				green = Input->GetPixelGreen(col, row);
				blue = Input->GetPixelBlue(col, row);
				srcImg.SetPixelColor(col, row, red, green, blue);
			} else {
				//get avg of neightboring pixels 
				red = Input->GetPixelRed(col - 1, row - 1)/8 + Input->GetPixelRed(col, row - 1)/8 + 
					Input->GetPixelRed(col + 1, row - 1)/8 + Input->GetPixelRed(col - 1, row)/8 + 
					Input->GetPixelRed(col + 1, row)/8 + Input->GetPixelRed(col - 1, row + 1)/8 +
					Input->GetPixelRed(col, row + 1)/8 + Input->GetPixelRed(col + 1, row + 1)/8;
				green = Input->GetPixelGreen(col - 1, row - 1)/8 + Input->GetPixelGreen(col, row - 1)/8 + 
					Input->GetPixelGreen(col + 1, row - 1)/8 + Input->GetPixelGreen(col - 1, row)/8 + 
					Input->GetPixelGreen(col + 1, row)/8 + Input->GetPixelGreen(col - 1, row + 1)/8 +
					Input->GetPixelGreen(col, row + 1)/8 + Input->GetPixelGreen(col + 1, row + 1)/8;
				blue = Input->GetPixelBlue(col - 1, row - 1)/8 + Input->GetPixelBlue(col, row - 1)/8 + 
					Input->GetPixelBlue(col + 1, row - 1)/8 + Input->GetPixelBlue(col - 1, row)/8 + 
					Input->GetPixelBlue(col + 1, row)/8 + Input->GetPixelBlue(col - 1, row + 1)/8 +
					Input->GetPixelBlue(col, row + 1)/8 + Input->GetPixelBlue(col + 1, row + 1)/8;
				srcImg.SetPixelColor(col, row, red, green, blue);
			}
		}
	}
}

Color::Color(int w, int h, unsigned char r, unsigned char g, unsigned char b) {
	width = w;
	height = h;
	red = r;
	green = g;
	blue = b;
}

void Color::Update() {
	//the Color is the base because it has no Input or Input2 and hence we have nothing to call update on and thus we simply call execute 
	char msg[128];
	sprintf(msg, "%s: about to execute", SourceName());
	Logger::LogEvent(msg);

	//now that we updated the input(s) we are ready to call execute
	Execute();

	sprintf(msg, "%s: done executing", SourceName());
	Logger::LogEvent(msg);
}

void Color::Execute() {
	srcImg.ResetSize(width, height);
	for(int col=0; col<width; col++) {
		for(int row=0; row<height; row++) {
			srcImg.SetPixelColor(col, row, red, green, blue);
		}
	}
}

const char *Color::SourceName() {
	return "Color";
}

void CheckSum::OutputCheckSum(const char *filename) {
	unsigned char red = 0;
	unsigned char green = 0;
	unsigned char blue = 0;
	int width = Input->GetWidth();
	int height = Input->GetHeight();
	for(int col=0; col<width; col++) {
		for(int row=0; row<height; row++) {
			red += Input->GetPixelRed(col, row);
			green += Input->GetPixelGreen(col, row);
			blue += Input->GetPixelBlue(col, row);
		}
	}

	FILE *f_out = fopen(filename, "w");
	fprintf(f_out, "CHECKSUM: %d, %d, %d\n", red, green, blue);
	fclose(f_out);
}

const char *CheckSum::SinkName() {
	return "CheckSum";
}









