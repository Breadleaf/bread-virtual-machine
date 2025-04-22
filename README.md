# Bread Virtual Machine

## Table of Contents

- [Motivation](#motivation)
- [Virtual Macine](#virtual-machine)
    - [Memory](#memory)
    - [Registers](#registers)

## Motivation

I am making this since I really really enjoy low level programming but there
are a few concepts I dont yet fully understand. I find that a lot of projects
in school tend to give me large systems with loads of specifications to read
and follow through without really giving me the chance to really learn why some
things are the way they are for myself. This project serves as a true capstone
project where I make a VM, ISA, and language all of my own creation.

## Virtual Machine

### Memory

NOTE: this section was last updated: `April 21 2025 11 PM MST`

`0x000 - 0x3FF` is how large the memory mapping is. This equates to `1KiB` or
`8,192b` of addressable ram.

The memory will be layed out as follows:
- `0x000 - 0x007`: general purpose registers (8 8bit registers)
- `0x008 - 0x047`: constants
- `0x048 - 0x087`: globals
- `0x088 - 0x27F`: shared dynamic memory
- `0x280 - 0x3FF`: program instructions

In human readable format:
- 8 Bytes of general purpose registers
- 64 Bytes of constant / read only values
- 64 Bytes of global values
- 448 Bytes of shared dynamic memory between the stack and heap
- 512 Bytes of program instructions

### Registers

NOTE: this section was last updated: `April 21 2025 11 PM MST`

Registers are entirely general purpose. Eventually there will be a calling
convention with them. But for now, any of them can be used for anything.
