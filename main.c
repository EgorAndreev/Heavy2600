#define MOS6507MODE
#include "6502.h"
#include "memory.h"
#include <stdio.h>
#include "bitUtils.h"
#include "tia.h"
#include "windows.h"

int main(int argc, char* argv[]) {
	initMemory("kernel_13.bin");
	cpuReset();
	initGraph();
	while (true) {
		int c = cpuStep();
		
		for (int i = 0; i < c; i++) {
			tiaStep(getWsyncState());
		}
		
	}
	getchar();
}