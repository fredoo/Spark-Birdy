/*! Hardware Abstraction Layer
 * Mars 2013
 *
 */
#include "TYPEDEF.h"
#include <msp430fr5739.h>	// Necessary for the usages of register names
#include "HAL.h"
#include "Flash.h"
#include "Buffer_Circ.h"
#include "Var_FRAM_EXTERN.h"

extern char*    pRFTXTrame,pGPSTXTrame;	// Pointeur vers la trame à envoyer
extern char     RFTXTrameLen ;		// Taille de la trame à envoyer
extern BOOL WDT_STATUS;
//TO DO
//void InitSPIMem();
//void InitADC(); // Tension Batterie
//char* ReadDataSPI(long Address);
//void WriteDataSPI(long Address, char*);

void Init_HAL()
{
	InitIO();
	InitClockSystem();
	InitUARTRF();
	InitUARTGPS();
	Init_SPI();
}

/*!_______________________________ Basic MSP430FR5739 Init. Functions */
/*!	\fn InitClockSystem()
 * Initializes all the clocks (MCLK/SMCLK/ACLK/LFXT1)
 * -------------------------------------------------
 *	+ Init RTC Oscillator (32768Hz)
 *	+ Init DCO (Digitally Controlled Oscillator @8MHz)
 *	+ MCLK 		= DCOCLK 	= 8MHz
 *	+ SMCLK	 	= DCOCLK/2	= 4MHz
 *	+ ACLK	 	= VLOCLK/4 	= 3kHz
 *
 */
void InitClockSystem()
{
    CSCTL0_H = 0xA5;					// Password to modify CCSTLx registers
    CSCTL1 |= DCOFSEL0 + DCOFSEL1;            		// Set DCO frequency to 8MHz
    CSCTL2 = SELM__DCOCLK + SELS__DCOCLK+ SELA__VLOCLK ;// Select DCOCLK as clock source for MCLK, SMCLK & ACLK
    CSCTL3 = DIVM__1  + DIVS__2 + DIVA__4;      	// MCLK = DCOCLK/1 | SMCLK = DCOCLK/2 | ACLK = VLOCLK/4
    CSCTL4 |= XT1DRIVE_0 ;				// Lowest current drive for RTC oscillator
    CSCTL4 &= ~XT1OFF;					// Makes sure the XT1OFF is not set
}

/*!	void InitIO()
 * Initializes all the IOs
 */
void InitIO()
{
    /* Port 1 Output Register */
    P1OUT = 0;

    /* Port 1 Port Select Register 1 */
    P1SEL1 = BIT3 + BIT6 + BIT7;			// P1.3 as UCB0STE - P1.6 as UCB0SIMO - P1.7 as UCB0SOMI

    /* Port 1 Direction Register */
    P1DIR = 0;

    /* Port 1 Interrupt Edge Select Register */
    P1IES = 0;

    /* Port 1 Interrupt Flag Register */
    P1IFG = 0;

    /* Port 2 Output Register */
    P2OUT = 0;

    /* Port 2 Port Select Register 1 */
    P2SEL1 = BIT0 + BIT1 + BIT2 + BIT5 + BIT6;	// P2.0 as UCA0TXD - P2.1 as UCA0 RXD- P2.2 as UCB0CLK - P2.5 as UCA1TXD - P2.6 as UCA1RXD

    /* Port 2 Direction Register */
    P2DIR = BIT4;							// P2.4 as output for GPS_STBY control

    /* Port 2 Interrupt Edge Select Register */
    P2IES = 0;

    /* Port 2 Interrupt Flag Register */
    P2IFG = 0;

    /* Port 3 Output Register */
    P3OUT = 0;

    /* Port 3 Direction Register */
    P3DIR = BIT1 + BIT4 + BIT5;			//P3.1 as Status LED - P3.4 as RF_EN - P3.5 as RF_CFG

    /* Port 3 Interrupt Edge Select Register */
    P3IES = 0;

    /* Port 3 Interrupt Flag Register */
    P3IFG = 0;

    /* Port 4 Output Register */
    P4OUT = 0;

    /* Port 4 Direction Register */
    P4DIR = 0;

    /* Port 4 Interrupt Edge Select Register */
    P4IES = 0;

    /* Port 4 Interrupt Flag Register */
    P4IFG = 0;

    /* Port J Output Register */
    PJOUT = 0;

    /* Port J Port Select Register 0 */
    PJSEL0 = BIT4 + BIT5;				// PJ.4 as XIN - PJ.5 as XOUT

    /* Port J Direction Register */
    PJDIR = 0;
}


