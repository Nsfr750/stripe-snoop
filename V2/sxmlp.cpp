/**
 * @file sxmlp.cpp
 * @brief Extremely simple XML parser
 * 
 * XML parser to read config.xml, among other things. Didn't really need a full
 * XML parser like SAX or expat, so sxmlp was handwritten for Stripe Snoop.
 *
 * @author Acidus (acidus@msblabs.org)
 *
 * This file is part of Stripe Snoop (http://stripesnoop.sourceforge.net)
 *
 * Stripe Snoop is licensed under the GPL. See COPYING for more info
 *
 * Copyright (C) 2005 Acidus, Most Significant Bit Labs
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sxmlp.h"

SXMLP::SXMLP() {
	rootTagName = NULL;
	num = 0;
}

char * SXMLP::getRootName() {
	return rootTagName;
}

void SXMLP::setRootName(char * s) {
	if (rootTagName != NULL)
		delete [] rootTagName;
	rootTagName = new char[strlen(s)+1];
	strcpy(rootTagName, s);
}

void SXMLP::addElement(char *s, char *t) {
	//printf("adding element \"%s\" \"%s\" %d\n", s, t, num);
	char * a = new char[strlen(s)+1];
	char * b = new char[strlen(t)+1];
	strcpy(a,s);
	strcpy(b,t);
	elements.push(a);
	values.push(b);
	num++;
}

char * SXMLP::nextName() {
	if(num == 0){
		//printf("NAME IS NULL!\n");
		return NULL;
	} else {
		char * a = elements.front();
		char * b = new char[strlen(a)+1];
		//printf("name is \"%s\" %d\n", a, num);
		strcpy(b,a);
		elements.pop();
		return b;
	}
}


char * SXMLP::nextValue() {
	if(num == 0) {
		//printf("VALUE IS NULL!\n");
		return NULL;
		
	} else {
		char * a = values.front();
		char * b = new char[strlen(a)+1];
		//printf("value is \"%s\" %d\n", a, num);
		strcpy(b,a);
		values.pop();
		num--;  //only now can we take num down
		return b;
	}
}

char * SXMLP::trimSpaces(char *s) {
	int i = strlen(s)+1;
	char * temp = s; 
	bool flag = false; //have we hit characters yet?
	for(int j = 0; j < i; j++)
		if((s[j]>=32 && flag) || s[j]> 32) {
			*temp++ =s[j];
			flag = true;
		}
	*temp++ = '\0';
	return s;
}

int SXMLP::isElement(char *s) {
	int i = strlen(s);
	if(s[0] !='<' || s[i-1]!='>' || strncmp(s,"<!--",4) == 0)
	       return 0; //no element
	if(strchr(s,'/') != NULL) {
		//all on 1 line, or closing tag?
		if( strchr(&s[1], '<') == NULL)
			return 2; //closing element
		else
			return 3; //start and close on same line
	}
	return 1; //starting element
}	

char * SXMLP::elementName(char *s) {
	int i = strlen(s);
	char * t = new char[i];
	memset(t,0,i);
	for(int j = 1; j < i; j++) {
		if(s[j] == '>')
			break;
		t[j-1] = s[j];
	}
	return t;
}

char * SXMLP::elementValue(char *s) {
	int i = strlen(s);
	char * t = new char[i];
	char * a = t;
	int flag = 0;
	for(int j = 1; j < i; j++) {
		if(s[j] == '<')
			flag = 2;
		if(flag == 1)
			*a++ = s[j];
		if(flag == 2)
			*a++ = '\0';
		if(s[j] == '>')
			flag = 1;;
	}
	//printf("element value is \"%s\"\n", t);
	return t;
}

bool SXMLP::loadFile(char *filename) {
	
	char * buffer = new char[80];	
	FILE * fin;
	
	if ( (fin = fopen(filename,"r")) == NULL) {
		printf("File not found\n");
		return false;
	}

	//is this an xml file?
	if (fgets(buffer,80,fin) == NULL) {
		printf("Error reading file\n");
		return false;
	}
	if (strncmp(buffer,"<?xml",4) != 0) {
		printf("Not an XML file!\n");
		return false;
	}
	
	do {
		memset(buffer,0,80);
		fgets(buffer,80,fin);
		if (strlen(buffer) > 0) {
			buffer[strlen(buffer)-1] = '\0';
			buffer = trimSpaces(buffer);
			//do it

			int i = isElement(buffer);
			char * n = elementName(buffer);
			
			if(i == 1)
				setRootName(n);
			else if(i==3) {
				char * v = elementValue(buffer);
				addElement(n, v);
				delete [] v;
			}
			delete [] n;			
		}
		
	} while( !feof(fin));
	return true;
}
