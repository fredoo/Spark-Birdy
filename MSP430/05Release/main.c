#include "main.h"

//1ko available for variables in RAM
BYTE * Trame_Travail;

BYTE*	Trame_GGA;
BYTE*	Trame_GSA;
BYTE*	Trame_RMC;
BYTE*	Trame_VTG;

BYTE 	Trame_Recues; // Flag : 1- GGA 2-GSA 4-RMC 8-VTG
BYTE	VBat;
BYTE	TempC;

BYTE Status;

BYTE EtatAEF;
BYTE Acquisition_OK;
BYTE CompteurAcquisitionGPS;
BYTE iCpt;
BYTE Reveil_Urgent;
BOOL WDT_STATUS;

int main(void)
{
	Init_HAL();
	GetGPSFIX();
	EtatAEF=AcquisitionGPS;
	SleepGPS();
	Trame_Recues=0;
	InitBC(&BC_RF);
	InitBC(&BC_GPS);
	InitPage(&Page_Courante);
	RechercheDernierePageStockee(&DernierePageStockee);

	while(1)
	{
		switch (EtatAEF)
		{
			case AcquisitionGPS:
				WakeGPS();
				do
				{
					Acquisition_OK = FALSE;
					CompteurAcquisitionGPS=0;
					WDT_Enable(180);  // 3mn
					Trame_Travail=TRAME_NMEA;
					do
					{
						if (!FileVide(&BC_GPS)) // car Recu du GPS
						{
// fabriquer la trame à l'adresse Trame_Travail
// Si type de trame pas encore recu
//		stocké trame en faisant pointé le bon pointeur au bon endroit
// 		mettre à jour le Flag de la trame recu
						}

					}while((Trame_Recues & 0x0f != 0x0f) &&(! WDT_FIN()));
					WDT_Disable();
					if (Trame_Recues & 0x0f == 0x0f)// les 4 trames recus
					{
						SleepGPS();
						while (!FileVide(&BC_GPS))
							Defiler(&BC_GPS);
// fabriquer Trame_ZIP
						for (iCpt=0; iCpt <22;iCpt++)
							Page_Courante.TR[Page_Courante.NbTrame][iCpt]=Trame_ZIP[iCpt];
						Page_Courante.NbTrame++;
						Acquisition_OK = TRUE;
						DerniereHeureAcquisitionGPS=getRTCTime ();
					}
					else
						CompteurAcquisitionGPS++;
					if (Page_Courante.NbTrame==23)
					{
						DernierePageStockee ++;
						FLASH_WRITE_PAGE(DernierePageStockee,&Page_Courante);
						InitPage(&Page_Courante);
					}
				}while((CompteurAcquisitionGPS <=3) && (Acquisition_OK == FALSE));
			break;
			case CommunicationRF:
				EnableReceiveDataRF();
				WDT_Enable(30);  // 30s
				while (FileVide(&BC_RF) &&(! WDT_FIN()));

				WDT_Disable();
				if (!FileVide(&BC_RF))
				{
// attendre mon ID
// gérer le protocole
					Reveil_Urgent=FALSE;
				}
				else
				{
					SleepRF();
					Reveil_Urgent=TRUE;
				}

			break;
		}
// Rechercher prochain réveil
// Si (prochain réveil > 30 s  && DerniereHeureAcquisitionGPS-heurecourante < intervalle GPS)
// alors si (Reveil_Urgent ==TRUE && encore dans plage RF)
//			alors EtatAEF=CommunicationRF
//			sinon EtatAEF= AcquisitionGPS
// sinon EtatAEF= AcquisitionGPS
	}


}
