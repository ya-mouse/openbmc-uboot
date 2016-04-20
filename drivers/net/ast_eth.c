#include <common.h>

#if defined(CONFIG_CMD_NET)
#include <malloc.h>
#include <net.h>

#include <ast2400_hw.h>

/* registers of AST ethernet MAC */
#define ISR_REG				0x00 /* interrups status register */
#define IER_REG				0x04 /* interrupt maks register */
#define MAC_MADR_REG		0x08 /* MAC address (Most significant) */
#define MAC_LADR_REG		0x0C /* MAC address (Least significant) */

#define MAHT0_REG			0x10 /* Multicast Address Hash Table 0 register */
#define MAHT1_REG			0x14 /* Multicast Address Hash Table 1 register */
#define TXPD_REG			0x18 /* Transmit Poll Demand register */
#define RXPD_REG			0x1C /* Receive Poll Demand register */
#define TXR_BADR_REG		0x20 /* Transmit Ring Base Address register */
#define RXR_BADR_REG		0x24 /* Receive Ring Base Address register */

#define HPTXPD_REG			0x28 /* High Priority Transmit Poll Demand register */
#define HPTXR_BADR_REG		0x2C /* High Priority Receive Poll Demand register */

#define ITC_REG				0x30 /* interrupt timer control register */
#define APTC_REG			0x34 /* Automatic Polling Timer control register */
#define DBLAC_REG			0x38 /* DMA Burst Length and Arbitration control register */
#define DMAFIFOS_REG		0x3C

#define FEAR_REG			0x44
#define TPAFCR_REG			0x48
#define RBSR_REG			0x4C /* Receive Buffer Size */
#define MACCR_REG			0x50 /* MAC control register */
#define MACSR_REG			0x54 /* MAC status register */
#define PHYCR_REG			0x60 /* PHY control register */
#define PHYDATA_REG			0x64 /* PHY Write Data register */
#define FCR_REG				0x68 /* Flow Control register */
#define BPR_REG				0x6C /* back pressure register */
#define WOLCR_REG			0x70 /* Wake-On-Lan control register */
#define WOLSR_REG			0x74 /* Wake-On-Lan status register */
#define WFCRC_REG			0x78 /* Wake-up Frame CRC register */
#define WFBM1_REG			0x80 /* wake-up frame byte mask 1st double word register */
#define WFBM2_REG			0x84 /* wake-up frame byte mask 2nd double word register */
#define WFBM3_REG			0x88 /* wake-up frame byte mask 3rd double word register */
#define WFBM4_REG			0x8C /* wake-up frame byte mask 4th double word register */

/* bits of MACCR */
#define MACCR_SW_RST			(1UL<<31)	// software reset/
#define MACCR_SPEED_100M_MODE	(1UL<<19)
#define MACCR_RX_UDPCS_FAIL		(1UL<<18)
#define MACCR_RX_BROADPKT		(1UL<<17)	// Receiving broadcast packet
#define MACCR_RX_MULTIPKT		(1UL<<16)	// receiving multicast packet
#define MACCR_RX_HT_EN			(1UL<<15)
#define MACCR_RX_ALLADR			(1UL<<14)	// not check incoming packet's destination address
#define MACCR_JUMBO_LF			(1UL<<13)
#define MACCR_RX_RUNT			(1UL<<12)	// Store incoming packet even its length is les than 64 byte
#define MACCR_CRC_CHK			(1UL<<11)
#define MACCR_CRC_APD			(1UL<<10)	// append crc to transmit packet
#define MACCR_GMAC_MODE			(1UL<<9)
#define MACCR_FULLDUP			(1UL<<8)	// full duplex
#define MACCR_ENRX_IN_HALFTX	(1UL<<7)
#define MACCR_LOOP_EN			(1UL<<6)	// Internal loop-back
#define MACCR_HPTXR_EN			(1UL<<5)
#define MACCR_REMOVE_VLAN		(1UL<<4)
#define MACCR_RXMAC_EN			(1UL<<3) 	// receiver enable
#define MACCR_TXMAC_EN			(1UL<<2)	// transmitter enable
#define MACCR_RXDMA_EN			(1UL<<1)	// enable DMA receiving channel
#define MACCR_TXDMA_EN			(1UL<<0)	// enable DMA transmitting channel

