#include "types.h"
#include <stdbool.h>
#include <time.h>
#ifndef __6502__
#define __6502__
//6502 processor
// -----------------------------------------------------------------
//6502 registers
// ----------------
//A — аккумулятор, 8 бит;
//X, Y — индексные регистры, 8 бит;
//PC — счетчик команд, 16 бит;
//S — указатель стека, 8 бит;
//P — регистр состояния;
	//C(P0) — перенос;
	//Z(P1) — ноль;
	//I(P2) — запрет внешних прерываний — IRQ(I = 0 — прерывания разрешены);
	//D(P3) — режим BCD для инструкций сложения и вычитания с переносом;
	//B(P4) — обработка прерывания(B = 1 после выполнения команды BRK);
	//1 (P5)— не используется, равен 1;
	//V(P6) — переполнение;
	//N(P7) — знак.Равен старшему биту значения, загруженного в A, X или Y в результате выполнения операции(кроме TXS).
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
static Registers registers = {0,0,0,0,0,0};
static bool wsynced = false;
bool* getWsyncState();
//----------------------
//Стек
static void stackPushByte(BYTE data);
static void stackPushWord(WORD data);
static BYTE stackPopByte();
static WORD stackPopWord();
//----------------------
void cpuReset(void);
void cpuStep(void);
void updateNZ(BYTE data);
//Режимы адресации
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
//Набор инструкций
// ---------------------
//Сложение с переносом. Рез в A
static void ADC(BYTE data);
//Логическое И. Рез в A
static void AND(BYTE data);
//Сдвиг влево на 1. Рез по адресу. 
static void ASL(WORD addr);
//Сдвиг аккумулятора влево на 1. Рез в аккумуляторе.
static void ASLACC();
//Переход, если нет переноса
static void BCC(WORD addr);
//Переход, если есть перенос
static void BCS(WORD addr);
//Переход, если результат нулевой
static void BEQ(WORD addr);
//Установка флагов в соответствии с результатом выполнения поразрядного логического И над содержимым аккумулятора и операнда.
static void BIT(BYTE data);
//Переход, если результат отрицательный
static void BMI(WORD addr);
//Переход, если результат не нулевой
static void BNE(WORD addr);
//Переход, если результат положительный
static void BPL(WORD addr);
//Прерывание BRK
static void BRK();
// TODO opc
//Переход, если (флаг V = 0) нет переноса в знаковый разряд
static void BVC(WORD addr);
//Переход, если (флаг V = 1) есть перенос в знаковый разряд
static void BVS(WORD addr);
//Сброс флага C
static void CLC();
//Сброс флага D
static void CLD();
//Сброс флага I (разрешение прерываний)
static void CLI();
//Сброс флага V
static void CLV();
//Установка флагов в соответствии с результатом вычитания операнда из содержимого аккумулятора
static void CMP(BYTE data);
//Установка флагов в соответствии с результатом вычитания операнда из содержимого регистра X
static void CPX(BYTE data);
//Установка флагов в соответствии с результатом вычитания операнда из содержимого аккумулятора
static void CPY(BYTE data);
//Уменьшение операнда на 1
static void DEC(WORD addr);
//Уменьшение X на 1
static void DEX();
//Уменьшение Y на 1
static void DEY();
//XOR для A и операнда. Результат в A
static void EOR(BYTE data);
//Инкремент данных по адресу
static void INC(WORD addr);
//инкремент X
static void INX();
//инкремент Y
static void INY();
//Переход на адрес
static void JMP(WORD addr);
//Переход на адрес с сохранением текущего
static void JSR(WORD addr);
//Загрузка опперанда в аккумулятор
static void LDA(BYTE data);
//Загрузка опперанда в X
static void LDX(BYTE data);
//Загрузка опперанда в Y
static void LDY(BYTE data);
//Сдвиг операнда вправо (деление на 2)
static void LSRACC();
static void LSR(WORD addr);
//логическое ИЛИ содержимого аккумулятора и операнда
static void ORA(BYTE data);
//аккумулятор в стек
static void PHA();
//статусный регистр в стек
static void PHP();
//Аккумулятор из стека
static void PLA();
//Статусный регистр из стека
static void PLP();
//Циклический сдвиг операнда влево
static void ROLACC();
static void ROL(WORD addr);
//Циклический сдвиг операнда вправо
static void RORACC();
static void ROR(WORD addr);
//Возврат из прерывания
static void RTI();
//Возврат из процедуры
static void RTS();
//Вычитание с переносом
static void SBC(BYTE data);
//Установка флага C
static void SEC();
//Установка флага D
static void SED();
//Устновка флага I
static void SEI();
//Запись аккумулятора в память
static void STA(WORD addr);
//Запись X в память
static void STX(WORD addr);
//Запись Y в память
static void STY(WORD addr);
#endif 
