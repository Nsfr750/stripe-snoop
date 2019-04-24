/**
 * @file reader.cpp
 * @brief Controls all supported readers.
 * 
 * Contains code to initialize, read, and create config files for all
 * support reader types. Contains OS and Platform specific code, that
 * is included in #ifdefs
 *
 * @author Acidus (acidus@msblabs.org)
 *
 * This file is part of Stripe Snoop (http://stripesnoop.sourceforge.net)
 *
 * Stripe Snoop is licensed under the GPL. See COPYING for more info
 *
 * Copyright (C) 2005 Acidus, Most Significant Bit Labs
 */
#include "reader.h"
#include "card.h"
#include "track.h"
#include "misc.h"
#include "bitstream.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// necessary I/O for a Windows build
#ifdef _WIN32
 /* prototypes for Inp32out32.dll calls */
 short _stdcall Inp32(short PortAddress);
 void _stdcall Out32(short PortAddress, short data);
#endif

// necessary I/O for a Linux build
#ifdef __linux__
 #include <sys/io.h>
 #include <sys/types.h>
 #include <unistd.h>
 #define Inp32 inb
#endif

 
char * createTag(char *n, char * v) {
	char * temp = new char[80];
	sprintf(temp,"<%s>%s</%s>", n, v, n);
       	return temp;
}	
 
char * createTag(char *n, bool b) {
	char * temp = new char[80];
	sprintf(temp,"<%s>%s</%s>", n, (b) ? "true" : "false" , n);
       	return temp;
}	
 
char * createTag(char *n, int i) {
	char * temp = new char[80];
	sprintf(temp,"<%s>%d</%s>", n, i, n);
       	return temp;
}
 
Reader::Reader()
{
	name = NULL;
	interface = 0;
	init = false;
	readableTracks.clear();	
	verbose = true;
}

char * Reader::getName() const {
	return (name != NULL) ? name : (char *) "Undefined";
}

void Reader::setName(char * s) {
	if(name != NULL)
		delete [] name;
	name = new char[strlen(s)+1];
	strcpy(name,s);
}

void Reader::setVerbose(bool b) {
	verbose = b;
}


bool Reader::canReadTrack(const int &t) const {
	if(!readableTracks.empty()) {
		for(unsigned int i=0; i < readableTracks.size(); i++) {
			if(readableTracks.at(i) == t)
				return true;
		}
	}
	return false;
}

void Reader::setCanReadTrack(const int & t) {
	if(!canReadTrack(t)) {
		readableTracks.push_back(t);
	}
}

//-------------------------------------------------------------- DirectReader

DirectReader::DirectReader() : Reader() {
	
	port = 0x379;
	setName("Parallel Based Track 2 Reader");
	usesCP = false;
	
	CLK1 = CLK2 = CLK3 = 0;
	DATA1 = DATA2 = DATA3 = 0;
}

DirectReader::DirectReader(int p, int cp, int c1, int d1,
		           int c2, int d2, int c3, int d3) : Reader() {
	port = p;
	if(cp > 0) {
		usesCP = true;
		CP = cp;
	} else {
		usesCP = false;
		CP = 0;
	}

	if(c1 == 0 || d1 == 0) {
		CLK1 = 0;
		DATA1 = 0;
	} else {
		setCanReadTrack(1);
		CLK1 = c1;
		DATA1 = d1;
	}

	if(c2 == 0 || d2 == 0) {
		CLK2 = 0;
		DATA2 = 0;
	} else {
		setCanReadTrack(2);
		CLK2 = c2;
		DATA2 = d2;
	}
	if(c3 == 0 || d3 == 0) {
		CLK3 = 0;
		DATA3 = 0;
	} else {
		setCanReadTrack(3);
		CLK3 = c3;
		DATA3 = d3;
	}
}