/* bits of PHY R/W Register */
#define MIIWR                   (1UL<<27)
#define MIIRD                   (1UL<<26)
#define MDC_CYCTHR              0x34

/* Marvell 88E1111 register bits */
#define MARVELL_88E1111_OUI		0x005043
#define PHY_SPEED_MASK          0xC000
#define PHY_DUPLEX_MASK         0x2000
#define SPEED_1000M             0x02
#define SPEED_100M              0x01
#define SPEED_10M               0x00
#define DUPLEX_FULL             0x01
#define DUPLEX_HALF             0x00
#define RESOLVED_BIT            0x800

/* Broadcom BCM5221 register bits */
#define BROADCOM_BCM5221_OUI	0x001018
#define BROADCOM_BCM5241_OUI	0x0050EF
#define PHY_SPEED_DUPLEX_MASK   0x01E0
#define PHY_100M_DUPLEX         0x0100
#define PHY_100M_HALF           0x0080
#define PHY_10M_DUPLEX          0x0040
#define PHY_10M_HALF            0x0020

#define REALTEK_RTL82X1X_OUI	0x000732

/* Descriptor bits. */
#define TXDMA_OWN	0x80000000	/* TX Own Bit */
#define RXPKT_RDY	0x80000000	/* RX Own Bit */
#define EDOTR		0x40000000	/* TX End Of Ring */
#define EDORR		0x40000000	/* RX End Of Ring */
#define FTS			0x20000000	/* TX First Descriptor Segment */
#define FTS			0x20000000	/* RX First Descriptor Segment */
#define LTS			0x10000000	/* TX Last Descriptor Segment */
#define LRS			0x10000000	/* RX Last Descriptor Segment */
#define TD_SET		0x08000000	/* Setup Packet */
#define RX_ODD_NB	0x00400000
#define RUNT		0x00200000
#define FTL			0x00100000
#define CRC_ERR		0x00080000	/* CRC error */
#define RX_ERR		0x00040000	/* RX error */

/* bits of interrupt status/enable register */
#define ISR_RXBUF_UNAVA				0x00000004

#define POLL_DEMAND	1 /* value written to TXPD */

#define NUM_RX_DESC PKTBUFSRX	/* Number of RX descriptors   */
#define NUM_TX_DESC 1			/* Number of TX descriptors   */
#define RX_BUFF_SZ  PKTSIZE_ALIGN

#define ETH_ALEN 6
#define TOUT_LOOP 1000000
#define PHY_WAIT_TIMES 300000
#define MIN_DATA_LEN 60 /* minimum data length of ethernet packet */

/* ring descriptor structure */
struct ast_eth_desc {
	volatile s32 status;
	u32 control;
	u32 reserved;
	u32 buf;
};

struct ast_eth_priv {
	int id;
};

static struct ast_eth_desc rx_ring[NUM_RX_DESC] __attribute__ ((aligned(32))); /* RX descriptor ring */
static struct ast_eth_desc tx_ring[NUM_TX_DESC] __attribute__ ((aligned(32))); /* TX descriptor ring */
static int rx_new; /* RX descriptor ring pointer */
static int tx_new; /* TX descriptor ring pointer */

#if (CONFIG_SPX_FEATURE_GLOBAL_NIC_COUNT == 4)
    static u32 ast_eth_reg_addr_base[4] = {0x1E660000, 0x1E680000,0x1e670000,0x1e690000};
#elif (CONFIG_SPX_FEATURE_GLOBAL_NIC_COUNT == 3)
    static u32 ast_eth_reg_addr_base[3] = {0x1E660000, 0x1E680000, 0x1e670000};
#else
static u32 ast_eth_reg_addr_base[2] = {0x1E660000, 0x1E680000};
#endif

static int ast_eth_read_reg(struct eth_device* dev, u_long addr)
{
	return le32_to_cpu(*((volatile u_long *) (addr + dev->iobase)));
}

