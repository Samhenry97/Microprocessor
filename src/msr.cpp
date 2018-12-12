#include "../lib/msr.hpp"
#include "../lib/util.hpp"

void IMSR::decode() {
	Instruction::decode();

	r = rawData & (1 << 22);
	field_mask = GET_MSR_FIELD_MASK(rawData);
	rotate_imm = GET_MSR_ROT_IMMED(rawData);
	immed_8 = GET_MSR_IMMED_8(rawData);
	rm = GET_MSR_RM(rawData);
}

void IMSR::execute() {
	CPU &cpu = CPU::instance();

	if((cpu.cpsr & MODE_MASK) == USR_mode) { cpu.fakeSwi(); return; }

	word operand;

	if(rawData & (1 << 25)) {
		operand = Util::rotateRight(immed_8, rotate_imm << 1);
	} else {
		operand = cpu[rm];
	}

	word byte_mask = 0;
	for(uint i = 0; i < 4; i++) {
		if(field_mask & (1 << i)) {
			byte_mask |= 0xFF << (i << 3);
		}
	}

	if(r) {
		word mask = byte_mask & (0xF00000DF);
		word spsr = cpu.getSPSR();
		spsr = (spsr & ~mask) | (operand & mask);
	} else {
		word mask;
		if(cpu.isPrivileged()) {
			mask = byte_mask & (0xF00000DF);
		} else {
			mask = byte_mask & (0xF0000000);
		}
		cpu.cpsr = (cpu.cpsr & ~mask) | (operand & mask);
	}
}

void IMSR::print() {
	cout << toString() << endl;
}

string IMSR::toString() {
	stringstream final;
	final << "msr" << condNames[cond] << " ";

	string field = "";
	if(field_mask & 8) { field += "f"; }
	if(field_mask & 4) { field += "s"; }
	if(field_mask & 2) { field += "x"; }
	if(field_mask & 1) { field += "c"; }

	string last = "";
	if(rawData & (1 << 25)) {
		last = "#" + to_string(Util::rotateRight(immed_8, rotate_imm << 1));
	} else {
		last = regNames[rm];
	}

	if(r) {
		final << "spsr_" << field << ", " << last;
	} else {
		final << "cpsr_" << field << ", " << last;
	}

	return final.str();
}