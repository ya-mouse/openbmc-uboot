#ifndef _AST2100_HW_H_
#define _AST2100_HW_H_

#define IO_ADDRESS(x)	(x)
#define MEM_ADDRESS(x)  (x)

/* -----------------------------------------------------------------
 *      	         SMC Registers
 * -----------------------------------------------------------------
*/
#define SMC_CE0_SEGMENT_AC_TIMING_REG   (AST_SMC_VA_BASE + 0x00)
#define SMC_CE0_CTRL_REG                (AST_SMC_VA_BASE + 0x04)
#define SMC_CE1_CTRL_REG                (AST_SMC_VA_BASE + 0x08)
#define SMC_CE2_CTRL_REG                (AST_SMC_VA_BASE + 0x0C)

/* -----------------------------------------------------------------
 *      	         AHB Registers
 * -----------------------------------------------------------------
*/
#define AHB_PROTECTION_KEY_REG          (AST_AHBC_VA_BASE + 0x00)
#define AHB_PRIORITY_CTRL_REG           (AST_AHBC_VA_BASE + 0x80)
#define AHB_INTERRUPT_CTRL_REG          (AST_AHBC_VA_BASE + 0x88)
#define AHB_ADDR_REMAP_REG              (AST_AHBC_VA_BASE + 0x8C)

/* -----------------------------------------------------------------
 *      	        SRAM Registers
 * -----------------------------------------------------------------
 */

 /* -------------------------------------------------------------
 *                  SDRAM Registers
 * -------------------------------------------------------------
 */
#define SDRAM_PROTECTION_KEY_REG            (AST_SDRAMC_VA_BASE + 0x00)
#define SDRAM_CONFIG_REG                    (AST_SDRAMC_VA_BASE + 0x04)
#define SDRAM_GRAP_MEM_PROTECTION_REG       (AST_SDRAMC_VA_BASE + 0x08)
#define SDRAM_REFRESH_TIMING_REG            (AST_SDRAMC_VA_BASE + 0x0C)
#define SDRAM_NSPEED_REG1                   (AST_SDRAMC_VA_BASE + 0x10)
#define SDRAM_LSPEED_REG1                   (AST_SDRAMC_VA_BASE + 0x14)
#define SDRAM_NSPEED_REG2                   (AST_SDRAMC_VA_BASE + 0x18)
#define SDRAM_LSPEED_REG2                   (AST_SDRAMC_VA_BASE + 0x1C)
#define SDRAM_NSPEED_DELAY_CTRL_REG         (AST_SDRAMC_VA_BASE + 0x20)
#define SDRAM_LSPEED_DELAY_CTRL_REG         (AST_SDRAMC_VA_BASE + 0x24)
#define SDRAM_MODE_SET_CTRL_REG             (AST_SDRAMC_VA_BASE + 0x28)
#define SDRAM_MRS_EMRS2_MODE_SET_REG        (AST_SDRAMC_VA_BASE + 0x2C)
#define SDRAM_MRS_EMRS3_MODE_SET_REG        (AST_SDRAMC_VA_BASE + 0x30)
#define SDRAM_PWR_CTRL_REG                  (AST_SDRAMC_VA_BASE + 0x34)
#define SDRAM_PAGE_MISS_LATENCY_MASK_REG    (AST_SDRAMC_VA_BASE + 0x38)
#define SDRAM_PRIORITY_GROUP_SET_REG        (AST_SDRAMC_VA_BASE + 0x3C)
#define SDRAM_MAX_GRANT_LENGTH_REG1         (AST_SDRAMC_VA_BASE + 0x40)
#define SDRAM_MAX_GRANT_LENGTH_REG2         (AST_SDRAMC_VA_BASE + 0x44)
#define SDRAM_MAX_GRANT_LENGTH_REG3         (AST_SDRAMC_VA_BASE + 0x48)
#define SDRAM_ECC_CTRL_STATUS_REG           (AST_SDRAMC_VA_BASE + 0x50)
#define SDRAM_ECC_SEGMENT_EN_REG            (AST_SDRAMC_VA_BASE + 0x54)
#define SDRAM_ECC_SCRUB_REQ_MASK_CTRL_REG   (AST_SDRAMC_VA_BASE + 0x58)
#define SDRAM_ECC_ADDR_FIRST_ERR_REG        (AST_SDRAMC_VA_BASE + 0x5C)
#define SDRAM_IO_BUFF_MODE_REG              (AST_SDRAMC_VA_BASE + 0x60)
#define SDRAM_DLL_CTRL_REG1                 (AST_SDRAMC_VA_BASE + 0x64)
#define SDRAM_DLL_CTRL_REG2                 (AST_SDRAMC_VA_BASE + 0x68)
#define SDRAM_DLL_CTRL_REG3                 (AST_SDRAMC_VA_BASE + 0x6C)
#define SDRAM_TEST_CTRL_STATUS_REG          (AST_SDRAMC_VA_BASE + 0x70)
#define SDRAM_TEST_START_ADDR_LENGTH_REG    (AST_SDRAMC_VA_BASE + 0x74)
#define SDRAM_TEST_FAIL_DQ_BIT_REG          (AST_SDRAMC_VA_BASE + 0x78)
#define SDRAM_TEST_INIT_VALUE_REG           (AST_SDRAMC_VA_BASE + 0x7C)
#define AST2100_COMPATIBLE_SCU_PASSWORD     (AST_SDRAMC_VA_BASE + 0x100)
#define AST2100_COMPATIBLE_SCU_MPLL_PARA    (AST_SDRAMC_VA_BASE + 0x120)

