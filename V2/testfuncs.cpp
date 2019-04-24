/**
 * @file testfuncs.cpp
 * @brief Various functions to test for a card fingerprint, formatters
 * 
 * Includes several tests to determine if something is valid (ie valid date,
 * valid MOD10 checksum, etc).
 * Also includes various function to extract and "beautify" or format output
 * used by TestResult.
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
#include <stdlib.h>
#include <ctype.h>
#include "testfuncs.h"
#include "ssflags.h"


extern SSFlags ssFlags;

// ================================================================= TESTS
/*
 * bool mod10check(char *card)
 *
 * Performs a Luhn (MOD10) check on the account number passed
 * as a string of characters and returns the result
 *
 * card - string of characters containing an account number
 * returns - Whether it passed MOD10 or not
 */

bool mod10check(char *card)
{
	unsigned char tmp, sum, i, j;
	sum = i = j = 0;
	//count the card digits
	while(card[i] >= '0' && card[i] <= '9')
		i++;
	while(i--) {
	    tmp = card[i] & 0x0F;
		if(j++ & 1) {
			if((tmp<<=1) > 9) {
				tmp -= 10;
				sum++;
			}
		}
		sum += tmp;
	}
	if(sum % 10 == 0)
		return true;
	return false;
}
/*
 * bool isMonth(char * d);
 *
 * Checks to see if character string contains a number representation
 * of a month or notaracters and returns the result
 *
 * d - string of 2 characters representing a month
 * returns - Whether it is a month
 */
bool isMonth(char * d) {
	char tmp[3] = {0,0,0};
	int i;
	//make sure we only have numbers
	for(i=0;i<2;i++)
		if(d[i]<48 || d[i]>57)
			return false;
	strncpy(tmp,d,2);
	i=atoi(tmp);
	if(i>=1 && i<=12) return true;
	return false;
}
/*
 * int lastDotm(int m, int y);
 *
 * Give a month and a year, returns the last day of the month (ie 30,
 * 31, 28, or 29);. Does calculate leapyear
 *
 * m - int of the month
 * y - 4 digit year
 * returns - the last day of the month
 */

