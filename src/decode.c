#include "rvemu.h"

#define QUADRANT(data) (((data) >> 0) & 0x3)

void insn_decode(insn_t *insn, u32 data) { 
    u32 quadrant = QUADRANT(data); 
    switch (quadrant) {
    case 0x00: fatal("unimplent");
    case 0x01: fatal("unimplent");
    case 0x10: fatal("unimplent");
    case 0x11: fatal("unimplent");
    default: unreachable();
    }
}