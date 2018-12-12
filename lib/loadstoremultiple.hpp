// File:	loadstore.hpp
// Author:	Sam Henry
// Class:	CpS 310, Microprocessor Architecture
// Date:	October 6, 2016
// Descr:	Contains all of the methods and data for the
//				Load/Store instruction

#ifndef LOADSTOREMULTIPLE_HPP
#define LOADSTOREMULTIPLE_HPP

#include "instruction.hpp"

#define GET_LSM_RN(rawData) ((rawData & 0xF0000) >> 16)
#define GET_LSM_REGLIST(rawData) (rawData & 0xFFFF)

// The class for all conditional or Load/Store instructions.
// It contains all data and methods for operating on these.
class ILoadStoreMultiple : public Instruction {
	bool p;		// Pre- or post-indexed?
	bool u;		// Are we subtracting or adding?
	bool s;		// Byte or word?
	bool w; 	// Write back?
	bool l;		// Load or store?
	byte rn;			// Registers to do things with
	word regList;// The addressing mode

public:
	// Creates the instruction with raw bits
	// <rawData> The individual bits of the instruction
	ILoadStoreMultiple(word rawData) : Instruction(rawData) { ; }

	// Destructor
	~ILoadStoreMultiple() { ; }

	// Decodes the instruction
	void decode();

	// Executes the instruction
	void execute();

	// Prints the instruction in assembly form
	void print();

	int countSetBits(word i);

	// Converts the instruction data into assembly form
	// <return> The string assembly form
	string toString();
};

#endif //LOADSTOREMULTIPLE_HPP