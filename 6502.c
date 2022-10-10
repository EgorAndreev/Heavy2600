#include "6502.h"
#include "memory.h"
#include <stdio.h>
#include "bitUtils.h"

void cpuReset(void)
{
	memset(&registers, 0, sizeof(Registers));
	registers.PC = readWord(0xFFFC);
	registers.S = 0x00ff;
	printf("Entry point: %#02x\n", registers.PC);
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
			ADC(readByte(getImmAddr()));
			break;
		case 0x65:
			ADC(readByte(getZpAddr()));
			break;
		case 0x75:
			ADC(readByte(getZpXAddr()));
			break;
		case 0x6D:
			ADC(readByte(getAbsAddr()));
			break;
		case 0x7D:
			ADC(readByte(getAbsXAddr()));
			break;
		case 0x79:
			ADC(readByte(getAbsYAddr()));
			break;
		case 0x61:
			ADC(readByte(getIndXAddr()));
			break;
		case 0x71:
			ADC(readByte(getIndYAddr()));
			break;

		//AND
		case 0x29:
			AND(readByte(getImmAddr()));
			break;
		case 0x25:
			AND(readByte(getZpAddr()));
			break;
		case 0x35:
			AND(readByte(getZpXAddr()));
			break;
		case 0x2D:
			AND(readByte(getAbsAddr()));
			break;
		case 0x3D:
			AND(readByte(getAbsXAddr()));
			break;
		case 0x39:
			AND(readByte(getAbsYAddr()));
			break;
		case 0x21:
			AND(readByte(getIndXAddr()));
			break;
		case 0x31:
			AND(readByte(getIndYAddr()));
			break;
		//ASL
		case 0x0A:
			ASLACC();
			registers.PC += 1;
			break;
		case 0x06:
			ASL(getZpAddr());
			break;
		case 0x16:
			ASL(getZpXAddr());
			break;
		case 0x0E:
			ASL(getAbsAddr());
			break;
		case 0x1E:
			ASL(getAbsXAddr());
			break;
		//BCC
		case 0x90:
			BCC(getRelAddr());
			break;

		//BCS
		case 0xB0:
			BCS(getRelAddr());
			break;

		//BEQ
		case 0xF0:
			BEQ(getRelAddr());
			break;

		//BIT
		case 0x24:
			BIT(getZpAddr());
			break;
		case 0x2C:
			BIT(getAbsAddr());
			break;
		//BMI
		case 0x30:
			BMI(getRelAddr());
			break;
		//BNE
		case 0xD0:
			BNE(getRelAddr());
			break;
		//BPL
		case 0x10:
			BPL(getRelAddr());
			break;
		//LDA
		case 0xa9:
			LDA(readByte(getImmAddr()));
			break;
		case 0xa5:
			LDA(readByte(getZpAddr()));
			break;
		case 0xb5:
			LDA(readByte(getZpXAddr()));	 
			break;
		case 0xad:
			LDA(readByte(getAbsAddr())); 
			break;
		case 0xBD:
			LDA(readByte(getAbsXAddr()));
			break;
		case 0xB9:
			LDA(readByte(getAbsYAddr()));
			break;
		case 0xA1:
			LDA(readByte(getIndXAddr()));
			break;
		case 0xB1:
			LDA(readByte(getIndYAddr()));
			break;

		//STA
		case 0x85:
			STA(getZpAddr()); 
			break;
		case 0x95:
			STA(getZpXAddr());
			break;
		case 0x8D:
			STA(getAbsAddr()); 
			break;
		case 0x9D:
			STA(getAbsXAddr());
			break;
		case 0x99:
			STA(getAbsYAddr());
			break;
		case 0x81:
			STA(getIndXAddr());
			break;
		case 0x91:
			STA(getIndYAddr());
			 
			break;

		//LDX
		case 0xa2:
			LDX(readByte(getImmAddr()));
			 
			break;
		case 0xa6:
			LDX(readByte(getZpAddr()));
			 
			break;
		case 0xb6:
			LDX(readByte(getZpYAddr()));
			 
			break;
		case 0xae:
			LDX(readByte(getAbsAddr()));
			break;
		case 0xbe:
			LDX(readByte(getAbsYAddr()));
			break;

		//INX
		case 0xe8:
			INX();
			registers.PC += 1;
			break;

		//STX
		case 0x86:
			STX(getZpAddr());
			 
			break;
		case 0x96:
			STX(getZpYAddr());
			 
			break;
		case 0x8e:
			STX(getAbsAddr());
			 
			break;

		//JMP
		case 0x4c:
			JMP(getAbsAddr());
			 
			break;
		case 0x6c:
			JMP(getAbsAddr());
			 
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