void StatusLedToggle()
{
	P3OUT ^= BIT1;
}
//_______________________________ GPS PA6H Related Functions
/*!	\fn InitUartGPS()
 * 	UART GPS
 * 	Configure eUSCI_A0
 * 	UART- 9600 - 8N1
 *	P2.0 - UCA0TXD
 *	P2.1 - UCA0RXD
 *
 */
void InitUARTGPS()
{
	// Configure UART0 pins for secondary function
	P2SEL1 |= BIT0 + BIT1;			//P2.0 - UCA0TXD
	P2SEL0 &= ~(BIT0 + BIT1);		//P2.1 - UCA0RXD
	// Configure UART0
	UCA0CTL1 |= UCSWRST; 			// Software Reset Enable to configure
	UCA0CTL1 = UCSSEL__SMCLK;        // Set SMCLK = 4MHz as UCBRCLK
	UCA0BR0 = 26;                    // UCA0BRW =0x0026  9600 baud (Table 18-5)
	UCA0MCTLW |= 0xB601;            // UCBRS0 = 0xB6
	// UCBRF0 = 0x0
	// UCOS16 = 0x1
	UCA0CTL1 &= ~UCSWRST;           // Release from reset
}


void WakeGPS()
{
    P2OUT &= ~BIT4;	// Turn VCC GPS ON (PMOS Command is Active LOW)
}
void SleepGPS() 	// Only STBY Power for memory
{
    P2OUT |= BIT4;			// Turn VCC GPS OFF
}
BOOL GetGPSFIX()	// Loop forever until first GPS correct positioning
{
	BOOL FIX=FALSE;
	WakeGPS();
	while(!FIX)
	{
		// Dépiler le buffer circulaire BC_GPS
		// Vérifier dans la trame GGA si un FIX a été effectué
		// Modifier la valeur de BOOL
	}
}

void EnableReceiveDataGPS()
{
    UCA0IE |= UCRXIE;	 // enable RX interrupt flag on eUSCA0 / GPS
    __enable_interrupt();
}

char readGPSUART()
{
    return UCA0RXBUF;
}

void TransmitDataGPS(char* pdata)
{
//	UCA1IE |= UCTXIE;	 	// enable TX interrupt flag on eUSCA1 / RF
//	__enable_interrupt();
	while(*pdata != '\0')
		{
		 while( UCA0STATW & UCBUSY != UCBUSY );
			UCA0TXBUF= *pdata;
			pdata++;
		}

}

//_______________________________  RF Module HM-TRP Related Functions
/*!	\fn void InitUartRF()
 * 	UART RF
 * 	Configure eUSCI_A1
 * 	UART- 9600 - 8N1
 *	P2.5 - UCA0TXD
 *	P2.6 - UCA0RXD
 *
 */
void InitUARTRF()
{
    // Configure UART1 pins
    P2SEL1 |= BIT5 + BIT6;			//P2.5 - UCA0TXD
    P2SEL0 &= ~(BIT5 + BIT6);		//P2.6 - UCA0RXD
    // Configure UART0
    UCA1CTL1 |= UCSWRST; 			// Software Reset Enable to configure
    UCA1CTL1 = UCSSEL__SMCLK;       // Set SMCLK = 4MHz as UCBRCLK
    UCA1BR0 = 26;                   // UCA1BRW =0x0026  9600 baud (Table 18-5)
    UCA1MCTLW |= 0xB601;            // UCBRS0 = 0B6
    								// UCBRF0 = 0x0
    								// UCOS16 = 0x1
    UCA1CTL1 &= ~UCSWRST;			// Release from reset
}

