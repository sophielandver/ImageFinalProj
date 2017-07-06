#include <PNMwriter.h>
#include <image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void PNMwriter::Write(char *filename) {
	FILE *f_out = fopen(filename, "w");
	fprintf(f_out, "%s\n%d %d\n%d\n", Input->GetMagicNum(), Input->GetWidth(), Input->GetHeight(), Input->GetMaxVal());
	fwrite(Input->GetData(), sizeof(unsigned char), Input->GetWidth()*Input->GetHeight()*3, f_out);
	fclose(f_out);
}

const char *PNMwriter::SinkName() {
	return "PNMwriter";
}