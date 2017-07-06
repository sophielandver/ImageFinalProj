#ifndef SINK_H
#define SINK_H

#include <image.h>

class Sink {
	protected: 
		const Image *Input; //will be used as the input of the filter or writer 
		const Image *Input2;
	public:
		Sink();
		void SetInput(Image *img);
		void SetInput2(Image *img);
		const Image* GetInput();
		const Image* GetInput2();
		virtual const char *SinkName() = 0;
};

#endif