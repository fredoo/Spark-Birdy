/**************************************************************************
* Copyright 2011 Spansion LLC. All Rights Reserved.
*
* This software is owned and published by:
* Spansion LLC, 915 DeGuigne Drive, Sunnyvale, CA 94088 ("Spansion").
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software constitutes driver source code for use in programming Spansion's
* Flash memory components. This software is licensed by Spansion to be adapted only
* for use in systems utilizing Spansion's Flash memories. Spansion is not be
* responsible for misuse or illegal use of this software for devices not
* supported herein.  Spansion is providing this source code "AS IS" and will
* not be responsible for issues arising from incorrect user implementation
* of the source code herein.
*
* Spansion MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
* REGARDING THE SOFTWARE, ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED
* USE, INCLUDING, WITHOUT LIMITATION, NO IMPLIED WARRANTY OF MERCHANTABILITY,
* FITNESS FOR A  PARTICULAR PURPOSE OR USE, OR NONINFRINGEMENT.  Spansion WILL
* HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT, NEGLIGENCE OR
* OTHERWISE) FOR ANY DAMAGES ARISING FROM USE OR INABILITY TO USE THE SOFTWARE,
* INCLUDING, WITHOUT LIMITATION, ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA, SAVINGS OR PROFITS,
* EVEN IF Spansion HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
*
* This software may be replicated in part or whole for the licensed use,
* with the restriction that this Copyright notice must be included with
* this software, whether used in part or whole, at all times.
*/

//! Library inspired from the official SPANSION Low Level Driver (SLLD) for SPI FLASH
//! PFISTER Frédéric - April 2013
//! pfister.fred@gmail.com

//! This has been used and tested with SPANSION S25FL256S and S25FL512S
//! And the MCU was a MSP430FR5739

/*! slld_hal_example.c - SLLD Hardware Abstraction Layer example Code */
#include <msp430fr5739.h>
#include <stdio.h>

#include "TYPEDEF.h"
#include "Flash_inc.h"
#include "Flash.h"
#include "Buffer_Circ.h"
#include "Var_FRAM_EXTERN.h"

/*! Requirements per PCB Design and Datasheet
 * P2.2 - 	FLASH_CLK
 * P1.6 - 	FLASH_SIMO
 * P1.7 - 	FLASH_SOMI
 * P3.7	- 	FLASH_CS
 * Mode 3 with CPOL = 1 and, CPHA = 1
 * CS is enabled when Low
 * RX Buffer : UCB0RXBUF
 * TX Buffer : UCB0TXBUF */
#define SPITXDATA   UCB0TXBUF
#define SPIRXDATA   UCB0RXBUF
/*! ***************************************************************************
 * INIT_SPI - HAL INIT SPI function
 *
 ***************************************************************************/
void INIT_SPI()
{
	P1SEL1 |= BIT3 + BIT6 + BIT7;	//!Select eUSCI Pin Function
	P2SEL1 |= BIT2;					//!Select eUSCI Pin Function
	P3DIR |=BIT7;
	P3OUT |=BIT7;
	P1OUT &=0;

	UCB0CTLW0 |= UCSWRST;		//! **Put state machine in reset**
	__no_operation();

	UCB0CTLW0 |= 	UCMSB+ UCMST + UCSYNC + UCCKPH;		 //! 4-pin, 8-bit, SPI master CKPH = 0 CKPL=0 , MSB

	UCB0CTLW0 |= UCSSEL__SMCLK;              //! SMCLK = 4MHz - No prescaler
	UCB0CTLW0 &= ~UCSWRST;                   //! **Initialize USCI state machine**
	__no_operation();
}

/*! ***************************************************************************
 * FLASH_READ - HAL read function
 *
 *  input :
 *          Ad              	  system address to be used
 *          p           		  Pointer to the data buffer where to store the read data
 ***************************************************************************/
