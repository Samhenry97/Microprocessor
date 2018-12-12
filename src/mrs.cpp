#include "../lib/mrs.hpp"

void IMRS::decode() {
	Instruction::decode();

	rd = GET_MRS_RD(rawData);
	r = rawData & (1 << 22);
}

void IMRS::execute() {
	CPU &cpu = CPU::instance();

	if((cpu.cpsr & MODE_MASK) == USR_mode) { cpu.fakeSwi(); return; }

	if(r) {
		cpu.r(rd) = cpu.getSPSR();
	} else {
		cpu.r(rd) = cpu.cpsr;
	}
}

void IMRS::print() {
	cout << toString() << endl;
}

string IMRS::toString() {
	stringstream final;

	final << "mrs" << condNames[cond] << " " << regNames[rd] << ", " << (r ? "spsr" : "cpsr");

	return final.str();
}