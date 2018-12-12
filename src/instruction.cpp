// File:	instruction.cpp
// Author:	Sam Henry
// Class:	CpS 310, Microprocessor Architecture
// Date:	October 6, 2016
// Descr:	Contains the implementations of methods and data 
//				for a basic instruction inside the microprocessor.

#include "../lib/instruction.hpp"

string Instruction::regNames[16] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "ip", "sp", "lr", "pc"};
string Instruction::condNames[16] = {"eq", "ne", "cs", "cc", "mi", "pl", "vs", "vc", "hi", "ls", "ge", "lt", "le", "al", ""};

Instruction::Instruction(word data) {
	rawData = data;
}

void Instruction::decode() {
	cond = GET_COND(rawData);
	type = GET_TYPE(rawData);
}