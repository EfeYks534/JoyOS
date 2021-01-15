%include "Common/CommonAsm.inc"

section .text

extern Schedule

; extern void TaskSwitch(uint64_t stack_top);

global TaskSwitch

global TaskYield

TaskSwitch:
	MOV RSP, RDI

	POPAQ
	ADD RSP, 0x10
	IRETQ

TaskYield:
	MOV RSI, QWORD [RSP]
	ADD RSP, 0x08
	MOV RAX, RSP

	MOV RDI, SS

	PUSH RDI
	PUSH RAX
	PUSHF

	MOV RAX, CS
	PUSH RAX

	PUSH RSI

	PUSH QWORD 0
	PUSH QWORD 0
	PUSHAQ

	MOV RDI, RSP
	CLD
	CALL Schedule
