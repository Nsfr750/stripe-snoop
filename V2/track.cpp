/**
 * @file track.cpp
 * @brief Holds bits and represented characters of a magstripe track
 * 
 * Track contains a bistream as well as the decoded characters of the stream
 * All character sets and decoding steps are done here
 *
 * @author Acidus (acidus@msblabs.org)
 *
 * This file is part of Stripe Snoop (http://stripesnoop.sourceforge.net)
 *
 * Stripe Snoop is licensed under the GPL. See COPYING for more info
 *
 * Copyright (C) 2005 Acidus, Most Significant Bit Labs
 */

#include "track.h"
#include "bitstream.h"
#include "testfuncs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Track::Track(const Bytef * bs, const int &size, const int &num) {
 *
 * Constructor for readers that capture bitstreams
 * 
 * bs - array representing bitstream
 * size - length of the array
 * num - track number to associate
 */
Track::Track(const Bytef * bs, const int &size, const int &num) {
	bitstream = new Bitstream(bs, size);
	bitstream->print();
	number = num;
	characters = NULL;
	decoded = false;
	charSet = 0;
	verbose = true;
}

/* Constructor for decoded characters. Used by readers that capture decoded
 * characters and not bitstreams
 *
 * Track::Track(const char * s, const int& num) {
 *
 * s - String of decoder characters
 * num - track number to associate
 */
Track::Track(const char * s, const int& num) {
	//printf("Creating Track %d\n",num);
	bitstream = NULL;
	characters = NULL;
	fieldBuffer = NULL;
	charSet = NONE;
	setChars(s);
	number = num;
	verbose = true;

}

int Track::getNumFields() const {
	return fields.size();
}

char * Track::getField(const int &i) const {
	if(!fields.empty())
		return fields.at(i);
	return NULL;
}

void Track::setVerbose(const bool &v) {
	verbose = v;
}

int Track::getNumber() const {
	return number;
}

char * Track::getChars() const {
	return (characters != NULL) ? characters : (char *) "Not Decoded";
}

int Track::getCharSet(void) const {
	return charSet;
}

bool Track::isValid() const {
	return decoded;
}

void Track::decode() {
	//serial readers will have already decoded
	if(!decoded) {
		//try all the character sets we know
		if(!parseBCD()) {
			//if (verbose) printf("Trying Alpha\n");
			parseAlpha();
		}
	}
}

// private functions

void Track::setChars(const char *s) {
	if(isBCD(s))
		setChars(s, NUMERIC);
	else if (isAlpha(s))
		setChars(s, ALPHANUMERIC);
}


void Track::setChars(const char *s, const int &i) {
	charSet = i;
	//printf("\"%s\" is characterset: %d\n",s,i);
	if(characters != NULL) {
		delete [] characters;
		delete [] fieldBuffer;
	}
	characters = new char[strlen(s)+1];
	fieldBuffer = new char[strlen(s)+1];
	strcpy(characters, s);
	strcpy(fieldBuffer, s);
	decoded = true; //mark decode as valid
	extractFields();
}

/**
 * @fn bool isDelim(char ch)
 *
 * compares a character to a list of delimiters and
 * returns whether the character is a delimiter
 *
 * @param ch char to check
 * @return true or false on delim status
 */
bool Track::isDelim(const char ch) const {
	char * ptr = new char[8];
	char * tmp = ptr; //so we can delete it later
	memset(ptr,0,8);
	if (charSet==ALPHANUMERIC)
		strcpy(ptr, ALPHADELIMS);
	else if (charSet==NUMERIC)
		strcpy(ptr, NUMERICDELIMS);

	while(*ptr) {
		if (*ptr++ == ch) {
			delete [] tmp;
			return true;
		}
	}
	delete [] tmp;
	return false;
}

void Track::extractFields() {

	fields.clear();

	char * chars = 	fieldBuffer ;
	//move forward one character
	chars++;
	char * start = chars;
	//printf("extracting\n");
	while(*chars != 0) {
		//printf(".");
		if(isDelim(*chars)) {
			//make delim a '\0', and move forward one
			*chars++='\0';
			//add ptr to field to array
			//printf("adding \"%s\"\n",start);
			fields.push_back(start);
			//set new pointer
			start=chars;
		} else {
			//go forward one
			chars++;
		}
	}
}





//------------------------------

