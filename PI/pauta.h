#include "pnm_manipulation.h"


#define p_seminima_path_ppm 	"./pauta_imagens/p_seminima.ppm"
#define p_semibreve_path_ppm 	"./pauta_imagens/p_semibreve.ppm"
#define p_minima_path_ppm 	"./pauta_imagens/p_minima.ppm"
#define p_colcheia_path_ppm 	"./pauta_imagens/p_colcheia.ppm"
#define p_semicolcheia_path_ppm "./pauta_imagens/p_semicolcheia.ppm"
#define p_fusa_path_ppm 	"./pauta_imagens/p_fusa.ppm"
#define clave_de_sol_ppm	"./pauta_imagens/clave-sol.ppm"
#define seminima_path_ppm 	"./pauta_imagens/seminima.ppm"
#define semibreve_path_ppm 	"./pauta_imagens/semibreve.ppm"
#define minima_path_ppm 	"./pauta_imagens/minima.ppm"
#define colcheia_path_ppm 	"./pauta_imagens/colcheia.ppm"
#define semicolcheia_path_ppm 	"./pauta_imagens/semicolcheia.ppm"
#define fusa_path_ppm 		"./pauta_imagens/fusa.ppm"
#define repeticao_ppm		"./pauta_imagens/repeticao.ppm"
#define prefixo			"./pauta_imagens/abc/"

/*dado que os sustenidos sao iguais as bemols, só representamos os sustenidos*/
#define Do "C"
#define Do_s "C#"

#define Re "D"
#define Re_s "D#"
#define Re_b "DB"

#define Mi "E"
#define Mi_b "EB"

#define Fa "F"
#define Fa_s "F#"

#define Sol "G"
#define Sol_s "G#"
#define Sol_b "GB"

#define La "A"
#define La_s "A#"
#define La_b "AB"

#define Si "B"
#define Si_b "BB"

#define Do_m "CM"

#define Pausa "PAUSA"
#define Rep "REPETE"
#define Comp "COMPASSO"
#define acordS "("
#define acordE ")"

/*espaco inicial*/
#define inicial_spc 100

/*distancia entre linhas, por defeito: 10pixeis*/
#define dbl 20

/*espessura das linhas, por defeito: 3pixeis*/
#define espl 3

/*espessura das linhas dos compassos, por defeito 2pixeis*/
#define espCl 2

/*comprimento da pauta*/
#define comprimento 1024

/*altura da pauta = 4*dbl + 2*dbl + 2*dbl + 5*espl
* 4espacos entre linhas + 2margens (superiror e inferior) + 5linhas*/
#define altura (4*dbl + 2*dbl + 2*dbl + 5*espl + dbl)

/*coordenada y da 1ª linha (de baixo para cima)
* altura da pauta - margem inferior*/
#define y_first_line (altura-3*dbl)

/*tamanho de cada nota (l*l)*/
#define x_nota 18

/*largura compasso*/
#define x_compasso (espCl+4)

/*espaco entre as notas*/
#define x_spc_nota 10

/*posicao das notas nas linhas para a clave de sol*/
#define meio_linha	(espl/2)
#define meio_esp	(dbl/2)


#define yDo	(yMi + dbl + espl)
#define yDo_s	yDo

#define yRe	(yFa + dbl + espl)
#define yRe_s	yRe
#define yRe_b	yRe

#define yMi	(ySol + dbl + espl)
#define yMi_b	yMi

#define yFa	(yLa + dbl + espl)
#define yFa_s	yFa

#define ySol 	(y_first_line - dbl - espl - meio_linha)
#define ySol_s	ySol
#define ySol_b	ySol

#define yLa	(ySol + meio_linha - espl - meio_esp)
#define yLa_s	yLa
#define yLa_b	yLa

#define ySi	(ySol - dbl -espl)
#define ySi_b	ySi

#define yDo_m	(yLa - dbl -espl)


#define y_pausa yLa -1
#define yrep1	yDo_m
#define yrep2	yLa

