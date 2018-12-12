#include "../lib/addressmode.hpp"
#include "../lib/util.hpp"

const string AddressMode::shiftTypes[4] = {"lsl", "lsr", "asr", "ror"};

word AddressMode::shiftReg(word &first, word &shiftCode, word &second) {
	bool bitSet;
	word ans;

	switch(shiftCode) {
		case LSL : return first << second;
		case LSR : return first >> second;
		case ASR : 
			bitSet = first & 0x80000000;
			ans = first >> second;
			if(bitSet) {
				for(uint i = 31; i > 31 - second; i--) {
					ans |= (1 << i);
				}
			}
			return ans;
		case ROR : return Util::rotateRight(first, second);
		default : return 0; // Make the compiler happy
	}
}

void AddressMode::decode() { 
	if(!(rawData & (1 << 25))) {
		type = Immediate;

		offset_12 = GET_ADR_OFFSET(rawData);
	} else if(GET_ADR_REG_MASK(rawData) == 0) {
		type = Register;

		rm = GET_ADR_RM(rawData);
	} else {
		type = Scaled;

		shift_imm = GET_ADR_SHIFT_IMM(rawData);
		shift = GET_ADR_SHIFT(rawData);
		rm = GET_ADR_RM(rawData);
	}
}

word AddressMode::execute() {
	if(type == Immediate) {
		return offset_12;
	} else if(type == Register) {
		return CPU::instance()[rm];
	} else if(type == Scaled) {
		return shiftReg(CPU::instance().r(rm), shift, shift_imm);
	} else {
		CPU::instance().shell->panic("Unknown address mode in ldr/str!");
		return 0;
	}
}

void AddressMode::print() {
	cout << toString() << endl;
}

string AddressMode::toString() {
	stringstream final;

	string u = rawData & (1 << 23) ? "" : "-";
	
	if(type == Immediate) {
		final << "#" << u << offset_12;
	} else if(type == Register) {
		final << Instruction::regNames[rm];
	} else if(type == Scaled) {
		final << Instruction::regNames[rm] << ", " << shiftTypes[shift] << "#" << shift_imm;
	}

	return final.str();
}