#include "types.h"
#include <stdbool.h>
#include <time.h>
#ifndef __6502__
#define __6502__
//6502 processor
// -----------------------------------------------------------------
//6502 registers
// ----------------
//A � �����������, 8 ���;
//X, Y � ��������� ��������, 8 ���;
//PC � ������� ������, 16 ���;
//S � ��������� �����, 8 ���;
//P � ������� ���������;
	//C(P0) � �������;
	//Z(P1) � ����;
	//I(P2) � ������ ������� ���������� � IRQ(I = 0 � ���������� ���������);
	//D(P3) � ����� BCD ��� ���������� �������� � ��������� � ���������;
	//B(P4) � ��������� ����������(B = 1 ����� ���������� ������� BRK);
	//1 (P5)� �� ������������, ����� 1;
	//V(P6) � ������������;
	//N(P7) � ����.����� �������� ���� ��������, ������������ � A, X ��� Y � ���������� ���������� ��������(����� TXS).
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
//����
static void stackPushByte(BYTE data);
static void stackPushWord(WORD data);
static BYTE stackPopByte();
static WORD stackPopWord();
//----------------------
void cpuReset(void);
void cpuStep(void);
void updateNZ(BYTE data);
//������ ���������
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
//����� ����������
// ---------------------
//�������� � ���������. ��� � A
static void ADC(BYTE data);
//���������� �. ��� � A
static void AND(BYTE data);
//����� ����� �� 1. ��� �� ������. 
static void ASL(WORD addr);
//����� ������������ ����� �� 1. ��� � ������������.
static void ASLACC();
//�������, ���� ��� ��������
static void BCC(WORD addr);
//�������, ���� ���� �������
static void BCS(WORD addr);
//�������, ���� ��������� �������
static void BEQ(WORD addr);
//��������� ������ � ������������ � ����������� ���������� ������������ ����������� � ��� ���������� ������������ � ��������.
static void BIT(BYTE data);
//�������, ���� ��������� �������������
static void BMI(WORD addr);
//�������, ���� ��������� �� �������
static void BNE(WORD addr);
//�������, ���� ��������� �������������
static void BPL(WORD addr);
//���������� BRK
static void BRK();
// TODO opc
//�������� ��������� � �����������
static void LDA(BYTE data);
//������ ������������ � ������
static void STA(WORD addr);
//�������� ��������� � X
static void LDX(BYTE data);
//��������� X
static void INX();
//������ X � ������
static void STX(WORD addr);
//������� �� �����
static void JMP(WORD addr);
#endif 
