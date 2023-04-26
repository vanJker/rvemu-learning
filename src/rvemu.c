#include "rvemu.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fatal("load program: ./rvemu $(program)");
    }

    machine_t machine = {0};
    machine_load_program(&machine, argv[1]);

    while (true) {
        enum exit_reason_t exit_reason = machine_step(&machine);
        assert(exit_reason == ecall);
    }

    return 0;
}
