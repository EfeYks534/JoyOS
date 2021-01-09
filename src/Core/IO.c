#include "IOLib.h"

void Out8(uint16_t port, uint8_t data)
{ asm volatile("outb %0, %1" : : "a"(data), "Nd"(port)); }

void Out16(uint16_t port, uint16_t data)
{ asm volatile("outw %0, %1" : : "a"(data), "Nd"(port)); }

void Out32(uint16_t port, uint32_t data)
{ asm volatile("outl %0, %1" : : "a"(data), "Nd"(port)); }

uint8_t In8(uint16_t port)
{ uint8_t ret = 0; asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port) ); return ret; }

uint16_t In16(uint16_t port)
{ uint16_t ret = 0; asm volatile("inw %1, %0" : "=a"(ret) : "Nd"(port) ); return ret; }

uint32_t In32(uint16_t port)
{ uint32_t ret = 0; asm volatile("inl %1, %0" : "=a"(ret) : "Nd"(port) ); return ret; }

void Out8W(uint16_t port, uint8_t data)
{ Out8(port, data); IOWait(); }

void Out16W(uint16_t port, uint16_t data)
{ Out16(port, data); IOWait(); }

void Out32W(uint16_t port, uint32_t data)
{ Out32(port, data); IOWait(); }

void IOWait()
{
	asm volatile("outb %%al, $0x80" : : "a"(0));
}
