/*
 * Interrupt_Vectors.c

 *
 *  Created on: 4 avr. 2013
 *      Author: Fredoo
 */

#include "msp430.h"
/*! Timer0 A0 interrupt service routine
 *
 */
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
{

}


/*! Timer1 A0 interrupt service routine
 *
 */
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR(void)
{

}

/*!
 * GPS UART Interrupt Routine
 *
 */
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR_HOOK(void)
{
	switch(__even_in_range(UCA0IV,USCI_UART_UCTXCPTIFG))
	{
	case USCI_NONE:				break;
	case USCI_UART_UCRXIFG:		break;
	case USCI_UART_UCSTTIFG:	break;
	case USCI_UART_UCTXCPTIFG:	break;
	}
}

/*!
 * RF UART Interrupt Routine
 *
 */
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR_HOOK(void)
{
	switch(__even_in_range(UCA1IV,USCI_UART_UCTXCPTIFG))
	{
	case USCI_NONE:				break;
	case USCI_UART_UCRXIFG:		break;
	case USCI_UART_UCSTTIFG:	break;
	case USCI_UART_UCTXCPTIFG: 	break;
	}
}

/*!
 * Memory SPI Interrupt Routine
 *
 */
#pragma vector=USCI_B0_VECTOR
__interrupt void USCI_B0_ISR (void)
{
    switch (__even_in_range(UCB0IV,4)){
        //Vector 2 - RXIFG
        case USCI_SPI_UCRXIFG:	break;
        case USCI_SPI_UCTXIFG:	break;
        default:				break;
    }
}

/*!
 * Real Time Clock Interrupt Routine
 *
 */
#pragma vector=RTC_VECTOR
__interrupt void RTC_B_ISR (void)
{
	switch (__even_in_range(RTCIV,12)){
	case RTC_NONE: 		break;  /* No Interrupt pending */
	case RTC_RTCRDYIFG:         /* RTC ready: RTCRDYIFG */
						break;
	case RTC_RTCTEVIFG:         /* RTC interval timer: RTCTEVIFG */

		//Read out New Time a Minute Later BREAKPOINT HERE
		//Ex: newTime = RTC_B_getCalendarTime(RTC_B_BASE);
		break;
	case RTC_RTCAIFG:         	/* RTC user alarm: RTCAIFG */
						break;
	case RTC_RT0PSIFG: 	break;  	/* RTC prescaler 0: RT0PSIFG */
	case RTC_RT1PSIFG: 	break; 	/* RTC prescaler 1: RT1PSIFG */
	case RTC_RTCOFIFG: 	break; 	/* RTC Oscillator fault */
	default: break;
	}
}

//ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
	switch (__even_in_range(ADC10IV,12)){
	case  ADC10IV_NONE: 		break; /* No Interrupt pending */
	case  ADC10IV_ADC10OVIFG:	break; /* ADC10OVIFG : Conversion result overflow */
	case  ADC10IV_ADC10TOVIFG:	break; /* ADC10TOVIFG conversion time overflow */
	case  ADC10IV_ADC10HIIFG:	break; /* ADC10HIIFG */
	case  ADC10IV_ADC10LOIFG:	break; /* ADC10LOIFG */
	case ADC10IV_ADC10INIFG:	break; /* ADC10INIFG */
	case ADC10IV_ADC10IFG:        /* ADC10IFG */
		//Automatically clears ADC10IFG0 by reading memory buffer
								break;
	default: break;
	}
}
