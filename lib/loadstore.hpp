#ifndef LOADSTORE_HPP
#define LOADSTORE_HPP

// File:	loadstore.hpp
// Author:	Sam Henry
// Class:	CpS 310, Microprocessor Architecture
// Date:	October 6, 2016
// Descr:	Contains all of the methods and data for the
//				Load/Store instruction

#include "instruction.hpp"
#include "addressmode.hpp"

#define GET_LS_RN(rawData) ((rawData & 0xF0000) >> 16)
#define GET_LS_RD(rawData) ((rawData & 0xF000) >> 12)

// The class for all conditional or Load/Store instructions.
// It contains all data and methods for operating on these.
class ILoadStore : public Instruction {
	bool p;		// Pre- or post-indexed?
	bool u;		// Are we subtracting or adding?
	bool b;		// Byte or word?
	bool w; 	// Write back?
	bool l;		// Load or store?
	word rn, rd;			// Registers to do things with
	AddressMode addressMode;// The addressing mode

public:
	// Creates the instruction with raw bits
	// <rawData> The individual bits of the instruction
	ILoadStore(word rawData) : Instruction(rawData), addressMode(rawData) { ; }

	// Destructor
	~ILoadStore() { ; }

	// Decodes the instruction
	void decode();

	// Executes the instruction
	void execute();

	void executeByte(word &address);

	void executeWord(word &address);

	// Prints the instruction in assembly form
	void print();

	// Converts the instruction data into assembly form
	// <return> The string assembly form
	string toString();
};

#endif //LOADSTORE_HPP