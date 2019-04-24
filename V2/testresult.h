/*
 * Class TestResult
 *
 */

#ifndef TESTRESULT_H
#define TESTRESULT_H
#include "card.h"

/*
 * class TestResult
 *
 * This class stores a any information found in the card database
 * about a type of card, (ie account number, expire, etc)
 * This ios the class returned by the db after a search
 * if a match was found, numFields >=1 and the members will
 * be populated
 */

typedef std::vector<char *> stringVec;
typedef std::vector<int> intVec;

class TestResult {
public:
	TestResult();         // Default constructor
	void setCardType(char * s);
	void setNotes(char *s);
	void setUnknowns(char *s);

	void addTag(char * s, char * t);
	void addExtraTag(char *s);

	char * getNameTag(int i);
	char * getDataTag(int i);
	char * getExtraTag(int i);

	char * getCardType(void);
	char * getNotes(void) const;
	char * getUnknowns(void) const;	
	int getNumTags(void) const;
	int getNumExtraTags(void) const;
	
	bool isValid(void) const;

private:
	bool tagExists(char * n) const;
	
private:
	char * cardType;

	stringVec nameTags;
	stringVec dataTags;
	stringVec extraTags;
	intVec freq;
	
	char * notes;
	char * unknowns;	
	bool valid;

};
#endif
