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
//�������, ���� (���� V = 0) ��� �������� � �������� ������
static void BVC(WORD addr);
//�������, ���� (���� V = 1) ���� ������� � �������� ������
static void BVS(WORD addr);
//����� ����� C
static void CLC();
//����� ����� D
static void CLD();
//����� ����� I (���������� ����������)
static void CLI();
//����� ����� V
static void CLV();
//��������� ������ � ������������ � ����������� ��������� �������� �� ����������� ������������
static void CMP(BYTE data);
//��������� ������ � ������������ � ����������� ��������� �������� �� ����������� �������� X
static void CPX(BYTE data);
//��������� ������ � ������������ � ����������� ��������� �������� �� ����������� ������������
static void CPY(BYTE data);
//���������� �������� �� 1
static void DEC(WORD addr);
//���������� X �� 1
static void DEX();
//���������� Y �� 1
static void DEY();
//XOR ��� A � ��������. ��������� � A
static void EOR(BYTE data);
//��������� ������ �� ������
static void INC(WORD addr);
//��������� X
static void INX();
//��������� Y
static void INY();
//������� �� �����
static void JMP(WORD addr);
//������� �� ����� � ����������� ��������
static void JSR(WORD addr);
//�������� ��������� � �����������
static void LDA(BYTE data);
//�������� ��������� � X
static void LDX(BYTE data);
//�������� ��������� � Y
static void LDY(BYTE data);
//����� �������� ������ (������� �� 2)
static void LSRACC();
static void LSR(WORD addr);
//���������� ��� ����������� ������������ � ��������
static void ORA(BYTE data);
//����������� � ����
static void PHA();
//��������� ������� � ����
static void PHP();
//����������� �� �����
static void PLA();
//��������� ������� �� �����
static void PLP();
//����������� ����� �������� �����
static void ROLACC();
static void ROL(WORD addr);
//����������� ����� �������� ������
static void RORACC();
static void ROR(WORD addr);
//������� �� ����������
static void RTI();
//������� �� ���������
static void RTS();
//��������� � ���������
static void SBC(BYTE data);
//��������� ����� C
static void SEC();
//��������� ����� D
static void SED();
//�������� ����� I
static void SEI();
//������ ������������ � ������
static void STA(WORD addr);
//������ X � ������
static void STX(WORD addr);
//������ Y � ������
static void STY(WORD addr);
#endif 
