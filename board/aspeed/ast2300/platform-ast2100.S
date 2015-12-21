/* Sdram Configuration for the board */
/* Copyright (c) American Megatrends, Inc */

#include <config.h>
#include <version.h>
#include "ast2100_hw.h"


/* IMPORTANT: r10 should not be destroyed in this function*/
.globl lowlevel_init
lowlevel_init:
    /* save lr */
    mov r4, lr

sdram_config:

    ldr r0, =AST_SCU_VA_BASE        /* 1E6E:2000 */
    ldr r1, =0x1688a8a8
    str r1, [r0]

    ldr r0, =SCU_SOC_SCRATCH1_REG   /* 1E6E:2040 */
    ldr r1, [r0]
    orr r1, r1, #0x80
    str r1, [r0]

    ldr r0, =SCU_SOC_SCRATCH1_REG   /* 1E6E:2040 */
    ldr r1, [r0]
    bic r1, r1, #0xFFFFFFBF
    mov r2, r1, lsr #6
    cmp r2, #0x01
    beq platform_exit

    /* DRAM Setting */
    ldr r0, =SCU_M_PLL_PARAM_REG            /* 1E6E:2020 */
    ldr r1, =0x00004120		                /* 200MHz */ /* 41f0 */
    str r1, [r0]

    ldr r0, =SDRAM_PROTECTION_KEY_REG       /* 1E6E:0000 */
    ldr r1, =0xfc600309
    str r1, [r0]

    ldr r0, =SDRAM_DLL_CTRL_REG3            /* 1E6E:006C */
    ldr r1, =0x00909090
    str r1, [r0]

    ldr r0, =SDRAM_DLL_CTRL_REG1            /* 1E6E:0064 */
    ldr r1, =0x00050000
    str r1, [r0]


    ldr r0, =SCU_HW_STRAPPING_REG     		/* 1E6E:2070 */
    ldr r1, [r0]
    ldr r2, =0x0000000C
    and	r1, r1, r2
    mov	r2, r1, lsl #2

    ldr r0, =SDRAM_CONFIG_REG               /* 1E6E:0004 */
#ifdef CONFIG_1G_DDRII
    ldr r1, =0x00000d89
#endif
#ifdef CONFIG_512M_DDRII
    ldr r1, =0x00000585
