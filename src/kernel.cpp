// Author:      Sam Henry
// Class:       CpS 310: Microprocessor Archictecture
// Desc:        Simulates an ARM processor
// Date:        September 17, 2016

#include <cstddef>

// ArmSim Kernel/Shell Interface definition
#include "../lib/types.hpp"
#include "../lib/cpu.hpp"

char const **ask_info(void) {
    return CPU::instance().info;
}

// This function initializes the CPU, setting all the 
// registers and flags to 0, and then logging a message
// back to the host.
// <host> The shell calling the initialize function
void ask_init(const AskHostServices *host) {
    CPU::instance().init();
    CPU::instance().shell = host;

    host->log_msg("CPU initialized");
}

// This method sets the current configuration flag to the new flag given.
// <flags> The new flags
void ask_config_set(AskConfig flags) {
    CPU::instance().config = flags;
}

// This method returns the current configuration flag.
AskConfig ask_config_get(void) {
    return CPU::instance().config;
}

// This method returns the statistics of the processor.
// <statistics> The stats from the host
void ask_stats_report(AskStats *stats) {
    //Since there are no pointers in the struct, a straight-up copy is fine.
    *stats = CPU::instance().stats;
}

// This method gets the current value of the given register
// <bank> TBA
// <index> The index of the register to get
word ask_reg_get(AskMode bank, int index) {
    return CPU::instance().askRegGet(bank, index);
}

// This method sets the value of the given register
// <bank> TBA
// <index> The register to set
// <value> The value to set the register to.
void ask_reg_set(AskMode bank, int index, word value) {
    CPU::instance().askRegSet(bank, index, value);
}

word ask_cpsr_get(void) {
    return CPU::instance().cpsr;
}

void ask_cpsr_set(word value) {
    CPU::instance().cpsr = value;
}

int	ask_cpu_running(void) {
    return CPU::instance().running;
}

void ask_cpu_signal(AskSignal signal) {
    switch(signal) {
        case as_halt: CPU::instance().halt = true; return;;
        case as_irq: CPU::instance().irq = true; return;
        case as_fiq: CPU::instance().fiq = true; return;
        default: return;
    }
}

// This method adds the number of cycles to the instruction count.
// <cycles> The amount of cycles to add
int	ask_cpu_run(int cycles) {
    CPU &cpu = CPU::instance();
    cpu.start(cycles);

    return cpu.stats.instructions;
}