int Track::findSSBCD() const {
	Bytef * buffer = bitstream->getBits();
	int size = bitstream->getSize();
	int k;
	for (k = 0; k < size - 4; k++) {
		if (buffer[k] != 0) {
			break;
		}
	}
	printf("first nonzero @ %d\n", k);
	if( (buffer[k] == 1) &&
	    (buffer[k + 1] == 1) &&
	    (buffer[k + 2] == 0) && 
	    (buffer[k + 3] == 1) && 
	    (buffer[k + 4] == 0)) {
		if (verbose) printf("Start char located at: %d\n", k);
		return k;
	}
	if(verbose) {
		if (k == size -4) {
			printf("Start char not found\n");
		} else {
			printf("Start char not first character\n");
		}
	}
	return -1;
}

int Track::findESBCD(const int &ss) const {
	Bytef * buffer = bitstream->getBits();
	int size = bitstream->getSize();
	int k;
	int p=1;
	for (k = ss; k < size -4; k+=5) 	{
		printf("Char %d: \"%d%d%d%d%d\"\n", p, buffer[k], buffer[k+1], buffer[k+2], buffer[k+3], buffer[k+4]);

		if ((buffer[k] == 1) &&
		   (buffer[k + 1] == 1) &&
		   (buffer[k + 2] == 1) &&
		   (buffer[k + 3] == 1) &&
		   (buffer[k + 4] == 1)) {
			break;
		}
	p++;
	}
	if(k >= size -4) 	{
		if (verbose) printf("End Char not found\n");
		return -1;
	}
	if(verbose) printf("End Char located at: %d\n", k);
	return k;
}

bool Track::errorCheckBCD(const int &start, const int &end) const {
	
	Bytef * buffer = bitstream->getBits();
	int size = bitstream->getSize();
	//Phase 1----------Are our inputs even valid?
	if(start < 0 || end < 0 || start > end) {
		return false;
	}
	
	//Phase 2---------Does it pass its LRC?
	char lrc[5];
	int i, j;

	memset(lrc,0,5);
	//Just XOR the Data Bits of each character, not the parity bit*/
	for(i=start;i<=end;i+=5)
		for(j=0;j<4;j++)
    			lrc[j] = lrc[j] ^ buffer[i+j];
	//Calculate LRC Odd Parity bit
	i = 0;
	for(j = 0; j<4;j++)
		if(lrc[j]==1)
			i++;
	if( (i % 2) == 0) lrc[4]=1;
	//valid LRC?
	if(end+5 <=(size - 5) && strncmp(lrc,(char *)&buffer[end+5],5)!=0)
			return false;

	//Phase 3----------Parity Checks
//	for(i=start;i<=end;i+=5) {
//		if(!parityCheck(&buffer[i], ODD, 5)) {
//		if (verbose) printf("Parity Check Failed at %d\n",i);
//			return false;
//		}
//	}
	return true;

}


int Track::findSSAlpha() const{
	Bytef * buffer = bitstream->getBits();
	int size = bitstream->getSize();
	int k;

	for (k = 0;k < size - 6; k++) 	{
		if (buffer[k] !=0) {
			break;
		}
	}
	printf("First Nonzero @ %d\n",k);
	if( (buffer[k] == 1) &&
	    (buffer[k + 1] == 0) && 
	    (buffer[k + 2] == 1) &&
	    (buffer[k + 3] == 0) &&
	    (buffer[k + 4] == 0) &&
	    (buffer[k + 5] == 0) &&
	    (buffer[k + 6] == 1)) {
		if (verbose) printf("Start char located at: %d\n", k);
		return k;
	}
	if(verbose) {
		if (k == size - 6) {
			printf("Start char not found\n");
		} else {
			printf("Start char not first character\n");
		}
	}
	return -1;
}

int Track::findESAlpha(const int &ss) const {
	Bytef * buffer = bitstream->getBits();
	int size = bitstream->getSize();
	int k;
	for(k = ss;k <  size - 6; k += 7) {
		if ((buffer[k] == 1) &&
		    (buffer[k + 1] == 1) && 
		    (buffer[k + 2] == 1) && 
		    (buffer[k + 3] == 1) && 
		    (buffer[k + 4] == 1) && 
		    (buffer[k + 5] == 0) && 
		    (buffer[k + 6] == 0)) {
			break;
		}
	}
	
	if(k >= size - 6) {
		if (verbose) printf("End Char not found\n");
		return -1;
	}
	if(verbose) printf("End Char located at: %d\n", k);
	return k;
}