/*-------------------------------------------------------------------
 *				    SCU Registers
 *--------------------------------------------------------------------
 */
#define SCU_KEY_CONTROL_REG			    (AST_SCU_VA_BASE +  0x00)
#define SCU_SYS_RESET_REG               (AST_SCU_VA_BASE +  0x04)
#define SCU_CLK_SELECT_REG              (AST_SCU_VA_BASE +  0x08)
#define SCU_CLK_STOP_REG                (AST_SCU_VA_BASE +  0x0C)
#define SCU_OSC_COUNT_CTRL_REG			(AST_SCU_VA_BASE +  0x10)
#define SCU_OSC_COUNT_VALUE_REG         (AST_SCU_VA_BASE +  0x14)
#define SCU_INTR_CRTL_VALUE_REG         (AST_SCU_VA_BASE +  0x18)
#define SCU_32CLK_ERR_CORRECT_REG       (AST_SCU_VA_BASE +  0x1C)
#define SCU_M_PLL_PARAM_REG             (AST_SCU_VA_BASE +  0x20)
#define SCU_H_PLL_PARAM_REG             (AST_SCU_VA_BASE +  0x24)

#define SCU_SOC_SCRATCH1_REG            (AST_SCU_VA_BASE +  0x40)
#define SCU_SOC_SCRATCH2_REG            (AST_SCU_VA_BASE +  0x44)
#define SCU_HW_STRAPPING_REG            (AST_SCU_VA_BASE +  0x70)

/* -------------------------------------------------------------------
 *  					Timer Registers
 * -------------------------------------------------------------------
 */
#define TIMER1_COUNT_REG                (AST_TIMER_VA_BASE + 0x00)
#define TIMER1_RELOAD_REG               (AST_TIMER_VA_BASE + 0x04)
#define TIMER1_FIRST_MATCH_REG          (AST_TIMER_VA_BASE + 0x08)
#define TIMER1_SEC_MATCH_REG            (AST_TIMER_VA_BASE + 0x0C)

#define TIMER2_COUNT_REG                (AST_TIMER_VA_BASE + 0x10)
#define TIMER2_RELOAD_REG               (AST_TIMER_VA_BASE + 0x14)
#define TIMER2_FIRST_MATCH_REG          (AST_TIMER_VA_BASE + 0x18)
#define TIMER2_SEC_MATCH_REG            (AST_TIMER_VA_BASE + 0x1C)

#define TIMER3_COUNT_REG                (AST_TIMER_VA_BASE + 0x20)
#define TIMER3_RELOAD_REG               (AST_TIMER_VA_BASE + 0x24)
#define TIMER3_FIRST_MATCH_REG          (AST_TIMER_VA_BASE + 0x28)
#define TIMER3_SEC_MATCH_REG            (AST_TIMER_VA_BASE + 0x2C)

#define TIMER_CONTROL_REG               (AST_TIMER_VA_BASE + 0x30)

