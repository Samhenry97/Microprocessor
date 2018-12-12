// File:	mul.cpp
// Author:	Sam Henry
// Class:	CpS 310, Microprocessor Architecture
// Date:	October 6, 2016
// Descr:	Contains the implementations for data and methods for a 
//				mul instruction. 

#include "../lib/mul.hpp"

void IMul::decode() {
	Instruction::decode();

	rd = MUL_RD(rawData);
	rs = MUL_RS(rawData);
	rm = MUL_RM(rawData);
	updateFlags = MUL_S(rawData);
}

void IMul::execute() {
	CPU &cpu = CPU::instance();
	
	cpu.r(rd) = cpu[rm] * cpu[rs];	
}

void IMul::print() {
	cout << toString() << "\n";
}

string IMul::toString() {
	return "mul" + condNames[cond] + " " + regNames[rd] + ", " + regNames[rm] + ", " + regNames[rs];
}