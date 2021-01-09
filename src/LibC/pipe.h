#pragma once

#include <stdint.h>
#include <stddef.h>

int64_t popen();

int pstat(int64_t pipe);

void poll(int64_t pipe);

void flush(int64_t pipe);

void write(const void *buf, size_t len, int64_t pipe);

void read(void *buf, size_t len, int64_t pipe);
