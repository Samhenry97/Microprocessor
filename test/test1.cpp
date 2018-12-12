#include <iostream>
#include <cassert>

#include "../lib/cpu.hpp"
#include "../lib/dataprocess.hpp"
#include "../lib/loadstore.hpp"
#include "../lib/instruction.hpp"
#include "../lib/operand2.hpp"

using namespace std;

int main() {
	CPU::instance().init();

	IDataProcess i(0xE3A00014);			//mov r0, #20
	i.decode();
	i.execute();
	i.print();

	assert(CPU::instance()[0] == 20);	

	i = IDataProcess(0xE3A01032);		//mov r1, #50
	i.decode();
	i.execute();
	i.print();

	assert(CPU::instance()[1] == 50);

	i = IDataProcess(0xE3A03001);		//mov r3, #1
	i.decode();
	i.execute();
	i.print();

	assert(CPU::instance()[3] == 1);

	i = IDataProcess(0xE1A02311);		//mov r2, r1, lsl r3
	i.decode();
	i.execute();
	i.print();

	assert(CPU::instance()[2] == 100);

	i = IDataProcess(0xE1A02082); 		//mov r2, r2, lsl #1
	i.decode();
	i.execute();
	i.print();

	assert(CPU::instance()[2] == 200);

	ILoadStore ls(0xE5932100);			//ldr r2, [r3, #256]
	ls.decode();
	ls.print();

	i = IDataProcess(0xE2822032);		//add r2, r2, #50
	i.decode();
	i.execute();
	i.print();
	assert(CPU::instance()[2] == 250);

	cout << "ALL TESTS PASSED!" << endl;

	return 0;
}