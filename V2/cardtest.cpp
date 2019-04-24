/**
 * @file cardtest.cpp
 * @brief Defines tests for determine type of card.
 * 
 * Defines a series of tests to determine the type of card a swiped card is.
 * This includes examining number of tracks present, number of fields, size
 * of those fields, prefixes of fields, etc. This is a very large file!
 *
 * @author Acidus (acidus@msblabs.org)
 *
 * This file is part of Stripe Snoop (http://stripesnoop.sourceforge.net)
 *
 * Stripe Snoop is licensed under the GPL. See COPYING for more info
 *
 * Copyright (C) 2005 Acidus, Most Significant Bit Labs
 */


#include <stdio.h>
#include <stdlib.h>
#include "cardtest.h"
#include "card.h"
#include "testresult.h"
#include "testfuncs.h"

GenericTest::GenericTest() {
	requiredTracks.clear();
}

bool GenericTest::meetsRequirements(const Card & theCard) const {
	//printf("Meets requirements?\n");
	for(unsigned int i = 0; i < requiredTracks.size(); i++) {
		//printf("Does the card have: %d\n", i , theCard.hasTrack(requiredTracks.at(i)));
		if(theCard.hasTrack(requiredTracks.at(i)) == NO) {
			//printf("Failed requirement for track %d\n", requiredTracks.at(i));
			return false;
		}
	}
	return true;
}

void GenericTest::addRequiredTrack(const int &i) {
	requiredTracks.push_back(i);
}

VisaTest::VisaTest() {
	//Set requirements
	addRequiredTrack(1);
	addRequiredTrack(2);
}

//Checks for Visa Credit Card
//4xxxxxxxxxxxxxx@=YYMM101xxxxxxxx 16 digits
//4xxxxxxxxxxx@=YYMM101xxxxxxxx 13 digits

TestResult VisaTest::runTest(const Card & theCard) const{
	//printf("Running Visa Test...\n");
	TestResult result;

	bool tested = false;
	
	//Take care of Track 1
	if(theCard.hasTrack(1) == YES) {
		Track track1 = theCard.getTrack(1);
		//are we an Alphanumeric character set?
		if(track1.getCharSet() != ALPHANUMERIC) {
			return result;
		}
		//Do we have 3 fields?
		if(track1.getNumFields() != 3) {
			return result;
		}
		char * f1 = track1.getField(0);
		//is first character a B?
		if(*f1 != 'B') {
			return result;
		}
		f1++; //move past B
		//starts with a 4?
		if(*f1 != '4') {
			return result;
		}
		//is it 16 or 13 characters?
		if(strlen(f1) != 16 && strlen(f1) != 13) {
	       		return result;
		}
		
		//passes mod10
		if(!mod10check(f1)) {
			return result;
		}
		//has 101 constant or 120 Constant
		char * f3 = track1.getField(2);
		if(strncmp(&f3[4],"101",3) != 0) {
			return result;
		}
		//printf("All good\n");
		// === PASSED ALL THE TESTS! ===
		tested = true;
		result.setCardType("Visa Credit Card");
		char * tmp = extractName("L/F M", track1.getField(1));
		result.addTag("Issued To", tmp);
		delete [] tmp;
		result.addTag("Account Number", formatter("XXXX XXXX XXXX XXXX", f1));
		tmp = extractDate("YYMM", f3);
		result.addTag("Expires", tmp);
		delete [] tmp;
		//get E-pin
		tmp = new char [8];
		memset(tmp,0,8);
		strncpy(tmp,&f3[27],7);
		result.addTag("Encrypted PIN",tmp);
		delete [] tmp;

		tmp = bankLookup(VISABANKNAMES,f1,4);
		result.addTag("Issuing Bank", tmp);
		delete [] tmp;
		
	} else {
		//we couldn't read track 1
		result.addExtraTag("Full name of card holder");
		result.addExtraTag("Account number");
		result.addExtraTag("Expiration date");
		result.addExtraTag("Encrypted pin");
	} //end track 1
	//Take care of Track 2
	if(theCard.hasTrack(2) == YES) {
	
		Track track2 = theCard.getTrack(2);
		char * f1 = track2.getField(0);
		char * f2 = track2.getField(1);
		if(!tested) {
			//are we an Alphanumeric character set?
			if(track2.getCharSet() != NUMERIC) {
				return result;
			}
			//Do we have 2 fields?
			if(track2.getNumFields() != 2) {
				return result;
			}
			//starts with a 4?
			if(*f1 != '4') {
				return result;
			}
			//is it 16 or 13 characters?
			if(strlen(f1) !=16 && strlen(f1) != 13) {
		       		return result;
			}
			//passes mod10
			if(!mod10check(f1)) {
				return result;
			}
			//has 101 constant
			if(strncmp(&f2[4],"101",3) != 0) {
				return result;
			}
		} //end !tested
		// === PASSED ALL THE TESTS! ===
		tested = true;
		result.setCardType("VISA Credit Card");
		result.addTag("Account Number", formatter("XXXX XXXX XXXX XXXX", f1));
		char * tmp = extractDate("YYMM", f2);
		result.addTag("Expires", tmp);
		delete [] tmp;
		//get E-pin
		tmp = new char [8];
		memset(tmp,0,8);
		strncpy(tmp,&f2[27],7);
		result.addTag("Encrypted PIN",tmp);
		delete [] tmp;
		tmp = bankLookup(VISABANKNAMES,f1,4);
		result.addTag("Issuing Bank", tmp);
		delete [] tmp;

		
	} else {
		result.addExtraTag("Account number");
		result.addExtraTag("Expiration date");
		result.addExtraTag("Encrypted pin");
		
	} //end track 2
		
	return result;
}

