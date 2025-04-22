#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/* memory size calculations */

// #define BVM_MEM_SIZE_BITS 8 * pow(2, 10) // 8,192 bits OR 1,024 bytes OR 1KiB
#define BVM_MEM_SIZE_BITS 8 * (1 << 10) // 8,192 bits
#define BVM_MEM_SIZE_BYTES (BVM_MEM_SIZE_BITS / 8) // 1,024 bytes

/* memory partitioning */

#define BVM_REGI_START 0x000
#define BVM_REGI_COUNT 8
#define BVM_REGI_ENDIN 0x007
_Static_assert(
	BVM_REGI_ENDIN - BVM_REGI_START + 1 == BVM_REGI_COUNT,
	"register math is wrong"
);

#define BVM_CONS_START 0x008
#define BVM_CONS_COUNT 64
#define BVM_CONS_ENDIN 0x047
_Static_assert(
	BVM_CONS_ENDIN - BVM_CONS_START + 1 == BVM_CONS_COUNT,
	"constant math is wrong"
);

#define BVM_GLOB_START 0x048
#define BVM_GLOB_COUNT 64
#define BVM_GLOB_ENDIN 0x087
_Static_assert(
	BVM_GLOB_ENDIN - BVM_GLOB_START + 1 == BVM_GLOB_COUNT,
	"global math is wrong"
);

#define BVM_SHRE_START 0x088
#define BVM_SHRE_COUNT 376
#define BVM_SHRE_ENDIN 0x1FF
_Static_assert(
	BVM_SHRE_ENDIN - BVM_SHRE_START + 1 == BVM_SHRE_COUNT,
	"shared math is wrong"
);

#define BVM_CODE_START 0x200
#define BVM_CODE_COUNT 512
#define BVM_CODE_ENDIN 0x3FF
_Static_assert(
	BVM_CODE_ENDIN - BVM_CODE_START + 1 == BVM_CODE_COUNT,
	"code / instruction math is wrong"
);

/* ensure all start and ends are relatively correct */

_Static_assert(
	BVM_REGI_ENDIN == BVM_CONS_START - 1,
	"invalid register end OR constant start"
);

_Static_assert(
	BVM_CONS_ENDIN == BVM_GLOB_START - 1,
	"invalid constant end OR global start"
);


_Static_assert(
	BVM_GLOB_ENDIN == BVM_SHRE_START - 1,
	"invalid global end OR share start"
);

_Static_assert(
	BVM_SHRE_ENDIN == BVM_CODE_START - 1,
	"invalid share end OR code / instruction start"
);

/* check the start / end bounds of memory */

_Static_assert(
	BVM_REGI_ENDIN != 0,
	"invalid register start, registers must start at 0x000"
);

_Static_assert(
	BVM_CODE_ENDIN == BVM_MEM_SIZE_BYTES - 1,
	"invalid code / instruction end"
);

/* check all counts add up to the total memory size */

_Static_assert(
	BVM_REGI_COUNT
	+ BVM_CONS_COUNT
	+ BVM_GLOB_COUNT
	+ BVM_SHRE_COUNT
	+ BVM_CODE_COUNT == BVM_MEM_SIZE_BYTES,
	"memory map does not fit in memory size"
);

void move_pc(uint32_t *pc, uint32_t count) {
	*pc = (*pc + count) % BVM_MEM_SIZE_BYTES;
}

void increment_pc(uint32_t *pc) {
	move_pc(pc, 1);
}

int main() {
	uint8_t ram[BVM_MEM_SIZE_BYTES]; // 1 KiB
	for (uint32_t i = 0; i < BVM_MEM_SIZE_BYTES; i++) {
		ram[i] = 0;
	}

	uint16_t pc = 0;

	/* init process begin */



	/* init process end */

	pc = BVM_CODE_START;
}
