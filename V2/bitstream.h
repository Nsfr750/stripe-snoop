/*
 * Bitstream Class!
 */

#ifndef BITSTREAM_H
#define BITSTREAM_H

typedef unsigned char Bytef;

class Bitstream {
public:
	Bitstream(const Bytef *, const int&);
	~Bitstream(void);
	Bytef * getBits(void) const;
	int getSize(void) const;
	void reverse(void);
	void print(void) const;

protected:
	
	Bytef * bitstream;
	int size;
};

#endif