#endif
    orr r1, r1, r2
    str r1, [r0]

    ldr r0, =SDRAM_GRAP_MEM_PROTECTION_REG  /* 1E6E:0008 */
    ldr r1, =0x0011030F		                /* VGA */
    str r1, [r0]

    ldr r0, =SDRAM_NSPEED_REG1              /* 1E6E:0010 */
    ldr r1, =0x32302926 /* =0x22201725 */
    str r1, [r0]

    ldr r0, =SDRAM_NSPEED_REG2              /* 1E6E:0018 */
    ldr r1, =0x274c0122 /* =0x1e29011a */
    str r1, [r0]

    ldr r0, =SDRAM_NSPEED_DELAY_CTRL_REG    /* 1E6E:0020 */
    ldr r1, =0x00cE2222 /* =0x00c82222 */
    str r1, [r0]

    ldr r0, =SDRAM_LSPEED_REG1              /* 1E6E:0014 */
    ldr r1, =0x01001523 /* =0x22201725 */
    str r1, [r0]

    ldr r0, =SDRAM_LSPEED_REG2              /* 1E6E:001c */
    ldr r1, =0x1024010d /* =0x1e29011a */
    str r1, [r0]

    ldr r0, =SDRAM_LSPEED_DELAY_CTRL_REG    /* 1E6E:0024 */
    ldr r1, =0x00cb2522 /* =0x00c82222 */
    str r1, [r0]

    ldr r0, =SDRAM_PAGE_MISS_LATENCY_MASK_REG   /* 1E6E:0038 */
    ldr r1, =0xffffff82
    str r1, [r0]

    ldr r0, =SDRAM_PRIORITY_GROUP_SET_REG   /* 1E6E:003C */
    ldr r1, =0x00000000
    str r1, [r0]

    ldr r0, =SDRAM_MAX_GRANT_LENGTH_REG1    /* 1E6E:0040 */
    ldr r1, =0x00f00000
    str r1, [r0]

    ldr r0, =SDRAM_MAX_GRANT_LENGTH_REG2    /* 1E6E:0044 */
    ldr r1, =0x00000000
    str r1, [r0]

    ldr r0, =SDRAM_MAX_GRANT_LENGTH_REG3    /* 1E6E:0048 */
    ldr r1, =0x00000000
    str r1, [r0]

    ldr r0, =0x1e6e004c
    ldr r1, =0x00000000
    str r1, [r0]

    ldr r0, =SDRAM_ECC_CTRL_STATUS_REG      /* 1E6E:0050 */
    ldr r1, =0x00000000
    str r1, [r0]

    ldr r0, =SDRAM_ECC_SEGMENT_EN_REG       /* 1E6E:0054 */
    ldr r1, =0x00000000
    str r1, [r0]

    ldr r0, =SDRAM_ECC_SCRUB_REQ_MASK_CTRL_REG  /* 1E6E:0058 */
    ldr r1, =0x00000000
    str r1, [r0]

    ldr r0, =SDRAM_ECC_ADDR_FIRST_ERR_REG   /* 1E6E:005C */
    ldr r1, =0x00000000
    str r1, [r0]

    ldr r0, =SDRAM_IO_BUFF_MODE_REG         /* 1E6E:0060 */
    ldr r1, =0x032AA02a
    str r1, [r0]

    ldr r0, =SDRAM_DLL_CTRL_REG1            /* 1E6E:0064 */
    ldr r1, =0x002d3000
    str r1, [r0]

    ldr r0, =SDRAM_DLL_CTRL_REG2            /* 1E6E:0068 */
    ldr r1, =0x02020202
    str r1, [r0]

    ldr r0, =SDRAM_TEST_CTRL_STATUS_REG     /* 1E6E:0070 */
    ldr r1, =0x00000000
    str r1, [r0]

    ldr r0, =SDRAM_TEST_START_ADDR_LENGTH_REG   /* 1E6E:0074 */
    ldr r1, =0x00000000
    str r1, [r0]

    ldr r0, =SDRAM_TEST_FAIL_DQ_BIT_REG     /* 1E6E:0078 */
    ldr r1, =0x00000000
    str r1, [r0]

    ldr r0, =SDRAM_TEST_INIT_VALUE_REG      /* 1E6E:007C */
    ldr r1, =0x00000000
    str r1, [r0]

    ldr r0, =SDRAM_PWR_CTRL_REG             /* 1E6E:0034 */
    ldr r1, =0x00000001
    str r1, [r0]

    /* Delay about 400us */
    ldr r2, =0x00000400
