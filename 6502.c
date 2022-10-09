#include "6502.h"
#include "memory.h"
#include <stdio.h>
#include "bitUtils.h"


void addBrCycles(WORD addr)
{
	if (isPageCrossed(addr, registers.PC)) { ticks += 4; }
	else { ticks += 3; }
}

void cpuReset(void)
{
	memset(&registers, 0, sizeof(Registers));
	registers.PC = readWord(0xFFFC);
	registers.S = 0x00ff;
	printf("Entry point: %#02x\n", registers.PC);
	start = clock();
}

void stackPushByte(BYTE data)
{
	writeByte(0x0100 | (WORD)registers.S, data);
	registers.S -= 1;
	printf("Stack now at: %#04x\n", registers.S);

}
void stackPushWord(WORD data)
{
	stackPushByte(HIGH(data));
	stackPushByte(LOW(data));
	printf("Stack now at: %#04x\n", registers.S);
}
BYTE stackPopByte()
{
	registers.S += 1;
	BYTE data = readByte(0x0100 | (WORD)registers.S);
	printf("Stack now: %#04x\n", registers.S);
	return data;
}
WORD stackPopWord()
{
	BYTE lo = stackPopByte();
	BYTE hi = stackPopByte();
	return (WORD)((hi << 8) | lo);
}

