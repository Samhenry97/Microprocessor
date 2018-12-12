// File:	cpu.cpp
// Author:	Sam Henry
// Class:	CpS 310, Microprocessor Architecture
// Date:	October 6, 2016
// Descr:	Contains the implementations of the CPU class.

#include "../lib/cpu.hpp"
#include "../lib/dataprocess.hpp"
#include "../lib/loadstore.hpp"
#include "../lib/loadstoremultiple.hpp"
#include "../lib/mul.hpp"
#include "../lib/swi.hpp"
#include "../lib/branch.hpp"
#include "../lib/branchx.hpp"
#include "../lib/mrs.hpp"
#include "../lib/msr.hpp"

const char *CPU::info[] = { "author=shenr124", "api=1.0", "svc", "usr", "irq", "fiq", "cache", nullptr };
CPU *CPU::_instance = nullptr;

void CPU::init() {
	for(uint i = 0; i < NUM_REGISTERS; i++) {
		registers[i] = 0;
	}

	irq = fiq = halt = false;
	cpsr = 0xD3;		// Both interrupt flags set, SVC mode.
	spsr_irq = spsr_svc = spsr_fiq = 0;
	step = 1;
	config = ac_nothing;
	stats.instructions = stats.loads = stats.stores = stats.load_misses = stats.store_misses = 0;
}

void CPU::start(int initCycles) {
	cycles = initCycles;

	stats.instructions = 0;

	run();
}

void CPU::run() {
	running = true;

	while(running) {
		if(halt) { running = false; halt = false; break; }
		checkIRQ();
		checkFIQ();

		Instruction *i = getNextInstruction();
		i->decode();
		word curPc = registers[pc];
		//shell->log_msg(("Step " + to_string(step) + " " + i->toString()).c_str());
		if(conditionPassed(i->cond)) { i->execute(); }

		if(config & ac_trace_log) {
			shell->log_trace(step, curPc, cpsr, r(0), r(1), r(2), r(3), r(4), r(5), r(6), r(7), r(8), r(9), r(10), r(11), r(ip), r(sp), r(lr));
		}
		
		if(curPc == registers[pc]) { registers[pc] += 4; }
		step++;
		stats.instructions++;

		if(cycles != 0) {
			cycles--;
			if(cycles == 0) { running = false; }
		}
		
		delete i;
	}
}

void CPU::checkIRQ() {
	if(irq and not (cpsr & (1 << 7))) {
		irq = false;
		spsr_irq = cpsr;
		cpsr = (cpsr & 0xFFFFFFE0) | IRQ_mode | 0xC0;
		r(lr) = r(pc) + 4;
		r(pc) = 0x18;
	}
}

void CPU::checkFIQ() {
	if(fiq and not (cpsr & (1 << 6))) {
		fiq = false;
		spsr_fiq = cpsr;
		cpsr = (cpsr & 0xFFFFFFE0) | FIQ_mode | 0xC0;
		r(lr) = r(pc) + 4;
		r(pc) = 0x1C;
	}
}

bool CPU::conditionPassed(byte condition) {
	switch(condition) {
		case 0  : return check(ZF);
		case 1  : return not check(ZF);
		case 2  : return check(CF);
		case 3  : return not check(CF);
		case 4  : return check(NF);
		case 5  : return not check(NF);
		case 6  : return check(VF);
		case 7  : return not check(VF);
		case 8  : return check(CF) and not check(ZF);
		case 9  : return not check(CF) or check(ZF);
		case 10 : return check(NF) == check(VF);
		case 11 : return check(NF) != check(VF);
		case 12 : return not check(ZF) and (check(NF) == check(VF));
		case 13 : return check(ZF) or (check(NF) != check(VF));
		case 14 : return true;
		case 15 : shell->panic("Unsupported condition code: 1111");
		default : shell->panic("Invalid condition code!");
	}
	shell->panic("How did you even get here???");
	return true;
}

Instruction *CPU::getNextInstruction() {
	Instruction *i = nullptr;

	word iCode = memLoad(registers[pc]);

	// For this testing stuffs, I am basically masking the bits that are known
	// for the instruction, whether they are 1's or 0's. Then, I am testing to
	// see if they equal the known '1' bits.
	if((iCode & 0xFE0F0F0) == 0x90) {
		i = new IMul(iCode);
	} else if((iCode & 0xF000000) == 0xF000000) {
		i = new ISwi(iCode);
	} else if((iCode & 0xFFFFFF0) == 0x12FFF10) {
		i = new IBranchX(iCode);
	} else if((iCode & 0xDA0F000) == 0x120F000) {
		i = new IMSR(iCode);
	} else if((iCode & 0xFBF0FFF) == 0x10F0000) {
		i = new IMRS(iCode);
	} else if((iCode & 0xE000000) == 0x8000000) {
		i = new ILoadStoreMultiple(iCode);
	} else if((iCode & 0xE000000) == 0xA000000) {
		i = new IBranch(iCode);
	} else if((iCode & 0xC000000) == 0x4000000) {
		i = new ILoadStore(iCode);
	} else if((iCode & 0xC000000) == 0) {
		i = new IDataProcess(iCode);
	} else {
		shell->panic("Unimplemented instruction!! Aborting.");
	}

	return i;
}

bool CPU::check(byte code) {
	return (cpsr & (1 << code));
}

