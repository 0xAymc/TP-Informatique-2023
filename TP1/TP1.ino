/* This file has been prepared for Doxygen automatic documentation generation.*/

/*! \mainpage *********************************************************************
 *
 * \section Introduction
 *      Utilisaion du port série sur Arduino Uno
 * 
 * \author
 *      MCHARFI Ayman
 *
 * $Name: TP1 $
 * $Revision: 1.0 $
 * $Date: 23/11/2023 $  \n
 ******************************************************************************/


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




unsigned char data;
unsigned char flag = 0;

/*! \brief Fonction d'interruption
 *
 * Permet d'effectuer la reception uniquement lorsqu'on transmet
 * Cela evite que le microprocesseur reste bloqué sur l'attente d'une transmission 
 *
 */


ISR(USART_RX_vect){
data = UDR0;
flag = 1;
PORTB = 0b00100000;
}

/*! \brief Fonction Main
 *
 *  La fonction main permet d'initialiser l'USART
 *  et d'effectuer son code
 */

int main(void)
{

USART_Init(MYUBRR);

sei();

while(1){
  if (flag){
  USART_putsln("Bonjour");
  //_delay_ms(100);
  flag = 0;
  }
_delay_ms(1);
}

}

/*! \brief Fonction USART_puts
 *
 *  La fonction permet de lire et transmettre une chaine de caractères
 *  Elle pointe vers chaque char de la chaine
 */

void USART_puts(unsigned char *str)
{
do
{
USART_Transmit(*str);
} while (*++str!=0);
}

/*! \brief Fonction USART_putsln
 *
 *  La fonction permet de faire la même chose que la fonction USART_puts mais avec un retour a la ligne a chaque envoi
 *  
 */

void USART_putsln(unsigned char *str)
{
USART_puts(str);
USART_Transmit('\n');
USART_Transmit('\r');
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
UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
/* Set frame format: 8data, 2stop bit */
UCSR0C = (3<<UCSZ00);
DDRB = 0b00100000;
PORTB = 0b00000000;
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

/*! \brief Fonction de reception.
 *
 *  Permet d'effectuer la reception
 */

unsigned char USART_Receive(void)
{
/* Wait for data to be received */
while (!(UCSR0A & (1<<RXC0)))
;
/* Get and return received data from buffer */
return UDR0;
}