static void ast_eth_write_reg(struct eth_device* dev, int command, u_long addr)
{
	*((volatile u_long *) (addr + dev->iobase)) = cpu_to_le32(command);
}

static u16 ast_eth_read_phy_reg(struct eth_device* dev, u8 PHY_Register, u8 PHY_Address)
{
	u32 Data, Status = 0, Loop_Count = 0, PHY_Ready = 1;
	u16 Return_Data;

	ast_eth_write_reg(dev, (PHY_Register << 21) + (PHY_Address << 16) + MIIRD + MDC_CYCTHR, PHYCR_REG);

	do {
		Status = (ast_eth_read_reg(dev, PHYCR_REG) & MIIRD);
		Loop_Count ++;
		if (Loop_Count >= PHY_WAIT_TIMES) {
			PHY_Ready = 0;
			break;
		}
	} while (Status == MIIRD);
	
	if (PHY_Ready == 0) {
		printf("PHY is not ready\n");
		return 0;
	}

	Data = ast_eth_read_reg (dev, PHYDATA_REG);
	Return_Data = (Data >> 16);

	return Return_Data;
}

void ast_eth_write_phy_reg(struct eth_device* dev, u8 PHY_Register, u8 PHY_Address, u16 PHY_Data)
{
	u32 Status = 0, Loop_Count = 0, PHY_Ready = 1;

	ast_eth_write_reg(dev, PHY_Data, PHYDATA_REG);
	ast_eth_write_reg(dev, (PHY_Register << 21) + (PHY_Address << 16) + MIIWR + MDC_CYCTHR, PHYCR_REG);

	do {
		Status = (ast_eth_read_reg(dev, PHYCR_REG) & MIIWR);
		Loop_Count ++;
		if (Loop_Count >= PHY_WAIT_TIMES) {
			PHY_Ready = 0;
			break;
		}
	} while (Status == MIIWR);

	if (PHY_Ready == 0) {
		printf("PHY is not ready\n");
	}
}

#ifdef CONFIG_MACADDR_IN_EEPROM
static void ast_eth_read_addr_from_eeprom(int id, struct eth_device* dev)
{
	unsigned char mac_address[ETH_ALEN];
	char ethaddr_env[24];
	int valid_mac_addr_in_eeprom;
	int i;

	eeprom_init();
	if (id == 0) {
		eeprom_read(CFG_I2C_EEPROM_ADDR, CFG_EEPROM_MACADDR_OFFSET, mac_address, ETH_ALEN);
	} else if (id == 1) {
		eeprom_read(CFG_I2C_EEPROM_ADDR, CFG_EEPROM_MAC1ADDR_OFFSET, mac_address, ETH_ALEN);
	}
#if (CONFIG_SPX_FEATURE_GLOBAL_NIC_COUNT >= 3)

#ifdef CFG_EEPROM_MAC2ADDR_OFFSET    
    else if (id == 2) {
        eeprom_read(CFG_I2C_EEPROM_ADDR, CFG_EEPROM_MAC2ADDR_OFFSET, mac_address, ETH_ALEN);    
    }
#endif
#ifdef CFG_EEPROM_MAC3ADDR_OFFSET     
    else if (id == 3) {
        eeprom_read(CFG_I2C_EEPROM_ADDR, CFG_EEPROM_MAC3ADDR_OFFSET, mac_address, ETH_ALEN);    
    }
#endif    
#endif

	valid_mac_addr_in_eeprom = 0;
	for (i = 0; i < ETH_ALEN; i ++) {
		if (mac_address[i] != 0xFF) {
			valid_mac_addr_in_eeprom = 1;
		}
	}

	/* set MAC address to ethernet module */
	if (valid_mac_addr_in_eeprom) {
		sprintf(ethaddr_env, "%02X:%02X:%02X:%02X:%02X:%02X", mac_address[0], mac_address[1], mac_address[2], mac_address[3], mac_address[4], mac_address[5]);
		memcpy(dev->enetaddr, mac_address,6);
		if (id == 0) {
			setenv("ethaddr", ethaddr_env);
		} else if (id == 1) {
			setenv("eth1addr", ethaddr_env);
		} 
#if (CONFIG_SPX_FEATURE_GLOBAL_NIC_COUNT >= 3)
		else if (id == 2) {
			setenv("eth2addr", ethaddr_env);
		} else if (id == 3) {
			setenv("eth3addr", ethaddr_env);
		}
#endif		
		saveenv();
	}
}
#endif

