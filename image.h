#ifndef IMAGE_H
#define IMAGE_H

class Source; //need to do this, can't include source, because soure includes image too

class Image
{
		int width;
		int height;
		int maxval;
		char magicNum[3]; //we know for our projects we are always going to have "P6\0"
		unsigned char *data;
		class Source* source;

	public:
		Image(); //default constructor
		~Image();
		Image(int w, int h, int max, char *mag_num); //parameterized constructor 
		Image(Image &img); //copy constructor 
		void ResetSize(int w, int h);

		void setSource(Source *s);
		void Update() const;
		int GetWidth() const; 
		void SetWidth(int w);
		int GetHeight() const;
		void SetHeight(int h);
		int GetMaxVal() const;
		void SetMaxVal(int max);
		const char* GetMagicNum() const;
		void SetMagicNum(char *mag_num);
		unsigned char* GetData() const; 
		void SetData(unsigned char* d); //dont want to be using this though, instead use ResetSize

		void SetPixelColor(int col, int row, unsigned char red, unsigned char green, unsigned char blue);
		unsigned char GetPixelRed(int col, int row) const;
		unsigned char GetPixelGreen(int col, int row) const;
		unsigned char GetPixelBlue(int col, int row) const;

};

#endif 