ATMTest::ATMTest() {
	//Set requirements
	addRequiredTrack(1);
	addRequiredTrack(2);
}


TestResult ATMTest::runTest(const Card & theCard) const{
	TestResult result;

	bool tested = false;
	
	//Take care of Track 1
	if(theCard.hasTrack(1) == YES) {
		Track track1 = theCard.getTrack(1);
		//are we an Alphanumeric character set?
		if(track1.getCharSet() != ALPHANUMERIC) {
			return result;
		}
		//Do we have 3 fields?
		if(track1.getNumFields() != 3) {
			return result;
		}
		char * f1 = track1.getField(0);
		//is first character a B?
		if(*f1 != 'B') {
			return result;
		}
		f1++; //move past B
		//starts with a 5?
		if(*f1 != '5') {
			return result;
		}
		//is it 16 characters?
		if(strlen(f1) !=16 ) {
	       		return result;
		}
		//passes mod10
		if(!mod10check(f1)) {
			return result;
		}
		//has 101 constant
		char * f3 = track1.getField(2);
		if(strncmp(&f3[4],"101",3) != 0 && strncmp(&f3[4], "120", 3) != 0) {
			return result;
		}
		//printf("All good\n");
		// === PASSED ALL THE TESTS! ===
		tested = true;
		result.setCardType("ATM Card");
		char * tmp = extractName("L/F M", track1.getField(1));
		result.addTag("Issued To", tmp);
		delete [] tmp;
		result.addTag("Account Number", formatter("XXXX XXXX XXXX XXXX", f1));
		tmp = extractDate("YYMM", f3);
		result.addTag("Expires", tmp);
		delete [] tmp;
		//get E-pin
		//tmp = new char [8];
		//memset(tmp,0,8);
		//strncpy(tmp,&f3[27],7);
		//result.addTag("Encrypted PIN",tmp);
		//delete [] tmp;
		
	} else {
		//we couldn't read track 1
		result.addExtraTag("Full name of card holder");
		result.addExtraTag("Account number");
		result.addExtraTag("Expiration date");
		//result.addExtraTag("Encrypted pin");
	} //end track 1
	//Take care of Track 2
	if(theCard.hasTrack(2) == YES) {
	
		Track track2 = theCard.getTrack(2);
		char * f1 = track2.getField(0);
		char * f2 = track2.getField(1);
		if(!tested) {
			//are we an Alphanumeric character set?
			if(track2.getCharSet() != NUMERIC) {
				return result;
			}
			//Do we have 2 fields?
			if(track2.getNumFields() != 2) {
				return result;
			}
			//starts with a 4?
			if(*f1 != '5') {
				return result;
			}
			//is it 16 or 13 characters?
			if(strlen(f1) !=16) {
		       		return result;
			}
			//passes mod10
			if(!mod10check(f1)) {
				return result;
			}
			//has 101 or 120 constant
			if(strncmp(&f2[4],"101",3) != 0 && strncmp(&f2[4], "120", 3) != 0) {
				return result;
			}
		} //end !tested
		// === PASSED ALL THE TESTS! ===
		tested = true;
		result.setCardType("Generic ATM Card");
		result.addTag("Account Number", formatter("XXXX XXXX XXXX XXXX", f1));
		char * tmp = extractDate("YYMM", f2);
		result.addTag("Expires", tmp);
		delete [] tmp;
		//get E-pin
		//tmp = new char [8];
		//memset(tmp,0,8);
		//strncpy(tmp,&f2[27],7);
		//result.addTag("Encrypted PIN",tmp);
		//delete [] tmp;
		
	} else {
		result.addExtraTag("Account number");
		result.addExtraTag("Expiration date");
		//result.addExtraTag("Encrypted pin");
		
	} //end track 2
		
	return result;
}