static void ast_eth_set_ethaddr(struct eth_device* dev, bd_t* bis)
{
	struct ast_eth_priv *priv;

	ast_eth_write_reg(dev, ((dev->enetaddr[3] << 24) | (dev->enetaddr[2] << 16) | (dev->enetaddr[1] << 8) | dev->enetaddr[0]), MAC_LADR_REG);
	ast_eth_write_reg(dev, ((dev->enetaddr[5] << 8) | dev->enetaddr[4]), MAC_MADR_REG);
}

static void ast_eth_set_multipin(struct eth_device* dev)
{
	struct ast_eth_priv *priv;
	unsigned int scu_reg;
	
	*((volatile ulong *) SCU_KEY_CONTROL_REG) = 0x1688A8A8; /* unlock SCU */
	priv = (struct ast_eth_priv *) dev->priv;
	if (priv->id == 0) {
		scu_reg = *((volatile ulong *) (AST_SCU_VA_BASE + 0x88));	/* set MAC1 multi-function pin */
		scu_reg |= 0xC0000000;	
		*((volatile ulong *) (AST_SCU_VA_BASE + 0x88)) = scu_reg;
	} else if (priv->id == 1) {
		scu_reg = *((volatile ulong *) (AST_SCU_VA_BASE + 0x90));	/* set MAC2 multi-function pin */
		scu_reg |= 0x00000004;	
		*((volatile ulong *) (AST_SCU_VA_BASE + 0x90)) = scu_reg;
	}
	*((volatile ulong *) SCU_KEY_CONTROL_REG) = 0; /* lock SCU */
}

