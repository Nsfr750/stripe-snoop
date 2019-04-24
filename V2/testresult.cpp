/**
 * @file testresult.cpp
 * @brief Report containing details of card matched in database.
 * 
 * Contains data types and field names that have been parsed out
 * of the decoded characters of Track objects of a Card. This is formed
 * instead a CardTest object in the Database where this card has 
 * matched a "fingerprint" Stripe Snoop recognizes
 *
 * @author Acidus (acidus@msblabs.org)
 *
 * This file is part of Stripe Snoop (http://stripesnoop.sourceforge.net)
 *
 * Stripe Snoop is licensed under the GPL. See COPYING for more info
 *
 * Copyright (C) 2005 Acidus, Most Significant Bit Labs
 */

#include "testresult.h"
#include <string.h>

TestResult::TestResult()
{
	cardType = NULL;
	notes = NULL;
	valid = false;
	unknowns = NULL;
}

bool TestResult::isValid() const {
	return valid;
}

char * TestResult::getCardType() {
	if(cardType==NULL) {
		cardType = new char[10];
		strcpy(cardType, "Undefined");
	}
	return cardType;
}

char * TestResult::getNotes() const {
	return notes;
}
char * TestResult::getUnknowns() const {
	return unknowns;
}

void TestResult::addTag(char *s, char *t)
{
	//we must make sure not to add double entries
	if(!tagExists(s)) {
		char * a = new char[strlen(s)+1];
		char * b = new char[strlen(t)+1];
		strcpy(a,s);
		strcpy(b,t);
		nameTags.push_back(a);
		dataTags.push_back(b);
		valid = true;
	}
}

void TestResult::addExtraTag(char *s) {
	//do we already have it?
	for(unsigned int i = 0; i < extraTags.size(); i++) {
		if(strcmp(s, extraTags.at(i)) == 0) {
			//incremenet freq
			int &t = freq.at(i);
			t++;
			return;
		}
	}
	//add it
	char * a = new char[strlen(s)+1];
	strcpy(a,s);
	extraTags.push_back(a);
	freq.push_back(1);
}

int TestResult::getNumTags(void) const {
	return nameTags.size();
}

int TestResult::getNumExtraTags(void) const {
	return extraTags.size();
}

void TestResult::setCardType(char *s)
{
	if(cardType != NULL)
		delete cardType;
	cardType = new char[strlen(s)+1];
	strcpy(cardType,s);
	valid = true;
}

char * TestResult::getNameTag(int i)
{
	if((unsigned) i>= nameTags.size() || i<0)
		return "ERROR!";
	else
		return nameTags.at(i);
}

char * TestResult::getDataTag(int i)
{
	if((unsigned) i>= dataTags.size() || i<0)
		return "ERROR!";
	else
		return dataTags.at(i);
}

char * TestResult::getExtraTag(int i)
{
	if((unsigned) i>= extraTags.size() || i<0)
		return "ERROR!";
	else {
		char * txt = extraTags.at(i);
		bool flag = (freq.at(i) > 1) ? true: false;
		int len = strlen(txt) + ((flag) ? 5: 1);
		char * tmp = new char[len];
		memset(tmp, 0, len);
		if(flag)
			sprintf(tmp, "%s (%d)", txt, freq.at(i));
		else
			strcpy(tmp, txt);
		return tmp;
	}
}


void TestResult::setNotes(char *s)
{
	if(notes != NULL)
		delete notes;
	notes = new char[strlen(s)+1];
	strcpy(notes,s);
	valid = true;
}

void TestResult::setUnknowns(char *s)
{
	if(unknowns != NULL)
		delete unknowns;
	unknowns = new char[strlen(s)+1];
	strcpy(unknowns,s);
	valid = true;
}

bool TestResult::tagExists(char * s) const {
	for(unsigned int i = 0; i < nameTags.size(); i++) {
		if(strcmp(s, nameTags.at(i)) == 0) {
			return true;
		}
	}
	return false;
}
		
			
