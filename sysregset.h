// *****************************************
// SCTLR_EL1, System Control Register (EL1)
// *****************************************

#define	RESERVED	(3 << 28) | (3 << 22) | (1 << 20) | (1 << 11)
#define	I_CACHE_DISABLE	(0 << 12)
#define	D_CACHE_DISABLE	(0 << 12)
#define MMU_DISABLE	(0 << 0)

#define SCTLR_set	(RESERVED | I_CACHE_DISABLE | D_CACHE_DISABLE | MMU_DISABLE)

// *******************************************
// TCR_EL1, Translation Control Register (EL1)
// *******************************************

#define TCR_CONFIG_REGION_48bit (((64 - 48) << 0) | ((64 - 48) << 16))
#define TCR_CONFIG_4KB ((0b00 << 14) |  (0b10 << 30))
#define TCR_set (TCR_CONFIG_REGION_48bit | TCR_CONFIG_4KB)

// *****************************************************
// MAIR_EL1, Memory Attribute Indirection Register (EL1)
// *****************************************************

#define MAIR_DEVICE_nGnRnE 0b00000000
#define MAIR_NORMAL_NOCACHE 0b01000100
#define MAIR_IDX_DEVICE_nGnRnE 0
#define MAIR_IDX_NORMAL_NOCACHE 1

#define MAIR_set  ((MAIR_DEVICE_nGnRnE << (MAIR_IDX_DEVICE_nGnRnE * 8)) | (MAIR_NORMAL_NOCACHE << (MAIR_IDX_NORMAL_NOCACHE * 8)))

// ************************************************
// HCR_EL2, Hypervisor Configuration Register (EL2)
// ************************************************

#define HCR_RW	    	(1 << 31)
#define HCR_set		HCR_RW

// ********************************************
// SCR_EL3, Secure Configuration Register (EL3)
// ********************************************

#define SCR_RESERVED	(3 << 4)
#define SCR_SMD_enable	(1 << 7)
#define SCR_HC_enable	(1 << 8)
#define SCR_RW		(1 << 10)
#define SCR_NS		(1 << 0)
#define SCR_set		(SCR_RESERVED | SCR_SMD_enable | SCR_HC_enable | SCR_RW | SCR_NS)

// ***************************************
// SPSR_EL3, Saved Program Status Register (EL3) Page 389 of AArch64-Reference-Manual.
// ***************************************

#define SPSR_MASK_ALL 	(15 << 6)
#define SPSR_EL1h	(5 << 0)
#define SPSR_EL2h	(9 << 0)
#define SPSR_set	(SPSR_MASK_ALL | SPSR_EL1h)

#define ESR_ELx_EC_SHIFT		26
#define ESR_ELx_EC_SVC64		0x15
