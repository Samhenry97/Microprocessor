#include "../lib/branchx.hpp"

void IBranchX::decode() {
	Instruction::decode();

	rm = rawData & 0xF;
}

void IBranchX::execute() {
	CPU &cpu = CPU::instance();
	
	cpu.r(pc) = cpu[rm] & 0xFFFFFFFE;
}

void IBranchX::print() {
	cout << toString() << endl;;
}

string IBranchX::toString() {
	return "bx" + condNames[cond] + " " + regNames[rm];
}