#include "6502.h"
#include "memory.h"
#include <stdio.h>
#include "bitUtils.h"
void logReg()
{
	printf("Stack now at: %#04x\n", registers.S);
	printf("X now: %#04x\n", registers.X);
	printf("Y now: %#04x\n", registers.Y);
	printf("A now: %#04x\n", registers.A);
	printf("PC now at: %#04x\n", registers.PC);


}
bool* getWsyncState() {
	return &(wsynced);
}
void cpuReset(void)
{
	memset(&registers, 0, sizeof(Registers));
	registers.PC = readWord(0xFFFC);
	registers.S = 0x00ff;
	registers.P.B = true;
	registers.P.Z = false;
	registers.P.I = true;
	registers.P.D = false;
	registers.P.B = true;
	registers.P.V = false;
	registers.P.N = true;
	PHP();
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

WORD getImmAddr()
{
	registers.PC += 2;
	return registers.PC - 1;
}

WORD getZpAddr()
{
	registers.PC += 2;
	return readByte(registers.PC - 1);
}

WORD getZpXAddr()
{
	return getZpAddr() + registers.X;
}

WORD getZpYAddr()
{
	return getZpAddr() + registers.Y;
}

WORD getAbsAddr()
{
	registers.PC += 3;
	return readWord(registers.PC - 2);
}
WORD getAbsXAddr()
{
	return getAbsAddr() + registers.X;
}
WORD getAbsYAddr()
{
	return getAbsAddr() + registers.Y;
}

WORD getIndAddr()
{
	registers.PC += 3;
	return readWord(registers.PC - 2);
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
	registers.PC += 2;
	return registers.PC + (SIGNED_BYTE)readByte(registers.PC - 1);
}

int cpuStep(void)
{
	BYTE cycles = 0;
	BYTE opCode = readByte(registers.PC);
	BYTE H = (opCode >> 1 * 4) & 0x0F;
	BYTE L = (opCode >> 0 * 4) & 0x0F;
	OpCode o = allOpcodes[H][L];
	WORD oldPc = registers.PC;
	WORD addr = 0;
	switch (o.addressing) {
	case ImplAddr:
		registers.PC += 1;
		break;
	case ImmAddr:
		addr = getImmAddr();
		break;
	case ZpAddr:
		addr = getZpAddr();
		break;
	case ZpXAddr:
		addr = getZpXAddr();
		break;
	case ZpYAddr:
		addr = getZpYAddr();
		break;
	case AbsAddr:
		addr = getAbsAddr();
		break;
	case AbsXAddr:
		addr = getAbsXAddr();
		break;
	case AbsYAddr:
		addr = getAbsYAddr();
		break;
	case IndAddr:
		addr = getIndAddr();
		break;
	case IndXAddr:
		addr = getIndXAddr();
		break;
	case IndYAddr:
		addr = getIndYAddr();
		break;
	case RelAddr:
		addr = getRelAddr();
		break;
	}
	
	o.handler(addr);
	if (o.isCrossCycles == true) {
		if (o.addressing == RelAddr) {
			if (isPageCrossed(registers.PC, oldPc)) {
				cycles += o.cycles + 1;
			}
			else { cycles += o.cycles + 2; }
		}
		if (isPageCrossed(addr, oldPc)) { cycles += o.cycles + 1; }
	}
	else { cycles += o.cycles; }
	return cycles;


}

void updateNZ(BYTE data)
{
	registers.P.N = data < 0;
	registers.P.Z = data == 0;
}




void ADC(WORD addr)
{
	BYTE sum = registers.A + readByte(addr) + registers.P.C;
	if (registers.P.D) { //BCD
		if (((registers.A ^ readByte(addr) ^ sum) & 0x10) == 0x10)
		{
			sum += 0x06;
		}
		if ((sum & 0xf0) > 0x90)
		{
			sum += 0x60;
		}
	}
	registers.P.C = sum & 0x01; //Carry
	registers.P.V = ~(registers.A ^ readByte(addr)) & (registers.A ^ sum) & 0x80; //Overflow
	registers.A = sum;
	updateNZ(sum);
}

void AND(WORD addr)
{
	registers.A &= readByte(addr);
	updateNZ(registers.A);
}

void ASL(WORD addr)
{
	registers.P.C = internalMemory[addr] >> 8;
	internalMemory[addr] = internalMemory[addr] << 1;
	updateNZ(internalMemory[addr]);
}

void ASLACC() {
	registers.P.C = registers.A >> 8;
	registers.A = registers.A << 1;
	updateNZ(registers.A);
}

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

void BIT(WORD addr) {
	internalMemory[addr] = internalMemory[addr] & registers.A;
	registers.P.N = GetBit(internalMemory[addr], 7);
	registers.P.V = GetBit(internalMemory[addr], 6);
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
void BVC(WORD addr)
{
	if (registers.P.V == 0) {
		registers.PC = addr;
	}
}
void BVS(WORD addr)
{
	if (registers.P.V == 1) {
		registers.PC = addr;
	}
}
void CLC()
{
	registers.P.C = 0;
}
void CLD()
{
	registers.P.D = 0;
}
void CLI()
{
	registers.P.I = 0;
}
void CLV()
{
	registers.P.V = 0;
}
void CMP(WORD addr)
{
	BYTE res;
	res = registers.A - readByte(addr);
	registers.P.C = res & 0x01; //Carry
	updateNZ(res);
}
void CPX(WORD addr)
{
	BYTE res;
	res = registers.X - readByte(addr);
	registers.P.C = res & 0x01; //Carry
	updateNZ(res);
}
void CPY(WORD addr)
{
	BYTE res;
	res = registers.Y - readByte(addr);
	registers.P.C = res & 0x01; //Carry
	updateNZ(res);
}
void DEC(WORD addr)
{
	internalMemory[addr] -= 1;
	updateNZ(internalMemory[addr]);
}
void DEX()
{
	registers.X -= 1;
	updateNZ(registers.X);
}
void DEY()
{
	registers.Y -= 1;
	updateNZ(registers.Y);
}
void EOR(WORD addr)
{
	registers.A = !registers.A != !readByte(addr);//XOR
	updateNZ(registers.A);
}
void INC(WORD addr)
{
	internalMemory[addr] += 1;
	updateNZ(internalMemory[addr]);
}
void INX() {
	registers.X += 1;
	updateNZ(registers.X);
}
void INY()
{
	registers.Y += 1;
	updateNZ(registers.Y);
}

void LDA(WORD addr) {
	registers.A = readByte(addr);
	updateNZ(registers.A);
}
void STA(WORD addr) {
	internalMemory[addr] = registers.A;
	if (addr == 0x2) {
		wsynced = true;
	}
}

void LDX(WORD addr)
{
	registers.X = readByte(addr);
	updateNZ(registers.X);
}

void LDY(WORD addr)
{
	registers.Y = readByte(addr);
	updateNZ(registers.X);
}

void LSRACC()
{
	registers.A = registers.A >> 1;
	registers.P.Z = 0;
	registers.P.C = registers.A & 0x01; //Carry
}

void LSR(WORD addr)
{
	internalMemory[addr] = internalMemory[addr] >> 1;
	registers.P.Z = 0;
	registers.P.C = registers.A & 0x01; //Carry
}


void ORA(WORD addr)
{
	registers.A = registers.A & internalMemory[addr];
	updateNZ(registers.A);
}

void PHA()
{
	stackPushByte(registers.A);
}

void PHP()
{
	int* i = &registers.P;
	registers.P.NONE = 1;
	stackPushByte(i);
}

void PLA()
{
	registers.A = stackPopByte();
	updateNZ(registers.A);
}

void PLP()
{
	bool oldB = registers.P.B;
	BYTE d = stackPopByte();
	StateReg* st = &d;
	registers.P = *st;
	registers.P.B = (_Bool)oldB;
}

void ROLACC()
{
	registers.P.C = GetBit(registers.A, 7);
	registers.A = registers.A << 1;
	registers.A = SetBit(registers.A, 0);
	updateNZ(registers.A);
}

void ROL(WORD addr)
{
	registers.P.C = GetBit(internalMemory[addr], 7);
	internalMemory[addr] = internalMemory[addr] << 1;
	internalMemory[addr] = SetBit(internalMemory[addr], 0);
	updateNZ(internalMemory[addr]);
}

void RORACC()
{
	registers.P.C = GetBit(registers.A, 0);
	registers.A = registers.A >> 1;
	registers.A = SetBit(registers.A, 7);
	updateNZ(registers.A);
}

void ROR(WORD addr)
{
	registers.P.C = GetBit(internalMemory[addr], 0);
	internalMemory[addr] = internalMemory[addr] >> 1;
	internalMemory[addr] = SetBit(internalMemory[addr], 7);
	updateNZ(internalMemory[addr]);
}

void RTI()
{
	PLP();
	registers.PC = stackPopWord();
}

void RTS()
{
	registers.PC = stackPopWord();
	registers.PC += 1;
}

void SBC(WORD addr)
{
	ADC(~internalMemory[addr]);
}

void SEC()
{
	registers.P.C = true;
}

void SED()
{
	registers.P.D = true;
}

void SEI()
{
	registers.P.I = true;
}

void STX(WORD addr)
{
	internalMemory[addr] = registers.X;
}
void STY(WORD addr)
{
	internalMemory[addr] = registers.Y;
}
void TAX()
{
}
void TAY()
{
}
void TSX()
{
}
void TXA()
{
}
void TXS()
{
}
void TYA()
{
}
void NOP()
{
	//nop, just nop
}
void JMP(WORD addr)
{
	registers.PC = addr & 0x1fff;
}
void JSR(WORD addr)
{
	stackPushWord(registers.PC + 2);
	registers.PC = addr & 0x1fff;
}
