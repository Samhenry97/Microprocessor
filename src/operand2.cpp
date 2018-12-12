// File:	operand2.cppf

#include "../lib/operand2.hpp"

const string Operand2::shiftTypes[4] = {"lsl", "lsr", "asr", "ror"};

word Operand2::shiftReg(word first, word shiftCode, word second) {
	bool bitSet;
	word ans;

	switch(shiftCode) {
		case LSL : return first << second;
		case LSR : return first >> second;
		case ASR : 
			bitSet = (first & (1 << 31)) >> 31;
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

void Operand2::rotateRight(word &immediate, word &rotateAmt) {
	immediate = (immediate >> rotateAmt) | (immediate << (32 - rotateAmt));
}

void Operand2::decode() {
	if(rawData & (1 << 25)) {
		type = Immediate32;

		rotate_imm = GET_ROTATE_IMM(rawData) << 1; // Multiply by two
		immediate = GET_IMMEDIATE(rawData);
		rotateRight(immediate, rotate_imm);
	} else if(!(rawData & (1 << 4))) {
		type = ImmediateShift;

		shift_imm = GET_SHIFT_IMM(rawData);
		shift = GET_SHIFT(rawData);
		rm = GET_RM(rawData);
	} else if(!(rawData & (1 << 7)) and (rawData & (1 << 4))) {
		type = RegShift;

		rs = GET_RS(rawData);
		shift = GET_SHIFT(rawData);
		rm = GET_RM(rawData);
	} else {
		CPU::instance().shell->panic("Invalid Operand2.");
	}
}

word Operand2::execute() {
	CPU &cpu = CPU::instance();
	if(type == Immediate32) {
		return immediate;
	} else if(type == ImmediateShift) {
		return shiftReg(cpu[rm], shift, shift_imm);
	} else if(type == RegShift) {
		return shiftReg(cpu[rm], shift, cpu[rs]);
	}
	return -1; // Make compiler happy.
}

void Operand2::print() {
	cout << toString();
}

string Operand2::toString() {
	switch(type) {
		case Immediate32 :
			if(immediate != 0) { return ", #" + to_string(immediate); }
			return "";
		case ImmediateShift :
			if(shift_imm != 0) {
				return ", " + Instruction::regNames[rm] + ", " + shiftTypes[shift] + " #" + to_string(shift_imm);
			} else {
				return ", " + Instruction::regNames[rm];
			}
		case RegShift :
			return ", " + Instruction::regNames[rm] + ", " + shiftTypes[shift] + " " + Instruction::regNames[rs];
		default :
			return "Invalid Operand2";
	}
}