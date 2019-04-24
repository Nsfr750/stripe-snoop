
#ifndef LOADER_H
#define LOADER_H
/*
 * Loader - hands all the loading of the config file
 *
 */

#include "reader.h"

Reader * loadConfig(char * fn);

Reader * loadDirectReader();

Reader * loadSerialReader();


#endif
