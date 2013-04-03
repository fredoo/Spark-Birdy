/*
 * Buffer-Circ.c
 *
 *  Created on: 26 mars 2013
 *      Author: Pfister
 */


#include "Buffer_Circ.h"

void InitBC (BC *bc)
{
	bc->Tete=bc->Queue=0;
}

int FileVide(BC *bc)
{
	return bc->Tete==bc->Queue;
}

int FilePleine(BC *bc)
{
	return ((bc->Queue +1)&(TailleFile-1)) == bc->Tete;
}

char Defiler(BC *bc)
{
	char ValDefiler;

	ValDefiler=bc->corps[bc->Tete];
	bc->Tete=(bc->Tete+1)&(TailleFile-1);
	return ValDefiler;
}

void Enfiler(BC *bc, char c)
{
    bc->corps[bc->Queue]=c;
    bc->Queue=(bc->Queue+1)&(TailleFile-1);
}
