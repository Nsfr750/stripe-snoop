/**
 * @file main.cpp
 * @brief Stripe Snoop main executable.
 * 
 * Stripe Snoop 2.0 http://stripesnoop.sourceforge.net
 *
 * Stripe Snoop is a suite of research tools that captures, modifies, 
 * validates, generates, analyzes, and shares data from magstripe cards. The
 * data is captured through different hardware interfaces (or stdin), the
 * contents decoded into the correct character set, and then a CDDB-like
 * database attempts to figure out what the contents mean.
 *
 * Originally a proof of concept for an interfacing project, and then a spin
 * off from a research project, Stripe Snoop has matured in the definitive
 * software for accessing and understanding magstripes.
 *
 * Strip Snoop is released under the GNU General Public License
 *
 * To use a game port based hardware reader, only Windows
 * 95, 98, 98se, ME, NT4, 2K, XP, and Linux on the x86
 * platform are supported. If you are compiling on one of
 * these platforms, that is detected and the proper include
 * files are used. If compiling under Windows, make sure
 * your compiler defines _WIN32 (Visual C++ does). If
 * compiling under Linux, make sure your compiler defines
 * __linux__ (gcc does)
 *
 * If you use an option to use the gameport based reader
 * under Linux (ie "./ss" or "./ss -r"), you must be root.
 *
 * Use of a keyboard based reader doesn't require anything
 * special.
 *
 * @author Acidus (acidus@msblabs.org)
 *
 * This file is part of Stripe Snoop (http://stripesnoop.sourceforge.net)
 *
 * Stripe Snoop is licensed under the GPL. See COPYING for more info
 *
 * Copyright (C) 2005 Acidus, Most Significant Bit Labs
 */

#include <stdio.h>
#include <stdlib.h>

#ifdef __linux__
	#include <sys/types.h>
	#include <unistd.h>
#endif

#include "getopt.h"
#include "loader.h"
#include "reader.h"
#include "ssflags.h"
#include "card.h"
#include "database.h"
#include "misc.h"

//#include "parser.h"
//#include "database.h"

SSFlags ssFlags;

/*----------------------------------------------------------------------MAIN*/
int main(int argc, char* argv[])
{
	Card swipedCard;	
	int c;
//=====================================parse the command line
	
	while ((c = getopt (argc, argv, "vlc:i:")) != -1) {
        switch (c) {
            case 'v':
                ssFlags.VERBOSE = true;
                break;
            case 'l':
                ssFlags.LOOP = true;
                break;
            case 'c':
                ssFlags.CONFIG = true;
		ssFlags.setConfigFile(optarg);
                break;
            case 'i':
		ssFlags.INPUT = true;
                ssFlags.setFileInput(optarg);
                break;
            default:
                break;
        }
    }

	if(!ssFlags.RAW) {
		printf("Stripe Snoop Version 2.0\n");
		printf("http://stripesnoop.sourceforge.net  Acidus@msblabs.net\n\n");
	}


	//-----------------------------------Read
        Reader * myReader = NULL;
	if(!ssFlags.CONFIG) {
	    //default location
            myReader = loadConfig("config.xml");
	} else {
	    //userdefined Config file
	   
            myReader = loadConfig(ssFlags.config);
	}
           
	myReader->initReader();
	if(ssFlags.RAW) {
		myReader->readRaw();
		exit(1);
	}
	swipedCard = myReader->read();
	
	//----------------------- decode
	swipedCard.decodeTracks();
	swipedCard.printTracks();

	//----------------------Database
	/*SSDatabase theDB;


	printf("\n");
	TestResult result = theDB.runTests(swipedCard);
	if(result.isValid()) {
		//found a match
		char * foo = result.getCardType();
		if(isvowel(*foo))
			printf("Found an %s\n\n", foo);
		else
			printf("Found a %s\n\n", foo);

		int c=0;
		for(int i=0; i<result.getNumTags(); i++)
			if(strlen(result.getNameTag(i))>(unsigned)c)
					c=strlen(result.getNameTag(i));
		c = ((int) c+1) / 8;
		for(int i=0; i<result.getNumTags(); i++) {
			printf("%s:",result.getNameTag(i));
			for(int j = (int) (strlen(result.getNameTag(i))+1) /8; j<=c;j++)
				printf("\t");
			printf("%s\n",result.getDataTag(i));
		}
		if( (c = result.getNumExtraTags()) > 0) {
			printf("Other tracks on the card contain ");
			if(c > 1) {
				for(int i = 0; i < c - 1; i++)
					printf("%s, ", result.getExtraTag(i));
				printf("and ");
			}
			printf("%s.\n", result.getExtraTag(c-1));
		}
		foo = result.getUnknowns();
		if(foo != NULL) {
			printf("Data Possibly encoded: %s\n",foo);
		}
		foo = result.getNotes();
		if(foo != NULL) {
			printf("Notes: %s\n", foo);
		} 	

		
	} else {
		printf("No match in database\n");
	}
*/
	return 0;
	
}