BoardPassTest::BoardPassTest() {
	//Set requirements
	addRequiredTrack(1);
}

TestResult BoardPassTest::runTest(const Card & theCard) const {
	TestResult result;

	bool tested = false;
	char * t;
	int i;

	Track track1 = theCard.getTrack(1);
	//are we an Alphanumeric character set?
	if(track1.getCharSet() != ALPHANUMERIC) {
		return result;
	}
	//Do we have 1 fields?
	if(track1.getNumFields() != 1) {
		return result;
	}
	
	char * f1 = track1.getField(0);
	//is first character a W?
	if(*f1 != 'W') {
		return result;
	}

	if(strlen(f1) != 40) {
		return result;
	}

	//Check for spaces
	//012345678 0123 5 789 123 --
        if(f1[9] !=' ' || f1[14] !=' ' || f1[16] !=' ' || f1[20]!=' ' || f1[24]!=' ') {
	       return result;
	}	       
	
	// === PASSED ALL THE TESTS! ===
	
	tested = true;
	result.setCardType("Airline Boarding Pass");

	i = svIndexLookup(AIRLINENAMES, &f1[7], 2);
	if(i > 0) {
            result.addTag("Carrier", svExtract(AIRLINENAMES, i, 2, ';'));
	} else {
            result.addTag("Carrier", "Unregistered Code");
	}
	
	t = new char[5];
	memset(t,0,5);
	strncpy(t,&f1[10],4);
	if(t[4] == ' ')
		t[4] = '\0';
	result.addTag("Flight Number", t);
        memset(t,0,5);
	strncpy(t,&f1[21], 3);
	result.addTag("Seat", t);	
	
	i = svIndexLookup(AIRLINECLASSES, &f1[15], 1);
	if(i > 0) {
            result.addTag("Class", svExtract(AIRLINECLASSES, i, 2, ','));
	} else {
            result.addTag("Class", "Unregistered Code");
	}
	
	i = svIndexLookup(AIRPORTNAMES, &f1[1], 3);
	if(i > 0) {
            result.addTag("Departing Airport", svExtract(AIRPORTNAMES, i, 2, ';'));
	    result.addTag("Origin", svExtract(AIRPORTNAMES, i, 3, ';'));
	} else {
            result.addTag("Departing Airport", "Unregistered Code");
	}
	
	i = svIndexLookup(AIRPORTNAMES, &f1[4], 3);
	if(i > 0) {
            result.addTag("Destination Airport", svExtract(AIRPORTNAMES, i, 2, ';'));
	    result.addTag("Destination", svExtract(AIRPORTNAMES, i, 3, ';'));
	} else {
            result.addTag("Destination Airport", "Unregistered Code");
	}
	
	return result;
}

PlaneTicketTest::PlaneTicketTest() {
	//Set requirements
	addRequiredTrack(1);
}

TestResult PlaneTicketTest::runTest(const Card & theCard) const {
	TestResult result;

	bool tested = false;
	char * t;
	int i;

	Track track1 = theCard.getTrack(1);
	//are we an Alphanumeric character set?
	if(track1.getCharSet() != ALPHANUMERIC) {
		return result;
	}
	//Do we have 1 fields?
	if(track1.getNumFields() != 1) {
		return result;
	}
	
	char * f1 = track1.getField(0);
	//is first character a W?
	if(*f1 != 'W') {
		return result;
	}

	if(strlen(f1) != 60) {
		return result;
	}

	//Check for spaces
        if(f1[9] !=' ' || f1[14] !=' ' || f1[16] !=' ' || f1[20]!=' ' || f1[24]!=' ') {
	       return result;
	}	       
	
	// === PASSED ALL THE TESTS! ===
	
	tested = true;
	result.setCardType("Airline Ticket");

	result.addTag("Passenger", extractName("L/F M", &f1[28]));
	i = svIndexLookup(AIRLINENAMES, &f1[7], 2);
	if(i > 0) {
            result.addTag("Carrier", svExtract(AIRLINENAMES, i, 2, ';'));
	} else {
            result.addTag("Carrier", "Unregistered Code");
	}
	
	t = new char[5];
	memset(t,0,5);
	strncpy(t,&f1[10],4);
	if(t[0] == ' ') {
		for(int z =0; z < 3; z++)
			t[z] = t[z+1];
		t[3] = '\0';
	}
	result.addTag("Flight Number", t);
        memset(t,0,5);
	strncpy(t,&f1[21], 3);
	result.addTag("Seat", t);	
	
	i = svIndexLookup(AIRLINECLASSES, &f1[15], 1);
	if(i > 0) {
            result.addTag("Class", svExtract(AIRLINECLASSES, i, 2, ','));
	} else {
            result.addTag("Class", "Unregistered Code");
	}
	
	i = svIndexLookup(AIRPORTNAMES, &f1[1], 3);
	if(i > 0) {
            result.addTag("Departing Airport", svExtract(AIRPORTNAMES, i, 2, ';'));
	    result.addTag("Origin", svExtract(AIRPORTNAMES, i, 3, ';'));
	} else {
            result.addTag("Departing Airport", "Unregistered Code");
	}
	
	i = svIndexLookup(AIRPORTNAMES, &f1[4], 3);
	if(i > 0) {
            result.addTag("Destination Airport", svExtract(AIRPORTNAMES, i, 2, ';'));
	    result.addTag("Destination", svExtract(AIRPORTNAMES, i, 3, ';'));
	} else {
            result.addTag("Destination Airport", "Unregistered Code");
	}
	
	delete [] t;
	
	return result;
}



