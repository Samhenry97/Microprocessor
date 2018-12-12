#include "../lib/loadstoremultiple.hpp"
#include "../lib/util.hpp"
#include "../lib/cpu.hpp"

int ILoadStoreMultiple::countSetBits(word i) { // http://stackoverflow.com/questions/109023/how-to-count-the-number-of-set-bits-in-a-32-bit-integer
	i = i - ((i >> 1) & 0x55555555);
	i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
	return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

void ILoadStoreMultiple::decode() {
	Instruction::decode();

	p = rawData & 0x1000000;
	u = rawData & 0x800000;
	s = rawData & 0x400000;
	w = rawData & 0x200000;
	l = rawData & 0x100000;
	rn = GET_LSM_RN(rawData);
	regList = GET_LSM_REGLIST(rawData);
}

void ILoadStoreMultiple::execute() {
	CPU &cpu = CPU::instance();
	word startAddress = cpu[rn];
	word endAddress;
	word setBits = countSetBits(regList) << 2;

	if(p && u) {			// Increment Before
		endAddress = startAddress + setBits;
		if(w) { cpu.r(rn) = endAddress; }
		startAddress += 4;
	} else if(!p && u) {	// Increment After
		endAddress = startAddress + setBits - 4;
		if(w) { cpu.r(rn) = startAddress + setBits; }
	} else if(p && !u) {	// Decrement Before
		endAddress = startAddress - 4;
		startAddress = startAddress - setBits;
		if(w) { cpu.r(rn) = cpu[rn] - setBits; }
	} else if(!p && !u) {	// Decrement After
		endAddress = startAddress;
		startAddress = startAddress - setBits + 4;
		if(w) { cpu.r(rn) = endAddress - setBits; }
	}

	word address = startAddress;
	if(l) {		// Load
		for(uint i = 0; i <= 14; i++) {
			if(regList & (1 << i)) {
				cpu.r(i) = cpu.memLoad(address);
				address += 4;
			}
		}
		if(regList & (1 << pc)) {
			cpu.r(pc) = cpu.memLoad(address) & 0xFFFFFFFC;
		}
	} else {	// Store
		for(uint i = 0; i <= 15; i++) {
			if(regList & (1 << i)) {
				cpu.memStore(address, cpu[i]);
				address += 4;
			}
		}
	}
}

void ILoadStoreMultiple::print() {
	cout << toString() << endl;
}

string ILoadStoreMultiple::toString() {
	stringstream final;
	if(l) { 
		final << "ldm";
	} else {
		final << "stm";
	}

	final << condNames[cond];

	if(p && u) {
		final << "ib ";
	} else if(!p && u) {
		final << "ia ";
	} else if(p && !u) {
		final << "db ";
	} else {
		final << "da ";
	}

	final << regNames[rn];
	if(w) { final << "!"; }
	final << ", ";

	final << "{ ";
	word setBits = countSetBits(regList);
	word total = 0;
	for(uint i = 0; i < 15; i++) {
		if(regList & (1 << i)) {
			total++;
			final << regNames[i];
			if(setBits != total) { final << ", "; } 
		}
	}
	final << " }";
	return final.str();
}