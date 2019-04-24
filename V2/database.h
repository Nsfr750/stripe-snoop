#ifndef DATABASE_H
#define DATABASE_H

#include "card.h"
#include "cardtest.h"
#include "testresult.h"

typedef std::vector<GenericTest * >  testVec;

class SSDatabase {
public:
	
	SSDatabase();
	TestResult runTests(const Card &) const;

private:
	testVec allTests;
};

#endif
