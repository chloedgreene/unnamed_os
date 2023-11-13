#pragma once

#include <stdint.h>

static inline void outb(uint16_t port, uint8_t val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) : "memory");
}

static inline void outw(uint16_t port, uint8_t val)
{
    asm volatile ( "outw %0, %1" : : "a"(val), "Nd"(port) : "memory");
}

static inline void outl(uint16_t port, uint8_t val)
{
    asm volatile ( "outl %0, %1" : : "a"(val), "Nd"(port) : "memory");
}