void CPU::setFlag(byte code, bool flag) {
	cpsr = (cpsr & ~(1 << code)) | (flag << code);
}

void CPU::restoreCPSR() {
	switch(cpsr & MODE_MASK) {
		case SYS_mode: shell->panic("No SPSR for SYS mode."); break;
		case SVC_mode: cpsr = spsr_svc; break;
		case USR_mode: shell->panic("No SPSR for USR mode."); break;
		case IRQ_mode: cpsr = spsr_irq; break;
		case FIQ_mode: cpsr = spsr_fiq; break;
		default: shell->panic("Unsupported mode in restoreCPSR()!");
	}
}

word &CPU::getSPSR() {
	switch(cpsr & MODE_MASK) {
		case SYS_mode: shell->panic("No SPSR for SYS mode."); break;
		case USR_mode: shell->panic("No SPSR for USR mode."); break;
		case SVC_mode: return spsr_svc;
		case IRQ_mode: return spsr_irq;
		case FIQ_mode: return spsr_fiq;
		default: shell->panic("Unsupported mode in getSPSR()!");
	}
	shell->panic("How did you get here??");
	exit(0);
}

bool CPU::isPrivileged() {
	return not ((cpsr & MODE_MASK) == USR_mode);
}

CPU &CPU::instance() {
	if(_instance == nullptr) {
		_instance = new CPU();
	}
	return *_instance;
}

word &CPU::r(word index) {
	switch(cpsr & MODE_MASK) {
		case SYS_mode: return registers[index];
		case SVC_mode: 
			if(index == sp or index == lr) {
				return registers[index + SVC_offset];
			} else {
				return registers[index];
			}
		case USR_mode: return registers[index];
		case IRQ_mode:
			if(index == sp or index == lr) {
				return registers[index + IRQ_offset];
			} else {
				return registers[index];
			}
		case FIQ_mode:
			if(index > 7 and index < 15) {
				return registers[index + FIQ_offset];
			} else {
				return registers[index];
			}
		default: shell->panic("Unsupported mode!");
	}
	shell->panic("How did you get here?!");
	exit(0); // Make compiler happy
}

word CPU::operator[](word index) {
	if(index == pc) { return registers[index] + 8; }
	switch(cpsr & MODE_MASK) {
		case SYS_mode: return registers[index];
		case SVC_mode: 
			if(index == sp or index == lr) {
				return registers[index + SVC_offset];
			} else {
				return registers[index];
			}
		case USR_mode: return registers[index];
		case IRQ_mode:
			if(index == sp or index == lr) {
				return registers[index + IRQ_offset];
			} else {
				return registers[index];
			}
		case FIQ_mode:
			if(index > 7 and index < 15) {
				return registers[index + FIQ_offset];
			} else {
				return registers[index];
			}
		default: shell->panic("Unsupported mode!");
	}
	shell->panic("How did you get here?!");
	exit(0); // Make compiler happy
}

word CPU::memLoad(word address) {
	checkAddress(address);

	stats.loads++;

	if((config & ac_cache_on) and address < 0x80000000) {
		return cache.memLoad(address);
	} else {
		return shell->mem_load(address);
	}
}

void CPU::memStore(word address, word value) {
	checkAddress(address);

	stats.stores++;

	if((config & ac_cache_on) and address < 0x80000000) {
		cache.memStore(address, value);
	} else {
		shell->mem_store(address, value);
	}
}

void CPU::checkAddress(word address) {
	if((cpsr & MODE_MASK) == USR_mode and (address < 0x8000 or address > 0x80000000)) {
		fakeSwi();
	}
}

void CPU::fakeSwi() {
	spsr_svc = cpsr;
	cpsr = (cpsr & 0xFFFFFFE0) | SVC_mode;
	cpsr |= (1 << 7);
	r(lr) = r(pc) + 4;
	r(pc) = 0x8;
}

word CPU::askRegGet(AskMode mode, int index) {
	switch(mode) {
		case am_nil: return (*this)[index];
		case am_usr: return registers[index];
		case am_fiq: 
			if(index > 7 and index < 15) {
				return registers[index + FIQ_offset];
			} else {
				return registers[index];
			}
		case am_irq:
			if(index == sp or index == lr) {
				return registers[index + IRQ_offset];
			} else {
				return registers[index];
			}
		case am_svc:
			if(index == sp or index == lr) {
				return registers[index + SVC_offset];
			} else {
				return registers[index];
			}
		case am_sys: return registers[index];
		case am_max:
		default: shell->panic("Unsupported mode!"); break;
	}
	shell->panic("How did you get here?!");
	exit(0);
}

void CPU::askRegSet(AskMode mode, int index, word value) {
	switch(mode) {
		case am_nil: r(index) = value; break;
		case am_usr: registers[index] = value; break;
		case am_fiq: 
			if(index > 7 and index < 15) {
				registers[index + FIQ_offset] = value;
			} else {
				registers[index] = value;
			}
			break;
		case am_irq:
			if(index == sp or index == lr) {
				registers[index + IRQ_offset] = value;
			} else {
				registers[index] = value;
			}
			break;
		case am_svc:
			if(index == sp or index == lr) {
				registers[index + SVC_offset] = value;
			} else {
				registers[index] = value;
			}
			break;
		case am_sys: registers[index] = value; break;
		case am_max: 
		default: shell->panic("Unsupported mode!"); break;
	}
}