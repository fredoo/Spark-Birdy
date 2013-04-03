/* Global Positioning System
 * Mars 2013
 *
 */

// PROTOTYPES DES Fonctions GPS

// RAZ de la structure de donn�es
void resetDataGPSRegister(DataRegister*);// Reset data

/* Sert en th�orie � initialiser la com UART sur EUSCI_A0
 * et envoyer les trames au GPS pour notre utilisation
 * soit 9600 bauds pour la vitesse serie
 * 1 HZ en fr�quence d'emission des donn�es
 * une trame GGA GSA RMC VTG soit 5 trames (en comptant le PMTOP) toute les secondes
 */
void initGPS();

void TableInit(short* tableau, unsigned int cases);// Sert � initialiser un tableau de donn�es


short atos(char input); //Converti un char en int car le GPS envoie des char ASCII repr�sentables

short TabletoShort(short* Input, unsigned int Taille); // sert juste � concat�ner des caract�res en une valeur (1,2,8 = 128)

SBYTE TabletoSBYTE(short* Input, unsigned int Taille); // Pour Geo uniquement (valeur sur 8 bits sign�s)

void ReadSentence(char* Sentence, DataRegister* InputReg); // Cette fonction lit une phrase et remplit la structure GPSRegister en fonction de celle-ci

BOOL MakeData(char* buffer, DataRegister* InputReg, BYTE* InputTrame); // Fabrique une donn�e Apr�s cette fonction Register devrait �tre rempli

void MakeTrame(BYTE* Trame, DataRegister* GPSRegister); // Fabrique une trame Acquiert la temp�rature et tension � ce moment

