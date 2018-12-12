#ifndef MUL_HPP
#define MUL_HPP

// File:	dataprocess.hpp
// Author:	Sam Henry
// Class:	CpS 310, Microprocessor Architecture
// Date:	October 6, 2016
// Descr:	Contains the data and methods for a 
//				mul instruction. 

#include "instruction.hpp"
#include "cpu.hpp"

// All of the preprocessor stuffs for bit masks and manipulation
#define MUL_RD(rawData) ((rawData & 0xF0000) >> 16)
#define MUL_RS(rawData) ((rawData & 0xF00) >> 8)
#define MUL_RM(rawData) (rawData & 0xF)
#define MUL_S(rawData) ((rawData & 0x10000) >> 20)

// This class is a mul instruction class. It contains
// all of the fields for mul, and methods to act on them.
class IMul : public Instruction {
	static string names[16]; // The names of all of the instructions.
	
	bool updateFlags;	// Whether or not to update the flags
	word rd, rs, rm;	// The registers to operate on.
		
public:
	// Creates a mul instruction.
	// <rawData> The bits of the instrucion in word form.
	IMul(word rawData) : Instruction(rawData) { ; }

	// Destructor
	~IMul() { ; }

	// This method decodes the mul instruction.
	void decode();

	// This method executes/runs the mul instruction.
	void execute();

	// This method prints the mul instruction in assembly form.
	void print();

	// This method returns the mul instruction in assembly form.
	// <return> The assembly form
	string toString();
};

#endif //MUL_HPP