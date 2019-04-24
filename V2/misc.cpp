/**
 * @file misc.cpp
 * @brief Misc function used by various files.
 * 
 * Global file for misc function that really shouldn't go into any one class
 *
 * @author Acidus (acidus@msblabs.org)
 *
 * This file is part of Stripe Snoop (http://stripesnoop.sourceforge.net)
 *
 * Stripe Snoop is licensed under the GPL. See COPYING for more info
 *
 * Copyright (C) 2005 Acidus, Most Significant Bit Labs
 */

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "misc.h"
#include <math.h>
/**
 * converts a single hex character to an int
 *
 * @param c character to convert.
 * @return an int.
 */
int hex2int(char c) {
	if(isdigit(c))
		return (int) c - 48;
	if(isxdigit(c))
		return (int) (tolower(c) - 87);
	return 0;
}

int convertHex(char * s) {
	
	int k=0;	
	char * ptr = s;

	//eliminate leading 0x or 0X
	if(strncmp(ptr,"0x",2)==0 || strncmp(ptr,"0X",2)==0)
		ptr = ptr +2;
	//eliminate trailing \n	
	if(ptr[strlen(ptr)-1]=='\n')
		ptr[strlen(ptr)-1] = '\0';
	
	int numHexDigits = (int) strlen(ptr);
	for (int i = 0; i < numHexDigits; i++)
		k = ((int) pow((double) 16,(numHexDigits - 1 - i)))
			* hex2int(ptr[i]) +k; 
	
	if (k > 0 && k < 0xFFFF)
		return k;
	
	return -1;
}

void removeN(char * s) {
	if(s !=NULL) {
		int len = strlen(s);
		if(s[len-1] == '\n')
			s[len-1]='\0';
	}
}

bool atob(char *s) {
	char *t = s;
	while(*t != '\0')
		*t++ = toupper(*t);
	if(strcmp(s,"FALSE") == 0 )
		return false;
	return true;
}

bool isvowel(char s) {
	char vowels[] = {'A', 'E', 'I', 'O', 'U'};
	for(int i = 0; i <5; i++) {
		if( s == vowels[i] || s == vowels[i] + 32)
			return true;
	}
	return false;
}
