#ifndef BRANCH_HPP
#define BRANCH_HPP

// File:	branch.hpp
// Author:	Sam Henry
// Class:	CpS 310, Microprocessor Architecture
// Date:	October 6, 2016
// Descr:	Contains all of the methods and data for the
//				conditional instruction

#include "instruction.hpp"
#include "cpu.hpp"

#define GET_IMMED_24(rawData) (rawData & 0xFFFFFF)

// The class for all conditional or branching instructions.
// It contains all data and methods for operating on these.
class IBranch : public Instruction {
public:
	word immed_24;
	bool l;

	// Creates the conditional instruction with the individual bit data
	// <rawData> The individual bits
	IBranch(word rawData) : Instruction(rawData) { ; }

	// Destructor
	~IBranch() { ; }

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

#endif //BRANCH_HPP