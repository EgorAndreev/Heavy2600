#include "types.h"
#include <stdbool.h>
#include <time.h>
#ifndef __6502__
#define __6502__
//6502 processor
// -----------------------------------------------------------------
//6502 registers
// ----------------
//A Ч аккумул€тор, 8 бит;
//X, Y Ч индексные регистры, 8 бит;
//PC Ч счетчик команд, 16 бит;
//S Ч указатель стека, 8 бит;
//P Ч регистр состо€ни€;
	//C(P0) Ч перенос;
	//Z(P1) Ч ноль;
	//I(P2) Ч запрет внешних прерываний Ч IRQ(I = 0 Ч прерывани€ разрешены);
	//D(P3) Ч режим BCD дл€ инструкций сложени€ и вычитани€ с переносом;
	//B(P4) Ч обработка прерывани€(B = 1 после выполнени€ команды BRK);
	//1 (P5)Ч не используетс€, равен 1;
	//V(P6) Ч переполнение;
	//N(P7) Ч знак.–авен старшему биту значени€, загруженного в A, X или Y в результате выполнени€ операции(кроме TXS).
typedef union {
	struct {
		bool C : 1;
		bool Z : 1;
		bool I : 1;
		bool D : 1;
		bool B : 1;
		bool NONE : 1;
		bool V : 1;
		bool N : 1;
	};
} StateReg;
typedef struct {
	BYTE A;
	BYTE S;
	WORD PC;
	StateReg P;
	BYTE X; BYTE Y;

} Registers;
static Registers registers = { 0,0,0,0,0,0 };
//ѕроцессор остановлен
static bool wsynced = false;
bool* getWsyncState();
static void ADC(WORD data);
static void AND(WORD data);
static void ASL(WORD addr);
static void ASLACC();
static void BCC(WORD addr);
static void BCS(WORD addr);
static void BEQ(WORD addr);
static void BIT(WORD data);
static void BMI(WORD addr);
static void BNE(WORD addr);
static void BPL(WORD addr);
static void BRK();
static void BVC(WORD addr);
static void BVS(WORD addr);
static void CLC();
static void CLD();
static void CLI();
static void CLV();
static void CMP(WORD data);
static void CPX(WORD data);
static void CPY(WORD data);
static void DEC(WORD addr);
static void DEX();
static void DEY();
static void EOR(WORD data);
static void INC(WORD addr);
static void INX();
static void INY();
static void JMP(WORD addr);
static void JSR(WORD addr);
static void LDA(WORD data);
static void LDX(WORD data);
static void LDY(WORD data);
static void LSRACC();
static void LSR(WORD addr);
static void ORA(WORD data);
static void PHA();
static void PHP();
static void PLA();
static void PLP();
static void ROLACC();
static void ROL(WORD addr);
static void RORACC();
static void ROR(WORD addr);
static void RTI();
static void RTS();
static void SBC(WORD data);
static void SEC();
static void SED();
static void SEI();
static void STA(WORD addr);
static void STX(WORD addr);
static void STY(WORD addr);
#endif 

//ќпкоды
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
typedef struct structOpCode {
	void* handler;
	BYTE cycles;
	AddrMode addressing;
	bool PageCrossCycles;
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
//—тек
static void stackPushByte(BYTE data);
static void stackPushWord(WORD data);
static BYTE stackPopByte();
static WORD stackPopWord();
//----------------------
void cpuReset(void);
void cpuStep(void);
void updateNZ(BYTE data);
