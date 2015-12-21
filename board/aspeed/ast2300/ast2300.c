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
#include <command.h>
#include <pci.h>

static void ast2100_board_init(void);
static void ast2300_board_init(void);

int board_init (void)
{
    DECLARE_GLOBAL_DATA_PTR;
    unsigned char data;
    unsigned long reg;
    unsigned revision, chip_id;

    /* Show H/W Version */
    reg = (unsigned int) (*((ulong*) 0x1e6e207c));
    chip_id = (reg & 0xff000000) >> 24;
    revision = (reg & 0xff0000) >> 16;

    if (chip_id == 1) {
        ast2300_board_init();
    } else if (chip_id == 0) {
        ast2100_board_init();
    }

    /* arch number */
//    gd->bd->bi_arch_number = MACH_TYPE_ASPEED;
                                                                                                                             
    /* adress of boot parameters */
    gd->bd->bi_boot_params = 0x40000100;
                                                                                                                             
    return 0;
}

static void ast2100_board_init(void)
{
    DECLARE_GLOBAL_DATA_PTR;
    unsigned long reg;
    printf("AST2100\n");
    /* AHB Controller */
    *((volatile ulong*) 0x1E600000)  = 0xAEED1A03;	/* unlock AHB controller */ 
    *((volatile ulong*) 0x1E60008C)  = 0x01;		/* map DRAM to 0x00000000 */
    *((volatile ulong*) 0x1E600000)  = 0;

    *((volatile ulong*) 0x16000000) |= 0x00001c00;	/* enable Flash Write */ 

    /* SCU */
    *((volatile ulong*) 0x1e6e2000) = 0x1688A8A8;	/* unlock SCU */
    reg = *((volatile ulong*) 0x1e6e2008);		
    reg &= 0x1c0fffff;                                  
    reg |= 0x61B00000;						/* PCLK  = HPLL/8 */				
    *((volatile ulong*) 0x1e6e2008) = reg;     
    reg = *((volatile ulong*) 0x1e6e200c);		/* enable 2D Clk */
    *((volatile ulong*) 0x1e6e200c) &= 0xFFFFFFFD;

// set d2-pll & enable d2-pll D[21:20], D[4]
//    reg = *((volatile ulong*) 0x1e6e202c);
//    reg &= 0x001000;
//    *((volatile ulong*) 0x1e6e202c) = reg;

    reg = *((volatile ulong*) 0x1e6e200c);
    *((volatile ulong*) 0x1e6e200c) &= 0xFFFFFF7F;	/* enable USB1.1 Clk */

    reg = *((volatile ulong*) 0x1e6e2004);
    *((volatile ulong*) 0x1e6e2004) &= 0xFFFFFFF7;	/* no operation for USB1.1 */

    reg = *((volatile ulong*) 0x1e6e2074);
    *((volatile ulong*) 0x1e6e2074) |= 0x100000;	/* enable MAC #2 MDC/MDIO */

    *((volatile ulong*) 0x1e6e2000) = 0;
}

static void ast2300_board_init(void)
{
    DECLARE_GLOBAL_DATA_PTR;
    unsigned long reg;
    printf("AST2300\n");

    /* AHB Controller */
    *((volatile ulong*) 0x1E600000)  = 0xAEED1A03;	/* unlock AHB controller */ 
    *((volatile ulong*) 0x1E60008C) |= 0x01;		/* map DRAM to 0x00000000 */
#ifdef CONFIG_PCI
    *((volatile ulong*) 0x1E60008C) |= 0x30;		/* map PCI */
#endif

    /* Flash Controller */
#ifdef	CONFIG_FLASH_AST2300
    *((volatile ulong*) 0x1e620000) |= 0x0001000a;	/* enable Flash Write */     
#else
    *((volatile ulong*) 0x16000000) |= 0x00001c00;	/* enable Flash Write */ 
#endif    

    /* SCU */
    *((volatile ulong*) 0x1e6e2000) = 0x1688A8A8;	/* unlock SCU */
    reg = *((volatile ulong*) 0x1e6e2008);		
    reg &= 0x1c0fffff;                                  
    reg |= 0x61800000;						/* PCLK  = HPLL/8 */				
#ifdef CONFIG_AST1070	
	reg |= 0x300000;					/* LHCLK = HPLL/8 */
	reg |= 0x80000;					/* LPC Host Clock */
#endif	
    *((volatile ulong*) 0x1e6e2008) = reg;     
    reg = *((volatile ulong*) 0x1e6e200c);		/* enable 2D Clk */
    *((volatile ulong*) 0x1e6e200c) &= 0xFFFFFFFD;
/* enable wide screen. If your video driver does not support wide screen, don't
enable this bit 0x1e6e2040 D[0]*/
    reg = *((volatile ulong*) 0x1e6e2040);
    *((volatile ulong*) 0x1e6e2040) |= 0x01;
#ifdef CONFIG_AST1070                                                                          
/*set VPPL1 */

    *((volatile ulong*) 0x1e6e201c) = 0x6420;

// set d2-pll & enable d2-pll D[21:20], D[4]
    reg = *((volatile ulong*) 0x1e6e202c);
    reg &= 0xffcfffef;
    reg |= 0x00200010;
    *((volatile ulong*) 0x1e6e202c) = reg;

// set OSCCLK = VPLL1
    *((volatile ulong*) 0x1e6e2010) = 0x8;

// enable OSCCLK
    reg = *((volatile ulong*) 0x1e6e202c);
    reg &= 0xfffffffd;
    reg |= 0x00000002;
    *((volatile ulong*) 0x1e6e202c) = reg;

// enable AST1050's LPC master
    reg = *((volatile ulong*) 0x1e7890a0);
    *((volatile ulong*) 0x1e7890a0) |= 0x11;
#endif
}

int dram_init (void)
{
    DECLARE_GLOBAL_DATA_PTR;
                                                                                                                             
//    gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
//    gd->bd->bi_dram[0].size  = PHYS_SDRAM_1_SIZE;

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
int misc_init_r(void)
{
    unsigned int reg1, revision, chip_id;

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
    else if (chip_id == 0) {
	printf("AST2050/AST2150 series chip\n");
    }

#ifdef CONFIG_AST1070
	puts ("C/C:   ");
	revision = (unsigned int) (*((ulong*) 0x60002034));
	printf("AST1070 ID [%08x] \n", revision);
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
#else
  printf("No ETH, ");
#endif

  return ret;
}
