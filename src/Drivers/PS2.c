#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <Core/IOLib.h>
#include <Common/KernelLib.h>
#include <Core/PIC.h>
#include <Common/Registers.h>
#include "PS2Controller.h"

uint8_t DataAvailable(uint8_t channel)
{
	uint8_t stat = In8(0x64);

	uint8_t chn = ( stat & 0x20 ) ? CHANNEL_TWO : CHANNEL_ONE;

	return (stat & 1) && (chn == channel);
}

void PS2Flush()
{
	uint8_t stat = In8(0x64) & 1;

	while(stat) {
		stat = In8(0x64) & 1;
		In8(0x60);
	}
}

uint8_t PS2ReadData()
{
	uint8_t stat = In8(0x64) & 1;

	while(!stat)
		stat = In8(0x64) & 1;

	return In8(0x60);
}

uint8_t PS2ShouldResend()
{
	uint8_t data = PS2ReadData();

	if(data == PS2_ACK)
		return 0;

	if(data == PS2_RESEND)
		return 1;

	KernelPanic("PS2: Expected ACK/RESEND, got %xb", data);
}

uint8_t PS2SendCommand(uint8_t channel, uint8_t command, uint8_t exp_ack)
{
	uint8_t resend = 3;

	do {
		switch(channel)
		{
		case CHANNEL_TWO:
			Out8W(0x64, 0xD4);
			Out8W(0x60, command);
			break;
		case CHANNEL_ONE:
			Out8W(0x60, command);
			break;
		default:
			KernelPanic("Unreachable area reached");
		}
	} while(exp_ack && --resend && PS2ShouldResend());

	if(exp_ack && !resend)
		KernelPanic("PS2: Failed to receive ACK after 3 attempts");
}
