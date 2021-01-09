%include "Common/CommonAsm.inc"

.text:

global TaskSwitch

TaskSwitch:
	MOV RSP, [RDI]
	POPAQ
	ADD RSP, 0x10
	IRETQ
