/* Global Positioning System
 * Mars 2013
 *
 */

// PROTOTYPES DES Fonctions GPS

// RAZ de la structure de données
void resetDataGPSRegister(DataRegister*);// Reset data

/* Sert en théorie à initialiser la com UART sur EUSCI_A0
 * et envoyer les trames au GPS pour notre utilisation
 * soit 9600 bauds pour la vitesse serie
 * 1 HZ en fréquence d'emission des données
 * une trame GGA GSA RMC VTG soit 5 trames (en comptant le PMTOP) toute les secondes
 */
void initGPS();

void TableInit(short* tableau, unsigned int cases);// Sert à initialiser un tableau de données


short atos(char input); //Converti un char en int car le GPS envoie des char ASCII représentables

short TabletoShort(short* Input, unsigned int Taille); // sert juste à concaténer des caractères en une valeur (1,2,8 = 128)

SBYTE TabletoSBYTE(short* Input, unsigned int Taille); // Pour Geo uniquement (valeur sur 8 bits signés)

void ReadSentence(char* Sentence, DataRegister* InputReg); // Cette fonction lit une phrase et remplit la structure GPSRegister en fonction de celle-ci

BOOL MakeData(char* buffer, DataRegister* InputReg, BYTE* InputTrame); // Fabrique une donnée Après cette fonction Register devrait être rempli

void MakeTrame(BYTE* Trame, DataRegister* GPSRegister); // Fabrique une trame Acquiert la température et tension à ce moment

