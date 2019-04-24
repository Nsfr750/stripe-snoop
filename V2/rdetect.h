#include "reader.h"

Reader * queryReader(int i);

Reader * queryGameReader();

Reader * queryParallelReader();

Reader * querySerialReader();

void setupDirectIO();

void sortByValue(long * fluxes, int * mask);

int promptForPort(int i);

int main(int argc, char* argv[]);

