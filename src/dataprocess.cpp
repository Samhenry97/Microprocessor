// File:	dataprocess.cpp
// Author:	Sam Henry
// Class:	CpS 310, Microprocessor Architecture
// Date:	October 6, 2016
// Descr:	Contains the implementations for data and methods for a 
//				data processing instruction. 

#include "../lib/dataprocess.hpp"

string IDataProcess::names[16] = {"and", "eor", "sub", "rsb", "add", "adc", "sbc", "rsc", "tst", "teq", "cmp", "cmn", "orr", "mov", "bic", "mvn"};

void IDataProcess::decode() {
	Instruction::decode();

	opCode = GET_OPCODE(rawData);
	updateFlags = rawData & 20;
	rn = GET_RN(rawData);
	rd = GET_RD(rawData);
	op2.decode();
}

void IDataProcess::execute() {
	CPU &cpu = CPU::instance();

	switch(opCode) {
		case 0 : 	cpu.r(rd) = cpu[rn] & op2.execute(); break;					//and
		case 1 : 	cpu.r(rd) = cpu[rn] ^ op2.execute(); break;					//eor
		case 2 : 	cpu.r(rd) = cpu[rn] - op2.execute(); break;	//sub
		case 3 : 	cpu.r(rd) = op2.execute() - (sword) cpu[rn]; break;	//rsb
		case 4 : 	cpu.r(rd) = cpu[rn] + op2.execute(); break; //add
		case 10 : 	executeCMP(); break;										//cmp
		case 12 : 	cpu.r(rd) = cpu[rn] | op2.execute(); break; 				//orr
		case 13 :	executeMOV(); break; 										//mov
		case 14 :	cpu.r(rd) = cpu[rn] & (~op2.execute()); break;				//bic
		case 15 :	cpu.r(rd) = ~op2.execute(); break;							//mvn
		default : 	cpu.shell->panic(("Unsupported instruction!" + toString()).c_str());
	}
}

void IDataProcess::executeCMP() {
	CPU &cpu = CPU::instance();
	word a = cpu[rn];
	word b = op2.execute();
	word result = (sword) a - (sword) b;

	cpu.setFlag(NF, result & (1 << 31));
	cpu.setFlag(ZF, result == 0);
	cpu.setFlag(CF, b <= a);
	bool overflow = (((a & (1 << 31)) != (b & (1 << 31))) and ((a & (1 << 31)) != (result & (1 << 31))));
	cpu.setFlag(VF, overflow);
}

void IDataProcess::executeMOV() {
	CPU &cpu = CPU::instance();
	cpu.r(rd) = op2.execute();
	if(updateFlags and rd == pc) { cpu.restoreCPSR(); }
}

void IDataProcess::print() {
	cout << toString() << "\n";
}

string IDataProcess::toString() {
	if(opCode == 13 || opCode == 15) {
		return names[opCode] + condNames[cond] + " " + regNames[rd] + op2.toString();
	} else {
		return names[opCode] + condNames[cond] + " " + regNames[rd] + ", " + regNames[rn] + op2.toString();
	}
}