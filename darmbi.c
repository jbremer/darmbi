#include <stdio.h>
#include <stdint.h>
#include <darm.h>

uint32_t darmbi_bbl_length(const uint32_t *insns, uint32_t max_length)
{
    uint32_t length = 0; darm_t d;

    for (; length < max_length; length++) {
        if(darm_armv7_disasm(&d, insns[length]) < 0) {
            fprintf(stderr, "Unhandled or Invalid Instruction: 0x%08x\n",
                insns[length]);
            continue;
        }

        switch ((uint32_t) d.instr) {
        // common branching instructions
        case I_B: case I_BL: case I_BLX: case I_BX: case I_BXJ:
            return length;

        // instructions that can alter PC in their destination register
        case I_ADC: case I_ADD: case I_ADR: case I_AND: case I_ASR:
        case I_BIC: case I_EOR: case I_LDR: case I_LSL: case I_LSR:
        case I_MOV: case I_MVN: case I_ORR: case I_POP: case I_ROR:
        case I_RRX: case I_RSB: case I_RSC: case I_SBC: case I_SUB:
            if(d.Rd == PC) return length;
            break;

        // ldrd has two destination registers (namely, Rt and Rt2)
        case I_LDRD:
            if(d.Rt == PC || d.Rt2 == PC) return length;
            break;

        // instructions that can contain PC in their register list
        case I_LDM: case I_LDMDA: case I_LDMDB: case I_LDMIB:
            if((d.reglist >> PC) != 0) return length;
            break;
        }
    }

    return length;
}
