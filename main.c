#define MOS6507MODE
#include "6502.h"
#include "memory.h"
#include <stdio.h>
#include "bitUtils.h"
#include "tia.h"
#include "windows.h"

int main(int argc, char* argv[]) {
	initMemory("kernel_01.bin");
	cpuReset();
	initGraph();
	while (true) {
		if (!getCpuState()) {
			cpuStep();
		}
		tiaStep();
	}
	getchar();
}