void cpuStep(void)
{
	BYTE opCode = readByte(registers.PC);
	WORD opndaddr;
	switch (opCode) {
		//ADC
		case 0x69:
			ticks += 2;
			ADC(readByte(getImmAddr()));
			break;
		case 0x65:
			ticks += 3;
			ADC(readByte(getZpAddr()));
			break;
		case 0x75:
			ticks += 4;
			ADC(readByte(getZpXAddr()));
			break;
		case 0x6D:
			ticks += 4;
			ADC(readByte(getAbsAddr()));
			break;
		case 0x7D:
			opndaddr = getAbsXAddr();
			if (isPageCrossed(opndaddr, registers.PC)) { ticks += 5; }
			else { ticks += 4; }
			ADC(readByte(opndaddr));
			break;
		case 0x79:
			opndaddr = getAbsYAddr();
			if (isPageCrossed(opndaddr, registers.PC)) { ticks += 5; }
			else { ticks += 4; }
			ADC(readByte(opndaddr));
			break;
		case 0x61:
			ticks += 6;
			ADC(readByte(getIndXAddr()));
			break;
		case 0x71:
			opndaddr = getIndYAddr();
			if (isPageCrossed(opndaddr, registers.PC)) { ticks += 6; }
			else { ticks += 5; }
			ADC(readByte(opndaddr));
			break;

		//AND
		case 0x29:
			ticks += 2;
			AND(readByte(getImmAddr()));
			break;
		case 0x25:
			ticks += 3;
			AND(readByte(getZpAddr()));
			break;
		case 0x35:
			ticks += 4;
			AND(readByte(getZpXAddr()));
			break;
		case 0x2D:
			ticks += 4;
			AND(readByte(getAbsAddr()));
			break;
		case 0x3D:
			opndaddr = getAbsXAddr();
			AND(readByte(opndaddr));
			if (isPageCrossed(opndaddr, registers.PC)) { ticks += 5; }
			else { ticks += 4; }
			break;
		case 0x39:
			opndaddr = getAbsYAddr();
			AND(readByte(opndaddr));
			if (isPageCrossed(opndaddr, registers.PC)) { ticks += 5; }
			else { ticks += 4; }
			break;
		case 0x21:
			ticks += 6;
			AND(readByte(getIndXAddr()));
			break;
		case 0x31:
			opndaddr = getAbsXAddr();
			AND(readByte(getIndYAddr()));
			if (isPageCrossed(opndaddr, registers.PC)) { ticks += 6; }
			else { ticks += 5; }
			break;

		//ASL
		case 0x0A:
			ticks += 2;
			ASLACC();
			break;
		case 0x06:
			ticks += 5;
			ASL(getZpAddr());
			break;
		case 0x16:
			ticks += 6;
			ASL(getZpXAddr());
			break;
		case 0x0E:
			ticks += 6;
			ASL(getAbsAddr());
			break;
		case 0x1E:
			ticks += 7;
			ASL(getAbsXAddr());
			break;

		//BCC
		case 0x90:
			opndaddr = getRelAddr();
			BCC(opndaddr);
			addBrCycles(opndaddr);
			break;

		//BCS
		case 0xB0:
			opndaddr = getRelAddr();
			BCS(opndaddr);
			addBrCycles(opndaddr);
			break;

		//BEQ
		case 0xF0:
			opndaddr = getRelAddr();
			BEQ(opndaddr);
			addBrCycles(opndaddr);
			break;

		//BIT
		case 0x24:
			BIT(getZpAddr());
			ticks += 3;
			break;
		case 0x2C:
			BIT(getAbsAddr());
			ticks += 4;
			break;
		//BMI
		case 0x30:
			opndaddr = getRelAddr();
			BMI(opndaddr);
			addBrCycles(opndaddr);
			break;

		//BNE
		case 0xD0:
			opndaddr = getRelAddr();
			BNE(opndaddr);
			addBrCycles(opndaddr);
			break;

		//BPL
		case 0x10:
			opndaddr = getRelAddr();
			BPL(opndaddr);
			addBrCycles(opndaddr);
			break;

		//LDA
		case 0xa9:
			LDA(readByte(getImmAddr()));
			ticks += 2;
			break;
		case 0xa5:
			LDA(readByte(getZpAddr()));
			ticks += 3;
			break;
		case 0xb5:
			LDA(readByte(getZpXAddr()));
			ticks += 4;
			break;
		case 0xad:
			LDA(readByte(getAbsAddr()));
			ticks += 4;
			break;
		case 0xBD:
			opndaddr = getAbsXAddr();
			LDA(readByte(opndaddr));
			if (isPageCrossed(opndaddr, registers.PC)) { ticks += 5; }
			else { ticks += 4; }
			break;
		case 0xB9:
			opndaddr = getAbsYAddr();
			LDA(readByte(opndaddr));
			if (isPageCrossed(opndaddr, registers.PC)) { ticks += 5; }
			else { ticks += 4; }
			break;
		case 0xA1:
			opndaddr = getIndXAddr();
			LDA(readByte(opndaddr));
			ticks += 6;
			break;
		case 0xB1:
			opndaddr = getIndYAddr();
			LDA(readByte(opndaddr));
			if (isPageCrossed(opndaddr, registers.PC)) { ticks += 6; }
			else { ticks += 5; }
			break;

		//STA
		case 0x85:
			STA(getZpAddr());
			ticks += 2;
			break;
		case 0x95:
			STA(getZpXAddr());
			ticks += 4;
			break;
		case 0x8D:
			STA(getAbsAddr());
			ticks += 4;
			break;
		case 0x9D:
			STA(getAbsXAddr());
			ticks += 5;
			break;
		case 0x99:
			STA(getAbsYAddr());
			ticks += 5;
			break;
		case 0x81:
			STA(getIndXAddr());
			ticks += 6;
			break;
		case 0x91:
			STA(getIndYAddr());
			ticks += 6;
			break;

		//LDX
		case 0xa2:
			LDX(readByte(getImmAddr()));
			ticks += 2;
			break;
		case 0xa6:
			LDX(readByte(getZpAddr()));
			ticks += 3;
			break;
		case 0xb6:
			LDX(readByte(getZpYAddr()));
			ticks += 4;
			break;
		case 0xae:
			LDX(readByte(getAbsAddr()));
			ticks += 4;
			break;
		case 0xbe:
			opndaddr = getAbsYAddr();
			LDX(readByte(opndaddr));
			if (isPageCrossed(opndaddr, registers.PC)) { ticks += 5; }
			else { ticks += 4; }
			break;

		//INX
		case 0xe8:
			INX();
			ticks += 2;
			registers.PC += 1;
			break;

		//STX
		case 0x86:
			STX(getZpAddr());
			ticks += 2;
			break;
		case 0x96:
			STX(getZpYAddr());
			ticks += 4;
			break;
		case 0x8e:
			STX(getAbsAddr());
			ticks += 4;
			break;

		//JMP
		case 0x4c:
			JMP(getAbsAddr());
			ticks += 3;
			break;
		case 0x6c:
			JMP(getAbsAddr());
			ticks += 5;
			break;

		default:
			printf("Unknown instruction: %#02x\n", opCode);
		}
	
}

