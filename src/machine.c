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

enum exit_reason_t machine_step(machine_t *m) {
    while (true) {
        m->state.exit_reason = none;
        exec_block_interp(&m->state);
        assert(m->state.exit_reason != none);

        if (m->state.exit_reason == indirect_branch ||
            m->state.exit_reason == direct_branch) {
            m->state.pc = m->state.reenter_pc;
            continue;
        }

        break;
    }

    m->state.pc = m->state.reenter_pc;
    assert(m->state.exit_reason == ecall);
    return ecall;
}

void machine_setup(machine_t *m, int argc, char *argv[]) {
    size_t stack_size = 32 * 1024 * 1024; // stack size: 32M
    u64 stack = mmu_alloc(&m->mmu, stack_size);
    m->state.gp_regs[sp] = stack + stack_size; // sp -> stack top

    m->state.gp_regs[sp] -= 8; // auxv
    m->state.gp_regs[sp] -= 8; // envp
    m->state.gp_regs[sp] -= 8; // argv end，值为 0（因为 argv 以 0 结尾）

    u64 args = argc - 1;
    for (int i = args; i > 0; i--) {
        size_t len = strlen(argv[i]);
        u64 addr = mmu_alloc(&m->mmu, len+1);
        mmu_write(addr, (u8 *)argv[i], len+1); // 初始时堆内容全为零，所以字符串的结束符无需重复写入
        m->state.gp_regs[sp] -= 8; // argv[i]
        mmu_write(m->state.gp_regs[sp], (u8 *)&addr, sizeof(u64));
    }

    m->state.gp_regs[sp] -= 8; // argc
    mmu_write(m->state.gp_regs[sp], (u8 *)&args, sizeof(u64));
}