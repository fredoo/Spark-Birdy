#include <stdint.h>		// Necessary for data types


#define PAGESIZE 512		// PAGE is 512byte
#define TRAMEZIPSIZE	22	// TRAME is 22Byte
#define AcquisitionGPS 100
#define CommunicationRF 110

/* LLD System Specific Typedefs */
typedef unsigned short LLD_UINT16;  /* 16 bits wide */
typedef unsigned long  LLD_UINT32;  /* 32 bits wide */
typedef LLD_UINT32     ADDRESS;     /* Used for system level addressing */

/* Types */
typedef unsigned char BYTE;		/* 8 bits wide */
typedef char SBYTE;				/* 8 bits wide */
typedef unsigned short WORD;	/* 16 bits wide */
typedef unsigned long DWORD;	/* 32 bits wide */
typedef unsigned char BOOL;		/* Boolean */

/*! Boolean macros */
#ifndef TRUE
 #define TRUE  (1)
#endif
#ifndef FALSE
 #define FALSE (0)
#endif


//__________________Etapes______________________________________________
#define SwitchWait 0
#define InitForFix 1
#define GetFix 2
#define CreateFrame 3
#define ReadEcho 10
#define SendData 11
#define WaitDataCRC 12
#define WhatElse 13
#define ReadConfig 14

//__________________Constantes RF_______________________________________
#define ACK 6
#define NACK 21
#define STX 2
#define EOT 4
#define TOOWEAK 7
#define UPDATE 5

//______________Constantes Buffer GPS___________________________________
#define LIGNESBUFFER 5
#define LONGUEURBUFFERLINE 128

//______________Constantes Buffer RF____________________________________
#define RFBUFFER 128

// Enum des type à récupérer: correspond à l'index dans la trame (mot n° x)
typedef enum  { PDOP = 15, HDOP, VDOP }GSA;
typedef enum  { TIME = 1, Validity, Latitude, NS, Longitude , EW, DATE = 9 }RMC;
typedef enum  { FixStatus = 6, NBRSat, MSLALT = 9, GEOIDALESEP = 11 }GGA;
typedef enum  { Azimuth = 1, Vitesse = 7 }VTG;

//___________Structure de données________________________________________
typedef struct // Modèle de Structure contenant les données
{
	BYTE HH, MTE, SS;
	BYTE DA, MTH, YY;
	BYTE DD, LAMTE, LASS1, LASS2;
	BYTE DDD, LOMTE, LOSS1, LOSS2;
	BOOL EW, NS;
	WORD Alt;
	BYTE Vit;
	SBYTE Geo;
	WORD Azi;
	BYTE FIX;
	BYTE PDOP, HDOP, VDOP, VBat, TempC;
	BOOL GGA, GSA, RMC, VTG, Status;
}DataRegister;


//************** Data Structures ***************
//Calendar regroupe les valeurs du RTC
typedef struct {
    uint8_t Seconde; // 0 - 59
    uint8_t Minute; // 0 - 59
    uint8_t Heure; // 0 - 23
    uint8_t JourSemaine;
    uint8_t NumeroJour;
    uint8_t Mois;
    uint16_t Annee;
} Calendar;

//Structure regroupant la définition d'une période

// Une période c'est un mode de fonctionnement avec un intervalle de prise de position GPS propre
typedef struct {
    uint8_t HeureDebut;
    uint8_t MinuteDebut;
    uint8_t HeureFin;
    uint8_t	MinuteFin;
    uint8_t Intervalle;
} PeriodeGPS;

//TxRF: Période prédéfini de communication avec la base
typedef struct {
    uint8_t HeureDebut;
    uint8_t MinuteDebut;
    uint8_t Duree;
} FenetreRF;

// Structure regroupant les param. de configuration par l'utilisateur
typedef struct {
    PeriodeGPS	Per[4] ;
    FenetreRF	Fen[4] ;
} ConfLoggeur;


typedef uint8_t TRAME[22]; //22 Byte in one frame

typedef struct {
	uint8_t NbTrame;
	uint8_t Statut[3];
	uint8_t RFU[2];
	TRAME TR[23]; // 23 Frames in one page
}PAGE;