delay0:
    nop
    nop
    subs r2, r2, #1          /* until source end addreee [r2]    */
    bne delay0

    ldr r0, =SDRAM_MRS_EMRS2_MODE_SET_REG   /* 1E6E:002C */
    ldr r1, =0x00000942 /* =0x00000732 */
    str r1, [r0]

    ldr r0, =SDRAM_MRS_EMRS3_MODE_SET_REG   /* 1E6E:0030 */
    ldr r1, =0x00000040
    str r1, [r0]

    ldr r0, =SDRAM_MODE_SET_CTRL_REG        /* 1E6E:0028 */
    ldr r1, =0x00000005
    str r1, [r0]

    ldr r0, =SDRAM_MODE_SET_CTRL_REG        /* 1E6E:0028 */
    ldr r1, =0x00000007
    str r1, [r0]

    ldr r0, =SDRAM_MODE_SET_CTRL_REG        /* 1E6E:0028 */
    ldr r1, =0x00000003
    str r1, [r0]

    ldr r0, =SDRAM_MODE_SET_CTRL_REG        /* 1E6E:0028 */
    ldr r1, =0x00000001
    str r1, [r0]

    ldr r0, =SDRAM_REFRESH_TIMING_REG       /* 1E6E:000C */
    ldr r1, =0x00005A08
    str r1, [r0]

    ldr r0, =SDRAM_MRS_EMRS2_MODE_SET_REG   /* 1E6E:002C */
    ldr r1, =0x00000842 /* =0x00000632 */
    str r1, [r0]

    ldr r0, =SDRAM_MODE_SET_CTRL_REG        /* 1E6E:0028 */
    ldr r1, =0x00000001
    str r1, [r0]

    ldr r0, =SDRAM_MRS_EMRS3_MODE_SET_REG   /* 1E6E:0030 */
    ldr r1, =0x000003c0
    str r1, [r0]

    ldr r0, =SDRAM_MODE_SET_CTRL_REG        /* 1E6E:0028 */
    ldr r1, =0x00000003
    str r1, [r0]

    ldr r0, =SDRAM_MRS_EMRS3_MODE_SET_REG   /* 1E6E:0030 */
    ldr r1, =0x00000040
    str r1, [r0]

    ldr r0, =SDRAM_MODE_SET_CTRL_REG        /* 1E6E:0028 */
    ldr r1, =0x00000003
    str r1, [r0]

    ldr r0, =SDRAM_REFRESH_TIMING_REG       /* 1E6E:000C */
    ldr r1, =0x00005A21
    str r1, [r0]

    ldr r0, =SDRAM_PWR_CTRL_REG             /* 1E6E:0034 */
    ldr r1, =0x00007c03
    str r1, [r0]

    ldr r0, =AST2100_COMPATIBLE_SCU_MPLL_PARA   /* 1E6E:0120 */			/* for AST2000 compatible */
    ldr r1, =0x000005061 /* =0x00004c81 */			                /* 200MHz */
    str r1, [r0]

    /*Set Scratch register Bit 6 after ddr initial finished */
	ldr r0, =SCU_SOC_SCRATCH1_REG    /* 1E6E:2040 */
    ldr r1, [r0]
    orr r1, r1, #0x40
    str r1, [r0]

platform_exit:
    ldr r0, =0x1e784004
    mov r1, #0x00
    str r1, [r0]

    ldr r0, =0x1e78400c
    mov r1, #0x83
    str r1, [r0]

    ldr r0, =0x1e6e202c
    ldr r2, [r0]
    mov r2, r2, lsr #12
    tst r2, #0x01
    ldr r0, =0x1e784000
    moveq r1, #0x0D                              @ Baudrate 115200
    movne r1, #0x01                              @ Baudrate 115200, div13
#if defined(CONFIG_DRAM_UART_38400)
    moveq r1, #0x27                              @ Baudrate 38400
    movne r1, #0x03                              @ Baudrate 38400 , div13
#endif
    str r1, [r0]

    ldr r0, =0x1e784004
    mov r1, #0x00
    str r1, [r0]

    ldr r0, =0x1e78400c
    mov r1, #0x03
    str r1, [r0]

    ldr r0, =0x1e784010
    mov r1, #0x03
    str r1, [r0]

    ldr r0, =0x1e784008
    mov r1, #0x07
    str r1, [r0]

    ldr r0, =0x1e784000
    mov r1, #0x0D                                @ '\r'
    str r1, [r0]
    mov r1, #0x0A                                @ '\n'
    str r1, [r0]
    mov r1, #0x44                                @ 'D'
    str r1, [r0]
    mov r1, #0x52                                @ 'R'
    str r1, [r0]
    mov r1, #0x41                                @ 'A'
    str r1, [r0]
    mov r1, #0x4D                                @ 'M'
    str r1, [r0]

	/* Return Back to caller */
	mov		lr,	r4
	mov		pc, lr
