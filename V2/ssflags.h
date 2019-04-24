/*
 *
 *
 */

#ifndef SSFLAGS_H
#define SSFLAGS_H
class SSFlags {
public:
	SSFlags(); //constructor
        void setFileInput(char *);
	void setConfigFile(char *);
public:
	bool VERBOSE; //verbose flag
	bool RAW;     //RAW Flag
	bool INPUT;   //Input Mode
	bool FORCE;
	bool CONFIG;
	bool LOOP;
        char * fileinput;
	char * config;
	
};

#endif