bool DirectReader::writeXML(char *fn) const {
	//write this object to a file as XML
	FILE * fout;
	if( (fout = fopen(fn, "w")) == NULL) {
		printf("Error opening XML file to write\n");
		return false;
	}
	//frist write XML magic
	fprintf(fout,"<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n");
	//Reader tag
	fprintf(fout,"<DirectReader>\n");
	//characteristics
	fprintf(fout,"\t%s\n", createTag("name",getName()));
	fprintf(fout,"\t%s\n", createTag("port",port));
	fprintf(fout,"\t%s\n", createTag("track1",canReadTrack(1)));
	if(canReadTrack(1)) {
		fprintf(fout,"\t%s\n", createTag("CLK1",CLK1));
		fprintf(fout,"\t%s\n", createTag("DATA1",DATA1));
	
	}
	fprintf(fout,"\t%s\n", createTag("track2",canReadTrack(2)));
	if(canReadTrack(2)) {
		fprintf(fout,"\t%s\n", createTag("CLK2",CLK2));
		fprintf(fout,"\t%s\n", createTag("DATA2",DATA2));
	
	}
	fprintf(fout,"\t%s\n", createTag("track3",canReadTrack(3)));
	if(canReadTrack(3)) {
		fprintf(fout,"\t%s\n", createTag("CLK3",CLK3));
		fprintf(fout,"\t%s\n", createTag("DATA3",DATA3));
	
	}
	fprintf(fout,"\t%s\n", createTag("card present",usesCP));
	if(usesCP) {
		fprintf(fout,"\t%s\n", createTag("CP",CP));
	}

	//CLOSING TAG
	fprintf(fout,"</DirectReader>\n");

	fclose(fout);
	
	return true;
}

void DirectReader::readRaw() const {

	//Are we using a platform that supports Direct I/O?
	#if defined(_WIN32) || defined(__linux__)
	if(!init) {
		printf("Error! Hardware has not been initialized\n");
		exit(1);
	}

	int e=0;

	if(usesCP) {
		//wait for a card swipe!
		while( (Inp32(port) & CP) != 0) {}
		//Card Detected!
		while( (Inp32(port) & CP) != CP) {
			//trap the clock line
			do {
				e=Inp32(port);
			} while( (e & CLK2) !=0);
			
			if( (e & DATA2) ==0)
				printf("1");
			else
				printf("0");
			fflush(stdout);

			do {
				e=Inp32(port);
			} while( (e & CLK2) != CLK2);
			//done trapping the clock line
		}
	} else {
		while(1) {
			do {
				e=Inp32(port);
			}while( (e & CLK2) !=0);
			if( (e & DATA2) ==0)
				printf("1");
			else
				printf("0");
			fflush(stdout);
			do
			{
				e=Inp32(port);
			}while( (e & CLK2) != CLK2);
		}
	} //end if usesCP
	#else
		printf("Program not compiled to support this hardware\n");
		exit(1);
	#endif
}

Card DirectReader::read() const
{
	//Are we using a platform that supports Direct I/O?
	#if defined(_WIN32) || defined(__linux__)
	if(!init) {
		printf("Error! Hardware has not been initialized\n");
		exit(1);
	}
	
	Bytef * tempBits = new Bytef[1024]; //keep this abstract
	int e;
	int size;
	memset(tempBits,0,700);
		
	printf("Waiting for Card\n");
	if(usesCP) {
		printf("Using CP!\n");
		size = 0;
		//wait for a card swipe!
		while( (Inp32(port) & CP) != 0) {}
		while( (Inp32(port) & CP) != CP) {}
		e=Inp32(port);
		//Card Detected!
		while( (e & CP) != 0) {
			//trap the clock line
			do {
				e=Inp32(port);
			} while( (e & CLK2) !=0);
			//store the value
			tempBits[size]=e;
			size++;
			if(size==700) {
				break;
			}
			do {
				e=Inp32(port);
			} while( (e & CLK2) != CLK2);
			//done trapping the clock line
		}
	} else {
		for(size = 0; size < 240; size++) {
			//trap the clock line
			do {
				e=Inp32(port);
			} while( (e & CLK2) !=0);
			//store the value
			tempBits[size]=e;
			do {
				e=Inp32(port);
			} while( (e & CLK2) != CLK2);
			//done trapping the clock line
		}
	}
	printf("Creating Bitstream...\n");
	//strip it to a binary
	for(int k = 0; k < size ;k++) {
		tempBits[k] = (tempBits[k] & DATA2);
		if(tempBits[k]==0) {
			tempBits[k]=1;
		} else {
			tempBits[k]=0;
		}
	}
	//create the Track
	Track t2(tempBits, size, 2);
	Card theCard;
	theCard.addTrack(t2);
	printf("retuning the card\n");
	return theCard;
	
	#else
		printf("Not compiled with hardware Interface Support!\n");
		exit(1);
	#endif

}


