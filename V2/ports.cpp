/*
 * Ports - Developer tool for troubleshooting Direct Readers
 *
 * Shows you the number of times a data bit changes (The deltas)
 * Very useful!
 *
 * If you build this for Windows, be use you have included inpout32.lib
 * and have inpout32.dll in your Windows' system directory
 *
 * Ports is licensed under the GPL. See COPYING for more information
 * 
 * Copyright (c) 2005, Acidus, Most Significant Bit Labs
 */

//Sorry port is hardcoded in. This is for developers! It will not be
//pretty unless some people want to help contribute code.
#define PORT (0x379)

/* Standard Includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// necessary I/O for a Linux build
#ifdef __linux__
 #include <sys/io.h>
 #include <sys/types.h>
 #include <unistd.h>
 #define Inp32 inb
 #define Out32 outb
#endif

// necessary I/O for a Windows build
#ifdef _WIN32
 /* prototypes for Inp32out32.dll calls */
 short _stdcall Inp32(short PortAddress);
 void _stdcall Out32(short PortAddress, short data);
#endif

int main(int argc, char * argv[]) {

	bool usePower = false;
	
	printf("ports - Stripe Snoop interface developer tool\n");
	printf("(C) 2005 Acidus, Most Significant Bit Labs\n");

	if(argc==2) {
		if(strcmp(argv[1], "-?") ==0) {
			printf("ports [--power]\n");
			printf("\t--power - Use D0 (pin 2 of parallel port) to supply 5V power\n");
			exit(1);
		}
		if(strcmp(argv[1], "--power") ==0) {
			usePower = true;
		}
	}
	
#ifdef __linux__
	//For use of ss as normal user
	seteuid(0);
	// Notify Linux that we want to have unfettered I/O port access
	if(iopl(3)==-1) {
		printf("Must be root to access I/O ports\n");
		exit(1);
	}
#endif	
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
	//make sure there is no power on the data pins, just in case
	Out32(0, PORT - 1);
	//power up D0 if needed
	if(usePower) {
		printf("Powering 5V rail...\n");
		Out32(255, PORT);
	}

	printf("\nReading on Port 0x%x\n", PORT);
	
	time(&t1);
	printf("Please swipe card within 5 seconds\n");
	do
	{
		k=Inp32(PORT);
		for(i=0;i<5;i++)
			//only store changes!
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
	return 1;
	
}

