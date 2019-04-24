/*
 * testfuncs.cpp - Various functions to test for a card fingerprint
 */

#ifndef TESTFUNCS_H
#define TESTFUNCS_H

#define VISABANKNAMES "data/visa-pre.csv"
#define MASTERCARDBANKNAMES "data/mastercard-pre.csv"
#define AIRPORTNAMES "data/airportcodes.csv"
#define AIRLINENAMES "data/airline.csv"
#define AIRLINECLASSES "data/airline-classes.csv"
#define AAMVAREGIONS "data/aamva-regions.csv"


bool mod10check(char *card);

bool isMonth(char * d);

int lastDotm(int m, int y);

bool dateValid(char *m, char *d, char * y);

bool isAlpha(const char *);

bool isBCD(const char *);

// =============================================================== FORMATTERS

int expandYear(int i);

char * monthName(int x);

char * formatter(char * format, char * n);

char * extractDate(char * format, char * n);

char * extractName(char * format, char *n);

void reduceUpper(char * n);

char * bankLookup(char * fn, char * s, int len);

int svIndexLookup(char *fn, char * s, int len);

char * svExtract(char * fn, int j, int field, char sep);

char * svExtractor(char * s, char sep, int f);

char numToAlpha(int i);

#endif
