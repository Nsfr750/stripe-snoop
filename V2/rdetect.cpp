/**
 * @file rdetect.cpp
 * @brief Stand-alone tool to detect magstripe hardware.
 * 
 * rdetect prompted user for questions and autoprobe the physical wiring of
 * a magstripe device to create the hardware configuration file (config.xml)
 * that Stripe Snoop needs.
 *
 * @author Acidus (acidus@msblabs.org)
 *
 * This file is part of Stripe Snoop (http://stripesnoop.sourceforge.net)
 *
 * Stripe Snoop is licensed under the GPL. See COPYING for more info
 *
 * Copyright (C) 2005 Acidus, Most Significant Bit Labs
 */

/* Standard Includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "rdetect.h"
#include "reader.h"
#include "loader.h"
#include "misc.h"
#include "ssflags.h"

/** commandline options, needed by Reader instances*/
SSFlags ssFlags;

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

int port;

Reader * queryReader(int i) {
	switch (i) {
		case 1:
			return queryGameReader();
		case 2:
			return queryParallelReader();
		case 3:
			return querySerialReader();
		default:
			printf("Vizzini: \"INCONCEIVABLE!\"\n");
	}
	return NULL;
}
/**
 * queries GameReader on global port variable
 * @returns ptr to Reader for this magstripe reader
 */
Reader * queryGameReader() {
	time_t t1,t2;
	int i, k;
	/* index 0 = Bit 8 = 
	         1 = Bit 7 =
		 2 = Bit 6 =
		 3 = Bit 5 =
	*/
	long fluxes[5];
	int lastValue[5];
	int ands[5] = {128, 64, 32, 16, 8};
		
	for (i=0; i<5; i++) {
		fluxes[i]=0;
		lastValue[i]=0;
	}
	
	int port = promptForPort(1);
	setupDirectIO();
	
	time(&t1);
	printf("Please swipe card within 5 seconds\n");
	do
	{
		k=Inp32(port);
		for(i=0;i<5;i++)
			if( (k & ands[i]) != lastValue[i]) {
				fluxes[i]++;
				lastValue[i]= k & ands[i];
			}

		time(&t2);
	}while(t2-t1<5);

	printf("Processing...\nDeltas:");
	
	for(i=0;i<5;i++)
		printf("%d ", fluxes[i]);
	printf("\n");

	//bubble sort the fluxes and the bit masks for those bits"
	sortByValue(fluxes, ands);
	//We are assuming a Track 2 reader only! BAD!!!

	int cp =0;
	if (fluxes[2] == 2) {
		printf("Card Present line detected\n");
		cp = ands[2];
	} else {
		cp = 0;
	}

	DirectReader * theReader = new DirectReader(port, cp, 0, 0,
			                  ands[0], ands[1], 0, 0);
	return theReader;

}
/**
 * Currently unused
 */
Reader * queryParallelReader() {
	return NULL;
}

/**
 * Creates a Reader object for a Serial Reader (LINUX ONLY CURRENTLY)
 * @return ptr to Reader for magstripe reader
 */
Reader * querySerialReader() {
	#ifdef __linux__
	char buffer[64];
	int i;
	
	do {
		memset(buffer,0,64);
		printf("What Serial Port is the device attached to?\n");
		printf("1- COM1 (/dev/ttyS0)\n");
		printf("2- COM2 (/dev/ttyS1)\n");
		printf("3- Other\n");
		fgets(buffer,4,stdin);
		i = atoi(buffer);
	} while(i < 1 && i >3);
	switch(i) {
		case 1:
			strcpy(buffer,"/dev/ttyS0");
			break;
		case 2:
			strcpy(buffer,"/dev/ttyS1");
			break;
		case 3:
			printf("Please enter path and filename of serial port\n");
			fgets(buffer,64,stdin);
			buffer[63] = '\0'; //prevent overrun
			buffer[strlen(buffer)-1]='\0';
			break;
		default:
			printf("Vizzini: \"INCONCEIVABLE!\"\n");
	}
	
	SerialReader * theReader = new SerialReader(buffer);
	do {
		memset(buffer,0,64);
		printf("Does the Serial Reader terminate its output with a newline?\n");
		printf("1- YES\n");
		printf("2- NO\n");
		fgets(buffer,4,stdin);
		i = atoi(buffer);
	} while(i < 1 && i >2);
	if(i == 1) {
	    theReader->setCRFlag(true);
	} else {
            theReader->setCRFlag(false);
	}
	for(int k=1; k < 4; k++) {
		do {
			memset(buffer,0,64);
			printf("Does your Serial Reader Support Track %d\n", k);
			printf("1- YES\n");
			printf("2- NO\n");
			fgets(buffer,4,stdin);
			i = atoi(buffer);
		} while(i < 1 && i >2);
		if(i == 1) {
		    theReader->setCanReadTrack(k);
		}
	}
	
	return theReader;	

	#endif
	
	#ifdef _WIN32
	printf("Serial Readers not supported on Windows currently\n");
	return NULL;
	#endif
}
			
/**
 * calls necessary init functions for platform for direct i/o access
 */
void setupDirectIO()
{
	#ifdef __linux__
	//If binary is set to +s and owner is root then become root
	//else silently fail
	//For use of ss as normal user
	seteuid(0);

	// Notify Linux that we want to have unfettered I/O port access
	if(iopl(3)==-1) {
		printf("Must be root to access I/O ports\n");
		exit(1);
	}
	#endif
}
/**
 * sorts "deltas" in High -> low fashion
 * @param fluxes int[] containing # of deltas
 * @param mask int [] containing data bit number associated with given delta 
 */
void sortByValue(long * fluxes, int * mask)
{
	for(int i = 0; i < 5; i++) {
		int high = i;
		for(int j = i; j <5; j++) {
			if(fluxes[j] > fluxes[high])
			       high = j;
		}
		//swap them
		long temp = fluxes[high];
		int temp2 = mask[high];
		fluxes[high] = fluxes[i];
		mask[high] = mask[i];
		fluxes[i] = temp;
		mask[i] = temp2;
	}
}
/**
 * prompts user for hexidecimal port number
 * @param i switch for message type (1=Game, 2=parallel)
 */
int promptForPort(int i) {

	char * s = new char[9];
	int k;
	do {
		memset(s,0,9);
		printf("Enter port address (in hex) to use:\n");
		if(i==1) {
			printf("Game ports are traditionally on 0x201, but may be on\n");
			printf("a different port (SB Live cards under Linux are at 0xeff0)\n");
		} else
			printf("Parallel Port 1 is on 0x379\n");
		fgets(s,9,stdin);
		k = convertHex(s);
	} while(k < 1 && k > 0xFFFF);

	delete [] s;
	
	return k;
}

//-----------------------------------------------------------------MAIN
/**
 * Start of execution
 * @param argc number of command line params
 * @param argv char * [] of command line params
 */
int main(int argc, char* argv[])
{
	char s[4];
	int i;
	
	printf("Stripe Snoop - Magstripe Reader Detector\n");
	printf("Version 1.1\n\n");
	do {
		printf("Please select Interface:\n");
		printf("1- Game port\n");
		printf("2- Parallel port\n");
		printf("3- Serial port\n");
		fgets(s,4,stdin);
		i = atoi(s);
	} while(i < 1 && i >3);


	Reader * theReader = queryReader(i);
	if(theReader == NULL) {
		printf("theReader is NULL\n");
		exit(1);
	}
	theReader->writeXML("config.xml");

}

