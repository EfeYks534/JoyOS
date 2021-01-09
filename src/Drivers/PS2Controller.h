#pragma once

#define CHANNEL_ONE 0x0
#define CHANNEL_TWO 0x1

#define PS2_ACK    0xFA
#define PS2_RESEND 0xFE

#include <stdint.h>
#include <stddef.h>

uint8_t PS2SendCommand(uint8_t channel, uint8_t command, uint8_t exp_ack);

uint8_t PS2ShouldResend();

uint8_t PS2ReadData();

uint8_t DataAvailable(uint8_t channel);