bool DirectReader::initReader() {
	#ifdef __linux__
	//If binary is set to +s and owner is root then become root
	//else silently fail
	//For use of ss as normal user
	seteuid(0);
	
	// Notify Linux that we want to have unfettered I/O port access
	if(iopl(3)==-1) {
		printf("Must be root to access I/O ports\n");
		return false;
	}
	#endif
	
	if(verbose) {
		printf("Reader Hardware: Using port 0x%x\n",port);
	}
	init = true; //hardware successfully initialized!
	return true;

}

//-------------------------------------------------------------- Serial Reader

SerialReader::SerialReader() : Reader() {
	setName("Serial Port Reader");
}

SerialReader::SerialReader(char *s) : Reader() {
	
	setName("Serial Port Reader");
	device = new char[strlen(s)+1];
	flagCR=false;
	memset(device,0,strlen(s)+1);
	strcpy(device,s);
}


bool SerialReader::writeXML(char *fn) const {
	//write this object to a file as XML
	FILE * fout;
	if( (fout = fopen(fn, "w")) == NULL) {
		printf("Error opening XML file to write\n");
		return false;
	}
	//frist write XML magic
	fprintf(fout,"<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n");
	//Reader tag
	fprintf(fout,"<SerialReader>\n");
	//characteristics
	fprintf(fout,"\t%s\n", createTag("name",getName()));
	fprintf(fout,"\t%s\n", createTag("device",device));
	fprintf(fout,"\t%s\n", createTag("uses-CR", flagCR));
	fprintf(fout,"\t%s\n", createTag("reads-track1",canReadTrack(1)));
	fprintf(fout,"\t%s\n", createTag("reads-track2",canReadTrack(2)));
	fprintf(fout,"\t%s\n", createTag("reads-track3",canReadTrack(3)));

		

	//CLOSEING TAG
	fprintf(fout,"</SerialReader>\n");

	fclose(fout);
	
	return true;
}

void SerialReader::readRaw() const {

	printf("Serial Based Readers do not support raw mode\n");
	exit(1);
}

