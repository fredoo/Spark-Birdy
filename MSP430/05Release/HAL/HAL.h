/*! Hardware Abstraction Layer
 * Mars 2013
 *
 */
/*!*********** LOW LEVEL ******************/
void Init_HAL();			// Regroups all HAL Init Function
void InitClockSystem(); 	// Init. clocks & power modes
void InitUARTGPS();			// Configuration of eUSCIA0
void InitUARTRF();			// Configuration of eUSCIA1
void InitSPIMem();			// Configuration of eUSCIB0
void InitRTC();				// Configuration of RTC_B
void InitADC(); 			// Configuration of ADC
void InitIO();				// Configuration of all the I/O Pins
void StatusLedToggle();
void ModifUARTRF();

/*!_______________Real Time Clock*/
void MajRTC(Calendar); // UTC Time + Date From GPS
Calendar getRTCTime ();
void setAlarm(ConfLoggeur);
void startRTC();
void SleepCPU();

/*!_______________GPS*/
void WakeGPS(); 		// VCC GPS + Standby GPS
void SleepGPS(); 		// Standby GPS
void EnableReceiveDataGPS(); 	// Interruptions de UART
void TransmitDataGPS(char*);	// Interface avec UCA0TXBUF
char readGPSUART();		// Returns UCA0RXBUF
void GetGPSFIX();		// Turns ON GPS until FIX is acquired

/*!_______________RF */
void EnableReceiveDataRF(); // Interruptions de UART
void TransmitDataRF(char*);
void SleepRF();
void ConfigModeRF();
void TransmitChar(char c);


/*!_______________Watchdog Timer*/

void WDT_Enable();		//! Activate the Watchdog
void WDT_Disable();		//! Remove the Watchdog
void WDT_Init();		//! Initialize the timer
BOOL WDT_FIN();			//! Check if program aborted due to Watchdog
void SleepFor(unsigned int Seconds);

/*!______________ Offset index for RTC Control registers*/
#define OFS_CSCTL0             (0x0000)       /* CS Control Register 0 */
#define OFS_CSCTL1             (0x0002)       /* CS Control Register 1 */
#define OFS_CSCTL2             (0x0004)       /* CS Control Register 2 */
#define OFS_CSCTL3             (0x0006)       /* CS Control Register 3 */
#define OFS_CSCTL4             (0x0008)       /* CS Control Register 4 */
#define OFS_CSCTL5             (0x000A)       /* CS Control Register 5 */
#define OFS_CSCTL6             (0x000C)       /* CS Control Register 6 */

/*!______________ Macros to access registers*/
#define HWREG32(x)                                                              \
	(*((volatile uint32_t *)((uint16_t)x)))
#define HWREG16(x)                                                             \
	(*((volatile uint16_t *)((uint16_t)x)))
#define HWREG8(x)                                                             \
	(*((volatile uint8_t *)((uint16_t)x)))

