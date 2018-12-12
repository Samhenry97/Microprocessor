// File:	types.hpp
// Author:	Sam Henry
// Class:	CpS 310, Microprocessor Architecture
// Date:	October 6, 2016
// Descr:	Contains the user-defined types for the program
//				itself. Makes them more "my style" ;)

#ifndef TYPES_H
#define TYPES_H

#include "askapi.hpp"

typedef unsigned int uint;

typedef ask_mode AskMode;
typedef ask_signal AskSignal;
typedef ask_config AskConfig;
typedef ask_host_services AskHostServices;
typedef ask_stats AskStats;

// All of the different possible shifts
#define LSL 0
#define LSR 1
#define ASR 2
#define ROR 3

#endif //TYPES_H