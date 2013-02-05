#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

#include "trabalho.h"


void executa(char *command)
{
	int pid_exec, pid_fork, i = 0, background = 0, pid;
	
	char **argumentos = (char **)malloc(sizeof(char *));
	
	int estado;

	/*divide o command por espacos*/
	char *comando_tok = strtok(command," ");
	
	/*vamos criar um array com o nome do executavel e seus argumentos*/
	if( comando_tok != NULL )
	{
		do
		{
			
		
			if ( strcmp( comando_tok, "&" ) == 0 )
			{
				background = 1 ;
				argumentos[i] = NULL ;
			}
			else
			{
				argumentos[i] = comando_tok ;
				/*realocamos espaco para mis um argumento*/
				argumentos = (char **)realloc(argumentos,sizeof(char *)*(i+2));
			}
			
			i++ ;
			
			comando_tok = strtok( NULL, " " ) ;
		}
		while( ( comando_tok != NULL ) );
		
	}
	
	pid_fork = fork();
	
	pid = pid_exec = getpid();
	
	if(pid_fork == -1)
	{
		perror(" fork ");
		exit(0);
	}
	
	if(pid_fork == 0)
	{
		/*Processo filho, execucao do nosso programa*/
		
		execvp(argumentos[0], argumentos);
		perror(argumentos[0]);
				
	}
	else
	{
		if(background != 1)
			wait(&estado);
	}
	
}