/* --------------------------------------------------------------------
 *  						GPIO Registers
 * --------------------------------------------------------------------
 */

/* -----------------------------------------------------------------
 * 				 Interrupt Controller Register
 * -----------------------------------------------------------------
 */
#define IRQ_STATUS_REG	                (AST_IC_VA_BASE + 0x00)
#define FIQ_STATUS_REG	                (AST_IC_VA_BASE + 0x04)
#define RAW_INT_STATUS_REG	            (AST_IC_VA_BASE + 0x08)
#define IRQ_SELECT_REG	                (AST_IC_VA_BASE + 0x0C)
#define IRQ_ENABLE_REG	                (AST_IC_VA_BASE + 0x10)
#define IRQ_CLEAR_REG					(AST_IC_VA_BASE + 0x14)
#define SOFT_INT_REG					(AST_IC_VA_BASE + 0x18)
#define SOFT_INT_CLEAR_REG				(AST_IC_VA_BASE + 0x1C)
#define PROTECT_ENABLE_REG				(AST_IC_VA_BASE + 0x20)

/*-------------------------------------------------------------
 * 			  SSP Controllers Registers
 *  -----------------------------------------------------------
 */


/*---------------------------------------------------------------
 *   I2C Controllers Register
 *  ------------------------------------------------------------
 */

/*------------------------------------------------------------------
 *   DMA Controllers Registers
 *  ----------------------------------------------------------------
 */

/*------------------------------------------------------------------
 *				 RTC Register Locations
 *------------------------------------------------------------------*/


/*------------------------------------------------------------------
 *				 WDT Register Locations
 *------------------------------------------------------------------*/
#define WDT_CNT_STATUS_REG  (AST_WDT_VA_BASE + 0x00)
#define WDT_RELOAD_REG		(AST_WDT_VA_BASE + 0x04)
#define WDT_CNT_RESTART_REG (AST_WDT_VA_BASE + 0x08)
#define WDT_CONTROL_REG		(AST_WDT_VA_BASE + 0x0C)

/********************************************************************
   Internal Register Mapping Actual = 0x16000000 to 0x1E78CFFF
   After Align the base and size    = 0x16000000 to 0x1EFFFFFF
*********************************************************************/

#define AST_REGISTER_BASE				0x16000000
#define AST_REGISTER_SIZE				0x09000000

/*--------------------------- Individual Devices ------------------------- */
#define AST_SMC_BASE					0x16000000
#define AST_AHBC_BASE                   0x1E600000
#define AST_MAC1_BASE					0x1E660000
#define AST_MAC2_BASE					0x1E680000
#define AST_IC_BASE						0x1E6C0000
#define AST_SDRAMC_BASE                 0x1E6E0000
#define AST_SCU_BASE					0x1E6E2000
#define AST_GPIO_BASE					0x1E780000
#define AST_TIMER_BASE					0x1E782000
#define AST_UART1_BASE                  0x1E783000
#define AST_UART2_BASE                  0x1E784000
#define AST_WDT_BASE					0x1E785000


/*--------------- Virtual address of the IO Registers Region  ------------*/
#define AST_REGISTER_VA_BASE			IO_ADDRESS(AST_REGISTER_BASE)

#define AST_SMC_VA_BASE                 IO_ADDRESS(AST_SMC_BASE)
#define AST_AHBC_VA_BASE                IO_ADDRESS(AST_AHBC_BASE)
#define AST_MAC1_VA_BASE                IO_ADDRESS(AST_MAC1_BASE)
#define AST_IC_VA_BASE					IO_ADDRESS(AST_IC_BASE)
#define AST_SDRAMC_VA_BASE              IO_ADDRESS(AST_SDRAMC_BASE)
#define AST_SCU_VA_BASE					IO_ADDRESS(AST_SCU_BASE)
#define AST_TIMER_VA_BASE				IO_ADDRESS(AST_TIMER_BASE)
#define AST_UART1_VA_BASE               IO_ADDRESS(AST_UART1_BASE)
#define AST_UART2_VA_BASE               IO_ADDRESS(AST_UART2_BASE)
#define AST_WDT_VA_BASE					IO_ADDRESS(AST_WDT_BASE)


#endif
