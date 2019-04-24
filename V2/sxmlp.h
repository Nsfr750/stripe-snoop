
#ifndef SXMLP_H
#define SXMLP_H

#include <queue>

typedef std::queue<char *>  stringQ;


class SXMLP {
public:
	SXMLP();
	char * nextName(void);
	char * nextValue(void);
	bool loadFile(char *s);
	char * getRootName(void);
private:
	char * rootTagName;
	int num;
	stringQ elements;
	stringQ values;

	void setRootName(char *s);
	void addElement(char *s, char *t);
	char * trimSpaces(char *);
	int isElement(char *);
	char * elementName(char *);
	char * elementValue(char *);
		
};
/*
// default constructor
SXMLP::SXMLP();

//attempts to load a valid XML file into the parser
bool SXMLP::loadFile(char *filename);

//Returns the name of the next element
char * SXMLP::nextName();
//returns the value of the next element's tag
char * SXMLP::nextValue();


//-----------private functions

//set the name of the root tag
void SXMLP::setName(char * s);
//add a tag name and value to thte parser
void SXMLP::addElement(char *s, char *t);
//trims leading tabs and spaces
char * SXMLP::trimSpaces(char *s);
//what kind of element is this line? 
int SXMLP::isElement(char *s);
//extract element tag name from line
char * SXMLP::elementName(char *s);
//extract element's value from line
char * SXMLP::elementValue(char *s);
*/
#endif
	
