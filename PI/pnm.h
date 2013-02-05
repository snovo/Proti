#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>


#define P1 "P1"
#define P2 "P2"
#define P3 "P3"
#define P4 "P4"
#define P5 "P5"
#define P6 "P6"

#define mem 10
#define memdim 10
#define nBytes 1
#define pixeis 256


#define int_inic -999999

typedef struct pnm
{
	char *tipo;	//familia PNM a que a imagem pertence
	char **comentarios; //comentarios presentes na imagem
	
	int colunas;	//numero de colunas
	int linhas;	//numero de linhas
	
	int max_color;	//valor maximo na imagem (identificador da cor com o valor mais alto)
	int **rep;	//matriz com os valores da imagem
}PNM;

int 	* initBin();
int 	bin2dec(int *bin);
int 	* dec2bin(int dec);
void 	* addValores(int ** rep,int iLh, int iCl,int dec, int colunas);
PNM 	* createEPNM();
int 	inicPNM(PNM *pnm);
void 	freePNM(PNM *pnm);
int 	retiraVal(FILE *file, char c);
int 	nextChar(FILE *file);
void 	* addComment(FILE *file, char ** comentarios, char c);
int 	leTipo(FILE *file, PNM *pnm);
int 	leDim(FILE *file, PNM *pnm);
int 	leMPixel(FILE *file, PNM *pnm);
int 	nextChar2(FILE *file);
int 	leRep(FILE *file, PNM *pnm);
int 	readPNM(char * path,PNM *pnm);
int 	writePNM(PNM *pnm, char * path);
int 	insertComment(char ** comentarios, char *comment);

/*Caso o apontador para o PNM seja diferente de NULL*/
int freePNMandRead(char * path, PNM *pnm);

/*cria uma estrutura PNM e le um PNM para a estrutura
* previamente criada, retornando o apontador para
* a mesma caso tu tenha corrido bem*/
PNM * lePNM(char * path);


//enum pnm_erros
//{
	/*nao foi possivel abrir p ficheiro*/
#define	EF_NULL 66000
	/*FIM DE FICHEIRO*/
#define	EF_EOF 66001
	/*erro no ficheiro*/
#define	EF_ERROR 66002
	/*falha ao obter tipo de pnm*/
#define	PNM_T_FAIL 66003
	/*tipo errado*/
#define	PNM_T_ERROR 66004
	/*Dimensoes erradas (0x0)*/
#define	PNM_D_ERROR 66005
	/*falha ao obter dimensoes*/
#define	PNM_D_FAIL 66006
	/*dimensoes incompletas*/
#define	PNM_D_INC 66007
	/*falha ao obter pixel maximo*/
#define	PNM_M_FAIL 66008
	/*erro ao obter matriz*/
#define	PNM_R_FAIL 66009
//};