void updateNZ(BYTE data)
{
	registers.P.N = data >> 8;
	if (data == 0) { registers.P.Z = true; }
	else { registers.P.Z = false; }
}

	WORD getAbsAddr()
	{
		registers.PC += 2;
		return readWord(registers.PC - 1);

	}

	WORD getAbsXAddr()
	{
		registers.PC += 2;
		return readWord(registers.PC - 1) + registers.X;
	}

	WORD getAbsYAddr()
	{
		registers.PC += 2;
		return readWord(registers.PC - 1) + registers.Y;
	}

	WORD getImmAddr()
	{
		registers.PC += 2;
		return registers.PC - 1;
	}

	WORD getIndXAddr()
	{
		registers.PC += 2;
		return readWord(readByte(registers.PC - 1) + registers.X);
	}

	WORD getIndYAddr()
	{
		registers.PC += 2;
		return readWord(readWord(readByte(registers.PC - 1)) + registers.Y);
	}

	WORD getRelAddr()
	{
		return registers.PC + readByte(registers.PC++);
	}

	WORD getZpAddr()
	{
		registers.PC += 2;
		return readByte(registers.PC - 1);
	}

	WORD getZpXAddr()
	{
		registers.PC += 2;
		return readByte(registers.PC - 1) + registers.X;
	}

	WORD getZpYAddr()
	{
		registers.PC += 2;
		return readByte(registers.PC - 1) + registers.Y;
	} 

void ADC(BYTE data)
{
	BYTE sum = registers.A + data + registers.P.C;
	if (registers.P.D) { //BCD
		if (((registers.A ^ data ^ sum) & 0x10) == 0x10)
		{
			sum += 0x06;
		}
		if ((sum & 0xf0) > 0x90)
		{
			sum += 0x60;
		}
	}
	registers.P.C = sum > 0xFF; //Carry
	registers.P.V = ~(registers.A ^ data) & (registers.A ^ sum) & 0x80; //Overflow
	registers.A = sum;
	updateNZ(sum);
	printf("ADC - i=%#02x, o=%#02x\n", data, sum);
}

void AND(BYTE data)
{
	registers.A &= data;
	updateNZ(registers.A);
}

void ASL(WORD addr)
{
	registers.P.C = internalMemory[addr] >> 8;
	internalMemory[addr] = internalMemory[addr] << 1;
	updateNZ(internalMemory[addr]);}

void ASLACC() {
	registers.P.C = registers.A >> 8;
	registers.A = registers.A << 1;
	updateNZ(registers.A);}

void BCC(WORD addr) {
	if (!registers.P.C) {
		registers.PC = addr;
	}
}

void BCS(WORD addr) {
	if (registers.P.C) {
		registers.PC = addr;
	}
} 

void BEQ(WORD addr) {
	if (registers.P.Z) {
		registers.PC = addr;
	}
}

void BIT(BYTE data) {
	data = data & registers.A;
	registers.P.N = GetBit(data, 7);
	registers.P.V = GetBit(data, 6);
}
void BMI(WORD addr) {
	if (registers.P.N == 1) {
		registers.PC = addr;
	}
}
void BNE(WORD addr) {
	if (registers.P.Z == 0) {
		registers.PC = addr;
	}
}
void BPL(WORD addr) {
	if (registers.P.N == 0) {
		registers.PC = addr;
	}
}
void BRK() {
	//TODO BRK
}
//.... 
void LDA(BYTE data) {
	registers.A = data;
	updateNZ(registers.A);
}
void STA(WORD addr) {
	internalMemory[addr] = registers.A;
}

void LDX(BYTE data)
{
	registers.X = data;
	updateNZ(registers.X);
}
void INX() {
	registers.X += 1;
	updateNZ(registers.X);
}
void STX(WORD addr) {
	internalMemory[addr] = registers.X;
}
void JMP(WORD addr) {
	registers.PC = addr & 0x1fff;
}