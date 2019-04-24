/**
 * @file database.cpp
 * @brief Manages all the CardTest tests to run on a swiped card.
 * 
 * Wrapper class that holds all the tests for the different card types.
 * 1 function call will run all available tests on a given card,
 * searching for a "fingerprint" match.
 *
 * @author Acidus (acidus@msblabs.org)
 *
 * This file is part of Stripe Snoop (http://stripesnoop.sourceforge.net)
 *
 * Stripe Snoop is licensed under the GPL. See COPYING for more info
 *
 * Copyright (C) 2005 Acidus, Most Significant Bit Labs
 */

#include "database.h"

SSDatabase::SSDatabase() {
	//default contructor
	
	GenericTest * ptr;
	ptr= new VisaTest();
	allTests.push_back(ptr);
/*	ptr = new ATMTest();
	allTests.push_back(ptr);
	ptr = new BoardPassTest();
	allTests.push_back(ptr);
	ptr = new PlaneTicketTest();
	allTests.push_back(ptr);
	ptr = new BuzzcardNewTest();
	allTests.push_back(ptr);
	ptr = new BuzzcardOldTest();
	allTests.push_back(ptr);
	ptr = new MastercardTest();
	allTests.push_back(ptr);
	ptr = new NinetyNineXTest();
	allTests.push_back(ptr);
	ptr = new BarnesNobleGCTest();
	allTests.push_back(ptr);
	ptr = new KrogerTest();
	allTests.push_back(ptr);
	ptr = new GapGCTest();
	allTests.push_back(ptr);
	ptr = new DeltaSkyMilesTest();
	allTests.push_back(ptr);
	ptr = new RoyalCaribbeanSuiteTest();
	allTests.push_back(ptr);
	ptr = new OldNavyGCTest();
	allTests.push_back(ptr);
	ptr = new BestBuyGCTest();
	allTests.push_back(ptr);
	ptr = new ATTPhoneTest();
	allTests.push_back(ptr);
	ptr = new AAMVATest();
	allTests.push_back(ptr);*/





	
	//load all the tests and add to vector here!
	
}

TestResult SSDatabase::runTests(const Card & theCard) const {
	TestResult result;

	for(unsigned int i=0; i < allTests.size(); i++) {
		GenericTest * current = allTests.at(i);
		if(current->meetsRequirements(theCard)) {
			result = current->runTest(theCard);
			if(result.isValid())
				return result;
		}
	}
	return result;
}
