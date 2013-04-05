#include "TYPEDEF.h"
#include "HAL.h"
#include "GPS.h"

/* Sert en théorie à initialiser la com UART sur EUSCI_A0
 * et envoyer les trames au GPS pour notre utilisation
 * soit 9600 bauds pour la vitesse serie
 * 1 HZ en fréquence d'emission des données
 * une trame  GSA RMC VTG soit 5 trames (en comptant le PMTOP) toute les secondes
 */
void initGPS()
{
	//WakeGPS();
	TransmitDataGPS("$PMTK251,9600*17"); 		// Definit le baudrate
	TransmitDataGPS("$PMTK300,1000,0,0,0,0*1C");// 5 trames/secondes
	TransmitDataGPS("$PMTK314,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0*34");	// PGTOP, GGA, GSV, RMC,VTG
	//EnableReceiveDataGPS();
}

// RAZ de la structure de données
void resetDataGPSRegister(DataRegister* GPSRegister)
{
	(*GPSRegister).Alt = 0;
	(*GPSRegister).Geo = 0;
	(*GPSRegister).DA = 0;
	(*GPSRegister).DD = 0;
	(*GPSRegister).DDD = 0;
	(*GPSRegister).FIX = 0;
	(*GPSRegister).HDOP = 0;
	(*GPSRegister).HH = 0;
	(*GPSRegister).LAMTE = 0;
	(*GPSRegister).LASS1 = 0;
	(*GPSRegister).LASS2 = 0;
	(*GPSRegister).LOMTE = 0;
	(*GPSRegister).LOSS1 = 0;
	(*GPSRegister).LOSS2 = 0;
	(*GPSRegister).MTE = 0;
	(*GPSRegister).MTH = 0;
	(*GPSRegister).PDOP = 0;
	(*GPSRegister).GGA = FALSE;
	(*GPSRegister).GSA = FALSE;
	(*GPSRegister).RMC = FALSE;
	(*GPSRegister).VTG = FALSE;
	(*GPSRegister).SS = 0;
	(*GPSRegister).VDOP = 0;
	(*GPSRegister).Vit = 0;
	(*GPSRegister).YY = 0;
	(*GPSRegister).Azi = 0;
	(*GPSRegister).VBat = 0;
	(*GPSRegister).TempC = 0;
}// Reset data

void TableInit(short* tableau, unsigned int cases)// Sert à initialiser un tableau de données
{
	unsigned int i;
	for(i = cases; i > 0; i--)
	{
		tableau[i] = 0;
	}
}

short atos(char input)
{
	return (short)((((int)input >= 48) && ((int)input <= 57)) ? (int)input - 48 : -1);
}

short TabletoShort(short* Input, unsigned int Taille)
{
	int decallage = 1;
	short output = 0;

	while (Taille > 0)
	{
		if (Input[--Taille] != -1)
		{
			output += (short)(Input[Taille] * decallage);
			decallage *= 10;
		}
	}
	if (Input[0] == -1)
		output *= -1;
	return output;
}// Pour les autres (valeur sur 16 bits signés)

SBYTE TabletoSBYTE(short* Input, unsigned int Taille)
{
	int decallage = 1;
	SBYTE output = 0;

	while (Taille > 0)
	{
		if (Input[--Taille] != -1)
		{
			output += (SBYTE)(Input[Taille] * decallage);
			decallage *= 10;
		}
	}
	if (Input[0] == -1)
		output *= (SBYTE)(-1);
	return output;
}// Pour Geo uniquement (valeur sur 8 bits signés)

void MakeSentence()
{ // A revoir... pour exemple uniquement
// Cherche le premier $ puis rempli un tableau de Sentence NMEA

	bufferTemp=readGPSUART();
	// Recherche du premier debut de trame
	if(bufferTemp == 0x24) //Test for $
		init = true;
	if(init)
		buffer[indexLigne][buf_index]= bufferTemp;

	// Remise a la ligne pour chaque trame
	if(bufferTemp == 0x24) //Test for $
	{
		if(indexLigne<4)
		{
			indexLigne++;
			Status = 1; //Fin de trame donc appel de MakeTrame()
		}
		else
			indexLigne=0;		//
		buf_index=0;			//
	}
	if(buf_index<150)
		buf_index++;
	else
		buf_index=0;
}