//Checks for Georgia Institute of Technology Buzzcard
//OLD: 1570=xxxSSNxxx=0x=60177000xxxxxxxx 00 or 02
//PARK or TEMP 1570=000000000=00=60177000xxxxxxxx

BuzzcardOldTest::BuzzcardOldTest() {
	//Set requirements
	addRequiredTrack(2);
}

TestResult BuzzcardOldTest::runTest(const Card & theCard) const{
	TestResult result;
	char * f1 = NULL;
        char * f2 = NULL;
	char * f3 = NULL;
	char * f4 = NULL;

	if(theCard.hasTrack(2) == YES) {

		Track track2 = theCard.getTrack(2);
	
		//are we a Numeric character set?
		if(track2.getCharSet() != NUMERIC) return result;
	
		//Do we have 4 fields?
		if(track2.getNumFields() != 4) return result;
		f1 = track2.getField(0);
		f2 = track2.getField(1);
		f3 = track2.getField(2);
		f4 = track2.getField(3);

		//first field is 1570
		if(strcmp(f1,"1570") !=0) return result;
		//2nd field is 9 characters
	
		if(strlen(f2) !=9) return result;
		//3rd field ==00 || 02
		if( strcmp(f3,"00")!=0 && strcmp(f3,"02")!=0 )
			return result;


		//4th field starts with 60177000
		if(strncmp(f4,"60177000",8) !=0) return result;

		//WE ARE GOOD!
		//which type, old or parking/temp?
		if(strncmp(f2,"000000000",9)==0 || f2[0] == '8') {
			result.setCardType("Georgia Tech Parking or Temporary Card");
		} else {
			result.setCardType("Georgia Tech Buzzcard - Pre 2002 version");
			result.addTag("SSN/Student Number",formatter("XXX-XX-XXXX",f2));
		}
		result.addTag("Site ID:",f4);
	}
		
	return result;

}


// ================================================================= 4 Fields

//Checks for Georgia Institute of Technology Buzzcard
//NEW: ;1570=90xxxxxxx=00=60177000xxxxxxxx?
//       +6177000xxxxxxxxx?

BuzzcardNewTest::BuzzcardNewTest() {
	//Set requirements
	addRequiredTrack(2);
	addRequiredTrack(3);
}

TestResult BuzzcardNewTest::runTest(const Card & theCard) const{
	TestResult result;
	char * f1 = NULL;
        char * f2 = NULL;
	char * f3 = NULL;
	char * f4 = NULL;

	bool tested = false;

	if(theCard.hasTrack(2) == YES) {
		Track track2 = theCard.getTrack(2);
		//are we a Numeric character set?
		if(track2.getCharSet() != NUMERIC) return result;
		//Do we have 4 fields?
		if(track2.getNumFields() != 4) return result;
	
		f1 = track2.getField(0);
		f2 = track2.getField(1);
		f3 = track2.getField(2);
		f4 = track2.getField(3);
	
		//first field is 1570
		if(strcmp(f1,"1570") !=0) return result;
		//2nd field is 9 characters
		if(strlen(f2) !=9) return result;
		//first character is a "90"
		if(strncmp(f2, "90", 2) != 0) return result;
		//3rd field ==00
		if( strcmp(f3,"00")!=0) return result;
		//4th field starts with 60177000
		if(strncmp(f4,"60177000",8) !=0) return result;

		//WE ARE GOOD!
		//which type, old or parking/temp?
		result.setCardType("Georgia Tech Buzzcard - GTID version");
		result.addTag("Student Number",formatter("XXX-XX-XXXX",f2));
		result.addTag("Site ID:",f4);
		tested = true;
	} else {
		//couldn't read track 2
		result.addExtraTag("Student Number");
		result.addExtraTag("Site ID");
	}

	if(theCard.hasTrack(3) == YES) {
		if(!tested) {
			Track track3 = theCard.getTrack(3);
			if(track3.getCharSet() != NUMERIC) return result;
			//Do we have 1 field?
			if(track3.getNumFields() != 1) return result;
			f1 = track3.getField(1);
			//field starts with 60177000
			if(strncmp(f1,"60177000",8) !=0) return result;
		}
		
		result.setCardType("Georgia Tech Buzzcard - GTID version");
		result.addTag("Site ID:",f1);
	} else {
		//couldn't read track 1
		result.addExtraTag("Site ID");
	}

	return result;
}



