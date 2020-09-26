/*
 * bluetooth.c

 *
 *  Created on: 22 mars 2020
 *      Author: martin
 */

#include "bluetooth.h"


/* !! Ne pas oublier d'initialiser les UART dans le main :
		UART_init(UART6_ID,9600);	//bluetooth  //COM20 sur mon pc
		UART_init(UART2_ID,9600);  //printf		//COM17 sur mon pc
		//on utlise l'UART 2 pour printf(..)
		SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);
*/

void BLUETOOTH_init(void){
	UART_init(UART2_ID, 38400);
}

//fonction
//envoie donn�e PC sous forme d'une trame de 4 informations
//Rajouter la fonction UART_puts(..,..,..) dans la librairie de l'UART + definir des variables
void BLUETOOTH_envoi_trame(uint8_t info1, uint8_t info2, uint8_t info3, uint8_t info4){
	static uint8_t trame[4];
	trame[0] = info1;
	trame[1] = info2;
	trame[2] = info3;
	trame[3] = info4;
	UART_puts(UART2_ID, trame, 4);
}

//envoie un caract�re
void BLUETOOTH_envoi_caractere(uint8_t c){
	c=UART_get_next_byte(UART2_ID);
	UART_putc(UART2_ID,c);
}

//reception d'un caract�re
void BLUETOOTH_reception_caractere(void){
	if(UART_data_ready(UART2_ID)){
		uint8_t c;
		c = UART_get_next_byte(UART2_ID);
		//printf("%c",c);
	}
}
//reception d'une Trame de 4 informations
void BLUETOOTH_reception_trame(void){
	if(UART_data_ready(UART2_ID)){
		uint8_t trame[4];
		UART_gets(UART1_ID, trame, 7);
		uint8_t info1 = *trame;
		uint8_t info2 =*(trame+1);
		uint8_t info3 = *(trame+2);
		uint8_t info4 =*(trame+3);
		printf("%d %d %d %d",info1,info2,info3,info4);
	}
}

/*------------------- SECRETARY BLUETOOTH -------------------*/

#define BUFFER_SIZE	128
static void find_command(char * buffer);
static void echo_command(char * buffer);
static bool_e str_begin_with(char * string, char * begin);


/**
* @brief R�cup�re les octets re�us sur l� UART2, les rassemble dans un buffer et les traite si on re�oit '\n'
* @pre cette fonction doit �tre appel�e r�guli�rement dans la t�che de fond
* @pre les trames re�ues doivent se terminer par un '\n' et ne pas d�passer BUFFER_SIZE
* @post la taille du buffer est limit�e. Si de trop nombreux caract�res sont re�us, ils s'�crasent � la fin.
* @post le traitement des trames re�ues est sous-trait�
*/
void BLUETOOTH_process_main(void)
{
	static char buffer[BUFFER_SIZE];
	static uint8_t index = 0;
	char c;
	while(UART_data_ready(UART2_ID))
	{
		c = UART_getc(UART2_ID);
		buffer[index] = c;
		if(c =='\n')
		{
			buffer[index] = '\0';
			find_command(buffer);
			index = 0;
		}
		else if(index<BUFFER_SIZE-1)
		{
			index += 1;
		}
	}
}

/**
* @brief 	Fonction priv�e qui analyse le contenu de la chaine string pour voir si elle commence par le contenu de la chaine begin
* @param	string : la chaine dans laquelle on recherche
* @param	begin : la chaine que l'on recherche
* @pre		begin doit �tre de taille inf�rieure � string
* @ret		TRUE si string commence par begin. FALSE sinon.
*/
static bool_e str_begin_with(char * string, char * begin)
{
	bool_e ret;
	uint16_t i;
	ret = TRUE;
	for(i=0;begin[i]!=0;i++)
	{
		if(string[i]==0 || string[i]!=begin[i])
		{
			ret = FALSE;
			break;
		}
	}
	return ret;
}


static void find_command(char * buffer)
{
	bool_e badstring, start, stop;
	start = FALSE;
	stop = FALSE;
	badstring = FALSE; //On fait l'hypoth�se que la chaine est correcte.

	if(str_begin_with(buffer, "start"))
	{
		//On lance le programme via bluetooth
		//Créer un état wait for launching ds machine à états générale
		start = TRUE;
	}
	else if(str_begin_with(buffer, "stop"))
	{
		//On stoppe tous les moteurs via bluetooth
		MC_put_all_motors_off();
		stop = TRUE;
	}
	else if(str_begin_with(buffer, "help"))
	{
		printf("--- commands list ---\n");
		printf("start (To launch the program\n");
		printf("stop  (To stop all motors\n");
		printf("\"help\\n\" - show this commands list \n");
	}
	else
		badstring = TRUE;
	if(start)
		printf("PROGRAM LAUNCHED\n");
	if(stop)
		printf("MOTORS STOPPED\n");
	if(badstring)
		printf("Bad command -> press help to list commands\n");
}

static void echo_command(char * buffer)
{
	puts(buffer);
}

