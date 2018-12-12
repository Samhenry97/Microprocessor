#ifndef SWI_HPP
#define SWI_HPP

// File:	conditional.hpp
// Author:	Sam Henry
// Class:	CpS 310, Microprocessor Architecture
// Date:	October 6, 2016
// Descr:	Contains all of the methods and data for the
//				swi instruction

#include "instruction.hpp"
#include "cpu.hpp"

// The class for all swi or software interrupt instructions.
// It contains all data and methods for operating on these.
class ISwi : public Instruction {
public:
	word immed_24;

	// Creates the swi instruction with the individual bit data
	// <rawData> The individual bits
	ISwi(word rawData) : Instruction(rawData) { ; }

	// Decodes the swi instruction
	void decode();

	// Executes the swi instruction
	void execute();

	// Prints the swi instruction in assembly form
	void print();

	// Converts the data for the instruction into assembly string form
	// <return> The assembly form of the command
	string toString();
};

#endif //SWI_HPP