MastercardTest::MastercardTest() {
	//Set requirements
	addRequiredTrack(1);
	addRequiredTrack(2);
}


TestResult MastercardTest::runTest(const Card & theCard) const{
	TestResult result;

	bool tested = false;
	
	//Take care of Track 1
	if(theCard.hasTrack(1) == YES) {
		Track track1 = theCard.getTrack(1);
		//are we an Alphanumeric character set?
		if(track1.getCharSet() != ALPHANUMERIC) {
			return result;
		}
		//Do we have 3 fields?
		if(track1.getNumFields() != 3) {
			return result;
		}
		char * f1 = track1.getField(0);
		//is first character a B?
		if(*f1 != 'B') {
			return result;
		}
		f1++; //move past B


		//starts with a 51, 52, 53, 54, or 55
		if(f1[0]!='5' && 
			(f1[1] != '1'
			|| f1[1] != '2'
			|| f1[1] != '3'
			|| f1[1] != '4'
			|| f1[1] != '5'))
		 return result;


		//is it 16 characters?
		if(strlen(f1) != 16) {
	       		return result;
		}
		
		//passes mod10
		if(!mod10check(f1)) {
			return result;
		}
		//has 101 constant
		char * f3 = track1.getField(2);
		if(strncmp(&f3[4],"101",3) != 0) {
			return result;
		}
		//printf("All good\n");
		// === PASSED ALL THE TESTS! ===
		tested = true;
		result.setCardType("Mastercard Credit Card");
		char * tmp = extractName("L/F M", track1.getField(1));
		result.addTag("Issued To", tmp);
		delete [] tmp;
		result.addTag("Account Number", formatter("XXXX XXXX XXXX XXXX", f1));
		tmp = extractDate("YYMM", f3);
		result.addTag("Expires", tmp);
		delete [] tmp;
		//get E-pin
		tmp = new char [8];
		memset(tmp,0,8);
		strncpy(tmp,&f3[27],7);
		result.addTag("Encrypted PIN",tmp);
		delete [] tmp;

		tmp = bankLookup(MASTERCARDBANKNAMES,f1,4);
		result.addTag("Issuing Bank", tmp);
		delete [] tmp;
		
	}
	return result;
}


NinetyNineXTest::NinetyNineXTest() {
	//Set requirements
	addRequiredTrack(2);
}

TestResult NinetyNineXTest::runTest(const Card & theCard) const{
	TestResult result;
	char * f1 = NULL;

	if(theCard.hasTrack(2) == YES) {

		Track track2 = theCard.getTrack(2);
	
		//are we a Numeric character set?
		if(track2.getCharSet() != NUMERIC) return result;
	
		//Do we have 4 fields?
		if(track2.getNumFields() != 1) return result;
		f1 = track2.getField(0);

		//first field starts 997
		if(strncmp(f1,"997",3) !=0) return result;
	
		//WE ARE GOOD!
		//which type, old or parking/temp?
		
		result.setCardType("99X Freeloader Card");
		result.addTag("ID",formatter("XXX XXX XXXXX",f1));
		
		result.setNotes("Issued by Radio Station 99.7 in Atlanta GA, USA");
	}
		
	return result;

}

BarnesNobleGCTest::BarnesNobleGCTest() {
	//Set requirements
	addRequiredTrack(1);
	addRequiredTrack(2);
}

