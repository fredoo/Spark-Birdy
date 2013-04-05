#define TailleFile 128


typedef struct TFile
{
    char corps [TailleFile];
    int Tete;
    int Queue;
} BC;

void InitBC (BC *bc);
int FileVide(BC *bc);
int FilePleine(BC *bc);
char Defiler(BC *bc);
void Enfiler(BC *bc, char c);
void VideBuffer(BC *bc);
