/**
 * @file loader.cpp
 * @brief handles loading and configuring a hardware interface.
 * 
 * Reads "config.xml" (or specied config file) and creates a Reader object
 * representing the defined readers capabilities
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
#include "loader.h"
#include "sxmlp.h"
#include "misc.h"

SXMLP xml;

Reader * loadConfig(char * fn) {
	if (!xml.loadFile(fn)) {
		printf("Could not open/parse config file \"%s\"\n", fn);
		exit(1);
	}
	//what kind of reader do we have?
	char * name = xml.getRootName();
	//other reader types will go in this if-then-else tree
	//when they are supported
	
	if(strcmp(name,"DirectReader") == 0) {
		return loadDirectReader();
	}
	if(strcmp(name,"SerialReader") == 0) {
		return loadSerialReader();
	}

	printf("Vizzini: \"INCONCEIVABLE!\"\n");
	fflush(stdout);
	return NULL;
}

Reader * loadDirectReader() {
	//printf("In load direct\n");
	char * nextTag;
	
	int port, cp, c1, c2, c3, d1, d2, d3;
	
	port = cp = 0;
	c1 = c2 = c3 = 0;
	d1 = d2 = d3 = 0;
		
	
	while( (nextTag = xml.nextName()) != NULL) {
		//printf("*");
		if(strcmp(nextTag, "port") == 0) {
			port = atoi(xml.nextValue());
		} else if(strcmp(nextTag, "CP") == 0) {
			cp = atoi(xml.nextValue());
		} else 	if(strcmp(nextTag, "CLK1") == 0) {
			c1 = atoi(xml.nextValue());
		} else 	if(strcmp(nextTag, "DATA1") == 0) {
			d1 = atoi(xml.nextValue());
		} else 	if(strcmp(nextTag, "CLK2") == 0) {
			c2 = atoi(xml.nextValue());
		} else 	if(strcmp(nextTag, "DATA2") == 0) {
			d2 = atoi(xml.nextValue());
		} else 	if(strcmp(nextTag, "CLK3") == 0) {
			c3 = atoi(xml.nextValue());
		} else 	if(strcmp(nextTag, "DATA3") == 0) {
			d3 = atoi(xml.nextValue());
		} else {
			//burn it, so we stay in sync
			xml.nextValue();
		}
	}
	//printf("Attempting to construct\n");
	
	Reader * myReader= new DirectReader(port, cp, c1, d1, c2, d2, c3, d3);

	return myReader;
}

Reader * loadSerialReader() {
	char * nextTag;
	

	SerialReader * myReader= new SerialReader();

	
	while( (nextTag = xml.nextName()) != NULL) {
		if(strcmp(nextTag, "device") == 0) {
			myReader->setDevice(xml.nextValue());
		} else if(strcmp(nextTag, "uses-CR") == 0) {
			if(atob(xml.nextValue())) {
				myReader->setCRFlag(true);
			}
		} else if(strcmp(nextTag, "reads-track1") == 0) {
			if(atob(xml.nextValue())) {
				myReader->setCanReadTrack(1);
			}
		} else if(strcmp(nextTag, "reads-track2") == 0) {
			if(atob(xml.nextValue())) {
				myReader->setCanReadTrack(2);
			}
		} else if(strcmp(nextTag, "reads-track3") == 0) {
			if(atob(xml.nextValue())) {
				myReader->setCanReadTrack(3);
			}
		} else {
			//burn it, so we stay in sync
			xml.nextValue();
		}
	}

	return (Reader *) myReader;
}
