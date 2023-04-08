#include "rvemu.h"

void machine_load_program(machine_t *m, char *prog) {
    int fd = open(prog, O_RDONLY);
    if (fd == -1) {
        fatal(strerror(errno)); // strerror(error) 可以生成错误信息的字符串
    }

    // 确定 ELF entry
    mmu_load_elf(&m->mmu, fd);
    close(fd);

    // 设置 PC
    m->state.pc = (u64)m->mmu.entry;
}