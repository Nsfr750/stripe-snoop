/**
 * @file ssflags.cpp
 * @brief Command line flags for Stripe Snoop
 * 
 * Class that holds all the command line flags for Stripe Snoop
 *
 * @author Acidus (acidus@msblabs.org)
 *
 * This file is part of Stripe Snoop (http://stripesnoop.sourceforge.net)
 *
 * Stripe Snoop is licensed under the GPL. See COPYING for more info
 *
 * Copyright (C) 2005 Acidus, Most Significant Bit Labs
 */

#include "ssflags.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

SSFlags::SSFlags()
{
	VERBOSE=false;
	RAW=false;
	INPUT=false;
	FORCE=false;
	CONFIG = false;
	LOOP=false;
	fileinput = NULL;
	config = NULL;
}

void SSFlags::setFileInput(char * s) {
    if(fileinput != NULL)
        delete [] fileinput;
    fileinput = new char [strlen(s) + 1];
    memset(fileinput,0,strlen(s) + 1);
    strcpy(fileinput,s);
}

void SSFlags::setConfigFile(char * s) {
    if(config != NULL)
        delete [] config;
    config = new char [strlen(s) + 1];
    memset(config,0,strlen(s) + 1);
    strcpy(config,s);
}



