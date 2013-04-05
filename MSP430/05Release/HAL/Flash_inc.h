

/* slld.h - Header file for Spansion SPI Low Level Driver */

// ==================
// Status error flags
// ==================
#define HAS_NOTHING          0x00
#define HAS_STATBIT5_ERROR   0x01
#define HAS_STATBIT6_ERROR   0x02


// =======================
// SPI Flash Commands info
// =======================

#define SPI_WRSR_CMD                (0x01)
#define SPI_WRR_CMD                 (0x01)
#define SPI_PP_CMD                  (0x02)
#define SPI_READ_CMD                (0x03)
#define SPI_WRDI_CMD                (0x04)
#define SPI_RDSR_CMD                (0x05)
#define SPI_WREN_CMD                (0x06)
#define SPI_RDSR2_CMD               (0x07)  // Read Status Register-2
#define SPI_FAST_READ_CMD           (0x0B)
#define SPI_FAST_READ_4B_CMD        (0x0C)
#define SPI_FAST_READ_DDR_CMD       (0x0D)
#define SPI_FAST_READ_DDR_4B_CMD    (0x0E)
#define SPI_PP_4B_CMD               (0x12)
#define SPI_READ_4B_CMD             (0x13)
#define SPI_ABRD_CMD                (0x14)  // AutoBoot Register Read
#define SPI_ABWR_CMD                (0x15)  // AutoBoot Register Write
#define SPI_BRRD_CMD                (0x16)  // Bank Register Read
#define SPI_BRWR_CMD                (0x17)  // Bank Register Write
#define SPI_ECCRD_CMD               (0x18)  // ECC Read
#define SPI_P4E_CMD                 (0x20)
#define SPI_P4E4_CMD                (0x21)  // Parameter 4K-sector Erase (4Byte Addr)
#define SPI_RASP_CMD                (0x2B)
#define SPI_WASP_CMD                (0x2F)
#define SPI_CLSR_CMD                (0x30)
#define SPI_QPP_CMD                 (0x32)
#define SPI_RDSR3_CMD               (0x33)  // Read Status Register-3
#define SPI_QPP_4B_CMD              (0x34)
#define SPI_RCR_CMD                 (0x35)
#define SPI_QPP2_CMD                (0x38)  // Quad Page Program (3Byte Addr)
#define SPI_DUALIO_RD_CMD           (0x3B)
#define SPI_DUALIO_RD_4B_CMD        (0x3C)
#define SPI_P8E_CMD                 (0x40)
#define SPI_DLPRD_CMD               (0x41)  // Read Data Learning Pattern
#define SPI_OTPP_CMD                (0x42)
#define SPI_PROGRAM_SECURITY_CMD    (0x42)  // Program Security Register
#define SPI_PNVDLR_CMD              (0x43)  // Program NV Data Learning Register
#define SPI_ERASE_SECURITY_CMD      (0x44)  // Erase Security Register
#define SPI_READ_SECURITY_CMD       (0x48)  // Read Security Register
#define SPI_WVDLR_CMD               (0x4A)  // Write Volatile Data Learning Register
#define SPI_OTPR_CMD                (0x4B)
#define SPI_READ_UNIQUE_ID_CMD      (0x4B)  // Read Unique ID Number
#define SPI_P8E_4B_CMD              (0x4C)
#define SPI_WRITE_VOLATILE_CMD      (0x50)  // Write Enable for Volatile Status Register
#define SPI_BE32KB_CMD              (0x52)  // Block Erase 32KB
#define SPI_READ_SFDP_CMD           (0x5A)  // Read Serial Flash Discoverable Parameter Register
#define SPI_BE1_CMD                 (0x60)  // Bulk Erase
#define SPI_QUADIO_RD_CMD           (0x6B)
#define SPI_QUADIO_RD_4B_CMD        (0x6C)
#define SPI_ERS_SSP_CMD             (0x75)  // Erase / Program Suspend
#define SPI_SETBURSTWRAP_CMD        (0x77)  // Set Burst with Wrap
#define SPI_ERS_RES_CMD             (0x7A)  // Erase / Program Resume
#define SPI_PGSP_CMD                (0x85)  // Program Suspend
#define SPI_PGRS_CMD                (0x8A)  // Program Resume
#define SPI_READID_90_CMD           (0x90)
#define SPI_READID_DUAL_CMD         (0x92)  // Read Device ID by Dual
#define SPI_READID_QUAD_CMD         (0x94)  // Read Device ID by Quad
#define SPI_RDID_9F_CMD             (0x9F)
#define SPI_READ_ID_9F_CMD          (0x9F)
#define SPI_MPM_CMD                 (0xA3)
#define SPI_PLBWR_CMD               (0xA6)  // PPB Lock Bit Write
#define SPI_PLBRD_CMD               (0xA7)  // PPB Lock Bit Read
#define SPI_READ_ID_AB_CMD          (0xAB)
#define SPI_RDID_AB_CMD             (0xAB)
#define SPI_RES_CMD                 (0xAB)
#define SPI_SP_CMD                  (0xB9)
#define SPI_DP_CMD                  (0xB9)
#define SPI_DUALIO_HPRD_CMD         (0xBB)
#define SPI_DUALIO_HPRD_4B_CMD      (0xBC)
#define SPI_DDR_DUALIO_HPRD_CMD     (0xBD)
#define SPI_DDR_DUALIO_HPRD_4B_CMD  (0xBE)
#define SPI_BE_CMD                  (0xC7)  // BULK ERASE
#define SPI_SECTOR_ERASE_CMD        (0xD8)  // Sector ERASE
#define SPI_SE_CMD                  (0xD8)
#define SPI_SE_4B_CMD               (0xDC)
#define SPI_DYB_RD_CMD              (0xE0)  // DYB Read
#define SPI_DYB_PG_CMD              (0xE1)  // DYB Write
#define SPI_PPB_RD_CMD              (0xE2)  // PPB Read
#define SPI_PPB_PG_CMD              (0xE3)  // PPB Write
#define SPI_OCTALWORDREADQUAD_CMD   (0xE3)  // Octal Word Read Quad
#define SPI_PPB_ERS_CMD             (0xE4)  // PPB Erase
#define SPI_PASSRD_CMD              (0xE7)  // Password Read
#define SPI_RPWD_CMD                (0xE7)
#define SPI_WORDREADQUAD_CMD        (0xE7)  // Word Read Quad
#define SPI_PASSP_CMD               (0xE8)  // Password Program
#define SPI_WPWD_CMD                (0xE8)
#define SPI_PASSU_CMD               (0xE9)  // Password Unlock
#define SPI_QUADIO_HPRD_CMD         (0xEB)
#define SPI_QUADIO_HPRD_4B_CMD      (0xEC)
#define SPI_DDR_QUADIO_HPRD_CMD     (0xED)
#define SPI_DDR_QUADIO_HPRD_4B_CMD  (0xEE)
#define SPI_SOFTWARE_RESET          (0xF0)
#define SPI_RMB_CMD                 (0xFF)
#define SPI_READMODE_RESET_CMD      (0xFF)  // Continuous Read Mode Reset
#define SPI_WRB_PP_CMD              (0x00) // Reserved: Write buffer autodetect programming
#define SPI_RCVR_CMD                (0x00) // Reserved: Initiate recovery mode (manually refreshing ECC)
#define SPI_RCSP_CMD                (0x00) // Reserved: Recovery suspend
#define SPI_RCRS_CMD                (0x00) // Reserved: Recovery resume

/* Voltage States */
#define SLLD_P_VIL                  (0x0001)
#define SLLD_P_VIH                  (0x0002)


#ifndef UCHAR
typedef unsigned char   UCHAR;
#endif
#ifndef USHORT
typedef unsigned short  USHORT;
#endif
#ifndef UINT
typedef unsigned int    UINT;
#endif
#ifndef ULONG
typedef unsigned long   ULONG;
#endif
