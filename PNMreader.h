#ifndef PNMREADER_H
#define PNMREADER_H

#include <source.h>

class PNMreader : public Source {
	char* filename;
	public:
		PNMreader(char *f);
		~PNMreader();
		virtual void Update();
		virtual void Execute();	
		virtual const char *SourceName();
};

#endif