int lastDotm(int m, int y) {
	int dotm[12] = { 31, 31, 99, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	bool leap=false;

	m--;//map 1-12 to 0-11
	//not feb
	if(m!=1)
		return dotm[m];
	//leap year test
	if(y % 4 ==0) {
		leap=true;
		if(y % 100 == 0) {
			leap = false;
			if(y % 400 ==0)
				leap = true;
		}
	}
	return (leap) ? 29: 28;
}
/*
 * bool dateValid(char *m, char *d, char * y);
 *
 * see if the strings representing the parts of a date
 * all form a valid date (ie not 31st of Feburary);
 *
 * m - string of 2 characters containing the month
 * d - string of 2 characters containing the month
 * y - string of 4 characters containing the month
 * returns - Whether it is a date or not
 */
bool dateValid(char *m, char *d, char * y) {

	char day[3] = {0,0,0};
	char month[3]={0,0,0};
	char year[5]={0,0,0,0,0};
	int i;

	if(!isMonth(m)) return false;
	for(i=0;i<2;i++)
		if(d[i]<48 || d[i]>57)
			return false;
	for(i=0;i<4;i++)
		if(y[i]<48 || y[i]>57)
			return false;
	strncpy(day,d,2);
	strncpy(month,m,2);
	strncpy(year,y,4);
	i=atoi(day);
	if(i<=0 || i>=lastDotm(atoi(month), atoi(year)))
		return false;

	return true;
}


bool isAlpha(const char * s)
{
	//ignore first char
	unsigned int  i;
	for(i=1;i<strlen(s);i++)
		if(s[i]<32 || s[i] >95) {
			if(ssFlags.VERBOSE)
				printf("Invalid Alpha character \'%c\' detected at %d\n",s[i],i);
			return false;
		}
	return true;
}

bool isBCD(const char * s)
{
	//ignore first char
	unsigned int  i;
	for(i=1;i<strlen(s);i++)
		if(s[i]<48 || s[i] >63) {
			if(ssFlags.VERBOSE)
				printf("Invalid BCD character \'%c\' detected at %d\n",s[i],i);
			return false;
		}
	return true;
}
// =============================================================== FORMATTERS
/*
 * int expandYear(int i);
 *
 * takes a 2 digit date (ie 87); and attempt to expand it to a 4 digit
 * date (1987);. This is a nasty hack, and could cause errors when
 * with older dates (ie 1910 instead of 2010);
 *
 * i - int of a 2 digit date
 * returns - a 4 digit date as CCYY
 */
int expandYear(int i) {
	//try and expand a 2 digit YY to a CCYY
	if(i>=35 && i<=99)
		i+=1900;
	else
		i+=2000;
	return i;
}


/*
 * char * monthName(int x);
 *
 * returns the name of a month, given its number
 *
 * x - number of month (1-12);
 * returns - string of the months name;
 */
char * monthName(int x) {

	switch(x)
	{
		case 1:
			return "January";
			break;
		case 2:
			return "February";
			break;
		case 3:
			return "March";
			break;
		case 4:
			return "April";
			break;
		case 5:
			return "May";
			break;
		case 6:
			return "June";
			break;
		case 7:
			return "July";
			break;
		case 8:
			return "August";
			break;
		case 9:
			return "September";
			break;
		case 10:
			return "October";
			break;
		case 11:
			return "November";
			break;
		case 12:
			return "December";
			break;
		default:
			return "ERROR!";
	}
}

/*
 * char * formatter(char * format, char * n);
 *
 * returns a string of numbers that are spaced/divided according to a
 * formatting string provided
 *
 * format - format String that shows how numbers are divided
            (ie "XXX-XX-XXXX" for an SSN);
 * returns - correctly formatted numeric string
 */
char * formatter(char * format, char * n) {
	unsigned int i=0;
	int k=0;
	char * foo = new char[strlen(format)+1];
	for(i=0;i<strlen(format);i++)
	{
		if(format[i]=='X') {
			foo[i]=n[k];
			k++;
		}
		else
			foo[i]=format[i];
	}
	return foo;
}
/* only recognize:
   YYMM
*/
char * extractDate(char * format, char * n) {
	
	char * date = NULL;
	
	if(strcmp(format,"YYMM")==0) {
	       char temp[64];
	       memset(temp,0,64);
	       strncpy(temp,n,2);
	       int year = atoi(temp);
	       year = expandYear(year);
	     
	       memset(temp,0,64);
	       strncpy(temp,&n[2],2);
	       int month = atoi(temp);
	       sprintf(temp,"%s %d", monthName(month), year);

	       date = new char(strlen(temp)+1);
	       strcpy(date, temp);
	       
	}	       
	
	return date;
}

void reduceUpper(char * n) {
	char * tmp = n;
	//ignore leading spaces
	while(*tmp == ' ')
		tmp++;
	tmp++; //ignore first letter
	while(*tmp != '\0')
		*tmp++ = tolower(*tmp);
}

void trimSpaces(char *s) {
	//get rid of leading white space;
	while(*s == ' ')
		s++;
	for( int i = strlen(s) -1; i >=0 ; i--) {
		if(s[i] != ' ')
			break; //stop when we hit a character
		s[i] = '\0'; //fill with nulls
	}
}
	
/* "L/F M"
 */
char * extractName(char * format, char *n) {
	char * name = NULL;
        char temp[96];
	char * t;
	bool m = false;
	if(strcmp(format, "L/F M") == 0) {
		//trim n of leading and trailing spaces
		trimSpaces(n);
		
		//find last name
		char * slash = strchr(n, '/');
		char * lastName = new char [slash - n + 1];
		memset(lastName,0, slash - n + 1);
		strncpy(lastName, n, (slash - n));
		reduceUpper(lastName); //make it pretty
                
		t = slash;
		t++; //move to first letter!
		while(*t==' ' && *t!='\0')
			t++;
		//find seperator between F and M
		while(*t!='\0' && *t!=' ') {
			t++;
		}
		//printf("DONE! %d\n", t- slash);
		//find First name
		char * firstName = new char[t - slash];
		memset(firstName, 0, t - slash);
		strncpy(firstName, (slash+1), t - slash - 1);
                reduceUpper(firstName);
		
		memset(temp,0,96);
		
		
		//printf("Extracted FN \"%s\"\n", firstName);
                sprintf(temp,"%s %s", firstName, lastName);
		
		//form final name
		memset(temp, 0, 96);
		if(t == '\0') //no middle
			m = false;
		else {
			t++;
			if(isupper( (int) *t))
				m = true;
		}
		
		
		if(m) {
		  	//printf("M is true!\n");	
			sprintf(temp, "%s %c %s", firstName, *t, lastName);
		} else {
			sprintf(temp, "%s %s", firstName, lastName);
			//printf("False!\n");
			
		}
		
		delete [] lastName;
//		delete [] firstName;
		name = new char[strlen(temp+1)];
		memset(name, 0, strlen(temp+1));
		strcpy(name, temp);
		
	}

	if(strcmp(format, "L, F") == 0) {
		//trim n of leading and trailing spaces
		trimSpaces(n);
		
		//find last name
		char * slash = strchr(n, ',');
		char * lastName = new char [slash - n + 1];
		memset(lastName,0, slash - n + 1);
		strncpy(lastName, n, (slash - n));
		reduceUpper(lastName); //make it pretty
                slash++;
		t = slash;
		t++; //move to first letter!
		//find seperator between F and M
		while(*t!='\0' && *t!=' ') {
			t++;
		}
		//printf("DONE! %d\n", t- slash);
		//find First name
		char * firstName = new char[t - slash];
		memset(firstName, 0, t - slash);
		strncpy(firstName, (slash+1), t - slash - 1);
                reduceUpper(firstName);
		
		memset(temp,0,96);
		
		
		sprintf(temp, "%s %s", firstName, lastName);
		
		delete [] lastName;
//		delete [] firstName;
		name = new char[strlen(temp+1)];
		memset(name, 0, strlen(temp+1));
		strcpy(name, temp);
		
	}
	if(strcmp(format, "L$F$M") == 0) {
		//trim n of leading and trailing spaces
		trimSpaces(n);
		
		//find last name
		char * slash = strchr(n, '$');
		char * lastName = new char [slash - n + 1];
		memset(lastName,0, slash - n + 1);
		strncpy(lastName, n, (slash - n));
		reduceUpper(lastName); //make it pretty
                
		t = slash;
		t++; //move to first letter!
		//find seperator between F and M
		while(*t!='\0' && *t!='$') {
			t++;
		}
		//printf("DONE! %d\n", t- slash);
		//find First name
		char * firstName = new char[t - slash];
		memset(firstName, 0, t - slash);
		strncpy(firstName, (slash+1), t - slash - 1);
                reduceUpper(firstName);
		
		memset(temp,0,96);
		
		t++;//move to middle
		reduceUpper(t);
                sprintf(temp,"%s %s %s", firstName, t, lastName);
		printf("\"%s\"\n",temp);
		
		delete [] lastName;
//		delete [] firstName;
		name = new char[strlen(temp+1)];
		memset(name, 0, strlen(temp+1));
		strcpy(name, temp);
		
	}
	
	return name;
	
}


char * bankLookup(char * fn, char * s, int len) {
	
	FILE * fin;
	char * bankName = NULL;

	bool flag = true;
	if( (fin=fopen(fn,"r")) == NULL) {
		printf("Error: bankLookup(): Cannot open \"%s\"\n", fn);
	} else {
		char tmp[80];
		memset(tmp,0,80);
		while(!feof(fin))
		{
			fgets(tmp,80,fin);
			//ignore comments (starts with #)
			if(strlen(tmp)>6 && *tmp != '#') {
				if( strncmp(s,tmp,len) == 0) {
					int newLen = strlen(&tmp[len+1]);
					bankName = new char[newLen+1];
					memset(bankName, 0, newLen+1);
					strcpy(bankName, &tmp[len+1]);
					flag = false;
					break;
				}
			}
		}
		fclose(fin);
	}
	if(flag) {
		bankName = new char[8];
		memset(bankName,0,8);
		strcpy(bankName,"Unknown");
	}
	return bankName;
}
	

char numToAlpha(int i)
{
	char alphabet[26];

	strcpy(alphabet,"ABCDEFGHIJKLMNOPQRSTUVWXYZ");

	if (i > 26 || i < 1)
	{
		return '?';
	}

	return alphabet[i - 1];
}

int svIndexLookup(char *fn, char * s, int len) {
	
	FILE * fin;
	if( (fin=fopen(fn,"r")) == NULL) {
		printf("Error: svIndexLookup(): Cannot open \"%s\"\n", fn);
	} else {
		char tmp[120];
		memset(tmp,0,120);
		int i = 1;
		while(!feof(fin))
		{
			fgets(tmp,120,fin);
			if(strlen(tmp)>len) {
				if( strncmp(s,tmp, len) == 0) {
					return i;
				}
			}
			i++;
		}
		fclose(fin);
	}
	return -1;
}

char * svExtract(char * fn, int j, int field, char sep) {
	
	FILE * fin;
	if( (fin=fopen(fn,"r")) == NULL) {
		printf("Error: svExtract(): Cannot open \n", fn);
	} else {
		char tmp[120];
		memset(tmp,0,120);
		int i = 1;
		while(!feof(fin))
		{
			fgets(tmp,120,fin);
			if(i == j) {
			    return svExtractor(tmp, sep, field);
			}
			i++;
		}
		fclose(fin);
	}
	return NULL;
}

char * svExtractor(char * s, char sep, int f) {
	//extract the f field from the s cvs stream
	//while through to the correct field
	
	char * substring = new char[8];
	memset(substring,0,8);
	strcpy(substring, "Unknown");
	char * t;
	int field = 1;
	while(field != f) {
		if(*s == '\n' || *s == '\0')
			return substring;
		if(*s == sep)
			field++;
		s++;
	}
	t = s; // mark start
	//go to end of field
	while(*s != '\n' && *s != '\0' && *s !=sep)
		s++;
	//at end of field
	if(s - t > 0) {
		delete [] substring;
		substring = new char[s-t+1];
	        memset(substring,0,s-t+1);
		strncpy(substring,t,s-t);
	}
	return substring;
}	

