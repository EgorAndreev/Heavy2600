#include "types.h"
#include <stdbool.h>
#include <time.h>
#ifndef __6502__
#define __6502__
typedef struct {
		bool C : 1;
		bool Z : 1;
		bool I : 1;
		bool D : 1;
		bool B : 1;
		bool NONE : 1;
		bool V : 1;
		bool N : 1;
} StateReg;
typedef struct {
	BYTE A;
	BYTE S;
	WORD PC;
	StateReg P;
	BYTE X; BYTE Y;
} Registers;
static Registers registers = { 0,0,0,0,0,0 };
//Процессор остановлен
static bool wsynced = false;
bool* getWsyncState();
void ADC(WORD data);
void AND(WORD data);
void ASL(WORD addr);
void ASLACC();
void BCC(WORD addr);
void BCS(WORD addr);
void BEQ(WORD addr);
void BIT(WORD data);
void BMI(WORD addr);
void BNE(WORD addr);
void BPL(WORD addr);
void BRK();
void BVC(WORD addr);
void BVS(WORD addr);
void CLC();
void CLD();
void CLI();
void CLV();
void CMP(WORD data);
void CPX(WORD data);
void CPY(WORD data);
void DEC(WORD addr);
void DEX();
void DEY();
void EOR(WORD data);
void INC(WORD addr);
void INX();
void INY();
void JMP(WORD addr);
void JSR(WORD addr);
void LDA(WORD data);
void LDX(WORD data);
void LDY(WORD data);
void LSRACC();
void LSR(WORD addr);
void ORA(WORD data);
void PHA();
void PHP();
void PLA();
void PLP();
void ROLACC();
void ROL(WORD addr);
void RORACC();
void ROR(WORD addr);
void RTI();
void RTS();
void SBC(WORD data);
void SEC();
void SED();
void SEI();
void STA(WORD addr);
void STX(WORD addr);
void STY(WORD addr);
void TAX();
void TAY();
void TSX();
void TXA();
void TXS();
void TYA();
void NOP();
#endif 