static int ast_eth_set_mac_ctrl(struct eth_device* dev)
{
	unsigned long ctrl_reg, phy_ready = 1;
	u16 phy_reg, phy_speed, phy_duplex, phy_model;
	u32 phy_oui;
	int wait_count;
	int rc;

	ast_eth_set_multipin(dev);
	
	rc = 0;
	ctrl_reg = MACCR_SPEED_100M_MODE | MACCR_RX_ALLADR | MACCR_FULLDUP | MACCR_RXMAC_EN | MACCR_RXDMA_EN | MACCR_TXMAC_EN | MACCR_TXDMA_EN | MACCR_CRC_APD;

	/* OUI[23:22] are not represented in PHY ID registers */
	phy_reg = ast_eth_read_phy_reg(dev, 0x02, 0);
	phy_oui = (phy_reg & 0xFFFF) << 6; /* OUI[21:6] in ID_HI[15:0] */

	phy_reg = ast_eth_read_phy_reg(dev, 0x03, 0);
	phy_oui |= (phy_reg & 0xFFFF) >> 10; /* OUI[5:0] in ID_LO[15:10] */
	phy_model = (phy_reg & 0x03F0) >> 4; /* MODEL[5:0] in ID_LO[9:4] */

	switch (phy_oui) {
	case BROADCOM_BCM5221_OUI:
	case BROADCOM_BCM5241_OUI:
		phy_reg = ast_eth_read_phy_reg(dev, 0x18, 0);

		if ((phy_reg & 0x0001) == 0)
			ctrl_reg &= ~MACCR_FULLDUP;
		if ((phy_reg & 0x0002) == 0)
			ctrl_reg &= ~MACCR_SPEED_100M_MODE;
		break;
	case REALTEK_RTL82X1X_OUI:
		switch (phy_model) {
		case 0x01:
			phy_reg = ast_eth_read_phy_reg(dev, 0, 0);
			phy_duplex = (phy_reg & 0x100) ? DUPLEX_FULL : DUPLEX_HALF;
			phy_speed = (phy_reg & 0x2000) ? SPEED_100M : SPEED_10M;
			break;
		case 0x11:
			phy_reg = ast_eth_read_phy_reg(dev, 0x11, 0);
			phy_duplex = (phy_reg & 0x2000) ? DUPLEX_FULL : DUPLEX_HALF;
			phy_speed = (phy_reg & 0xc000) >> 14;
			break;
		default:
			phy_speed = SPEED_100M;
			phy_duplex = 1;
		}
		if (phy_speed == SPEED_1000M) {
			ctrl_reg |= MACCR_GMAC_MODE;
		} else { /* 10M or 100M */
			ctrl_reg &= ~MACCR_GMAC_MODE;
			if (phy_speed == SPEED_10M) {
				ctrl_reg &= ~MACCR_SPEED_100M_MODE;
			}
		}
		if (phy_duplex == DUPLEX_HALF) {
			ctrl_reg &= ~MACCR_FULLDUP;
		}
		break;
	case MARVELL_88E1111_OUI:
		phy_ready = 0;
		for (wait_count = 0; wait_count < PHY_WAIT_TIMES; wait_count ++) {
			phy_reg = ast_eth_read_phy_reg(dev, 0x11, 0) & RESOLVED_BIT;
			if (phy_reg == RESOLVED_BIT) {
				phy_ready = 1;
				break;
			}
		}

		if (phy_ready == 1) {
			phy_reg = ast_eth_read_phy_reg(dev, 0x11, 0);
			phy_speed = (phy_reg & PHY_SPEED_MASK) >> 14;
			phy_duplex = (phy_reg & PHY_DUPLEX_MASK) >> 13;

			if (phy_speed == SPEED_1000M) {
				ctrl_reg |= MACCR_GMAC_MODE;
			} else { /* 10M or 100M */
				ctrl_reg &= ~MACCR_GMAC_MODE;
				if (phy_speed == SPEED_10M) {
					ctrl_reg &= ~MACCR_SPEED_100M_MODE;
				}
			}

			if (phy_duplex == DUPLEX_HALF) {
				ctrl_reg &= ~MACCR_FULLDUP;
			}
		} else {
			printf("Marvel 88E1111 is not ready\n");
		}
		break;
	default: /* NC-SI */
		ctrl_reg |= MACCR_SPEED_100M_MODE;
		ctrl_reg |= MACCR_FULLDUP;
	}

	ast_eth_write_reg(dev, ctrl_reg, MACCR_REG);

	return rc;
}

static int ast_eth_init(struct eth_device* dev, bd_t* bis)
{
	u32 reg;
	int i;
	int rc;

	reg = ast_eth_read_reg(dev, MACCR_REG);
	reg |= MACCR_SW_RST;
	ast_eth_write_reg(dev, reg, MACCR_REG);

	do {
		reg = ast_eth_read_reg(dev, MACCR_REG);
		udelay(1000);
	} while (reg & MACCR_SW_RST);

    ast_eth_write_reg(dev, 0, IER_REG); /* disable interrupt */

	ast_eth_set_ethaddr(dev, bis);

	rc = ast_eth_set_mac_ctrl(dev);

	/* initialize receive ring descriptor list */
	for (i = 0; i < NUM_RX_DESC; i ++) {
		rx_ring[i].status = 0;
		rx_ring[i].buf = cpu_to_le32((u32) NetRxPackets[i]);
		rx_ring[i].reserved = 0;
	}

	/* initialize transmit ring descriptor list */
	for (i = 0; i < NUM_TX_DESC; i ++) {
		tx_ring[i].status = 0;
		tx_ring[i].control = 0;
		tx_ring[i].buf = 0;
		tx_ring[i].reserved = 0;
	}

	/* write the end of list marker to the descriptor lists. */
	rx_ring[NUM_RX_DESC - 1].status |= cpu_to_le32(EDORR);
	tx_ring[NUM_TX_DESC - 1].status |= cpu_to_le32(EDOTR);

	/* tell the adapter where the TX/RX rings are located. */
	ast_eth_write_reg(dev, ((u32) &tx_ring), TXR_BADR_REG);
	ast_eth_write_reg(dev, ((u32) &rx_ring), RXR_BADR_REG);

	/* initialize descriptor list index */
	tx_new = 0;
	rx_new = 0;

	/* start to transmit and/or receive */
	reg = ast_eth_read_reg(dev, MACCR_REG);
	reg |= MACCR_RXMAC_EN | MACCR_TXMAC_EN | MACCR_RXDMA_EN | MACCR_TXDMA_EN;
	ast_eth_write_reg(dev, reg, MACCR_REG); /* Enable the TX and/or RX */

	return 1;
}

