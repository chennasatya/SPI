/* SPI - an application for the Pololu Orangutan SVP
 *
 * This application uses the Pololu AVR C/C++ Library.  For help, see:
 * -User's guide: http://www.pololu.com/docs/0J20
 * -Command reference: http://www.pololu.com/docs/0J18
 *
 * Created: 4/25/2015 10:44:08 AM
 *  Author: Lovelyhearts
 */

#include <pololu/orangutan.h>
#include "HT1632.h"
int main()
{
	//PRR0 = 1 << PRSPI;
	
	
	char buffer[10] = "11:00";
	 
	DDRB = (1 << PINB4) | (1 << PINB5) | (1 << PINB7);
	
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0) | (1 << SPIE);
	
	SPDR = buffer[0];
	
	SPSR = (1 << SPIF);
	
	begin(1);
	setPixel(1);
	writeScreen();
	//while(! SPSR & (1 << SPIF));
	
	
	
	
	
}