void ReadSentence(char * Sentence, DataRegister* InputReg) // Cette fonction lit une phrase et remplit la structure GPSRegister en fonction de celle-ci
{
	unsigned short Cpt = 0, Counter, k;

	short Data[15]; // Constructeur Values
	BOOL PASSED = FALSE, FIX = FALSE;


	// replissage partiel du registre données En fonction de la phrase donnée par le GPS
	if(FIX)
		if ((Sentence[3] == 'G') && (Sentence[4] == 'S') && (Sentence[5] == 'A'))
		{
			for (Counter = 0, k = 0; (k <= (int)VDOP) && (Sentence[Counter] != '*'); Counter++)
			{
				if (Sentence[Counter] == ',')
				{
					k++;
					PASSED = FALSE;
				}
				else
					switch (k)
					{
					case (int)HDOP:
							if (!PASSED)
							{
								TableInit(Data, 15);
								unsigned int a = 0;
								while (Sentence[Counter + a] != '.')
									Data[a] = atos(Sentence[Counter + a++]);
								Data[a] = atos(Sentence[Counter + ++a]);
								(*InputReg).HDOP = (BYTE)TabletoShort(Data, a);
								PASSED = TRUE;
							}
					break;
					case (int)PDOP:
							if (!PASSED)
							{
								TableInit(Data, 15);
								unsigned int a = 0;
								while (Sentence[Counter + a] != '.')
									Data[a] = atos(Sentence[Counter + a++]);
								Data[a] = atos(Sentence[Counter + ++a]);
								(*InputReg).PDOP = (BYTE)TabletoShort(Data, a);
								PASSED = TRUE;
							}
					break;
					case (int)VDOP:
							if (!PASSED)
							{
								TableInit(Data, 15);
								unsigned int a = 0;
								while (Sentence[Counter + a] != '.')
									Data[a] = atos(Sentence[Counter + a++]);
								Data[a] = atos(Sentence[Counter + ++a]);
								(*InputReg).VDOP = (BYTE)TabletoShort(Data, a);
								PASSED = TRUE;
							}
					break;
					}
			}
			(*InputReg).GSA = TRUE;
		}
		else
			if ((Sentence[3] == 'G') && (Sentence[4] == 'G') && (Sentence[5] == 'A'))
			{
				for (Counter = 0, k = 0; (k <= (int)GEOIDALESEP) && (Sentence[Counter] != '*'); Counter++)
				{
					if (Sentence[Counter] == ',')
					{
						k++;
						PASSED = FALSE;
					}
					else
						switch (k)
						{
						case (int)MSLALT:
								if (!PASSED)
								{
									TableInit(Data, 15);
									unsigned int a = 0;
									while (Sentence[Counter + a] != '.')
										Data[a] = atos(Sentence[Counter + a++]);
									(*InputReg).Alt += (WORD)TabletoShort(Data, a);
									PASSED = TRUE;
								}
						break;
						case (int)GEOIDALESEP:
								if (!PASSED)
								{
									TableInit(Data, 15);
									unsigned int a = 0;
									while (Sentence[Counter + a] != '.')
										Data[a] = atos(Sentence[Counter + a++]);
									(*InputReg).Geo = TabletoSBYTE(Data, a);
									PASSED = TRUE;
								}
						break;
						case (int)FixStatus:
								if (!PASSED)
								{
									TableInit(Data, 15);
									Data[0] = atos(Sentence[Counter]);
									(*InputReg).Status = (TabletoShort(Data, 1) == 0) ? FALSE : TRUE;
									PASSED = TRUE;
								}
						break;
						case (int)NBRSat:
								if (!PASSED)
								{
									TableInit(Data, 15);
									unsigned int a = 0;
									while (Sentence[Counter + a] != ',')
										Data[a] = atos(Sentence[Counter + a++]);
									(*InputReg).FIX += (BYTE)TabletoShort(Data, a);
									PASSED = TRUE; ;
								}
						break;
						}
				}
				(*InputReg).GGA = TRUE;
			}
			else
				if ((Sentence[3] == 'R') && (Sentence[4] == 'M') && (Sentence[5] == 'C'))
				{
					for (Counter = 0, k = 0; (k <= (int)DATE) && (Sentence[Counter] != '*'); Counter++)
					{
						if (Sentence[Counter] == ',')
						{
							k++;
							PASSED = FALSE;
						}
						else
							switch (k)
							{
							case (int)DATE:
									if (!PASSED)
									{
										TableInit(Data, 15);
										Data[0] = atos(Sentence[Counter]);
										Data[1] = atos(Sentence[Counter + 1]);
										(*InputReg).DA = (BYTE)TabletoShort(Data, 2);
										TableInit(Data, 15);
										Data[0] = atos(Sentence[Counter + 2]);
										Data[1] = atos(Sentence[Counter + 3]);
										(*InputReg).MTH = (BYTE)TabletoShort(Data, 2);
										TableInit(Data, 15);
										Data[0] = atos(Sentence[Counter + 4]);
										Data[1] = atos(Sentence[Counter + 5]);
										(*InputReg).YY = (BYTE)TabletoShort(Data, 2);
										PASSED = TRUE;
									}
							break;
							case (int)EW:
									TableInit(Data, 15);
							Data[0] = atos(Sentence[Counter]);
							(*InputReg).EW = (TabletoShort(Data, 1) == 0) ? FALSE : TRUE;
							break;
							case (int)NS:
									TableInit(Data, 15);
							Data[0] = atos(Sentence[Counter]);
							(*InputReg).NS = (TabletoShort(Data, 1) == 0) ? FALSE : TRUE;
							break;
							case (int)TIME:
									if (!PASSED)
									{
										TableInit(Data, 15);
										Data[0] = atos(Sentence[Counter]);
										Data[1] = atos(Sentence[Counter + 1]);
										(*InputReg).HH = (BYTE)TabletoShort(Data, 2);
										TableInit(Data, 15);
										Data[0] = atos(Sentence[Counter + 2]);
										Data[1] = atos(Sentence[Counter + 3]);
										(*InputReg).MTE = (BYTE)TabletoShort(Data, 2);
										TableInit(Data, 15);
										Data[0] = atos(Sentence[Counter + 4]);
										Data[1] = atos(Sentence[Counter + 5]);
										(*InputReg).SS = (BYTE)TabletoShort(Data, 2);
										PASSED = TRUE;
									}
							break;
							case (int)Latitude:
									if (!PASSED)
									{
										TableInit(Data, 15);
										Data[0] = atos(Sentence[Counter]);
										Data[1] = atos(Sentence[Counter + 1]);
										(*InputReg).DD = (BYTE)TabletoShort(Data, 2);
										TableInit(Data, 15);
										Data[0] = atos(Sentence[Counter + 2]);
										Data[1] = atos(Sentence[Counter + 3]);
										(*InputReg).LAMTE = (BYTE)TabletoShort(Data, 2);
										TableInit(Data, 15);
										Data[0] = atos(Sentence[Counter + 5]);
										Data[1] = atos(Sentence[Counter + 6]);
										(*InputReg).LASS1 = (BYTE)TabletoShort(Data, 2);
										TableInit(Data, 15);
										Data[0] = atos(Sentence[Counter + 7]);
										Data[1] = atos(Sentence[Counter + 8]);
										(*InputReg).LASS2 = (BYTE)TabletoShort(Data, 2);
										PASSED = TRUE;
									}
							break;
							case (int)Longitude:
									if (!PASSED)
									{
										TableInit(Data, 15);
										Data[0] = atos(Sentence[Counter]);
										Data[1] = atos(Sentence[Counter + 1]);
										Data[2] = atos(Sentence[Counter + 2]);
										(*InputReg).DDD = (BYTE)TabletoShort(Data, 3);
										TableInit(Data, 15);
										Data[0] = atos(Sentence[Counter + 3]);
										Data[1] = atos(Sentence[Counter + 4]);
										(*InputReg).LOMTE = (BYTE)TabletoShort(Data, 2);
										TableInit(Data, 15);
										Data[0] = atos(Sentence[Counter + 6]);
										Data[1] = atos(Sentence[Counter + 7]);
										(*InputReg).LOSS1 = (BYTE)TabletoShort(Data, 2);
										TableInit(Data, 15);
										Data[0] = atos(Sentence[Counter + 8]);
										Data[1] = atos(Sentence[Counter + 9]);
										(*InputReg).LOSS2 = (BYTE)TabletoShort(Data, 2);
										PASSED = TRUE;
									}
							break;
							}
					}
					(*InputReg).RMC = TRUE;
				}
				else
					if ((Sentence[3] == 'V') && (Sentence[4] == 'T') && (Sentence[5] == 'G'))
					{
						for (Counter = 0, k = 0; (k <= (int)Vitesse) && (Sentence[Counter] != '*'); Counter++)
						{
							if (Sentence[Counter] == ',')
							{
								k++;
								PASSED = FALSE;
							}
							else
								switch (k)
								{
								case (int)Vitesse:
										if (!PASSED)
										{
											TableInit(Data, 15);
											Cpt = 0;
											while (Sentence[Counter + Cpt] != '.')
												Data[0 + Cpt] = atos(Sentence[Counter + Cpt++]);
											(*InputReg).Vit = (BYTE)(TabletoShort(Data, Cpt));
											PASSED = TRUE;
										}
								break;
								case (int)Azimuth:
										if (!PASSED)
										{
											TableInit(Data, 15);
											Cpt = 0;
											while (Sentence[Counter + Cpt] != '.')
												Data[0 + Cpt] = atos(Sentence[Counter + Cpt++]);
											(*InputReg).Azi = (WORD)(TabletoShort(Data, Cpt));
											PASSED = TRUE;
										}
								break;
								}
						}
						(*InputReg).VTG = TRUE;
					}
}//  Lecture et exploitation d'une phrase

