#define _GNU_SOURCE
#include "pauta.h"
#include <string.h>

#define	FGIF_JPG 5
#define FPNM 6
#define	FTXT 7


void executa(char *command);
int menu_texto();
int gravarPNM(PNM * pnm, char * dest , int ascii_or_binario);
int menu_gravar(char *dest);
int menu_abrir(char *org);
int menu_conv(char *path);
int menu_opcao(int *opcao, int mim, int max);
PNM * leFich(char *path, PNM *pnm2, char * familia);
PNM * inVH(PNM * pnm);
PNM * inVV(PNM * pnm);
PNM * rodaRD(PNM * pnm);
PNM * rodaRE(PNM * pnm);
int modo_texto();
int fileType(char * path);
char * conv_gif_jpg(char * path, char * familia);