//Опкоды
typedef enum enumAddrMode {
	ImplAddr = 0,
	ImmAddr,
	ZpAddr,
	ZpXAddr,
	ZpYAddr,
	AbsAddr,
	AbsXAddr,
	AbsYAddr,
	IndAddr,
	IndXAddr,
	IndYAddr,
	RelAddr,
	AccAddr
} AddrMode;
typedef void(*opcHandler)(WORD);
typedef struct structOpCode {
	opcHandler handler;
	BYTE cycles;
	AddrMode addressing;
	bool isCrossCycles;
}OpCode;
static OpCode allOpcodes[16][16] = {
	{{&BRK, 7, ImplAddr, false}, {&ORA, 6, IndXAddr, false}, {0}, {0}, {0}, {&ORA, 3, ZpAddr, false}, {&ASL, 5, ZpAddr, false}, {0}, {&PHP, 3, ImplAddr, false}, {&ORA, 2, ImmAddr, false}, {&ASLACC, 2, AccAddr, false}, {0}, {0}, {&ORA, 4, AbsAddr, false}, {&ASL, 6, AbsAddr, false}, {0}},
	{{&BPL, 2, RelAddr, true},{&ORA, 5, IndYAddr, true},{0},{0},{0},{&ORA, 4, ZpXAddr, false},{&ASL, 6, ZpXAddr, false},{0},{&CLC, 2, ImplAddr, false},{&ORA, 4,AbsYAddr, true},{0},{0},{0},{&ORA, 4, AbsXAddr, true},{&ASL, 7, AbsXAddr, false},{0}},
	{{&JSR, 6, AbsAddr, false}, {&AND, 6, IndXAddr, false}, {0}, {0}, {&BIT, 3, ZpAddr, false}, {&AND, 3, ZpAddr, false}, {&ROL, 5, ZpAddr, false}, {0}, {&PLP, 4, ImplAddr, false}, {&AND, 2, ImmAddr, false}, {&ROLACC, 2, AccAddr, false}, {0}, {&BIT, 4,AbsAddr, false}, {&AND, 4,AbsAddr, false}, {&ROL, 6, AbsAddr, false}, {0}},
	{{&BMI, 2, RelAddr, true}, {&AND, 5, IndYAddr, true}, {0}, {0}, {0}, {&AND, 4,ZpXAddr, false}, {&ROL, 6, ZpXAddr, false}, {0}, {&SEC, 2, ImplAddr, false}, {&AND,4, AbsYAddr, true}, {0}, {0}, {0}, {&AND, 4, AbsXAddr, true}, {&ROL, 7, AbsXAddr, false}, {0}},
	{{&RTI, 6, ImplAddr, false},{&EOR,6,IndXAddr, false},{0},{0},{0},{&EOR, 3,ZpAddr, false},{&LSR, 5,ZpAddr, false},{0},{&PHA, 3, ImplAddr, false},{&EOR, 2,ImmAddr, false},{&LSRACC, 2,AccAddr, false},{0},{&JMP, 3, AbsAddr, false},{&EOR, 4,AbsAddr, false},{&LSR, 6,AbsAddr, false},{0}},
	{{&BVC, 2,RelAddr, true},{&EOR,5,IndYAddr, true},{0},{0},{0},{&EOR, 4,ZpXAddr, false},{&LSR,6,ZpXAddr, false},{0},{&CLI, 2,ImplAddr, false},{&EOR,4,AbsYAddr, true},{0},{0},{0},{&EOR, 4,AbsXAddr, true},{&LSR, 7,AbsXAddr, false},{0}},
	{{&RTS, 6, ImplAddr, false},{&ADC, 6,IndXAddr, false},{0},{0},{0},{&ADC, 3,ZpAddr, false},{&ROR,5,ZpAddr, false},{0},{&PLA, 4,ImplAddr, false},{&ADC, 2,ImmAddr, false},{&RORACC, 2,AccAddr, false},{0},{&JMP, 5,IndAddr, false},{&ADC, 4, AbsAddr, false},{&ROR, 6,AbsAddr, false},{0}},
	{{&BVS, 2, RelAddr, true},{&ADC,5, IndYAddr, true},{0},{0},{0},{&ADC, 4, ZpXAddr, false},{&ROR, 6,ZpXAddr, false},{0},{&SEI,2,ImplAddr, false},{&ADC,4,AbsYAddr, true},{0},{0},{0},{&ADC, 4, AbsXAddr, true},{&ROR, 7, AbsXAddr, false},{0}},
	{{0},{&STA, 6, IndXAddr, false},{0},{0},{&STY, 3, ZpAddr, false},{&STA, 3, ZpAddr, false},{&STX, 3,ZpAddr, false},{0},{&DEY, 2,ImplAddr,false},{0},{&TXA, 2, ImplAddr, false},{0},{&STY, 4,AbsAddr, false},{&STA, 4,AbsAddr,false},{&STX, 4,AbsAddr, false},{0}},
	{{&BCC,2, RelAddr, true},{&STA, 6, IndYAddr, false},{0},{0},{&STY, 4, ZpXAddr, false},{&STA, 4, ZpXAddr, false},{&STX, 4, ZpYAddr, false},{0},{&TYA, 2, ImplAddr, false},{&STA, 5,AbsYAddr, false},{&TXS, 2,ImplAddr, false},{0},{0},{&STA, 5, AbsXAddr, false},{0},{0}},
	{{&LDY, 2,ImmAddr, false},{&LDA, 6, IndXAddr,false},{&LDX, 3,ZpAddr, false},{0},{&LDY, 3,ZpAddr, false},{&LDA, 3,ZpAddr, false},{&LDX, 3,ZpAddr, false},{0},{&TAY, 2,ImplAddr, false},{&LDA, 2, ImmAddr, false},{&TAX, 2,ImplAddr, false},{0},{&LDY, 4, AbsAddr, false},{&LDA, 4,AbsAddr, false},{&LDX,4,AbsAddr, false},{0}},
	{{&BCS, 2,RelAddr, true},{&LDA, 5,IndYAddr,true},{0},{0},{&LDY,4,ZpXAddr, false},{&LDA, 4,ZpXAddr, false},{&LDX, 4,ZpYAddr,false},{0},{&CLV, 2,ImplAddr, false},{&LDA,5,AbsYAddr,true},{&TSX, 2,ImplAddr, false},{0},{&LDY, 4,AbsXAddr, true},{&LDA, 4,AbsXAddr, true},{&LDX, 4,AbsYAddr, true},{0}},
	{{&CPY, 2,ImmAddr, false},{&CMP, 6,IndXAddr, false},{0},{0},{&CPY,3,ZpAddr, false},{&CMP,3,ZpAddr,false},{&DEC, 5,ZpAddr, false},{0},{&INY, 2,ImplAddr, false},{&CMP, 2,ImmAddr, false},{&DEX, 2,ImplAddr, false},{0},{&CPY, 4,AbsAddr, false},{&CMP, 4, AbsAddr, false},{&DEC,6,AbsAddr,false},{0}},
	{{&BNE, 2,RelAddr, true},{&CMP, 5, IndYAddr, true},{0},{0},{0},{&CMP, 4, ZpXAddr, false},{&DEC, 6,ZpXAddr, false},{0},{&CLD, 2,ImplAddr, false},{&CMP, 4,AbsYAddr, true},{0},{0},{0},{&CMP, 4,AbsXAddr, true},{&DEC, 7,AbsXAddr,false}, {0}},
	{{&CPX, 2,ImmAddr, false},{&SBC, 6,IndXAddr, false},{0},{0},{&CPX, 3,ZpAddr,false},{&SBC, 3,ZpAddr, false},{&INC, 5,ZpAddr, false},{0},{&INX,2,ImplAddr,false},{&SBC,2,ImmAddr, false},{&NOP, 2,ImplAddr,false},{0},{&CPX,4,AbsAddr,false},{&SBC,4,AbsAddr, false},{&INC,6,AbsAddr,false},{0}},
	{{&BEQ, 2,RelAddr, true},{&SBC,5,IndYAddr, true},{0},{0},{0},{&SBC, 4,ZpXAddr, false},{&INC, 6,ZpXAddr, false},{0},{&SED, 2,ImplAddr,false},{&SBC, 4,AbsYAddr,true},{0},{0},{0},{&SBC, 4,AbsXAddr, true},{&INC, 7,AbsXAddr,false},{0} }
};
//----------------------
//Стек
static void stackPushByte(BYTE data);
static void stackPushWord(WORD data);
static BYTE stackPopByte();
static WORD stackPopWord();
//----------------------
static WORD getImmAddr();
static WORD getZpAddr();
static WORD getZpXAddr();
static WORD getZpYAddr();
static WORD getAbsXAddr();
static WORD getAbsYAddr();
static WORD getIndAddr();
static WORD getIndXAddr();
static WORD getIndYAddr();
static WORD getRelAddr();

//----------------------
void cpuReset(void);
int cpuStep(void);
void updateNZ(BYTE data);
