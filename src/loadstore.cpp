// File:	loadstore.cpp
// Author:	Sam Henry
// Class:	CpS 310, Microprocessor Architecture
// Date:	October 6, 2016
// Descr:	Contains the implementations of the methods and data
//				for the Load/Store instruction.

#include "../lib/loadstore.hpp"
#include "../lib/util.hpp"

void ILoadStore::decode() {
	Instruction::decode();

	p = rawData & 0x1000000;
	u = rawData & 0x800000;
	b = rawData & 0x400000;
	w = rawData & 0x200000;
	l = rawData & 0x100000;

	rn = GET_LS_RN(rawData);
	rd = GET_LS_RD(rawData);

	addressMode.decode();
}

void ILoadStore::execute() {
	CPU &cpu = CPU::instance();
	word address;
	if(p) {		// Pre-indexed
		if(w) {
			if(u) {
				cpu.r(rn) += addressMode.execute();
			} else {
				cpu.r(rn) -= addressMode.execute();
			}
			address = cpu[rn];
		} else {
			if(u) {
				address = cpu[rn] + addressMode.execute();
			} else {
				address = cpu[rn] - addressMode.execute();
			}
		}
	} else {	// Post-indexed
		address = cpu[rn];
		if(u) {
			cpu.r(rn) += addressMode.execute();
		} else {
			cpu.r(rn) -= addressMode.execute();
		}
	}

	if(b) {
		executeByte(address);
	} else {
		if(address & 0b11) { cpu.shell->panic("Misaligned load!"); }
		executeWord(address);
	}
}

void ILoadStore::executeByte(word &address) {
	CPU &cpu = CPU::instance();

	if(l) {
		word amtBack = address & 0b11;
		word alignedAddr = address - amtBack;
		amtBack <<= 3;

		cpu.r(rd) = (cpu.memLoad(alignedAddr) >> amtBack) & 0xFF;
	} else {
		word amtBack = address & 0b11;
		word alignedAddr = address - amtBack;
		amtBack <<= 3;

		word current = cpu.memLoad(alignedAddr);
		word mask = 0xFF << amtBack;
		word toStore = (cpu[rd] & 0xFF) << amtBack;
		current = (current & ~mask) | toStore;

		cpu.memStore(alignedAddr, current);
	}	
}

void ILoadStore::executeWord(word &address) {
	CPU &cpu = CPU::instance();

	if(l) {
		cpu.r(rd) = cpu.memLoad(address);
	} else {
		cpu.memStore(address, cpu[rd]);
	}
}

void ILoadStore::print() {
	cout << toString() << "\n";
}

string ILoadStore::toString() {
	stringstream final;
	if(l) {
		final << "ldr";
	} else {
		final << "str";
	}

	if(b) {
		final << "b";
	}

	final << condNames[cond] << " ";

	if(p) { // Pre-indexed
		final << regNames[rd] << ", [" << regNames[rn] << ", " << addressMode.toString() << "]";
		if(w) { final << "!"; }
	} else {
		final << regNames[rd] << ", [" << regNames[rn] << "] " << addressMode.toString();
	}

	return final.str();
}