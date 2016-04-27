/*
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <common.h>
#include <asm/io.h>
#include <command.h>
#include <pci.h>
#include "ast2400_hw.h"

#define AHB_REG_BASE		0x1E600000
#define AHB_UNLOCK_MAGIC 	0xAEED1A03
#define AHB_REMAP_REG		0x8C

int board_init (void)
{
    DECLARE_GLOBAL_DATA_PTR;
    unsigned long reg;

    *((volatile ulong *) AHB_REG_BASE) = AHB_UNLOCK_MAGIC; /* unlock AHB controller */
    *((volatile ulong *) (AHB_REG_BASE + AHB_REMAP_REG)) = 0x01; /* memory remap */
    *((volatile ulong *) AHB_REG_BASE) = 0; /* lock AHB controller */

    /* Flash Controller */
#ifdef	CONFIG_FLASH_AST2300
    *((volatile ulong*) 0x1e620000) |= 0x800f0000;	/* enable Flash Write */
#else
    *((volatile ulong*) AST_SMC_BASE) |= 0x00001c00; /* enable Flash Write */
#endif

    *((volatile ulong*) SCU_KEY_CONTROL_REG) = 0x1688A8A8; /* unlock SCU */

    reg = *((volatile ulong*) SCU_CLK_SELECT_REG); /* LHCLK = HPLL/8 */
    reg &= 0x1c0fffff; /* PCLK  = HPLL/8 */
    reg |= 0x61b00000; /* BHCLK = HPLL/8 */ /* 0x61b0 and 0x6180 */
    *((volatile ulong*) SCU_CLK_SELECT_REG) = reg;

    reg = *((volatile ulong*) SCU_CLK_STOP_REG); /* enable 2D Clk */
    reg &= 0xFFFFFFFD;
    *((volatile ulong*) SCU_CLK_STOP_REG) = reg;

    /* Enable the reference clock divider (div13) for UART1 and UART2 */
    *((volatile unsigned long *) SCU_MISC_CONTROL_REG) |= 0x1000;

#if 0
    /* Fix Console bug on DOS SCU90: Enable I2C3~14*/ //Disable I2C6 AIYAM015
    *(volatile u32 *) (AST_SCU_VA_BASE + 0x90) |= 0xFF7A000;

    /* Enable SPICS1# or EXTRST# function pin SCU80[15]=1 */
    /* Fix Console bug on DOS SCU80: Enable UART3 and UART4 setting*/
    *(volatile u32 *) (AST_SCU_VA_BASE + 0x80) |= 0xFFFF8000;

    /* Fix Console bug on DOS SCU84: Enable UART1 and UART2 setting , Enable VGA setting*/
    *(volatile u32 *) (AST_SCU_VA_BASE + 0x84) |= 0xFFFFF000;

    /* Enable external SOC reset function SCU3C[3]=1 */
    *(volatile u32 *) (AST_SCU_VA_BASE + 0x3C) |= 0x8;

    *((volatile ulong*) SCU_KEY_CONTROL_REG) = 0; /* lock SCU */
#endif

    /* arch number */
    gd->bd->bi_arch_number = 900; //MACH_TYPE_ASPEED;

    /* adress of boot parameters */
    gd->bd->bi_boot_params = 0x40000100;

    timer_init();

    return 0;
}

int dram_init (void)
{
    DECLARE_GLOBAL_DATA_PTR;

    /* dram_init must store complete ramsize in gd->ram_size */
    gd->ram_size = get_ram_size((void *)PHYS_SDRAM_1, PHYS_SDRAM_1_SIZE);

    return 0;
}

/*
SCU7C: Silicon Revision ID Register
D[31:24]: Chip ID
0: AST2050/AST2100/AST2150/AST2200/AST3000
1: AST2300

D[23:16] Silicon revision ID for AST2300 generation and later
0: A0
1: A1
2: A2
.
.
.
FPGA revision starts from 0x80


D[11:8] Bounding option

D[7:0] Silicon revision ID for AST2050/AST2100 generation (for software compatible)
0: A0
1: A1
2: A2
3: A3
.
.
FPGA revision starts from 0x08, 8~10 means A0, 11+ means A1, AST2300 should be assigned to 3
*/
int wait_calibration_done()
{
	DECLARE_GLOBAL_DATA_PTR;
	unsigned char data;
	unsigned long reg, count = 0;

	do {
		udelay(1000);
		count++;
		if (count >= 1000) {

			return 1;
		}
	} while ((*(volatile ulong*) 0x1e6ec000) & 0xf00);

//	printf ("count = %d\n", count);

	return 0;
}

/* AST1070 Calibration
Program 0x101 to 0x1e6ec000
Wait till 1e6ec000 [8] = 0
Check 0x1e6ec004 = 0x5a5a5a5a
*/
int ast1070_calibration()
{
	DECLARE_GLOBAL_DATA_PTR;
	unsigned char data;
	unsigned long reg, i, j;

	//only for 2 chip
	for (i = 0; i < 2; i++) {
		for (j = 0; j < 4; j++) {
//			printf ("chip = %d, delay = %d\n", i, j);
			*((volatile ulong*) 0x1e6ec000) = (j << (12 + i * 2)) + (1 << (8 + i)) + 0x01;
//			printf ("1e6ec000 = %x\n", *(volatile ulong*)0x1e6ec000);
			if (!wait_calibration_done()) {
				if ((*(volatile ulong*) 0x1e6ec004) == 0x5a5a5a5a) {
//					printf ("calibration result: chip %d pass, timing = %d\n", i, j);
					break;
				}
				else {
//					printf ("calibration result: chip %d fail, timing = %d\n", i, j);
				}
			}
		}
	}

	return 0;
}

