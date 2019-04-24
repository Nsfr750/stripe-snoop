#ifndef READER_H
#define READER_H
/*
 * class Reader
 *
 * This class stores a decoded BCD track, and the different fields
 * its divided into
 */

#include "card.h"

typedef std::vector<int>  intVec;

//abstarct!
class Reader {

public:
	Reader();
	char * getName() const;
	void setName(char *);
	
	void setVerbose(bool);
	
	bool canReadTrack(const int &) const;
	void setCanReadTrack(const int&);
	
	//-----------funcs
	virtual void readRaw() const =0; //read in raw mode from the interface
        virtual bool initReader() = 0;//init hardware
	virtual Card read() const =0; //read from the hardware interface!	
	virtual bool writeXML(char *) const =0; //write this object as XML from disk;
protected:
	char * name;
	int interface;
	intVec readableTracks;
	
	//internal variables
	bool init;	//used to track if hardware is inited
	bool verbose;	//am I being verbose?

};


// used by parallel port and gameport based readers
class DirectReader : public Reader{

public:

	DirectReader();	
	DirectReader(int, int, int, int, int, int, int, int);
	virtual void readRaw() const; //read in raw mode from the interface
        virtual bool initReader();
	virtual Card read() const;	//read from the hardware interface!	
	virtual bool writeXML(char *) const;
	
protected:

	int port;
	bool usesCP;
	
	int CP;
	int CLK1;
	int DATA1;
	int CLK2;
	int DATA2;
	int CLK3;
	int DATA3;	

};

//-----------------------------------------------------------------Serial Reader

class SerialReader : public Reader{

public:

	SerialReader();
	SerialReader(char *);	
	void setDevice(char *);
	void setCRFlag(bool);
	virtual void readRaw() const; //read in raw mode from the interface
        virtual bool initReader();
	virtual Card read() const;	//read from the hardware interface!	
	virtual bool writeXML(char *) const;
	
protected:

	char * device;
	FILE * fin;
	bool SerialReader::validInput(const char *) const;
	char * parseTrack(const char *, const char, const char) const;
        bool flagCR;
};


#endif
