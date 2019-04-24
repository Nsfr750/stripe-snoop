/*
 * Class CardTest
 *
 */

#ifndef CARDTEST_H
#define CARDTEST_H
#include "card.h"
#include "testresult.h"

typedef std::vector<int>  intVec;


class GenericTest {
public:
	GenericTest();
	bool meetsRequirements(const Card &) const;
	virtual TestResult runTest(const Card &) const = 0;

protected:
	void addRequiredTrack(const int& i); 
	intVec requiredTracks;
};

class VisaTest : public GenericTest {
public:
	VisaTest();
	virtual TestResult runTest(const Card&) const;

};

class ATMTest : public GenericTest {
public:
	ATMTest();
	virtual TestResult runTest(const Card&) const;

};

class BoardPassTest : public GenericTest {
public:
	BoardPassTest();
	virtual TestResult runTest(const Card&) const;

};

class PlaneTicketTest : public GenericTest {
public:
	PlaneTicketTest();
	virtual TestResult runTest(const Card&) const;

};

class BuzzcardOldTest : public GenericTest {
public:
	BuzzcardOldTest();
	virtual TestResult runTest(const Card&) const;

};

class BuzzcardNewTest : public GenericTest {
public:
	BuzzcardNewTest();
	virtual TestResult runTest(const Card&) const;

};

class MastercardTest : public GenericTest {
public:
	MastercardTest();
	virtual TestResult runTest(const Card&) const;

};

class NinetyNineXTest : public GenericTest {
public:
	NinetyNineXTest();
	virtual TestResult runTest(const Card&) const;

};

class BarnesNobleGCTest : public GenericTest {
public:
	BarnesNobleGCTest();
	virtual TestResult runTest(const Card&) const;

};

class KrogerTest : public GenericTest {
public:
	KrogerTest();
	virtual TestResult runTest(const Card&) const;

};

class GapGCTest : public GenericTest {
public:
	GapGCTest();
	virtual TestResult runTest(const Card&) const;

};

class DeltaSkyMilesTest : public GenericTest {
public:
	DeltaSkyMilesTest();
	virtual TestResult runTest(const Card&) const;

};

class RoyalCaribbeanSuiteTest : public GenericTest {
public:
	RoyalCaribbeanSuiteTest();
	virtual TestResult runTest(const Card&) const;

};

class OldNavyGCTest : public GenericTest {
public:
	OldNavyGCTest();
	virtual TestResult runTest(const Card&) const;

};

class BestBuyGCTest : public GenericTest {
public:
	BestBuyGCTest();
	virtual TestResult runTest(const Card&) const;

};


class ATTPhoneTest : public GenericTest {
public:
	ATTPhoneTest();
	virtual TestResult runTest(const Card&) const;

};


class AAMVATest : public GenericTest {
public:
	AAMVATest();
	virtual TestResult runTest(const Card&) const;

};


#endif
