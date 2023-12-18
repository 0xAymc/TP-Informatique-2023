
/*!

\mainpage 
 *
 * \section Introduction
 *      Utilisation du port série sur Arduino Mega
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
 *      Utilisation du port série sur Arduino Mega
 * \author
 *      MCHARFI Ayman
 *
 * $Name: Atelier_1 $
 * $Revision: 1.0 $
 * $Date: 23/11/2023 $  \n
 ******************************************************************************/


//! Fréquence du processeur
#define FOSC 16000000 
//! Taux de bauds pour la communication série
#define BAUD 9600      
//! Calcul du registre UBRR
#define MYUBRR FOSC/16/BAUD-1 

//! Variable pour stocker les données reçues
unsigned char data;        
//! Drapeau d'interruption pour USART0
unsigned char flag0 = 0;   
//! Drapeau d'interruption pour USART3
unsigned char flag3 = 0;   

/*! \brief Fonction d'interruption
 *
 * Permet d'effectuer la réception uniquement lorsqu'on transmet.
 * Cela évite que le microprocesseur reste bloqué sur l'attente d'une transmission.
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
    // Initialisation de l'USART0 et de l'USART3
    USART0_Init(MYUBRR);    
    USART3_Init(MYUBRR);

    // Activation des interruptions globales
    sei();

    // Boucle principale
    while(1){
        // Transmission des données de USART3 vers USART0
        if (flag3){
            USART0_Transmit(data);
            flag3 = 0;
        }
        // Transmission des données de USART0 vers USART3
        if (flag0){
            USART3_Transmit(data);
            flag0 = 0;
        }
        _delay_ms(1);
    }
}

/*! \brief Fonction USART_puts
 *
 *  La fonction permet de lire et transmettre une chaîne de caractères.
 *  Elle pointe vers chaque char de la chaîne.
 */


void USART_puts(unsigned char *str)
{
    do {
        USART0_Transmit(*str);
    } while (*++str != 0);
}


/*! \brief Fonction USART_putsln
 *
 *  La fonction permet de faire la même chose que la fonction USART_puts mais avec un retour à la ligne à chaque envoi.

 */

void USART_putsln(unsigned char *str)
{
    // Transmission de la chaîne suivie d'un retour à la ligne et d'un retour chariot
    USART_puts(str);
    USART0_Transmit('\n');
    USART0_Transmit('\r');
}


/*! \brief Initialisation de l'USART.
 *
 *  Permet de configurer les registres.
 */
void USART0_Init(unsigned int ubrr)
{  
    /* Configuration du taux de bauds */
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    /* Activer le récepteur et l'émetteur avec interruption de réception */
    UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
    /* Configuration du format de trame : 8 bits de données, 2 bits d'arrêt */
    UCSR0C = (3<<UCSZ00);
}

//! Initialisation de l'USART3 avec la même configuration que l'USART0
void USART3_Init(unsigned int ubrr)
{  
    /* Configuration du taux de bauds */
    UBRR3H = (unsigned char)(ubrr>>8);
    UBRR3L = (unsigned char)ubrr;
    /* Activer le récepteur et l'émetteur avec interruption de réception */
    UCSR3B = (1<<RXEN3)|(1<<TXEN3)|(1<<RXCIE3);
    /* Configuration du format de trame : 8 bits de données, 2 bits d'arrêt */
    UCSR3C = (3<<UCSZ30);
}

/*! \brief Fonction de transmission.
 *
 *  Permet d'effectuer la transmission.
 */
void USART0_Transmit(unsigned char data)
{
    /* Attendre que le tampon de transmission soit vide */
    while (!(UCSR0A & (1<<UDRE0)));
    /* Mettre les données dans le tampon, envoie les données */
    UDR0 = data;
}

//! Fonction de transmission pour USART3 avec la même configuration que USART0_Transmit
void USART3_Transmit(unsigned char data)
{
    /* Attendre que le tampon de transmission soit vide */
    while (!(UCSR3A & (1<<UDRE3)));
    /* Mettre les données dans le tampon, envoie les données */
    UDR3 = data;
}

/*! \brief Fonction de réception.
 *
 *  Permet d'effectuer la réception.
 */
unsigned char USART0_Receive(void)
{
    /* Attendre que des données soient reçues */
    while (!(UCSR0A & (1<<RXC0)));
    /* Obtenir et renvoyer les données reçues du tampon */
    return UDR0;
}

//! Fonction de réception pour USART3 avec la même configuration que USART0_Receive
unsigned char USART3_Receive(void)
{
    /* Attendre que des données soient reçues */
    while (!(UCSR3A & (1<<RXC3)));
    /* Obtenir et renvoyer les données reçues du tampon */
    return UDR3;
}