#define yText	altura-10

/*definicao das cores das notas*/
#define cDo "ff0000"
#define cDo_s "ff3300"

#define cRe "ff6600"
#define cRe_s "ffcc00"
#define cRe_b "ff3300"

#define cMi "ffff33"
#define cMi_b "ffcc00"

#define cFa "00ff00"
#define cFa_s "00aa00"

#define cSol "004400"
#define cSol_s "0000ff"
#define cSol_b "00aa00"

#define cLa "330033"
#define cLa_s "990066"
#define cLa_b "0000ff"

#define cSi "ff0066"
#define cSi_b "990066"

#define cDo_m "ff0000"


typedef struct notas
{
	char *nota;
	char * letra;
	int pausa;
	struct notas *next;
} Notas;


/*as repeticoes e pausas sao contados como notas*/
typedef struct musica
{
	int acordes;
	int notas;
	int compassos;
	
	char * titulo;
	Notas * letra;
} Musica;





//Funcoes

/*converte um RGB em Hexadecimal para um array de inteiros (resp. [r g b])*/
int * hexa_2_dec(char * hexa);

/*cria um pnm P6 colando a imagem I1 com a I2*/
PNM * colaPNM(PNM *i1, PNM *i2);

/*cria o texto associado a uma nota*/
PNM * criaTexto(char * text);

/*cria e inicializa uma estrutura Musica*/
Musica * createMusica();

/*Insere uma nota (char *) numa estrutura Musica*/
int insertNota(Musica *musica, char *nota, int pausa);

/*associa uma "letra" de uma musica a uma nota*/
int insertLetra(Musica * musica, char * letra);

/*verifica se str é uma nota ou nao*/
int checkNota(char * str);

/*Dado um ficheiro com as notas da musica e uma Estrutura Musica, o conteudo
* do ficheiro é armazenado na estrutura*/
int readMusic(char * path, Musica * musica);

/*calcula o numero de segmentos de uma pauta que serão necessarios criar 
* para que se possam colocar todas as notas na pauta*/
int calcula_n_pauta(Musica *musica);

/*cria uma pauta (sem as notas, somente linhas) no formato P6*/
PNM * createPauta(Musica *musica,int mul_altura) ;

/*Uma pauta por defeito é escrita no format P6 ou P3, dai que, apenas
* tratemos a troca de cores para este formato*/
int swapColor(PNM * pnm, int oldcolor, int borda, int *newcolor);

/*funcao que dada uma nota (PNM), uma pauta (PNM), introduz a nota na pauta,
* sobrepondo a nota à pauta na posicao correcta.
* y_nota é a posicao onde a nota vai ficar e nao a coordenada; a coordenada
* é dada por (y_nota -(x_nota/2))*/
int sbrPNM(PNM *nota, PNM * pauta,int y_nota , int xx_nota);

/*insere a clave de sol (PNM) na pauta (PNM)*/
int insereCSol(PNM *clave, PNM *pauta, int y_inic);

/*Insere na pauta uma linha de separacao de compassos e/ou a linha 
* suplementar do Do.
* PNM onde a alterar, coordenadas x e y de inicializacao e finalizacao
* de insercao do segmento de recta*/
int insereSegLinha(PNM *pauta,int yy_start, int yy_end, int xx_start, int xx_end);

/*funcao utilizada em criaPMusical para colocar os objectos 
* (linhas, notas, etc) nas pautas*/
int sbr(PNM * objecto,
	PNM * pauta,
	int yy_nota,
	int xx_inc,
	int *xx_nota,
	int *i,
	int _do);
	
/*funcao que determina a nota a inserir e actualiza o yy_nota, o xx_inc e
* ainda troca a cor da nota*/
int detNota(PNM * nota, char * objecto, int *yy_nota,
	 int *xx_inc, int acorde, int i, int *_do);
	 
/*cria a pauta musical referente à musica "path"*/
PNM * criaPMusical(char *path);
