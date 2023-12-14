/* This file has been prepared for Doxygen automatic documentation generation.*/

/*! \mainpage *********************************************************************
 *
 * \section Introduction
 *      Utilisaion du port série sur Arduino Mega
 * 
 * \author
 *      MCHARFI Ayman
 *
 * $Name: Atelier_1 $
 * $Revision: 1.0 $
 * $Date: 23/11/2023 $  \n
 ******************************************************************************/


/*! \file *********************************************************************
 *
 * \brief
 *      Utilisaion du port série sur Arduino Mega
 * \author
 *      MCHARFI Ayman
 *
 * $Name: Atelier_1 $
 * $Revision: 1.0 $
 * $Date: 23/11/2023 $  \n
 ******************************************************************************/


#define FOSC 16000000 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1




unsigned char data;
unsigned char flag0 = 0;
unsigned char flag3 = 0;

/*! \brief Fonction d'interruption
 *
 * Permet d'effectuer la reception uniquement lorsqu'on transmet
 * Cela evite que le microprocesseur reste bloqué sur l'attente d'une transmission 
 *
 */


ISR(USART0_RX_vect){
data = UDR0;
flag0 = 1;
}

ISR(USART3_RX_vect){
data = UDR3;
flag3 = 1;
}

/*! \brief Fonction Main
 *
 *  La fonction main permet d'initialiser l'USART
 *  et d'effectuer son code
 */

int main(void)
{

USART0_Init(MYUBRR);
USART3_Init(MYUBRR);

sei();

while(1){
  if (flag3){
  USART0_Transmit(data);
  flag3 = 0;
  }
  if (flag0){
  USART3_Transmit(data);
  flag0 = 0;
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

void USART0_Init(unsigned int ubrr)
{  
/*Set baud rate */
UBRR0H = (unsigned char)(ubrr>>8);
UBRR0L = (unsigned char)ubrr;
/*Enable receiver and transmitter */
UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
/* Set frame format: 8data, 2stop bit */
UCSR0C = (3<<UCSZ00);
}

void USART3_Init(unsigned int ubrr)
{  
/*Set baud rate */
UBRR3H = (unsigned char)(ubrr>>8);
UBRR3L = (unsigned char)ubrr;
/*Enable receiver and transmitter */
UCSR3B = (1<<RXEN3)|(1<<TXEN3)|(1<<RXCIE3);
/* Set frame format: 8data, 2stop bit */
UCSR3C = (3<<UCSZ30);
}

/*! \brief Fonction de transmission.
 *
 *  Permet d'effectuer la transmission
 */

void USART0_Transmit(unsigned char data)
{
/* Wait for empty transmit buffer */
while (!(UCSR0A & (1<<UDRE0)))
;
/* Put data into buffer, sends the data */
UDR0 = data;
}

void USART3_Transmit(unsigned char data)
{
/* Wait for empty transmit buffer */
while (!(UCSR3A & (1<<UDRE3)))
;
/* Put data into buffer, sends the data */
UDR3 = data;
}

/*! \brief Fonction de reception.
 *
 *  Permet d'effectuer la reception
 */

unsigned char USART0_Receive(void)
{
/* Wait for data to be received */
while (!(UCSR0A & (1<<RXC0)))
;
/* Get and return received data from buffer */
return UDR0;
}

unsigned char USART3_Receive(void)
{
/* Wait for data to be received */
while (!(UCSR3A & (1<<RXC3)))
;
/* Get and return received data from buffer */
return UDR3;
}

