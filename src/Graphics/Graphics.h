#pragma once

#include <stivale.h>
#include <stddef.h>
#include <stdint.h>
#include "WinMan.h"

void GrFlush();

void GrRestoreLimits();

void GrRender(struct Window *win);

void GrInit(struct stivale_struct *data);

uint32_t GrWidth();

uint32_t GrHeight();

void GrSprite(uint32_t *bitmap, uint32_t x, uint32_t y, uint32_t w, uint32_t h);

void GrRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color);

void GrFillRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color);

void GrDrawChar(uint8_t c, uint32_t xx, uint32_t yy, uint32_t color);

void GrDrawText(uint8_t *text, uint32_t xx, uint32_t yy, uint32_t color);
