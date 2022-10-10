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
typedef struct {
	bool C;
	bool Z;
	bool I;
	bool D;
	bool B;
	bool V;
	bool N;
} StateReg;
typedef struct {
	BYTE A;		
	BYTE S;		
	WORD PC;	
	StateReg P;
	BYTE X; BYTE Y;

} Registers;
static Registers registers = {0,0,0,0,0,0};
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
//–ежимы адресации
// ---------------------
static WORD getAbsAddr();
static WORD getAbsXAddr();
static WORD getAbsYAddr();
static WORD getImmAddr();
static WORD getIndXAddr();
static WORD getIndYAddr();
static WORD getZpAddr();
static WORD getZpXAddr();
static WORD getZpYAddr();
static WORD getRelAddr();
//Ќабор инструкций
// ---------------------
//—ложение с переносом. –ез в A
static void ADC(BYTE data);
//Ћогическое ». –ез в A
static void AND(BYTE data);
//—двиг влево на 1. –ез по адресу. 
static void ASL(WORD addr);
//—двиг аккумул€тора влево на 1. –ез в аккумул€торе.
static void ASLACC();
//ѕереход, если нет переноса
static void BCC(WORD addr);
//ѕереход, если есть перенос
static void BCS(WORD addr);
//ѕереход, если результат нулевой
static void BEQ(WORD addr);
//”становка флагов в соответствии с результатом выполнени€ поразр€дного логического » над содержимым аккумул€тора и операнда.
static void BIT(BYTE data);
//ѕереход, если результат отрицательный
static void BMI(WORD addr);
//ѕереход, если результат не нулевой
static void BNE(WORD addr);
//ѕереход, если результат положительный
static void BPL(WORD addr);
//ѕрерывание BRK
static void BRK();
// TODO opc
//«агрузка опперанда в аккумул€тор
static void LDA(BYTE data);
//«апись аккумул€тора в пам€ть
static void STA(WORD addr);
//«агрузка опперанда в X
static void LDX(BYTE data);
//инкремент X
static void INX();
//«апись X в пам€ть
static void STX(WORD addr);
//ѕереход на адрем
static void JMP(WORD addr);
#endif 
