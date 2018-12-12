// File:	dataprocess.cpp
// Author:	Sam Henry
// Class:	CpS 310, Microprocessor Architecture
// Date:	October 6, 2016
// Descr:	Contains the implementations for data and methods for a
//				data processing instruction.

#include "../lib/swi.hpp"

void ISwi::decode() {
	Instruction::decode();

	immed_24 = rawData & 0xFFFFFF;
}

void ISwi::execute() {
	CPU &cpu = CPU::instance();
	if(immed_24 == 0) {
		cpu.running = false;
	} else {
		cpu.spsr_svc = cpu.cpsr;
		cpu.cpsr = (cpu.cpsr & 0xFFFFFFE0) | SVC_mode;
		cpu.cpsr |= 0x80;
		cpu.r(lr) = cpu.r(pc) + 4;
		cpu.r(pc) = 0x8;
	}
}

void ISwi::print() {
	cout << toString() << "\n";
}

string ISwi::toString() {
	return "swi" + condNames[cond] + " " + to_string(immed_24);
}