static int ast_eth_send(struct eth_device* dev, volatile void *packet, int length)
{
	int status = -1;
	int pad = 0;
	int i;

	if (length <= 0) {
		printf("%s: bad packet size: %d\n", dev->name, length);
		goto Done;
	}

	/* check ownership before transmit */
	for (i = 0; tx_ring[tx_new].status & cpu_to_le32(TXDMA_OWN); i++) {
		if (i >= TOUT_LOOP) {
			printf("%s: TX buffer not ready\n", dev->name);
			goto Done;
		}
	}

	/* pad 0 to buffer if data length less than ethernet minimun packet length */
	if (length < MIN_DATA_LEN) {
		pad = 1;
		memset ((void *)cpu_to_le32((u32) (packet + length)), 0, MIN_DATA_LEN - length);
	}

	/* set transmit descriptor */
	tx_ring[tx_new].buf = cpu_to_le32(((u32) packet));
	tx_ring[tx_new].status |= cpu_to_le32(LTS | FTS);
	if (pad)
		tx_ring[tx_new].status |= cpu_to_le32(MIN_DATA_LEN);
	else
		tx_ring[tx_new].status |= cpu_to_le32(length);
	tx_ring[tx_new].status |= cpu_to_le32(TXDMA_OWN);

	ast_eth_write_reg(dev, POLL_DEMAND, TXPD_REG); /* transmit poll demand */

	/* check ownership after transmit */
	for (i = 0; tx_ring[tx_new].status & cpu_to_le32(TXDMA_OWN); i ++) {
		if (i >= TOUT_LOOP) {
			printf("%s: TX buffer not ready\n", dev->name);
			goto Done;
		}
	}

	if (le32_to_cpu(tx_ring[tx_new].status) & CRC_ERR) {
		tx_ring[tx_new].status = 0;
		printf("%s: TX CRC error\n", dev->name);
		goto Done;
	}

	status = length;

 Done:
    tx_new = (tx_new + 1) % NUM_TX_DESC;

	return status;
}

static int ast_eth_recv(struct eth_device* dev)
{
	s32 status;
	int length = 0;
	u32 reg;

	for ( ; ; ) {
		status = (s32)le32_to_cpu(rx_ring[rx_new].status);

		if ((status & RXPKT_RDY) == 0) {
			break;
		}

		if (status & LRS) { /* Valid frame status. */
			if (status & (RX_ERR | CRC_ERR | FTL | RUNT | RX_ODD_NB)) { /* There are some errors */
				printf("%s: RX error, status = 0x%08X\n", dev->name, status);
			} else { /* A valid frame received. */
				length = (le32_to_cpu(rx_ring[rx_new].status) & 0x3FFF);

				/* Pass the packet up to the protocol layers. */
				NetReceive(NetRxPackets[rx_new], length - 4);
			}

			/* Change buffer ownership for this frame, back to the adapter. */
			rx_ring[rx_new].status &= cpu_to_le32(~(RXPKT_RDY));
		}

		/* Update entry information. */
		rx_new = (rx_new + 1) % NUM_RX_DESC;
	}

	reg = ast_eth_read_reg(dev, ISR_REG);
	if(reg & ISR_RXBUF_UNAVA){
		reg |= 	ISR_RXBUF_UNAVA;		
		ast_eth_write_reg(dev, reg, ISR_REG);		/* Clear RU Bit */
		reg = ast_eth_read_reg(dev, RXPD_REG);
		reg = 0 ;
		ast_eth_write_reg(dev, reg, RXPD_REG);		/* Issue Recv Poll Demand */
	}

	return length;
}