TestResult BarnesNobleGCTest::runTest(const Card & theCard) const{
	TestResult result;
	char * f1 = NULL;
	char * f2 = NULL;

	if(theCard.hasTrack(1) == YES) {

		Track track1 = theCard.getTrack(1);
		//are we a Numeric character set?
		if(track1.getCharSet() != ALPHANUMERIC) return result;
	
		if(track1.getNumFields() != 3) return result;
		f1 = track1.getField(0);
		f2 = track1.getField(1);

		//1st is 16 characters
		if(strlen(f1)!=16) return result;
		//starts with B
		if(f1[0] != 'B') return result;
		//move
		f1++;
		//passes mod10
		if(!mod10check(f1)) return result;
		//has 5045 constant
		if(strncmp(f1,"5045",4)!=0) return result;

		//WE ARE GOOD!
		result.setCardType("Barnes and Noble Giftcard");
		result.addTag("Account Number",f1);
		result.setUnknowns("Store Number or State it was issued in");
	}
		
	return result;

}


KrogerTest::KrogerTest() {
	//Set requirements
	addRequiredTrack(1);
	addRequiredTrack(2);
}

TestResult KrogerTest::runTest(const Card & theCard) const{
	TestResult result;
	char * f1 = NULL;
	char * f2 = NULL;
	char * f3 = NULL;

	if(theCard.hasTrack(1) == YES) {

		Track track1 = theCard.getTrack(1);
		//are we a Numeric character set?
		if(track1.getCharSet() != ALPHANUMERIC) return result;
	
		if(track1.getNumFields() != 3) return result;
		f1 = track1.getField(0);
		f2 = track1.getField(1);
		f3 = track1.getField(2);

		if(strcmp(f2,"CARD/S") !=0) return result;

		//1st is 15 characters
		if(strlen(f1)!=15) return result;
		//snd is at least 3 characters
		if(strlen(f3)<3) return result;
		//has 603 constant
		if(strncmp(f1,"603",3)!=0) return result;

		//WE ARE GOOD!
		result.setCardType("Kroger Plus Shopping Card");
		result.addTag("Account Number",f1);
		result.setUnknowns("Store Number or State it was issued in");
	}
		
	return result;

}

GapGCTest::GapGCTest() {
	//Set requirements
	addRequiredTrack(1);
}

TestResult GapGCTest::runTest(const Card & theCard) const{
	TestResult result;
	char * f1 = NULL;
	char * f2 = NULL;
	char * f3 = NULL;

	if(theCard.hasTrack(1) == YES) {

		Track track1 = theCard.getTrack(1);
		//are we a Numeric character set?
		if(track1.getCharSet() != ALPHANUMERIC) return result;
	
		if(track1.getNumFields() != 3) return result;
		f1 = track1.getField(0);
		f2 = track1.getField(1);
		f3 = track1.getField(2);

		
		if(*f1!='B') return result;
		f1++;
		
		//1st is 16 characters
		if(strlen(f1)!=16) return result;
		//starts with 600
		if(strncmp(f1,"600",3) != 0) return result;
		if(!mod10check(f1)) return result;
		if(strncmp(f2,"GAURT",5) != 0) return result;

		//WE ARE GOOD!
		result.setCardType("Gap Clothing Gift Card");
		result.addTag("Account Number",f1);
		result.setUnknowns("Store Number or State it was issued in");
	}
		
	return result;

}


DeltaSkyMilesTest::DeltaSkyMilesTest() {
	//Set requirements
	addRequiredTrack(1);
	addRequiredTrack(2);
}

TestResult DeltaSkyMilesTest::runTest(const Card & theCard) const{
	TestResult result;
	char * f1 = NULL;
	char * f2 = NULL;
	char * f3 = NULL;

	if(theCard.hasTrack(1) == YES) {

		Track track1 = theCard.getTrack(1);
		//are we a Numeric character set?
		if(track1.getCharSet() != ALPHANUMERIC) return result;
	
		if(track1.getNumFields() != 3) return result;
		f1 = track1.getField(0);
		f2 = track1.getField(1);
		
		f3 = (theCard.getTrack(2)).getField(1);
		if(strlen(f3) != 11) return result;
		f3++;
		if(*f3 != '2') return result;		
		if(strncmp(f1, "BDL", 3) != 0) return result;

		//WE ARE GOOD!
		result.setCardType("Delta Airlines Sky Miles Card");
		result.addTag("Account Number",f3);
	}
		
	return result;

}

RoyalCaribbeanSuiteTest::RoyalCaribbeanSuiteTest() {
	//Set requirements
	addRequiredTrack(1);
	addRequiredTrack(2);
}

