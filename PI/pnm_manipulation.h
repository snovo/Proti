#include "pnm.h"

/*devolve uma copia de um PNM*/
PNM * copia(PNM *pnm);

/*devolve uma copia da matriz de comentarios de um PNM*/
char ** copiaComments(char ** comments);

/*devolve uma copia da matriz de pixeis de um PNM*/
int ** copiaRep(int ** rep, int mul_col, int colunas, int linhas);

/*Determina o pixel maximo de um formato*/
int detMax_color(int **rep, int cl, int lh);



/*----------------------------converte----------------------------*/
PNM *converte(PNM *pnm, char*tipoDes);


/* ____________________________de P1 para ... ______________________________*/
	
/*converte um PNM do formato P1 para o formato P4*/
PNM * p1_2_p4(PNM *pnm);

/*converte um PNM do formato P1 para o formato P2*/
PNM * p1_2_p2(PNM *pnm);

/*converte um PNM do formato P1 para o formato P5*/
PNM * p1_2_p5(PNM *pnm);

/*converte um PNM do formato P1 para o formato P3*/
PNM * p1_2_p3(PNM *pnm);

/*converte um PNM do formato P1 para o formato P6*/
PNM * p1_2_p6(PNM *pnm);

/*_______________________________de P2 para ...______________________________*/

/*converte um PNM do formato P4 para o formato P1*/
PNM * p2_2_p5(PNM *pnm);

/*converte de P2 para P1*/
PNM * p2_2_p1(PNM * pnm);

/*converte de P2 para o formato P4*/
PNM * p2_2_p4(PNM * pnm);

/*converte de P2 para P3*/
PNM * p2_2_p3(PNM *pnm);

/*converte de P2 para o formato P6*/
PNM * p2_2_p6(PNM *pnm);

/*______________________________de P3 para ... _____________________________*/

/*converte um PNM do formato P4 para o formato P1*/
PNM * p3_2_p6(PNM *pnm);

/*converte de P3 para P1*/
PNM * p3_2_p1(PNM *pnm);

/*converte de P3 para o formato P4*/
PNM * p3_2_p4(PNM *pnm);

/*converte de P3 para P2*/
PNM * p3_2_p2(PNM *pnm);

/*converte de P3 para o formato P5*/
PNM * p3_2_p5(PNM *pnm);



/*________________________________de P4 para ... ____________________________*/

/*converte um PNM do formato P4 para o formato P1*/
PNM * p4_2_p1(PNM *pnm);

/*converte de P4 para P2*/
#define p4_2_p2(pnm)p1_2_p2(pnm)

/*converte de P4 para P5*/
#define p4_2_p5(pnm)p1_2_p5(pnm)

/*converte um PNM do formato P4 para o formato P3*/
#define p4_2_p3(pnm)p1_2_p3(pnm)

/*converte um PNM do formato P4 para o formato P6*/
#define p4_2_p6(pnm)p1_2_p6(pnm)




/*______________________________de P5 para ... _____________________________*/

/*converte um PNM do formato P5 para o formato P1*/
PNM * p5_2_p2(PNM *pnm);

/*converte de P5 para P1*/
#define p5_2_p1(pnm) p2_2_p1(pnm)

/*converte de P5 para P4*/
#define p5_2_p4(pnm) p2_2_p4(pnm)

/*converte de P5 para P3*/
#define p5_2_p3(pnm) p2_2_p3(pnm)

/*converte de P5 para o formato P6*/
#define p5_2_p6(pnm) p2_2_p6(pnm)



/*______________________________de P6 para ... _____________________________*/

/*converte um PNM do formato P6 para o formato P1*/
PNM * p6_2_p3(PNM *pnm);

/*converte de P6 para P1*/
#define p6_2_p1(pnm) p3_2_p1(pnm)

/*converte de P6 para P4*/
#define p6_2_p4(pnm) p3_2_p4(pnm)

/*converte de P6 para P2*/
#define p6_2_p2(pnm) p3_2_p2(pnm)

/*converte de P6 para P5*/
#define p6_2_p5(pnm) p3_2_p5(pnm)




/*___________________________Rodar 90º p/ direita___________________________*/

PNM * rodarD(PNM *pnm);

/*___________________________Rodar 90º p/ esquerda__________________________*/

PNM * rodarE(PNM *pnm);

/*__________________________Inverter Horizontalmente________________________*/

PNM * invH(PNM *pnm);

/*___________________________Inverter Verticalmente_________________________*/

PNM * invV(PNM *pnm);