static void watchdog_init()
{
#ifdef CONFIG_ASPEED_ENABLE_WATCHDOG
#define AST_WDT_BASE 0x1e785000
#define AST_WDT_CLK (1*1000*1000) /* 1M clock source */
  u32 reload = AST_WDT_CLK * CONFIG_ASPEED_WATCHDOG_TIMEOUT;
  /* set the reload value */
  __raw_writel(reload, AST_WDT_BASE + 0x04);
  /* magic word to reload */
  __raw_writel(0x4755, AST_WDT_BASE + 0x08);
  /* start the watchdog with 1M clk src and reset whole chip */
  __raw_writel(0x33, AST_WDT_BASE + 0x0c);
  printf("Watchdog: %us\n", CONFIG_ASPEED_WATCHDOG_TIMEOUT);
#endif
}

int misc_init_r(void)
{
    unsigned int reg, reg1, revision, chip_id, lpc_plus;

#ifdef CONFIG_AST1070
	//Reset AST1070 and AST2400 engine [bit 23:15]
	reg = *((volatile ulong*) 0x1e7890a0);
	reg &= ~0x808000;
	*((volatile ulong*) 0x1e7890a0) = reg;

	udelay(5000);

	lpc_plus = (*((volatile ulong*) 0x1e780070)) & 0x2;

	reg = *((volatile ulong*) 0x1e7890a0);
	reg |= 0x800000;
	*((volatile ulong*) 0x1e7890a0) = reg;

	udelay(1000);

	reg = *((volatile ulong*) 0x1e7890a0);
	reg |= 0x008000;
	*((volatile ulong*) 0x1e7890a0) = reg;


	if(lpc_plus) {
		*((volatile ulong*) 0x1E60008C) |= 0x011;		/* map DRAM to 0x00000000 and LPC+ 0x70000000*/

		//SCU multi-Function pin
		reg = *((volatile ulong*) 0x1e6e2090);
		reg |= (1 << 30);
		*((volatile ulong*) 0x1e6e2090) = reg;
		//LPC+ Engine Enable
		reg = *((volatile ulong*) 0x1e6ec000);
		reg |= 1;
		*((volatile ulong*) 0x1e6ec000) = reg;

		ast1070_calibration();

	} else {
		// enable AST1050's LPC master
		reg = *((volatile ulong*) 0x1e7890a0);
		*((volatile ulong*) 0x1e7890a0) |= 0x11;

	}

#endif
    /* Show H/W Version */
    reg1 = (unsigned int) (*((ulong*) 0x1e6e207c));
    chip_id = (reg1 & 0xff000000) >> 24;
    revision = (reg1 & 0xff0000) >> 16;

    puts ("H/W:   ");
    if (chip_id == 1) {
    	if (revision >= 0x80) {
    		printf("AST2300 series FPGA Rev. %02x \n", revision);
    	}
    	else {
    		printf("AST2300 series chip Rev. %02x \n", revision);
    	}
    }
    else if (chip_id == 2) {
    	printf("AST2400 series chip Rev. %02x \n", revision);
    }
    else if (chip_id == 0) {
		printf("AST2050/AST2150 series chip\n");
    }

#ifdef CONFIG_AST1070
	if(lpc_plus) {
		puts ("C/C:   LPC+ :");
		revision = (unsigned int) (*((ulong*) 0x70002034));
		printf("AST1070 ID [%08x] ", revision);

		if((*((volatile ulong*) 0x1e780070)) & 0x4) {
			if((unsigned int) (*((ulong*) 0x70012034)) == 0x10700001)
				printf(", 2nd : AST1070 ID [%08x] \n", (unsigned int) (*((ulong*) 0x70012034)));
			else
				printf("\n");
		} else {
			printf("\n");
		}
	} else {
		puts ("C/C:   LPC  :");
		revision = (unsigned int) (*((ulong*) 0x60002034));
		printf("LPC : AST1070 ID [%08x] \n", revision);

	}
#endif

#ifdef	CONFIG_PCI
    pci_init ();
#endif

    if (getenv ("verify") == NULL) {
	setenv ("verify", "n");
    }
    if (getenv ("eeprom") == NULL) {
	setenv ("eeprom", "y");
    }

    watchdog_init();
}

#ifdef	CONFIG_PCI
static struct pci_controller hose;

extern void aspeed_init_pci (struct pci_controller *hose);

void pci_init_board(void)
{
    aspeed_init_pci(&hose);
}
#endif

int board_eth_init(bd_t *bis)
{
  int ret = -1;
#if defined(CONFIG_ASPEEDNIC)
  ret = aspeednic_initialize(bis);
#elif defined(CONFIG_ASPEED_ETH)
  ret = ast_eth_initialize(bis);
#else
  printf("No ETH, ");
#endif

  return ret;
}