TestResult RoyalCaribbeanSuiteTest::runTest(const Card & theCard) const{
	TestResult result;
	char * f1 = NULL;
	char * f2 = NULL;
	char * f3 = NULL;

	if(theCard.hasTrack(1) == YES) {

		Track track1 = theCard.getTrack(1);
		//are we a Numeric character set?
		if(track1.getCharSet() != ALPHANUMERIC) return result;
	
		if(track1.getNumFields() != 5) return result;
		Track track2 = theCard.getTrack(2);

		f1 = track1.getField(0);
		if(strlen(f1) != 32) return result;
		f2 = track2.getField(1);
		if(strlen(f2) != 0) return result;
		
		//WE ARE GOOD!
		result.setCardType("Royal Caribbean Cruise Ship Card");
		char * tmp = extractName("L, F", f1);
		result.addTag("Passenger Name", tmp);
		f1 = track1.getField(2);
		result.addTag("Cabin Number", f1);
		f2 = track2.getField(0);
		while(*f2 == '0' && *f2 !='\0')
			f2++;
		result.addTag("Folio Number", f2);
		result.setUnknowns("Dining Room, Dining Time, Table Number, Sail Date");

		delete [] tmp;
	}
		
	return result;

}


OldNavyGCTest::OldNavyGCTest() {
	//Set requirements
	addRequiredTrack(1);
	addRequiredTrack(2);
}

TestResult OldNavyGCTest::runTest(const Card & theCard) const{
	TestResult result;
	char * f1 = NULL;
	char * f2 = NULL;
	char * f3 = NULL;

	if(theCard.hasTrack(1) == YES) {

		Track track1 = theCard.getTrack(1);
		//are we a Numeric character set?
		if(track1.getCharSet() != ALPHANUMERIC) return result;
	
		if(track1.getNumFields() != 3) return result;
		f1 = track1.getField(0);
		f2 = track1.getField(1);
		f3 = track1.getField(2);

		
		if(*f1!='B') return result;
		f1++;
		
		//1st is 16 characters
		if(strlen(f1)!=16) return result;
		//starts with 600
		if(strncmp(f1,"600",3) != 0) return result;
		if(!mod10check(f1)) return result;
		if(strncmp(f2,"ONURT",5) != 0) return result;

		//WE ARE GOOD!
		result.setCardType("Old Navy Clothing Gift Card");
		result.addTag("Account Number",f1);
		result.setUnknowns("Store Number or State it was issued in");
	}
		
	return result;

}

BestBuyGCTest::BestBuyGCTest() {
	//Set requirements
	addRequiredTrack(1);
	addRequiredTrack(2);
}

TestResult BestBuyGCTest::runTest(const Card & theCard) const{
	TestResult result;

	bool tested = false;
	
	//Take care of Track 1
	if(theCard.hasTrack(1) == YES) {
		Track track1 = theCard.getTrack(1);
		//are we an Alphanumeric character set?
		if(track1.getCharSet() != ALPHANUMERIC) {
			return result;
		}
		//Do we have 3 fields?
		if(track1.getNumFields() != 3) {
			return result;
		}
		char * f1 = track1.getField(1);
		if(strcmp(f1," /                        ") != 0 ) return result;

		f1 = track1.getField(0);
		//is first character a B?
		if(*f1 != 'B') return result;

		char * f2 = theCard.getTrack(2).getField(0);
		//starts with a 3790
		if(strncmp(f2, "3790",4) != 0) return result;
	
		if(strlen(f2) != 15) return result;

		//passes mod10
		if(!mod10check(f2)) return result;
		f2 = theCard.getTrack(2).getField(1);
		if(strncmp(f2,"1309101",7) != 0) return result;

		//printf("All good\n");
		// === PASSED ALL THE TESTS! ===
		tested = true;
		result.setCardType("Best Buy Gift Card");
		result.addTag("Account Number", f1);
		result.setUnknowns("Issuing Store Number");
		result.setNotes("Cards managed by American Express, with valid AMEX Credit Card Number");
	
	} //end track 1
		
	return result;
}

ATTPhoneTest::ATTPhoneTest() {
	//Set requirements
	addRequiredTrack(1);
	addRequiredTrack(2);
	addRequiredTrack(3);
}

TestResult ATTPhoneTest::runTest(const Card & theCard) const {
	TestResult result;
	char * f1 = NULL;
	char * f2 = NULL;
	char * f3 = NULL;

	if(theCard.hasTrack(1) == YES) {

		Track track1 = theCard.getTrack(1);
		//are we a Numeric character set?
		if(track1.getCharSet() != ALPHANUMERIC) return result;
	
		if(track1.getNumFields() != 4) return result;
		f1 = track1.getField(0);
		if(strlen(f1) != 1) return result;

		if(*f1 != 'B') return result;
		f1 = track1.getField(1);
		if(strcmp(f1," /") != 0) return result;

		f1 = track1.getField(2);
		if(strlen(f1) != 0) return result;
		f1 = track1.getField(3);

		if(*f1!='C') return result;
		
		f1 = theCard.getTrack(2).getField(0);
		f1+=4;

		//WE ARE GOOD!
		result.setCardType("AT&T Corporate Calling Card");
		result.addTag("Account Number", formatter("XXX XXX XXXX XXXX",f1));
		result.setNotes("This card has lots of unknown fields, that contain possibly phone numbers, network codes, etc.");
	}
		
	return result;

}

