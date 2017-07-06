#include <PNMreader.h>
#include <image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <logging.h>

PNMreader::PNMreader(char *f) {
	int stringLength = strlen(f);
	filename = new char[stringLength + 1]; //malloced. Can't use stack array because we would need to initalize it with a size and we dont know the size, it's different for each string
	strcpy(filename, f);
}

PNMreader::~PNMreader() {
	delete [] filename;
}

void PNMreader::Update() {
	//the reader is the base because it has no Input or Input2 and hence we have nothing to call update on and thus we simply call execute 
	char msg[128];
	sprintf(msg, "%s: about to execute", SourceName());
	Logger::LogEvent(msg);

	//now that we updated the input(s) we are ready to call execute
	Execute();

	sprintf(msg, "%s: done executing", SourceName());
	Logger::LogEvent(msg);
}

void PNMreader::Execute() {
	//read image from filename and put it into srcImg member from the Source class
	FILE *f_in  = fopen(filename, "r");
    if (f_in == NULL) {
		char msg[1024];
		sprintf(msg, "Unable to open image file \"%s\".", filename);
		DataFlowException e(SourceName(), msg);
		throw e;
	}
    int width;
    int height;
    int maxColor;
    char magicNumber[3];
	//read the ASCII from file
	fscanf(f_in, "%s\n%d %d\n%d\n", magicNumber, &width, &height, &maxColor); 

	//prepare the image for populating the data
	srcImg.ResetSize(width, height);
	srcImg.SetMaxVal(maxColor);
	srcImg.SetMagicNum(magicNumber);

	//now read the binary from file and put this into image.data i.e. 
	fread(srcImg.GetData(), sizeof(unsigned char), width*height*3, f_in);

	fclose(f_in);
}

const char *PNMreader::SourceName() {
	return "PNMreader";
}



