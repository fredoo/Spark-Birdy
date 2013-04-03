//8ko available in FRAM

#pragma SET_DATA_SECTION(".fram_data")		// Variables defined between these 2 PRAGMA will be stored in FRAM

//512 Bytes Page
PAGE Page_Courante;
PAGE Page_Travail;

// Buffer circulaires 128 Bytes
BC BC_GPS;
BC BC_RF;

//On déclare un tableau pouvant contenir 5 Trames NMEA et on utilise des pointeurs (évite des copie)
BYTE TRAME_NMEA [83*5];

BYTE Trame_ZIP[TRAMEZIPSIZE];

ADDRESS DernierePageStockee;

Calendar DerniereHeureAcquisitionGPS;
#pragma SET_DATA_SECTION()


