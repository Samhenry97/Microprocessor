#include "../lib/branch.hpp"

void IBranch::decode() {
	Instruction::decode();

	immed_24 = GET_IMMED_24(rawData);
	l = rawData & (1 << 24);
}

void IBranch::execute() {
	CPU &cpu = CPU::instance();
	if(l) { cpu.r(lr) = cpu.r(pc) + 4; }
	if(immed_24 & (1 << 23)) { immed_24 |= 0xFF000000; } 
	cpu.r(pc) += (immed_24 << 2) + 8;
}

void IBranch::print() {
	cout << toString() << endl;
}

string IBranch::toString() {
	stringstream final;

	final << "b";
	if(l) { final << "l"; }
	final << condNames[cond] << " " << (immed_24 << 2);

	return final.str();
}