void FLASH_READ_PAGE
(
		ADDRESS  Ad,                    //! Page to be used
		PAGE    *p                    	//! Pointer to the data buffer containing data to be written
)
{
	int cycle;
	BYTE * pageTmp;
	pageTmp = (BYTE*)p;

	//! Page address (Ad*512)
	Ad= Ad<<9;

	//! Select the device
	P3OUT &= ~BIT7; 			//!FLASH_CS low

	//! Send the command
	while((UCB0STAT & UCBUSY) == UCBUSY);
	SPITXDATA = SPI_READ_4B_CMD;	//!0x13 - Write command to UCB0TXBUF (SPI Buffer)

	//! Send the 4-byte address
	while((UCB0STAT & UCBUSY) == UCBUSY);
	SPITXDATA = (BYTE)((Ad >> 24) & 0x000000FF);
	while((UCB0STAT & UCBUSY) == UCBUSY);
	SPITXDATA = (BYTE)((Ad >> 16) & 0x000000FF);
	while((UCB0STAT & UCBUSY) == UCBUSY);
	SPITXDATA = (BYTE)((Ad >>  8) & 0x000000FF);
	while((UCB0STAT & UCBUSY) == UCBUSY);
	SPITXDATA = (BYTE) (Ad        & 0x000000FF);

	for(cycle = 0; cycle < PAGESIZE ; cycle++)
	{
		while((UCB0STAT & UCBUSY) == UCBUSY);
		SPITXDATA = 0x00; //! Send Dummy
		//! Read the data, store it to Page STRUCT
		*(pageTmp+cycle) = SPIRXDATA;
	}

	//! Deselect the device when finished
	while((UCB0STAT & UCBUSY) == UCBUSY);
	P3OUT |= BIT7; //!FLASH_CS HIGH
}


/*!**************************************************************************
 *  FLASH_WRITE - HAL write function
 *
 *  input :
 *        \param Ad                 system address to be used
 *        \param p              Pointer to the data buffer where to store the written data
 ***************************************************************************/

void FLASH_WRITE_PAGE
(
		ADDRESS  Ad,                    /*! Page to be used */
		PAGE    *p                    	/*! Pointer to the data buffer containing data to be written */
)
{
	unsigned int cycle;
	BYTE * pageTmp;
	pageTmp = (BYTE*)p;
	BYTE Status_Read;

	//! Page address (Ad*512)
	Ad= Ad<<9;

	//! Write Enable
	P3OUT &= ~BIT7; //!FLASH_CS low

		while((UCB0STAT & UCBUSY) == UCBUSY);
		SPITXDATA = SPI_WREN_CMD;
		while((UCB0STAT & UCBUSY) == UCBUSY);

	P3OUT |= BIT7; //!FLASH_CS HIGH
	_no_operation();

	//! Read status and untill WREN OK
	P3OUT &= ~BIT7; //!FLASH_CS low
		while((UCB0STAT & UCBUSY) == UCBUSY);
		SPITXDATA = SPI_RDSR_CMD; //! Send Read Status Register CMD
		do
		{
			SPITXDATA =0x00; 		//! Send Dummy
			Status_Read = SPIRXDATA;

		}while((Status_Read & 0x02) == 0);
	while((UCB0STAT & UCBUSY) == UCBUSY);
	P3OUT |= BIT7; //!FLASH_CS HIGH
	_no_operation();

	//! Send the read command
	P3OUT &= ~BIT7; //!FLASH_CS low
		while((UCB0STAT & UCBUSY) == UCBUSY);
		SPITXDATA = SPI_PP_4B_CMD;//!0x12 - Write command to UCB0TXBUF (SPI Buffer)

		//! Send the 4-byte address
		while((UCB0STAT & UCBUSY) == UCBUSY);
		SPITXDATA = (BYTE)((Ad >> 24) & 0x000000FF);
		while((UCB0STAT & UCBUSY) == UCBUSY);
		SPITXDATA = (BYTE)((Ad >> 16) & 0x000000FF);
		while((UCB0STAT & UCBUSY) == UCBUSY);
		SPITXDATA = (BYTE)((Ad >>  8) & 0x000000FF);
		while((UCB0STAT & UCBUSY) == UCBUSY);
		SPITXDATA = (BYTE) (Ad        & 0x000000FF);

		for(cycle = 0; cycle < PAGESIZE ; cycle++)
		{
			SPITXDATA = *(pageTmp+cycle);
		}

		//! Deselect the device when finished
	while((UCB0STAT & UCBUSY) == UCBUSY);
	P3OUT |= BIT7; //!FLASH_CS HIGH
}

