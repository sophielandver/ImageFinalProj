#ifndef SOURCE_H
#define SOURCE_H

#include <image.h>

class Source {
	protected:
		Image srcImg; //will be used to store the output of a filter or a reader. 
		virtual void Execute() = 0; //we dont define this so this doesnt need to be in source.C
	public:
		Source();
		virtual void Update() = 0; //pure virtual function
		Image* GetOutput();
		virtual const char *SourceName() = 0;
};

#endif