void ModifUARTRF()
    {

    // Configure UART0 @ 1200bps
    UCA1CTL1 |= UCSWRST; 			// Software Reset Enable to configure
    UCA1BR0 = 0x34;                 // UCA0BRW =0x0034  1200 baud
    UCA1MCTLW |= 0x4911;            // UCBRS0 = 0x49
    								// UCBRF0 = 0x1
    								// UCOS16 = 0x1
    UCA1CTL1 &= ~UCSWRST;			// Release from reset
    }

void EnableReceiveDataRF()
    {
    UCA1IE |= UCRXIE;	 	// enable RX interrupt flag on eUSCA1 / RF
    __enable_interrupt();
    }

void SendCharRF(char c)
{
	P3OUT |= BIT4;		// Set RF_CFG HIGH for communication
	P3OUT &= ~BIT5;		// Set RF_EN LOW for normal mode for data transceiver
	_no_operation();
	UCA1TXBUF = c;
}

void TransmitDataRF(char* p)
    {
	pRFTXTrame=p;
	while(*pRFTXTrame!='\0')
			{
				UCA1TXBUF = *pRFTXTrame;
				pRFTXTrame++;
				__delay_cycles(40000); //10ms);
			}
//    UCA1IE |= UCTXIE;	 	// enable TX interrupt flag on eUSCA1 / RF
//    __enable_interrupt();
//    UCA1TXBUF = *pRFTXTrame;
    }
void WakeRF()
{
    P3OUT |= BIT4;		// Set RF_CFG HIGH for communication & LOW for configuration mode
    P3OUT &= ~BIT5;		// Set RF_EN LOW for normal mode for data transceiver & set HIGH to sleep
    EnableReceiveDataRF();
}

char readRFUART()
{
    return UCA1RXBUF;
}

void SleepRF()
    {
    P3OUT &= ~BIT4;		// Set RF_CFG HIGH for communication & LOW for configuration mode
    P3OUT |= BIT5;		// Set HIGH to sleep
    }
void ConfigModeRF()
    {
    P3OUT &= ~BIT4;		// Set LOW for configuration mode
    P3OUT &= ~BIT5;		// Set RF_EN LOW for normal mode for data transceiver
    }

//_______________________________ RTC_B Related Functions
/*!
 * \fn void InitRTC()
 * 	Initialize RTC_B Module
 *
 *
 */
void InitRTC()
    {
    const uint32_t baseAddress = 0x0160; // Clock System Base Address
    const uint16_t xt1drive = XT1DRIVE_0;

    // Unlock CS control register
    CSCTL0 = CSKEY;

//    //If the drive setting is not already set to maximum
//    //Set it to max for LFXT startup
//    if ((HWREG16(baseAddress + OFS_CSCTL4) & XT1DRIVE_3) != XT1DRIVE_3)
//	{
//	//Highest drive setting for XT1startup
//	HWREG16(baseAddress + OFS_CSCTL4) |= XT1DRIVE1_L + XT1DRIVE0_L;
//	}

    CSCTL4 |= XT1DRIVE_2;
    CSCTL4 &= ~ (XTS+ XT1BYPASS);

    // Low Frequency Mode + enable
//    HWREG16(baseAddress + OFS_CSCTL4) &= ~(XTS+XT1BYPASS);

    //Wait for Crystal to stabilize
    while (HWREG8(baseAddress + OFS_CSCTL5) & XT1OFFG)
	{
	//Clear OSC fault flags
	HWREG8(baseAddress + OFS_CSCTL5) &= ~(XT1OFFG);

	//Clear OFIFG fault flag
	SFRIFG1 &= ~OFIFG;
	}

    //set to requested Drive mode
    HWREG16(baseAddress + OFS_CSCTL4) = ( HWREG16(baseAddress + OFS_CSCTL4) &~(XT1DRIVE_3)) | (xt1drive);

    //Switch ON XT1 oscillator
    HWREG16(baseAddress + OFS_CSCTL4) &= ~XT1OFF;

    }

