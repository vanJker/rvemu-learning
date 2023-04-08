#include "rvemu.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fatal("load program: ./rvemu $(program)");
    }

    machine_t machine = {0};
    machine_load_program(&machine, argv[1]);

    printf("entry: 0x%lx\n", machine.mmu.entry);
    return 0;
}
