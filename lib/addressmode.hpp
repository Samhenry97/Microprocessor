#ifndef ADDRESSMODE_HPP
#define ADDRESSMODE_HPP

#include "cpu.hpp"

#define GET_ADR_OFFSET(rawData) (rawData & 0xFFF)
#define GET_ADR_RM(rawData) (rawData & 0xF)
#define GET_ADR_SHIFT_IMM(rawData) ((rawData & 0xF80) >> 7)
#define GET_ADR_SHIFT(rawData) ((rawData & 0x60) >> 5)
#define GET_ADR_REG_MASK(rawData) ((rawData & 0xFF0) >> 4)

class AddressMode {
	// All of the names of the shift types
	static const string shiftTypes[4];
	
private:
	word rawData;

	word offset_12;
	word rm;
	word shift_imm;
	word shift;

private:
	enum AddrType { Immediate, Register, Scaled } type;

public:
	AddressMode(word data) : rawData(data) { ; }

	void decode();
	word execute();
	void print();
	string toString();
	word shiftReg(word &first, word &shiftCode, word &second);
};

#endif // ADDRESSMODE_HPP