Card SerialReader::read() const
{
	char buffer[1024];
	memset(buffer,0,1024);

	Card theCard;
	
	char * track1 = NULL;
	char * track2 = NULL;
	char * track3 = NULL;
	
	if(verbose) printf("Reading from %s\n",device);

	int count=0;
	char in;
	int i=0;
	memset(buffer,0,1024);
	do {
	        //Do the input end with a CR?
	   	if(!flagCR) {
	            //Damn! need to use "intuition to find tracks
		    do {
			    int f = fread(&in,1,1,fin);
			
                            if(i > 1023) {
                                buffer[1023]='\0';
                                printf("Input overflow! Dumping old buffer:\n");
                                printf("\"%s\"\n",buffer);
                                exit(1);
                            } else {
                                buffer[i++]=in;
                                if(in == '?') {
                                    count++;
				}
                            }
                    } while (count !=3);
		} else {
		    //sweet. just fgets it
                    fgets(buffer,1024,fin);
                }
		    
		buffer[1023]='\0';
		removeN(buffer);
	} while(!validInput(buffer));
	//Track 1
	if( (track1 = parseTrack(buffer, '%', '?')) == NULL) {
		if(canReadTrack(1)) {
			theCard.addMissingTrack(1);
		}
	} else if(canReadTrack(1)) {
		//catch serial readers reporting Empty Tracks
		if(strcmp(track1,"\%E?") ==0 || strcmp(track1,"%N?") == 0 ) {
			theCard.addMissingTrack(1);
		} else {
			Track tmp(track1, 1);
			theCard.addTrack(tmp);
		}	
	}
	//Track 2
	if( (track2 = parseTrack(buffer, ';', '?')) == NULL) {
		if(canReadTrack(2)) {
			theCard.addMissingTrack(2);
		}
	} else if(canReadTrack(2)) {
		//catch serial readers reporting Empty Tracks
		if(strcmp(track2,";E?") ==0 || strcmp(track2,";N?") == 0 ) {
			theCard.addMissingTrack(1);
		} else {
			Track tmp(track2, 2);
			theCard.addTrack(tmp);
		}	
	}

	//Track 3
	if( (track3 = parseTrack(buffer, '+', '?')) == NULL) {
		if(canReadTrack(3)) {
			theCard.addMissingTrack(3);
		}
	} else if(canReadTrack(3)) {
		//catch serial readers reporting Empty Tracks
		if(strcmp(track3,"+E?") ==0 || strcmp(track3,"+N?") == 0 ) {
			theCard.addMissingTrack(3);
		} else {
			Track tmp(track3, 3);
			theCard.addTrack(tmp);
		}	
	}
	return theCard;
}

void SerialReader::setCRFlag(bool b) {
	flagCR = b;
}

bool SerialReader::validInput(const char * s) const {
	int len = strlen(s);
	int i;
	char validStartChars[3] = { '%', ';', '+' };
	char validEndChars[1] = { '?' };
	if(len < 3) {
		//printf("Length is too short\n");
		return false;
	}
	//valid start character
	bool flag = false;
	for(int i = 0 ; i < strlen(s); i++) {
		for(int j = 0 ; i < 3; i++) {
			if( s[0] == validStartChars[i])
				flag = true;
		}
		if(flag) break;
	}
	if(i == strlen(s)) {
		//printf("didn't find start character\n");
		return false;
	}
	//valid end character
	for(i = 0 ; i < 1; i++) {
		if( s[len-1] == validEndChars[i])
			break;
	}
	if(i==1) {
		//printf("didn't find end character\n");
		return false;

	}
	return true;
}

char * SerialReader::parseTrack(const char * s, const char start, const char end) const {
	//printf("Searching \"%s\" for \'%c\' and \'%c\'\n",s,start,end);
	int len = strlen(s);
	int i, k;
	for(i =0; i < len; i++) {
		if(s[i]==start)
			break;
	}
	if(i==len) {
		//printf("didn't find %c\n",start);
		return NULL;
	}
	for(k= i + 1; k < len; k++) {
		if(s[k]==end)
			break;
	}
	if(k==len) {
		//printf("didn't find end %c\n",end);
		return NULL;
	}
	
	int size = k - i +2; //includes room for '\0'
	char * buffer = new char[size];
	memset(buffer,0,size);
	const char * tmp = &s[i];
	for(int j=0; j < size-1; j++)
		buffer[j]=*tmp++;

	//printf("returning \"%s\"\n",buffer);
	
	return buffer;
}


void SerialReader::setDevice(char * s) {
	if(device != NULL)
		delete [] device;
	device = new char[strlen(s)+1];
	strcpy(device,s);
}


bool SerialReader::initReader() {
	#ifdef __linux__
	//make sure echo and icanon options are set for the
	//serial port. Needed for ACR33B and other readers
	char buffer[64];
	
	//Shitty Code! Buffer overflow breeding ground right here. Fix me later
	sprintf(buffer, "stty -F %s -echo -icanon", device);
	system(buffer);
	//printf("Opening	\"%s\"\n",device);
	if( (fin = fopen(device, "r")) == NULL)
		return false;
	
	init = true;
	#endif
	#ifdef _WIN32
		printf("Windows Serial support not yet implemented!\n");
		printf("Besides, you shold be using Linux instead!\n");
	#endif	
	
	return true;
}