void MajRTC(Calendar GPSCurrentTime) // UTC Time + Date From GPS
    {
    RTCCTL01 	|= RTCHOLD;			// Put the RTC in Hold
    RTCCTL01  	|= RTCBCD;			// Using BCD mode
    // Update all the registers
    RTCSEC    	= GPSCurrentTime.Seconde;
    RTCMIN 		= GPSCurrentTime.Minute;
    RTCHOUR     = GPSCurrentTime.Heure;
    RTCDOW		= GPSCurrentTime.JourSemaine;
    RTCDAY		= GPSCurrentTime.NumeroJour;
    RTCMON    	= GPSCurrentTime.Mois;
    RTCYEAR    	= GPSCurrentTime.Annee;
    }

void setAlarm(ConfLoggeur cfg)
	{
		int MinHD = ((RTCAMIN & 0x70) >> 4), MinLD = (RTCAMIN & 0x0F);
		if(++MinLD == 10)
			if(++MinHD == 6)
				MinHD = MinLD = 0;
		RTCAMIN 	&= 	0x7F; // On clear l'interruption avant de changer
		RTCAMIN     |= 	((MinHD << 4) & 0x70);
		RTCAMIN     |= 	(MinLD & 0x0F);
	}

/*!
 *  Returns the Calendar Time stored in the Calendar registers of the RTC.

 This function returns the current Calendar time in the form of a Calendar
 structure.

 \return A Calendar structure containing the current time.
 * @return
 */


//
//*****************************************************************************
Calendar getRTCTime ()
    {
    Calendar tempCal;

    while ( !(RTCCTL01 & RTCRDY == RTCRDY) ) ;

    tempCal.Seconde    	= RTCSEC;
    tempCal.Minute    	= RTCMIN;
    tempCal.Heure      	= RTCHOUR;
    tempCal.JourSemaine = RTCDOW;
    tempCal.NumeroJour 	= RTCDAY;
    tempCal.Mois      	= RTCMON;
    tempCal.Annee       = RTCYEAR;

    return (tempCal) ;
    }

void startRTC()
    {
    // Clear Interrupts Flags
    RTCCTL01 &= ~ (RTCOFIFG + RTCTEVIFG + RTCAIFG);
    // Enable Interrupts
    RTCCTL01 |= RTCRDYIE + RTCTEVIE + RTCAIE;

    // START CLOCK
    RTCCTL01 &=~RTCHOLD;
    }



void SleepCPU()
{
  TA0CCTL0 = CCIE;                          // TACCR0 interrupt enabled
  TA0CCR0 = 65535;
  TA0CTL = TASSEL_2 + MC_1 + ID_3;          // SMCLK, UP mode , Input Divider/8

  __enable_interrupt();
  LPM0;       								// Enter LPM0
}

void WDT_Enable(unsigned int Seconds)	//! Activate Countdown Timer for
{
	TB0CCR0 = Seconds*46;									// Input Clock : 46Hz
	_enable_interrupts();
}
void WDT_Disable()
{
			//! Remove the Watchdog
}
void WDT_Init()
{
	TB0EX0 = TBIDEX__8;										// Input Divider Expansion. /8
	TB0CTL = TBSSEL__ACLK + MC__UP+ CNTL__16 + ID__8;		// ACLK(3kHz), UP mode, CNT Length : 16bit - Input Divider /8
	TB0CCTL0 = CCIE;                          				// TBCCR0 interrupt enabled
	//! Initialize the timer
}
BOOL WDT_FIN()//! Check if program aborted due to Watchdog
{
	return WDT_STATUS;
}
void SleepFor(unsigned int Seconds)
{

}

// void EnterLPM3.5();
// void ExitLPM3.5();


//Fonctions d'interruptions RTC
//Fonctions d'interruption Reception GPS
//Fonctions d'interruption RF
//Fonctions d'interruption ADC (tension température)

//________Fonction Bas Niveau_END__________________
