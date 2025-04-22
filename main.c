#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* utility helper functions */

void _Runtime_assert(bool condition, const char *message) {
	if (condition) return;
	printf("%s\n", message);
	exit(1);
}

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

#define BVM_RAM_START BVM_REGI_START
#define BVM_RAM_ENDIN BVM_CODE_ENDIN

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

/* program counter things */

// since we have 1KiB of ram, we know that is 2^10 thus we need a 10 bit pc. C
// doesn't have a 10 bit int so we will use a 16 bit int with a mask of 0x3FF

#define BVM_PC_MASK 0x3FF
_Static_assert(
	BVM_PC_MASK == BVM_MEM_SIZE_BYTES - 1,
	"program counter mask must mask the bits not used in the counter"
);

struct ProgramCounter {
	uint16_t position;
};

void ProgramCounter_set_position(
	struct ProgramCounter *self,
	uint16_t position
) {
	_Runtime_assert(
		position <= BVM_RAM_ENDIN,
		"ProgramCounter_set_position() - position not in bounds - high"
	);

	// this cannot actually be triggered since position is a unsigned int
	_Runtime_assert(
		position >= BVM_RAM_START,
		"ProgramCounter_set_position() - position not in bounds - low"
	);

	self->position = position;
}

void ProgramCounter_increment_position_by(
	struct ProgramCounter *self,
	uint16_t count
) {
	self->position = (self->position + count) & BVM_PC_MASK;
}

void ProgramCounter_increment_position(struct ProgramCounter *self) {
	ProgramCounter_increment_position_by(self, 1);
}

struct ProgramCounter *NewProgramCounter(void) {
	struct ProgramCounter *newProgramCounter = malloc(
		sizeof(struct ProgramCounter)
	);

	newProgramCounter->position = 0;

	return newProgramCounter;
}

/* integrated test suite */

#ifdef BVM_TEST_MODE 

#define NEW_TEST(name, stmt_list) \
	void name(void) { \
		stmt_list; \
	}

#define REGISTER_TEST(name) \
	printf("Running test: '%s'...\n", #name); \
	name(); \
	printf("Passed test: '%s'\n", #name)

NEW_TEST(PC_SET_POS, {
	struct ProgramCounter *pc = NewProgramCounter();
	
	_Runtime_assert(
		pc->position == 0,
		"expected pc init position to be 1"
	);

	ProgramCounter_increment_position(pc);
	_Runtime_assert(
		pc->position == 1,
		"expected pc position to be incremented to 1"
	);

	ProgramCounter_increment_position_by(pc, 5);
	_Runtime_assert(
		pc->position == 6,
		"expected pc position to be incremented to 6"
	);

	ProgramCounter_set_position(pc, 0x3FF);
	_Runtime_assert(
		pc->position == 0x3FF,
		"expected pc position to be set to 0x3FF"
	);
})

int main(void) {
	REGISTER_TEST(PC_SET_POS);
}

#else

/* vm functions */

int main(void) {
	/* setup virtual machine */

	uint8_t ram[BVM_MEM_SIZE_BYTES]; // 1 KiB
	for (uint32_t i = 0; i < BVM_MEM_SIZE_BYTES; i++) {
		ram[i] = 0;
	}

	struct ProgramCounter *pc = NewProgramCounter();

	/* init process begin */

	// load program into memory, and do other stuff

	/* init process end */

	ProgramCounter_set_position(pc, BVM_CODE_START);
}

#endif // BVM_TEST_MODE
