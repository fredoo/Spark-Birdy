//8ko available in FRAM

//512 Bytes Page
extern PAGE Page_Courante;
extern PAGE Page_Travail;

// Buffer circulaires 128 Bytes
extern BC BC_GPS;
extern BC BC_RF;

//On déclare un tableau pouvant contenir 5 Trames NMEA et on utilise des pointeurs (évite des copie)
extern BYTE TRAME_NMEA [83*5];

extern BYTE Trame_ZIP[25];

extern ADDRESS DernierePageStockee;

extern Calendar DerniereHeureAcquisitionGPS;