static void ast_eth_halt(struct eth_device* dev)
{
	u32 reg;

    reg = ast_eth_read_reg(dev, MACCR_REG);
    reg &= ~(MACCR_RXMAC_EN | MACCR_TXMAC_EN | MACCR_RXDMA_EN | MACCR_TXDMA_EN);
    ast_eth_write_reg(dev, reg, MACCR_REG); /* Disable the TX and/or RX */
}

int ast_eth_initialize(bd_t *bis)
{
	unsigned int scu_reg;
	struct eth_device *dev;
	struct ast_eth_priv *priv;
	int use_num;
	int i;

	#ifdef CONFIG_NET_MULTI
	    if(CONFIG_SPX_FEATURE_GLOBAL_NIC_COUNT > 0)
	        use_num = CONFIG_SPX_FEATURE_GLOBAL_NIC_COUNT;
	    else
	        use_num = 1;
	#else
	use_num = 1;
	#endif

	*((volatile ulong *) SCU_KEY_CONTROL_REG) = 0x1688A8A8; /* unlock SCU */

#if (CONFIG_SPX_FEATURE_GLOBAL_NIC_COUNT >= 3)
	*((volatile u_long *) SCU_F0_REG) = cpu_to_le32(0xAEED0001); /* Enable MAC#3 & MAC#4 */
	*((volatile u_long *) SCU_MAC_CLOCK_DELAY) = 0x222255; /* MAC Interface Clock Delay Setting */
#endif
	/* enable ethernet MAC clock */
	scu_reg = *((volatile unsigned long *) SCU_CLK_STOP_REG);
	if (use_num == 4)
	    scu_reg &= (~0x00F00000);
	else if (use_num == 3)
	    scu_reg &= (~0x00700000);		
	else if (use_num == 2)
		scu_reg &= (~0x00300000);
	else
		scu_reg &= (~0x00100000);
	*((volatile unsigned long *) SCU_CLK_STOP_REG) = scu_reg;

	udelay(10 * 1000);

	/* stop ethernet MAC reset */
	scu_reg = *((volatile unsigned long *) SCU_SYS_RESET_REG);
	if (use_num == 4)
	    scu_reg &= (~0x0C001800);
	else if (use_num == 3)
	    scu_reg &= (~0x04001800);
	else if (use_num == 2)
		scu_reg &= (~0x00001800);
	else
		scu_reg &= (~0x00000800);
	*((volatile unsigned long *) SCU_SYS_RESET_REG) = scu_reg;

	*((volatile ulong *) (AST_SCU_VA_BASE + 0x48)) = 0x00002255; /* set MAC clock delay */

	scu_reg = *((volatile unsigned long *) SCU_HW_STRAPPING_REG);
	if ((scu_reg & 0x00000040) || (scu_reg & 0x00000080)) {
		scu_reg = *((volatile unsigned long *) SCU_CLK_SELECT_REG);
		scu_reg &= ~(0x00070000);
		scu_reg |= 0x00010000;
		*((volatile ulong *) SCU_CLK_SELECT_REG) = scu_reg; /* set MAC clock divider to 4 */
	}

	*((volatile ulong *) SCU_KEY_CONTROL_REG) = 0; /* lock SCU */

	udelay(10 * 1000);

	for (i = 0; i < use_num; i ++) {
		dev = (struct eth_device *) malloc(sizeof(struct eth_device));
		sprintf(dev->name, "ast_eth%d", i);
		dev->iobase = ast_eth_reg_addr_base[i];
		dev->init = ast_eth_init;
		dev->halt = ast_eth_halt;
		dev->send = ast_eth_send;
		dev->recv = ast_eth_recv;

		priv = (struct ast_eth_priv *) malloc(sizeof(struct ast_eth_priv));
		priv->id = i;
		dev->priv = priv;
		#ifdef CONFIG_MACADDR_IN_EEPROM
		ast_eth_read_addr_from_eeprom(i, dev);
		#endif
		eth_register(dev);
	}

	return 0;
}

#endif /* CFG_CMD_NET */
