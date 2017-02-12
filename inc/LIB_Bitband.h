/***********************************************************************************************************************
* \file     LIB_Bitband.h
* \author   Boyan Bonev
* \version  1.0
* \date     11 Feb 2017
* \brief    Bit-banding maps a complete word of memory onto a single bit in the bit-band region. For example, writing 
*           to one of the alias words will set or clear the corresponding bit in the bitband region. This allows every 
*           individual bit in the bit-banding region to be directly accessible from a word-aligned address using a 
*           single LDR instruction. It also allows individual bits to be toggled from C without performing a
*           read-modify-write sequence of instructions.
*
*           http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dai0179b/CHDJHIDF.html
*
* \copyright
***********************************************************************************************************************/

#ifndef LIB_BIT_BAND_H
#define LIB_BIT_BAND_H

/* Bit band SRAM definitions */
#define BITBAND_SRAM_REF   0x20000000
#define BITBAND_SRAM_BASE  0x22000000

#define BITBAND_SRAM(a,b) ((BITBAND_SRAM_BASE + ((a-BITBAND_SRAM_REF)<<5) + (b<<2)))  // Convert SRAM address

/* Bit band PERIPHERAL definitions */
#define BITBAND_PERI_REF   0x40000000
#define BITBAND_PERI_BASE  0x42000000

#define BITBAND_PERI(a,b) ((BITBAND_PERI_BASE + ((a-BITBAND_PERI_REF)<<5) + (b<<2)))  // Convert PERI address

/* Basic bit band function definitions */
#define BITBAND_SRAM_ClearBit(a,b)      (*(volatile uint32_t *) (BITBAND_SRAM(a,b)) = 0)
#define BITBAND_SRAM_SetBit(a,b)        (*(volatile uint32_t *) (BITBAND_SRAM(a,b)) = 1)
#define BITBAND_SRAM_GetBit(a,b)        (*(volatile uint32_t *) (BITBAND_SRAM(a,b)))

#define BITBAND_PERI_ClearBit(a,b)      (*(volatile uint32_t *) (BITBAND_PERI(a,b)) = 0)
#define BITBAND_PERI_SetBit(a,b)        (*(volatile uint32_t *) (BITBAND_PERI(a,b)) = 1)
#define BITBAND_PERI_GetBit(a,b)        (*(volatile uint32_t *) (BITBAND_PERI(a,b)))

#endif /// LIB_BIT_BAND_H