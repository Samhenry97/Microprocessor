// File:	instruction.hpp
// Author:	Sam Henry
// Class:	CpS 310, Microprocessor Architecture
// Date:	October 6, 2016
// Descr:	Contains the methods and data for a basic
//				instruction inside the microprocessor.

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <map>
#include <iostream>
#include <sstream>
#include <string>

#include "askapi.hpp"

using std::string;
using std::to_string;
using std::stringstream;
using std::cout;
using std::endl;

// The bit shifting required to get the condition and type out
// of the instruction itself. I am using #define because it
// is slightly more efficient than creating a mask and using it.
// It also hides the implementations of the masking.
#define GET_COND(rawData) ((rawData & 0xF0000000) >> 28)
#define GET_TYPE(rawData) ((rawData & 0xE000000) >> 25)

// This class provides all of the basic data and methods for an
// instruction inside the ARM processor. It contains the "raw data"
// for the insruction, and methods to decode, execute, and print the
// instruction.
class Instruction {
public:
	word rawData; 	// The instruction in individual bits, waiting for processing
	word cond;		// The condition for the instruction (boolean stuffs)
	word type;		// The type of instruction (data processing, branch, or load/store)

	// Creates an instruction with the given raw data.
	// <data> The individual 32 bits for the instruction
	Instruction(word data);

	// Virtual destructor
	virtual ~Instruction() { ; }

	// This method decodes the instruction into "readable" parts.
	virtual void decode();

	// This method executes, or runs, the instruction itself. Its logic
	// is defined in the subclasses.
	virtual void execute() = 0;

	// This method prints the instruction to the console in the
	// actual assembly format.
	virtual void print() = 0;

	// This method creates a string in assembly-style of the instruction.
	virtual string toString() = 0;

public:
	static string regNames[16];
	static string condNames[16];
};

#endif