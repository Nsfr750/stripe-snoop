/**
 * @file bitstream.cpp
 * @brief Holds the raw bitstream of a magstripe track.
 * 
 * @author Acidus (acidus@msblabs.org)
 * 
 * This file is part of Stripe Snoop (http://stripesnoop.sourceforge.net)
 * 
 * Stripe Snoop is licensed under the GPL. See COPYING for more info
 * 
 * Copyright (C) 2005 Acidus, Most Significant Bit Labs
 */

#include "bitstream.h"
#include <string.h>
#include <stdio.h>


/**
 * constructor for readers with Direct IO
 * @param bs pointer to a constant bitstream array
 * @param s length of that bitstream
 */
Bitstream::Bitstream(const Bytef *bs, const int &s) {
	bitstream = new Bytef[s];
	memcpy(bitstream, bs, s);
	size = s;
}
/** Bitstream deconstuctor*/
Bitstream::~Bitstream() {
	delete [] bitstream; //avoids memory leaks
}

Bytef * Bitstream::getBits() const {
	return bitstream;
}

int Bitstream::getSize() const {
	return size;
}


void Bitstream::reverse() {
	Bytef * temp = new Bytef[size];
	for(int i = 0; i < size; i++)
		temp[i]=bitstream[size - 1 - i];
	memcpy(bitstream, temp, size);
	delete [] temp;
}

void Bitstream::print(void) const {
	for(int i=0; i< size; i++)
		printf("%c",bitstream[i]+48);
	printf(" --- size: %d\n",size);
}

