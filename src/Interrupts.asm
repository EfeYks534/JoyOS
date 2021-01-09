BITS 64

%include "Common/CommonAsm.inc"

section .text

extern SysCallHandler

global SysCallInterrupt

SysCallInterrupt:
	PUSH QWORD 0
	PUSH QWORD 0
	PUSHAQ

	MOV RDI, RSP
	CLD
	CALL SysCallHandler

	POPAQ
	ADD RSP, 0x10
	IRETQ

extern ExceptionHandler

extern SaveConInfo

global SaveConInt

SaveConInt:
	PUSH QWORD 0
	PUSH QWORD 0
	PUSHAQ

	MOV RDI, RSP
	CLD
	CALL SaveConInfo

	POPAQ

	ADD RSP, 0x10
	IRETQ

global ExceptionInterruptCommon

ExceptionInterruptCommon:
	PUSHAQ

	MOV RDI, RSP
	CLD
	CALL ExceptionHandler

	POPAQ

	ADD RSP, 0x10
	IRETQ


extern IRQHandler

global IRQInterruptCommon

IRQInterruptCommon:
	PUSHAQ

	MOV RDI, RSP
	CLD
	CALL IRQHandler

	POPAQ

	ADD RSP, 0x10
	IRETQ

%macro IRQ 1


global IRQInterrupt%1

IRQInterrupt%1:
	PUSH QWORD 0
	PUSH QWORD %1
	JMP IRQInterruptCommon

%endmacro

IRQ 0
IRQ 1
IRQ 2
IRQ 3
IRQ 4
IRQ 5
IRQ 6
IRQ 7

IRQ 8
IRQ 9
IRQ 10
IRQ 11
IRQ 12
IRQ 13
IRQ 14
IRQ 15

%macro EXCEPTION 2

global ExceptionInterrupt%1

ExceptionInterrupt%1:
	PUSH QWORD 0
	PUSH QWORD %2
	JMP ExceptionInterruptCommon

%endmacro

EXCEPTION DE, 0x00
EXCEPTION DB, 0x01
EXCEPTION NMI, 0x02
EXCEPTION BP, 0x03
EXCEPTION OF, 0x04
EXCEPTION BR, 0x05
EXCEPTION UD, 0x06
EXCEPTION NM, 0x07
EXCEPTION DF, 0x08
EXCEPTION CPSO, 0x09
EXCEPTION TS, 0x0A
EXCEPTION NP, 0x0B
EXCEPTION SS, 0x0C
EXCEPTION GP, 0x0D
EXCEPTION PF, 0x0E
EXCEPTION MF, 0x10
EXCEPTION AC, 0x11
EXCEPTION MC, 0x12
EXCEPTION XM, 0x13
EXCEPTION VE, 0x14
EXCEPTION SX, 0x1E