void MakeTrame(BYTE* InputTrame, DataRegister* InputReg)
{
	//__MAJ du RTC________
	Calendar Current;
	Current.Annee = InputReg->YY;
	Current.Mois = InputReg->MTH;
	Current.NumeroJour = InputReg->DA;
	Current.Heure = InputReg->HH;
	Current.Minute = InputReg->MTE;
	Current.Seconde = InputReg->SS;
	Current.JourSemaine = 0;

	MajRTC(Current); // Provoque l'arrêt du RTC
	startRTC(); // Alors on le redemarre

	(*InputReg).TempC = 0;//(BYTE)GetTempC();

	// Trame[0] 0x1F = TempC(MSB 4 bits sur 7)
	InputTrame[0] |= (BYTE)(((*InputReg).TempC >> 3) & 0x0F);

	// Trame[1] 0xE0 = TempC (LSB 3 bits sur 7) 0x1F = HH(5 bits sur 5)
	InputTrame[1] |= (BYTE)(((*InputReg).TempC << 5) & 0xE0);
	InputTrame[1] |= (*InputReg).HH;

	// Trame[2] 0xFC = MTE( 6 bits sur 6) 0x03= SS(MSB 2 bits sur 6)
	InputTrame[2] |= (BYTE)(((*InputReg).MTE << 2) & 0xFC);
	InputTrame[2] |= (BYTE)(((*InputReg).SS >> 4) & 0x03);

	// Trame[3] 0xF0 = SS(LSB 4 bit sur 6) 0x0F= DA(MSB 4 bits sur 5)
	InputTrame[3] |= (BYTE)(((*InputReg).SS << 4) & 0xF0);
	InputTrame[3] |= (BYTE)(((*InputReg).DA >> 1) & 0x0F);

	// Trame[4] 0x80 = DA(LSB 1 bit sur 5) 0x78= MTH(4 bits sur 4) 0x07 = YY(MSB 3 bits sur 7)
	InputTrame[4] |= (BYTE)(((*InputReg).DA << 7) & 0x80);
	InputTrame[4] |= (BYTE)(((*InputReg).MTH << 3) & 0x78);
	InputTrame[4] |= (BYTE)(((*InputReg).YY >> 4) & 0x07);

	// Trame[5] 0xF0 = YY(LSB 4 bits sur 7) 0x0F= DD(MSB 4 bits sur 7)
	InputTrame[5] |= (BYTE)(((*InputReg).YY << 4) & 0xF0);
	InputTrame[5] |= (BYTE)(((*InputReg).DD >> 3) & 0x0F);

	// Trame[6] 0xE0 = DD(LSB 3 bits sur 7) 0x1F= LAMTE (MSB 5 bits sur 6)
	InputTrame[6] |= (BYTE)(((*InputReg).DD << 5) & 0xE0);
	InputTrame[6] |= (BYTE)(((*InputReg).LAMTE >> 1) & 0x1F);

	// Trame[7] 0x80 = LAMTE(LSB 1 bit sur 6) 0x7F= LASS1( 7 bits sur 7)
	InputTrame[7] |= (BYTE)(((*InputReg).LAMTE << 7) & 0x80);
	InputTrame[7] |= (*InputReg).LASS1;

	// Trame[8] 0xFE = LASS2( 7 bits sur 7) 0x01 = DDD(MSB 1 bit sur 8)
	InputTrame[8] |= (BYTE)(((*InputReg).LASS2 << 1) & 0xFE);
	InputTrame[8] |= (BYTE)(((*InputReg).DDD >> 7) & 0x01);

	// Trame[9] 0xFE = DDD(LSB 7 bits sur 8) 0x01 = LOMTE(MSB 1 bit sur 6)
	InputTrame[9] |= (BYTE)(((*InputReg).DDD << 1) & 0xFE);
	InputTrame[9] |= (BYTE)(((*InputReg).LOMTE >> 5) & 0x01);

	// Trame[10] 0xF8 = LOMTE(LSB 5 bits sur 6) 0x07 = LOSS1(MSB 3 bits sur 7)
	InputTrame[10] |= (BYTE)(((*InputReg).LOMTE << 3) & 0xF8);
	InputTrame[10] |= (BYTE)(((*InputReg).LOSS1 >> 4) & 0x07);

	// Trame[11] 0xF0 = LOSS1(LSB 4 bits sur 7) 0x0F = LOSS2(MSB 4 bits sur 7)
	InputTrame[11] |= (BYTE)(((*InputReg).LOSS1 << 4) & 0xF0);
	InputTrame[11] |= (BYTE)(((*InputReg).LOSS2 >> 3) & 0x0F);

	// Trame[12] 0xE0 = LOSS2(LSB 3 bits sur 7) 0x1F = AZI(MSB 5 bits sur 9)
	InputTrame[12] |= (BYTE)(((*InputReg).LOSS2 << 5) & 0xE0);
	InputTrame[12] |= (BYTE)(((*InputReg).Azi >> 4) & 0x1F);

	// Trame[13] 0xF0 = AZI(LSB 4 bits sur 9) 0x0F = FIX(4 bits sur 4)
	InputTrame[13] |= (BYTE)(((*InputReg).Azi << 4) & 0xF0);
	InputTrame[13] |= (*InputReg).FIX;

	// Trame[14] 0xC0 = NSEW( 2 bits sur 2) 0x3F = ALT(MSB 6 bits sur 14)
	InputTrame[14] |= (BYTE)(((*InputReg).EW) ? 0x80 : 0x00);
	InputTrame[14] |= (BYTE)(((*InputReg).NS) ? 0x40 : 0x00);
	InputTrame[14] |= (BYTE)(((*InputReg).Alt >> 8) & 0x3F);

	// Trame[15] 0xFF = ALT(LSB 8 bits sur 14)
	InputTrame[15] |= (BYTE)((*InputReg).Alt & 0x00FF);

	// Trame[16] 0xFF = GEO( 8 bits sur 8)
	InputTrame[16] |= (BYTE)(*InputReg).Geo;

	// Trame[17] 0xFF = Vit( 8 bits sur 8)
	InputTrame[17] |= (*InputReg).Vit;

	// Trame[18] 0xFF = PDOP( 8 bits sur 8)
	InputTrame[18] |= (*InputReg).PDOP;

	// Trame[19] 0xFF = HDOP( 8 bits sur 8)
	InputTrame[19] |= (*InputReg).HDOP;

	// Trame[20] 0xFF = VDOP( 8 bits sur 8)
	InputTrame[20] |= (*InputReg).VDOP;

	// Trame[21] 0xFF = VBat( 8 bits sur 8)
	InputTrame[21] |= (*InputReg).VBat;
}// Creer une trame de donnée

BOOL MakeData(char* Sentence, DataRegister* InputReg, BYTE* InputTrame) // Fabrique une donnée Après cette fonction GPSRegister devrait être rempli
{
	ReadSentence(Sentence, InputReg);

	if (!(*InputReg).RMC || !(*InputReg).GSA || !(*InputReg).GGA || !(*InputReg).VTG) // Si Registre GPS est remplie
	{
		MakeTrame(InputTrame, InputReg); // fabrique une trame
		resetDataGPSRegister(InputReg); // Raz de Registre GPS
		return TRUE;
	}
	return FALSE;
}

