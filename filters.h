#ifndef FILTERS_H
#define FILTERS_H

#include <sink.h>
#include <source.h>

class Filter : public Source, public Sink {
	//can't initiate Filter because it doesnt define the Execute method
	public:
		virtual void Update();
		virtual const char *FilterName() = 0;
		virtual const char *SourceName();
		virtual const char *SinkName();

};

class Shrinker : public Filter {
	//use Input as input to the shrinker method and save the new small image we make in srcImg
	public:
		virtual void Execute();
		virtual const char *FilterName();
};

class LRConcat : public Filter {
	//use Input and Input2 as inputs to the lrconcat method and save the new big image we make in srcImg
	public:
		virtual void Execute();
		virtual const char *FilterName();

};

class TBConcat : public Filter {
	//use Input and Input2 as inputs to the tbconcat method and save the new big image we make in srcImg
	public:
		virtual void Execute();
		virtual const char *FilterName();

};

class Blender : public Filter {
	//use Input and Input2 as inputs to the blend method and save the new image we make in srcImg
	float factor;
	public:
		void SetFactor(float fa);
		virtual void Execute();
		virtual const char *FilterName();

};

class Mirror : public Filter {
	//use Input as input to the Mirror method and save the new image we make in srcImg
	public:
		virtual void Execute();
		virtual const char *FilterName();

};

class Rotate : public Filter {
	//use Input as input to the Rotate method and save the new image we make in srcImg
	public:
		virtual void Execute();
		virtual const char *FilterName();

};

class Subtract : public Filter {
	//use Input and Input2 as inputs to the Subtract method and save the new image we make in srcImg
	public:
		virtual void Execute();
		virtual const char *FilterName();

};

class Grayscale : public Filter {
	//use Input as input to the Grayscale method and save the new image we make in srcImg
	public:
		virtual void Execute();
		virtual const char *FilterName();

};

class Blur : public Filter {
	//use Input as input to the Blur method and save the new image we make in srcImg
	public:
		virtual void Execute();
		virtual const char *FilterName();

};

class Color : public Source {
	int width;
	int height;
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	public:
		Color(int w, int h, unsigned char r, unsigned char g, unsigned char b);
		virtual void Update();
		virtual void Execute();	
		virtual const char *SourceName();

};

class CheckSum : public Sink {
	public:
		void OutputCheckSum(const char *filename);
		virtual const char *SinkName();
};


#endif