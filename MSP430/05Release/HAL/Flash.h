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
/*! slld_hal.h - HAL Header file for Spansion SPI-flash Low Level Driver */


/*! public function prototypes */
void Init_SPI();							//! HAL INIT SPI function


//! HAL read functions API
void FLASH_READ_PAGE
(
		ADDRESS  ,                   //! Page to be used
		PAGE *	                    //! Pointer to the data buffer containing data to be written
);

//! HAL write functions API
void FLASH_WRITE_PAGE
(
		ADDRESS ,                    //! Page to be used
		PAGE    *                    //! Pointer to the data buffer containing data to be written
);

//! HAL Read Status function
BYTE READ_STATUS();

//! HAL Bulk Erase function
void BULK_ERASE();

//! HAL Software reset function
void SOFTWARE_RESET();

void READ_ID();

void InitPage(PAGE *p);

void RechercheDernierePageStockee(ADDRESS *DernierePageStockee);