int menu_texto()
{
	printf("\n\n");
	printf("\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("\t~                                                        ~\n");
	printf("\t~                 Universidade do Minho                  ~\n");
	printf("\t~                        2005-2006                       ~\n");
	printf("\t~                                                        ~\n");
	printf("\t~          Trabalho de Programação Imperativa            ~\n");
	printf("\t~                        Melhoria                        ~\n");
	printf("\t~                                                        ~\n");
	printf("\t~              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~              ~\n");
	printf("\t~                                                        ~\n");
	printf("\t~          [1]  Ler Ficheiro                             ~\n");
	printf("\t~          [2]  Guardar PNM gerado                       ~\n");
	printf("\t~                                                        ~\n");
	printf("\t~          [3]  Visualizar PNM gerado                    ~\n");
	printf("\t~                                                        ~\n");
	printf("\t~          [4]  Inverter Horizontalmente PNM gerado      ~\n");
	printf("\t~          [5]  Inverter Verticalmente PNM gerado        ~\n");
	printf("\t~          [6]  Rodar PNM 90º para a direita             ~\n");
	printf("\t~          [7]  Rodar PNM 90º para a esquerda            ~\n");
	printf("\t~                                                        ~\n");
	printf("\t~          [8]  Converter PNM para PBM                   ~\n");
	printf("\t~          [9]  Converter PNM para PGM                   ~\n");
	printf("\t~          [10] Converter PNM para PPM                   ~\n");
	printf("\t~                                                        ~\n");
	printf("\t~                                                        ~\n");
	printf("\t~          [0]  Sair                                     ~\n");
	printf("\t~                                                        ~\n");
	printf("\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

	return 0;	
}

/*grava um PNM em dest em formato ascii ou nao (ascii = 1)*/
int gravarPNM(PNM * pnm, char * dest , int ascii)
{
	
	if(strlen(dest) <= 0) return EXIT_FAILURE;
	
	if(pnm == NULL) return EXIT_FAILURE;

	char * tipo = strdup(pnm->tipo);
	
	/*queremos converter um binario para um ascii*/
	/*binario*/
	if(ascii != 1)
	{
		if(strcmp(tipo,P1) == 0 )
			pnm = p1_2_p4(pnm);
		else if(strcmp(tipo,P2) == 0 )
			pnm = p2_2_p5(pnm);
		else if(strcmp(tipo,P3) == 0 )
			pnm = p3_2_p6(pnm);
	}
	/*ascii*/
	else
	{
		if(strcmp(tipo,P4) == 0 )
			pnm = p4_2_p1(pnm);
		else if(strcmp(tipo,P5) == 0 )
			pnm = p5_2_p2(pnm);
		else if(strcmp(tipo,P6) == 0 )
			pnm = p6_2_p3(pnm);
	}
	
	writePNM(pnm, dest);

	return EXIT_SUCCESS;
}

int menu_gravar(char *dest)
{
	printf("\n\tNome do ficheiro: ");
	fflush(stdin);
	scanf("%s",dest);
	fflush(stdin);
	
	printf("\n");
	printf("\n\tGravar em:\n");
	printf("\n\t   [1] Ascii");
	printf("\n\t   [0] Binário\n");
	
	return 0;
}

int menu_conv(char *path)
{
	fflush(stdin);
	
	printf("\n");
	printf("\n\tEm que familia deseja converter \n\t    [%s]?\n",path);
	printf("\n\t   [0] PGM");
	printf("\n\t   [1] PGM");
	printf("\n\t   [2] PBM\n");
	
	return 0;
}

int menu_abrir(char *org)
{
	printf("\n\tNome do ficheiro: ");
	fflush(stdin);
	scanf("%s",org);
	fflush(stdin);
	
	return 0;
}



int menu_opcao(int *opcao, int mim, int max)
{
	do{
		if(*opcao < mim || *opcao > max)
			printf("\n\t\tOpção errada. Escolha um digito entre %d e %d\n",mim,max);
	
		printf("\n\tOpção: ");
		scanf("%d",&(*opcao));
	}while(*opcao < mim || *opcao > max);
	
	return 0;
}


PNM * inVH(PNM * pnm)
{
	if(pnm != NULL)
	{
		PNM * temp = invH(pnm);
		
		freePNM(pnm);
		return temp;
	}
	else return NULL;
}

PNM * inVV(PNM * pnm)
{
	if(pnm != NULL)
	{
		PNM * temp = invV(pnm);
		freePNM(pnm);
		return temp;
	}
	else return NULL;
}

PNM * rodaRD(PNM * pnm)
{
	if(pnm != NULL)
	{
		PNM * temp = rodarD(pnm);
		freePNM(pnm);
		return temp;
	}
	else return NULL;
}

PNM * rodaRE(PNM * pnm)
{
	if(pnm != NULL)
	{
		PNM * temp = rodarE(pnm);
		freePNM(pnm);
		return temp;
	}
	else return NULL;
}

int fileType(char * path)
{
	if(path == NULL)
		return EXIT_FAILURE;

	/*apontador para os ultimos 4caracteres*/
	char * ext = strrchr(path,'.');
	
	if(ext == NULL)
		return EXIT_FAILURE;
	
	/*vamos retirar o ponto da extensao*/
	ext++;

	int len = strlen(ext);
	
	for(len--; len >= 0; len--)
		ext[len] = tolower(ext[len]);
		
	if(strstr(ext,"jpeg") != NULL ||
	   strstr(ext,"jpg") != NULL ||
	   strstr(ext,"gif") != NULL
	  )
		return FGIF_JPG;
	else if(strstr(ext,"pnm") != NULL ||
	   strstr(ext,"ppm") != NULL ||
	   strstr(ext,"pbm") != NULL ||
	   strstr(ext,"pgm") != NULL
	   )
		return FPNM;
	else if(strstr(ext,"txt") != NULL)
		return FTXT;
	
	return EXIT_FAILURE;
}


char * conv_gif_jpg(char * path, char * familia)
{
	if(path == NULL) return NULL;

	/*extensao do ficheiro*/
	char * ext = strrchr(path,'.');
	
	int len = ( ext == NULL ? 0 : strlen(ext));
	
	/*nome do ficheiro*/
	char * nome = (char *)strndup(path,strlen(path) - len);

	char * conv = (char *) malloc(strlen(path));

	/*tamanho da path origem + tamanho do file destino + 9caracteres
	* para o resto do comando*/
	char * comando = (char *) malloc( 2*strlen(path) + 13 );

	if( strcmp(familia,"PPM") == 0)
	{
		sprintf(conv,"%s.ppm",nome);
	}
	else if( strcmp(familia,"PGM") == 0)
	{
		sprintf(conv,"%s.pgm",nome);
	}
	else if( strcmp(familia,"PBM") == 0)
	{
		sprintf(conv,"%s.pbm",nome);
	}
	
	sprintf(comando,"convert \"%s\" \"%s\"",path,conv);
	system(comando);
	
	free(comando);
	
	return conv;
}

/*le um ficheiro*/
PNM * leFich(char *path, PNM *pnm2,char * familia)
{
	if(pnm2 != NULL)
	{
		freePNM(pnm2);
	}
	
	int type = fileType(path);	

	if(type == FGIF_JPG)
		path = conv_gif_jpg(path,familia);
	
	type = fileType(path);
	
	
	
	PNM * pnm;
	
	/*se for da familia pnm*/
	if(type == FPNM)
	{
		pnm = lePNM(path);
	}
	/*se for do formato texto normal*/
	else if(type == FTXT)
	{
	       pnm = criaPMusical(path);
	}
	else
	{
	       return NULL;
	}
		
	return pnm;
		
}



int modo_texto()
{
	int opcao = 0, opcao2 = 0;
	
//	ascii_or_binario se 1 ascii, se 0 binario
	int ascii_or_binario = 0;
	char *dest = (char *)malloc(sizeof(char)*100);
	char *org = (char *)malloc(sizeof(char)*100);
	
	PNM *pnm = NULL;
	
	char * familia = NULL;
	
	do{
		menu_texto();
		menu_opcao(&opcao,0,10);
	
		switch(opcao)
		{
			case 1: menu_abrir(org);

				if(fileType(org) == FGIF_JPG)
				{
					menu_conv(org);
					menu_opcao(&opcao2,0,2);
				
					switch(opcao2)
					{
						case 0: familia = strdup("PPM");
							break;
						case 1: familia = strdup("PGM");
							break;
						case 2: familia = strdup("PBM");
							break;
						default : familia = NULL;
					}
					
				}
				
			
				pnm = leFich(org,pnm,familia);
				
				
				if(familia != NULL) free(familia);
				
				if(pnm == NULL) 
				{
					fprintf(stderr,"\n\tFicheiro Irreconhecivel:\n\t  [%s] não existe ou não é um ficheiro válido\n",org);
				}
				break;
				
			case 2: menu_gravar(dest);
				menu_opcao(&ascii_or_binario,0,1);
				
				if(gravarPNM(pnm, dest, ascii_or_binario) 
					== EXIT_FAILURE)
					printf("\n\tImpossivel Gravar Imagem em [%s]\n",dest);
				break;
			
			case 3: writePNM(pnm, "/tmp/temporario.ppm");
				executa(strdup("display /tmp/temporario.ppm &"));
				break;
				
			case 4: pnm = inVH(pnm);
				break;
				
			case 5: pnm = inVV(pnm);
				break;
				
			case 6: pnm = rodaRD(pnm);
				break;
				
			case 7: pnm = rodaRE(pnm);
				break;
				
			case 8: pnm = converte(pnm,P4);
				break;
				
			case 9: pnm = converte(pnm,P5);
				break;
				
			case 10: pnm = converte(pnm,P6);
				break;
			case 0: if(pnm != NULL)
					freePNM(pnm);
				break;
			default: printf("\n\t\tOpção errada\n");
		}
		
	}while(opcao != 0);
	
	return 0;
}
