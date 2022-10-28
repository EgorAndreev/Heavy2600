#include "6502.h"
#include "memory.h"
#include <stdio.h>
#include "bitUtils.h"
bool* getWsyncState() {
	return &(wsynced);
}
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
	static x = 0;
	x++;
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
		//BRK
		case 0x00:
			BRK();
			registers.PC += 1;
			break;
		//BVC
		case 0x50:
			BVC(getRelAddr());
			break;
		//BVS
		case 0x70:
			BVS(getRelAddr());
			break;
		//CLC
		case 0x18:
			CLC();
			registers.PC += 1;
			break;
		//CLD
		case 0xD8:
			CLD();
			registers.PC += 1;
			break;
		//CLI
		case 0x58:
			CLI();
			registers.PC += 1;
			break;
		//CLV
		case 0xB8:
			CLV();
			registers.PC += 1;
			break;
		//CMP
		case 0xc9 :
			CMP(readByte(getImmAddr()));
			break;
		case 0xc5:
			CMP(readByte(getZpAddr()));
			break;
		case 0xd5:
			CMP(readByte(getZpXAddr()));
			break;
		case 0xcd:
			CMP(readByte(getAbsAddr()));
			break;
		case 0xdd:
			CMP(readByte(getAbsXAddr()));
			break;
		case 0xd9:
			CMP(readByte(getAbsYAddr()));
			break;
		case 0xc1:
			CMP(readByte(getIndXAddr()));
			break;
		case 0xd1:
			CMP(readByte(getIndYAddr()));
			break;
		//CPX
		case 0xe0:
			CPX(readByte(getImmAddr()));
			break;
		case 0xe4:
			CPX(readByte(getZpAddr()));
			break;
		case 0xec:
			CPX(readByte(getAbsAddr()));
			break;
		//CPY
		case 0xc0:
			CPY(readByte(getImmAddr()));
			break;
		case 0xc4:
			CPY(readByte(getZpAddr()));
			break;
		case 0xcc:
			CPY(readByte(getAbsAddr()));
			break;
		//DEC
		case 0xc6:
			DEC(getZpAddr());
			break;
		case 0xd6:
			DEC(getZpXAddr());
			break;
		case 0xce:
			DEC(getAbsAddr());
			break;
		case 0xde:
			DEC(getAbsXAddr());
			break;
		//DEX
		case 0xca:
			DEX();
			registers.PC += 1;
			break;
		//DEY
		case 0x88:
			DEY();
			registers.PC += 1;
			break;
		//EOR
		case 0x49:
			EOR(readByte(getImmAddr()));
			break;
		case 0x45:
			EOR(readByte(getZpAddr()));
			break;
		case 0x55:
			EOR(readByte(getZpXAddr()));
			break;
		case 0x4d:
			EOR(readByte(getAbsAddr()));
			break;
		case 0x5d:
			EOR(readByte(getAbsXAddr()));
			break;
		case 0x59:
			EOR(readByte(getAbsYAddr()));
			break;
		case 0x41:
			EOR(readByte(getIndXAddr()));
			break;
		case 0x51:
			EOR(readByte(getIndYAddr()));
			break;
		//INC
		case 0xe6:
			INC(getZpAddr());
			break;
		case 0xf6:
			INC(getZpXAddr());
			break;
		case 0xee:
			INC(getAbsAddr());
			break;
		case 0xfe:
			INC(getAbsXAddr());
			break;
		//INX
		case 0xe8:
			INX();
			registers.PC += 1;
			break;
		//INY
		case 0xc8:
			INY();
			registers.PC += 1;
			break;
		//JMP
		case 0x4c:
			JMP(getAbsAddr());
			break;
		case 0x6c:
			JMP(getAbsAddr()); // INDERECT, BUT WHY????
			break;
		//JSR
		case 0x20:
			JSR(getAbsAddr());
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
		//LDY
		case 0xa0:
			LDY(readByte(getImmAddr()));
			break;
		case 0xa4:
			LDY(readByte(getZpAddr()));
			break;
		case 0xb4:
			LDY(readByte(getZpXAddr()));
			break;
		case 0xac:
			LDY(readByte(getAbsAddr()));
			break;
		case 0xbc:
			LDY(readByte(getAbsXAddr()));
			break;
		//LSR
		case 0x4A:
			LSRACC();
			break;
		case 0x46:
			LSR(getZpAddr());
			break;
		case 0x56:
			LSR(getZpXAddr());
			break;
		case 0x4E:
			LSR(getAbsAddr());
			break;
		case 0x5E:
			LSR(getAbsXAddr());
			break;
		//NOP
		case 0xea:
			registers.PC += 1;
			break;
		//ORA
		case 0x09:
			ORA(getImmAddr());
			break;
		case 0x05:
			ORA(getZpAddr());
			break;
		case 0x15:
			ORA(getZpXAddr());
			break;
		case 0x0D:
			ORA(getAbsAddr());
			break;
		case 0x1D:
			ORA(getAbsXAddr());
			break;
		case 0x19:
			ORA(getAbsYAddr());
			break;
		case 0x01:
			ORA(getIndXAddr());
			break;
		case 0x11:
			ORA(getIndYAddr());
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


		default:
			printf("Unknown instruction: %#02x\n", opCode);
		}
	
}

void updateNZ(BYTE data)
{
	registers.P.N = data < 0;
	registers.P.Z = data == 0;
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
	registers.P.C = sum & 0x01; //Carry
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
void CMP(BYTE data)
{
	BYTE res;
	res = registers.A - data;
	registers.P.C = res & 0x01; //Carry
	updateNZ(res);
}
void CPX(BYTE data)
{
	BYTE res;
	res = registers.X - data;
	registers.P.C = res & 0x01; //Carry
	updateNZ(res);
}
void CPY(BYTE data)
{
	BYTE res;
	res = registers.Y - data;
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
void EOR(BYTE data)
{
	registers.A = !registers.A != !data;//XOR
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
 
void LDA(BYTE data) {
	registers.A = data;
	updateNZ(registers.A);
}
void STA(WORD addr) {
	internalMemory[addr] = registers.A;
	if (addr == 0x2) {
		wsynced = true;
	}
}

void LDX(BYTE data)
{
	registers.X = data;
	updateNZ(registers.X);
}

void LDY(BYTE data)
{
	registers.Y = data;
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

void ORA(BYTE data)
{
	registers.A = registers.A & data;
	updateNZ(registers.A);
}

void STX(WORD addr) {
	internalMemory[addr] = registers.X;
}
void JMP(WORD addr) {
	registers.PC = addr & 0x1fff;
}
void JSR(WORD addr)
{
	stackPushWord(registers.PC + 2);
	registers.PC = addr & 0x1fff;
}
