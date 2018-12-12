// File:	cpu.hpp
// Author:	Sam Henry
// Class:	CpS 310, Microprocessor Architecture
// Date:	October 6, 2016
// Descr:	Contains all of the methods and data for the
//				simulated CPU

#ifndef CPU_H
#define CPU_H

#include <thread>
using std::thread;

#include "types.hpp"
#include "instruction.hpp"
#include "util.hpp"
#include "cache.hpp"

#define NUM_REGISTERS 27			// How many registers?
#define ip 12
#define sp 13
#define lr 14
#define pc 15
#define SVC_offset 3
#define IRQ_offset 5
#define FIQ_offset 12

#define SYS_mode 0b11111
#define SVC_mode 0b10011
#define USR_mode 0b10000
#define IRQ_mode 0b10010
#define FIQ_mode 0b10001

#define NF 31
#define ZF 30
#define CF 29
#define VF 28

#define MODE_MASK 0x1F

// This singleton class contains all of the methods and data for the CPU.
// It also contains a pointer to the shell, so it can talk to the shell.
class CPU {
private:
	word registers[NUM_REGISTERS]; 	// The simulated registers

public:
	word cpsr;
	word spsr_svc;
	word spsr_irq;
	word spsr_fiq;

	AskStats stats;					// Simulator statistics
	AskConfig config;				// The configuration flags
	const AskHostServices *shell; 	// So we can talk to the shell

	Cache cache;

	bool irq;
	bool fiq;
	bool halt;
	bool running;					// Is the CPU running?
	int cycles;						// How many cycles to run?
	int step;						// Current instruction

	// Initializes the registers and other stuffs
	void init();

	// Starts the CPU running
	// <cycles> Runs infinitely if 0 cycles, or the amount of cycles.
	void start(int initCycles);

	// Makes the CPU run the fetch, execute, decode cycle.
	void run();

	// If the IRQ interrupt pin is set, then switch to IRQ mode.
	void checkIRQ();

	// If the FIQ interrupt pin is set, then switch to FIQ mode.
	void checkFIQ();

	// Checks a flag and returns if it is set or not
	// <code> The condition code
	bool check(byte code);

	// Returns true or false based on the flags and condition
	// <condition> The condition to test
	bool conditionPassed(byte condition);

	// Set a flag to a new value
	// <code> The flag to set
	// <flag> The new value
	void setFlag(byte code, bool flag);

	// Changes from any exception or other mode back to SYS mode.
	void restoreCPSR();

	// Get the current mode's SPSR, if one exists. Else panic.
	word &getSPSR();

	// Is the processor in privileged mode?
	bool isPrivileged();

	// Looks to see what type the instruction is, and then returns the subclass fitting it.
	Instruction *getNextInstruction();

	// Returns a reference to the accessed register
	word &r(word index);

	// Overload of bracket operators for easy indexing into registers.
	word operator[](word index);

	// Loads a word from memory, performing checks based on mode.
	// <address> The address to load from.
	word memLoad(word address);

	// Stores a word to memory at the given address
	// <address> The address to store to.
	// <value> The value to store to memory
	void memStore(word address, word value);

	// Makes sure an address being loaded or stored is valid.
	void checkAddress(word address);

	// A USR mode memory access
	void fakeSwi();

	// Sets or gets a register based on an AskMode.
	// <mode> The register bank to choose from
	// <index> The register number
	// <value> If setting, the value to set it to
	word askRegGet(AskMode mode, int index);
	void askRegSet(AskMode mode, int index, word value);

private:
	// The singleton instance
	static CPU *_instance;

public:
	// Get the singleton instance
	// <return> The static instance
	static CPU &instance();

	// Info about the simulator
	static const char *info[8];
};

#endif //CPU_H