bool Track::errorCheckAlpha(const int &start, const int &end) const {

	//Phase 1----------Are our inputs even valid?
	if(start <0 || end <0 ||start >= end)
		return false;

	Bytef * buffer = bitstream->getBits();
	int size = bitstream->getSize();

	//Phase 2---------Does it pass its LRC?
	char lrc[7];
	int i, j;
	
	memset(lrc,0,7);
	//Just XOR the Data Bits of each character, not the parity bit*/
	for(i=start;i<=end;i+=7)
		for(j=5;j>-1;j--)
    		lrc[j] = lrc[j] ^ buffer[i+j];
	//Calculate LRC Odd Parity bit
	i = 0;
	for(j = 0; j<6;j++)
		    if(lrc[j]==1)
			i++;
	if( (i % 2) == 0) lrc[6]=1;
	//valid LRC?
	if(end+7 <=(size -7) && strncmp(lrc, (char *)&buffer[end+7],7)!=0)
			return false;

	//Phase 3----------Parity Checks
	for(i=start;i<=end;i+=7) {
		if(!parityCheck(&buffer[i], ODD, 7)) {
		if (verbose) printf("Parity Check Failed at %d\n",i);
			return false;
		}
	}
	return true;
}


bool Track::parityCheck(const Bytef *bs, const int &type,
			const int &bpc) const {
	int i,j=0;
	for(i=0;i<bpc-1;i++)
		if(bs[i]==1) j++;
	if((j % 2) ==0 && bs[bpc-1]!=type)
		return false;
	return true;
}

bool Track::parseBCD() {
	
	int start = findSSBCD();
	int end = findESBCD(start);
	/* is bitstream valid? */
	if(!errorCheckBCD(start, end)) {
		if (verbose) printf("BCD Error checking fails, Trying to parse backwards\n");
        	// try backwards
		bitstream->reverse();
		//bitstream->print();
		start = findSSBCD();
		end = findESBCD(start);
		if(!errorCheckBCD(start, end)) {
			//flip it back
			bitstream->reverse();
			printf("BCD Error checking failed in both directions\n");
			printf("Not a valid BCD Character set\n");
			return false;
		}
	}
	//we are good, so decode it
	decodeBCD(start, end);
	return true;
}

bool Track::parseAlpha() {
	
	int start = findSSAlpha();
	int end = findESAlpha(start);
	/* is bitstream valid? */
	if(!errorCheckAlpha(start, end)) {
		if (verbose) printf("Alpha Error checking fails, Trying to parse backwards\n");
        	/* try backwards */
		bitstream->reverse();
		start = findSSAlpha();
		end = findESAlpha(start);
		if(!errorCheckAlpha(start, end)) {
			printf("Alpha Error checking failed in both directions\n");
			printf("Not a valid Alpha Character set\n");
			return false;
		}
	}
	//we are good, so decode it
	decodeAlpha(start, end);
	return true;
}


void Track::decodeBCD(const int &start,const int &end)
{
	char * tempDecode = new char[41]; //can't have more than 40 characters
	memset(tempDecode,0,41);
	Bytef * buffer = bitstream->getBits(); //get the bitstream
	char *ptrNext;
	char BCD[17]; //will hold the character set string

	strcpy(BCD, "084<2:6>195=3;7?");
	ptrNext = tempDecode;

	for(int i = start; i <= end; i += 5 ) {
		int k = 0;
		for(int j = 0; j < 4; j++) {
			k = k * 2 + buffer[i + j];
		}
		if(k>=0 && k<=15) {
			*ptrNext++=BCD[k];
		} else {
			*ptrNext++='X'; //if we have an undefined character
		}
	}
	//set new decoded characters
	setChars(tempDecode, NUMERIC);
	delete [] tempDecode;
}


void Track::decodeAlpha(const int &start,const int &end)
{
	char * tempDecode = new char[80]; //can't have more than 79 characters
	memset(tempDecode,0,80);
	Bytef * buffer = bitstream->getBits(); //get the bitstream
	char *ptrNext;
	char alpha[65]; //will hold the character set string

	strcpy(alpha, " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_");
	ptrNext = tempDecode;

	for(int i = start; i <= end; i += 7 ) {
		int k = 0;
		for(int j=5;j>-1;j--)
			k=k*2+buffer[i+j];
		if(k>=0 && k<=63) {
			*ptrNext++=alpha[k];
		} else {
			*ptrNext++='X'; //if we have an undefined character
		}
	}
	//set new decoded characters
	setChars(tempDecode, ALPHANUMERIC);
	delete [] tempDecode;
}