//! HAL Read Status function
BYTE READ_STATUS()
{
	//! Select the device
	P3OUT &= ~BIT7; //!FLASH_CS low

	while((UCB0STAT & UCBUSY) == UCBUSY);
	SPITXDATA = SPI_RDSR_CMD;//!05h - Send Read Status Register CMD

	while((UCB0STAT & UCBUSY) == UCBUSY);
	SPITXDATA =0x00; 		//! Send Dummy


	//! Deselect the device when finished
	while((UCB0STAT & UCBUSY) == UCBUSY);
	P3OUT |= BIT7; //!FLASH_CS HIGH

	return SPIRXDATA;

}

//! HAL Sector Erase fnction
void BULK_ERASE()
{
	//! Select the device
	P3OUT &= ~BIT7; //!FLASH_CS low

	//! Write Enable
	while((UCB0STAT & UCBUSY) == UCBUSY);
	SPITXDATA = SPI_WREN_CMD;

	while((UCB0STAT & UCBUSY) == UCBUSY);
	SPITXDATA = SPI_BE_CMD; //! 0xC7 - BULK ERASE ... nothing left on the chip !

	//! Deselect the device when finished
	while((UCB0STAT & UCBUSY) == UCBUSY);
	P3OUT |= BIT7; //!FLASH_CS HIGH
}
//! HAL Software reset function
void SOFTWARE_RESET()
{
	//! Select the device
	P3OUT &= ~BIT7; //!FLASH_CS low

	while((UCB0STAT & UCBUSY) == UCBUSY);
	SPITXDATA = SPI_SOFTWARE_RESET; //! 0xF0 Complete memory erase

	//! Deselect the device when finished
	while((UCB0STAT & UCBUSY) == UCBUSY);
	P3OUT |= BIT7; //!FLASH_CS HIGH
}

void READ_ID()
{
	int cycle;

	//! Select the device
	P3OUT &= ~BIT7; //!FLASH_CS low

	//! Send the command
	SPITXDATA = SPI_RDID_9F_CMD; //! Write command to UCB0TXBUF (SPI Buffer)

	//! Send the 4-byte address
	while((UCB0STAT & UCBUSY) == UCBUSY);
	SPITXDATA = 0;
	while((UCB0STAT & UCBUSY) == UCBUSY);
	SPITXDATA = 0;
	while((UCB0STAT & UCBUSY) == UCBUSY);
	SPITXDATA = 0;
	while((UCB0STAT & UCBUSY) == UCBUSY);
	SPITXDATA = 0;

	for(cycle = 88; cycle > 0 ; cycle--)
	{
		while((UCB0STAT & UCBUSY) == UCBUSY);
		SPITXDATA = 0x0; //! Send Dummy
		// Read the data, store it to Page STRUCT
		//	*(buffer+cycle) = SPIRXDATA; // Not used, only to check with the oscilloscope/logic analyzer
	}

	//! Deselect the device when finished
	while((UCB0STAT & UCBUSY) == UCBUSY);
	P3OUT |= BIT7; //!FLASH_CS HIGH
}

void InitPage(PAGE *p)
{
	p->NbTrame=0;
	p->Statut[0]=0xff; // bit à 1 ==> pas transmis en RF.
	p->Statut[1]=0xff;
	p->Statut[2]=0xff;
}

void RechercheDernierePageStockee(ADDRESS *DernierePageStockee)
{
	*DernierePageStockee = 0;
	do
	{
		*DernierePageStockee += 1;
		FLASH_READ_PAGE(*DernierePageStockee,&Page_Travail);
	} while (Page_Travail.NbTrame==23);
}

/*****************************************************************************/
