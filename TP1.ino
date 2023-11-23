/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief
 *      Utilisaion du port série sur Arduino Uno
 * \author
 *      MCHARFI Ayman
 *
 * $Name: TP1 $
 * $Revision: 1.0 $
 * $Date: 23/11/2023 $  \n
 ******************************************************************************/


#define FOSC 16000000 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1


/*! \brief Fonction Main
 *
 *  La fonction main permet d'initialiser l'USART
 *  et d'effectuer la transmission
 */

int main(void)
{

USART_Init(MYUBRR);

while(1){
  USART_Transmit('A');
  _delay_ms(100);
}

}

/*! \brief Initialisation de l'USART.
 *
 *  Permet de configurer les registres
 */

void USART_Init(unsigned int ubrr)
{  
/*Set baud rate */
UBRR0H = (unsigned char)(ubrr>>8);
UBRR0L = (unsigned char)ubrr;
/*Enable receiver and transmitter */
UCSR0B = (1<<TXEN0);
/* Set frame format: 8data, 2stop bit */
UCSR0C = (3<<UCSZ00);
}

/*! \brief Fonction de transmission.
 *
 *  Permet d'effectuer la transmission
 */

void USART_Transmit(unsigned char data)
{
/* Wait for empty transmit buffer */
while (!(UCSR0A & (1<<UDRE0)))
;
/* Put data into buffer, sends the data */
UDR0 = data;
}