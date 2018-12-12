#ifndef MRS_HPP
#define MRS_HPP

// File:	msr.hpp
// Author:	Sam Henry
// Class:	CpS 310, Microprocessor Architecture
// Date:	October 6, 2016
// Descr:	Contains all of the methods and data for the
//				conditional instruction

#include "instruction.hpp"
#include "cpu.hpp"

#define GET_MRS_RD(rawData) ((rawData & 0xF000) >> 12)

// The class for all conditional or branching instructions.
// It contains all data and methods for operating on these.
class IMRS : public Instruction {
public:
	word rd;
	bool r;

	// Creates the conditional instruction with the individual bit data
	// <rawData> The individual bits
	IMRS(word rawData) : Instruction(rawData) { ; }

	// Destructor
	~IMRS() { ; }

	// Decodes the conditional instruction
	void decode();

	// Executes the conditional instruction
	void execute();

	// Prints the conditional instruction in assembly form
	void print();

	// Converts the data for the instruction into assembly string form
	// <return> The assembly form of the command
	string toString();
};

#endif //MRS_HPP