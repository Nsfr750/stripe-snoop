/**
 * Track Class!
 */



#ifndef TRACK_H
#define TRACK_H

#include "bitstream.h"
#include <vector>

typedef std::vector<char *>  stringVec;

//-----------------------Defines
//used for character sets
#define NONE 0
#define ALPHANUMERIC 1
#define NUMERIC 2
//parity defines
#define ODD 1
#define EVEN 0

#define ALPHADELIMS "%^?"
#define NUMERICDELIMS ":<>=?"



class Track {
public:
	Track(const Bytef *,const int&, const int&);	
	Track(const char *, const int&);
	void decode(void);
	char * getChars(void) const; //return decoded characters (if any)
	int getNumber(void) const; //returns track number
	bool isValid(void) const;
	void setVerbose(const bool&);
	int getNumFields(void) const;
	char * getField(const int&) const;
	int getCharSet(void) const;

private:
	
	Bitstream * bitstream;
	char * characters;
	bool decoded;
	int charSet;	//use defines
	int number;
	bool verbose;
	//bool
	
	//field stuff;
	char * fieldBuffer;
	stringVec fields;
	bool isDelim(const char) const;
	void extractFields();

	void setChars(const char *);	
	void setChars(const char *, const int &);
	bool parityCheck(const Bytef *, const int&, const int&) const;

	//-------------------------Supported Character sets
	//BCD	
	int findSSBCD(void) const;
	int findESBCD(const int&) const;
	bool errorCheckBCD(const int&, const int&) const;
	bool parseBCD();
	void decodeBCD(const int&, const int&);
	//ALPHA
	int findSSAlpha(void) const;
	int findESAlpha(const int&) const;
	bool errorCheckAlpha(const int&, const int&) const;
	bool parseAlpha();
	void decodeAlpha(const int&, const int&);


	void extractFields2();
	bool isDelim(const char, const char *) const;
	
};

#endif