/*
[acidus@vagabond ss]$ ./ss
Stripe Snoop Version 2.0
http://stripesnoop.sourceforge.net  Acidus@yak.net

Reading from /dev/ttyS0
Track 1:%CALOS ANGELES^BONNER$SEAN$LEONARD^143 S HAYWORTH 203^?:
Track 2:;636014043776468=050219759924?:

*/


AAMVATest::AAMVATest() {
	//Set requirements
	addRequiredTrack(1);
	addRequiredTrack(2);
}

TestResult AAMVATest::runTest(const Card & theCard) const {

	TestResult result;
	Track track1 = theCard.getTrack(1);
	Track track2 = theCard.getTrack(2);

	char *t2f1 = track2.getField(0);
	char *t2f2 = track2.getField(1);
	char *t1f2 = track1.getField(1);
	char *t1f3 = track1.getField(2);
	char *t1f1 = track1.getField(0);

	char day[3]={0,0,0};
	char year[5]={0,0,0,0,0};

	int i,j;
	char tmp2[80];
	char * tmp;

	//DO THE TEST!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	//1st is at least 7 characters
	if(strlen(t2f1) < 7) return result;
	//2nd is at least 12 characters
	if(strlen(t2f2) < 12) return result;
	//starts with "636"
	if(strncmp(t2f1,"636",3)!=0) return result;
 	//Expiration has a valid month for expire, or 77 88 99
	if(!isMonth(&t2f2[2]) && strncmp(&t2f2[2],"77",2)!=0
		&& strncmp(&t2f2[2],"88",2)!=0
		&& strncmp(&t2f2[2],"99",2)!=0)
			return result;
	
	//WE ARE GOOD!
	result.setCardType("AAMVA Compliant North American Driver's License");

	i = svIndexLookup(AAMVAREGIONS, t2f1, 6);
	if(i > 0) {
		result.addTag("Issuing Territory", svExtract(AAMVAREGIONS, i, 2, ','));
	} else {
            	result.addTag("Issuing Territory", "Unknown");
	}

	//funny month fix I think this causes a bug! California is fucked up
	/*Get Name*/
	tmp = extractName("L$F$M", t1f2);
	result.addTag("Issued To", tmp);
	delete [] tmp;
	result.addTag("Street Address", t1f3);
	result.addTag("City", &t1f1[2]);
	tmp = new char[2];
	tmp[0]=t1f1[0];
	tmp[1]=t1f1[1];
	result.addTag("State",tmp);

	result.addTag("License Number",&t2f1[6]);
	//get the DOB
	memset(tmp2,0,80);
	if(!isMonth(&t2f2[8])) {
		//fake month
		strncpy(tmp2,&t2f2[2],2);
	} else { 
		strncpy(tmp2,&t2f2[8],2); //grab month
	}
	strncpy(day,&t2f2[10],2);
	strncpy(year,&t2f2[4],4);
	i = atoi(tmp2);
	sprintf(tmp2,"%s %s, %s",monthName(i),day,year);
	result.addTag("Date of Birth",tmp2);

	//Get the expiration Date
	memset(year,0,5);
	strncpy(year,t2f2,2);;
	j=atoi(year);
	j = expandYear(j);

	if(!isMonth(&t2f2[8])) {
		if(strncmp(&t2f2[2],"77",2)==0) {
			strcpy(tmp2,"Never");
		} else if(strncmp(&t2f2[2],"88",2)==0) {
			sprintf(tmp2,"%s %d, %d",monthName(i),lastDotm(i,j),j);
		} else if(strncmp(&t2f2[2],"99",2)==0) {
			sprintf(tmp2,"%s %s, %d",monthName(i),day,j);
		}
	} else {
		//plain expiration date
		strncpy(tmp2,&t2f2[2],2); //grab month
		sprintf(tmp2,"%s %d",monthName(atoi(tmp2)),j);
	}
		/*
	card.add("Expires",tmp2);

	
	card.setNotes("This Standard is deployed not only in the US, but parts of Canada as well. Some States/Territories may use letters in their License Numbers. If this is the case, these numbers will be encoded using 2 numbers as follows: A=01, B=02 ... Z=26.");



	*/
	return result;

}

