#ifndef MSR_HPP
#define MSR_HPP

// File:	msr.hpp
// Author:	Sam Henry
// Class:	CpS 310, Microprocessor Architecture
// Date:	October 6, 2016
// Descr:	Contains all of the methods and data for the
//				conditional instruction

#include "instruction.hpp"
#include "cpu.hpp"

#define GET_MSR_RM(rawData) (rawData & 0xF)
#define GET_MSR_IMMED_8(rawData) (rawData & 0xFF)
#define GET_MSR_ROT_IMMED(rawData) ((rawData & 0xF00) >> 8)
#define GET_MSR_FIELD_MASK(rawData) ((rawData & 0xF0000) >> 16)

// The class for all conditional or branching instructions.
// It contains all data and methods for operating on these.
class IMSR : public Instruction {
public:
	bool r;
	word field_mask;
	word rotate_imm;
	word rm;
	word immed_8;

	// Creates the conditional instruction with the individual bit data
	// <rawData> The individual bits
	IMSR(word rawData) : Instruction(rawData) { ; }

	// Destructor
	~IMSR() { ; }

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

#endif //MSR_HPP