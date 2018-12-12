// File:	operand2.hpp
// Author:	Sam Henry
// Class:	CpS 310, Microprocessor Architecture
// Date:	October 6, 2016
// Descr:	Contains all of the methods and data for the
//				operand2 part of the data processing instruction.

#ifndef OPERAND2_H
#define OPERAND2_H

#include "cpu.hpp"
#include "types.hpp"

// The actual bit crunching for getting values.
#define GET_ROTATE_IMM(rawData) ((rawData & 0xF00) >> 8)
#define GET_IMMEDIATE(rawData) (rawData & 0xFF)
#define GET_SHIFT_IMM(rawData) ((rawData & 0xF80) >> 7)
#define GET_SHIFT(rawData) ((rawData & 0x60) >> 5)
#define GET_RM(rawData) (rawData & 0xF)
#define GET_RS(rawData) ((rawData & 0xF00) >> 8)


// This class contains all of the methods for decoding and executing
// the operand2 part of the data processing instruction. Each data
// processing insruction should have an instance of this, except
// for weird ones like "mul"
class Operand2 {
private:
	// All of the names of the shift types
	static const string shiftTypes[4];

private:
	word rawData; 		// The individual bits of the instruction

	word rotate_imm;	// The rotate right amount for immed_8
	word immediate;		// The immediate number
	word shift_imm;		// The amount to shift
	word rs;			// Register rs
	word shift;			// Shift type
	word rm;			// Register rm

private:
	// The type of the Operand2.
	enum Op2Type { Immediate32, ImmediateShift, RegShift, Invalid } type;

public:
	// Creates the Operand2 with the individual bits
	// <data> The individual bits
	Operand2(word data) : rawData(data) { ; }

	// Decodes the Operand2
	void decode();

	// Executes the Operand2
	// <return> The value resulting from executing
	word execute();

	// Prints the Operand2
	void print();

	// Returns the assembly form of Operand2
	string toString();

	// Shifts (using shift type) the register by a certain amount
	// <first> The number to shift
	// <shiftCode> The type of shift - types are in the #define up top
	// <second> The amount to shift
	// <return> The shifted value
	word shiftReg(word first, word shiftCode, word second);

	// Rotates <immediate> by <rotateAmt>
	// <immediate> The value to rotate
	// <rotateAmt> The amount to rotate
	void rotateRight(word &immediate, word &rotateAmt);
};

#endif //OPERAND2_H