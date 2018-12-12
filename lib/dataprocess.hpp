#ifndef DATAPROCESS_HPP
#define DATAPROCESS_HPP

// File:	dataprocess.hpp
// Author:	Sam Henry
// Class:	CpS 310, Microprocessor Architecture
// Date:	October 6, 2016
// Descr:	Contains the data and methods for a 
//				data processing instruction. 

#include "instruction.hpp"
#include "operand2.hpp"
#include "cpu.hpp"

// All of the preprocessor stuffs for bit masks and manipulation
#define GET_OPCODE(rawData) ((rawData & 0x1E00000) >> 21)
#define GET_RN(rawData) ((rawData & 0xF0000) >> 16)
#define GET_RD(rawData) ((rawData & 0xF000) >> 12)


// This class is a data processing instruction class. It contains
// all of the fields for data processing, and methods to act on them.
class IDataProcess : public Instruction {
	static string names[16]; // The names of all of the instructions.
	
public:
	// Creates a data processing instruction.
	// <rawData> The bits of the instrucion in word form.
	IDataProcess(word rawData) : Instruction(rawData), op2(rawData) { ; }

	// Destructor
	~IDataProcess() { ; }

	word opCode;		// The code of the instruction, e.g. mov/add
	word rn, rd;		// The registers to act on for the instruction
	bool updateFlags;	// Whether or not to update the flags
	Operand2 op2;		// The operand2 of the instruction.

	// This method decodes the data processing instruction.
	void decode();

	// This method executes/runs the data processing instruction.
	void execute();

	// This method prints the data processing instruction in assembly form.
	void print();

	// Executes the compare instruction
	void executeCMP();

	// Executes the mov instruction
	void executeMOV();

	// This method returns the data processing instruction in assembly form.
	// <return> The assembly form
	string toString();
};

#endif //DATAPROCESS_HPP