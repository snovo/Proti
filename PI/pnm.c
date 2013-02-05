#include "pnm.h"

int * dec2bin(int dec)
{
	int * bin = (int *)malloc(sizeof(int)*8);
	int i = 7, valor = dec;
	
	while(i >= 0)
	{
		bin[i] = valor%2;
		valor = valor/2;
		i--;
	}
	
	return bin;
}


int *initBin()
{
	int bits = nBytes*8, i = 0;
	
	int * bin = (int *)malloc(sizeof(int)*bits);
	
	for(i = 0; i < bits; i++)
		bin[i] = 0;
		
	return bin;
}

int bin2dec(int *bin)
{
	int retorno = 0;
	int bits = nBytes*8, i = 0;
	bits--;
	
	for(i = 0; i <= bits; i++)
		if(i != 0) retorno+=(2<<(i-1))*(bin[bits-i]);
		else retorno+=(2>>1)*(bin[bits-i]);
		
	return retorno;
}

void * addValores(int ** rep,int iLh, int iCl,int dec, int colunas)
{
	int * binario = dec2bin(dec);
	int i = iCl + 7;
	
	if(i > colunas - 1) i = colunas-1;
	
	while(i >= iCl)
	{
		rep[iLh][i] = binario[i - iCl];
		i--;
	}
	
	return NULL;
}

/*cria e devolve o apontador para um PNM vazio*/
PNM * createEPNM()
{
	PNM *pnm = (PNM *) malloc(sizeof(PNM));
	
	inicPNM(pnm);
	
	return pnm;
}

int inicPNM(PNM *pnm)
{
	pnm->colunas = int_inic;
	pnm->linhas = int_inic;
	pnm->max_color = int_inic;
	
	pnm->tipo = (char *) malloc(3*sizeof(char));
	pnm->comentarios = (char **) malloc(mem*sizeof(char *));
	pnm->comentarios[0] = NULL;
	pnm->rep = (int **) malloc(mem*sizeof(int *));
	
	return EXIT_SUCCESS;
}

/*transforma a string comentario num comentario em PNMs
* e insere-o numa lista de comentarios*/
int insertComment(char ** comentarios, char *comentario)
{
	int i = 0;

	char * comment = (char *)malloc(sizeof(char)*strlen(comentario)+2);
	
	sprintf(comment,"#%s",comentario);
	
	/*Vamos por a apontar para o fim da lista*/
	while(comentarios[i] != NULL)
		i++;
	
	/*realocamos memoria para receber o novo comentario, sem perder os que 
	* la estao. 
	* (i+2) para se poder efectuar ...[i+1]=NULL*/
	comentarios = (char **) realloc(comentarios, (i+2)*sizeof(char *));

	/*adicionamos o novo comentarios à matriz*/
	comentarios[i]=strdup(comment);
	
	/*apontamos o proximo para NULL, apenas para uso interno*/
	comentarios[i+1] = NULL;

	free(comment);
	
	return EXIT_SUCCESS;
}

/*liberta a memória alocada por um PNM*/
void freePNM(PNM *pnm)
{
	int comment = 0, pixel = 0, linhas = 0;
	
	if(pnm != NULL)
	{
		
		if(pnm->tipo != NULL)
			free(pnm->tipo);
		
		if(pnm->comentarios != NULL)
		{
			while(pnm->comentarios[comment] != NULL)
				free(pnm->comentarios[comment++]);
			free(pnm->comentarios);
		}
		
		if(pnm->rep != NULL)
		{
			linhas = pnm->linhas;
			
			while(pixel < linhas)
				free(pnm->rep[pixel++]);
				
			free(pnm->rep);
		}
		
		
		free(pnm);
	}
}

/*devolve o proximo inteiro entre caracteres brancos do ficheiro*/
int retiraVal(FILE *file, char c)
{
	char *valor = (char *)malloc(sizeof(char)*mem);
	char ch = c;
	int retorno = 0;
	int i = 0;
	
	do
	{
		if(ch == EOF)
			return EF_EOF;
	
		i++;
		/*se a condicao for verdadeira realocamos memoria*/
		if(i%mem == 0)
		{
			valor = (char *) realloc(valor, (sizeof(char)+ i + mem) );
		}
		valor[i-1]=ch;
		/*queremos imediatamente o proximo caracter, mesmo que este
		* seja branco*/
		ch = fgetc(file);
	
	/*passa por "todos" os caracteres brancos(' ','\n')*/	
	}while( ch != ' ' && ch != '\n' && ch != EOF);
	
	
	if(i%mem == 0)
	{
		valor = (char *) realloc(valor, (sizeof(char)+ i + mem) );
	}
	valor[i]='\0';
	
	retorno = atoi(valor);
	
	free(valor);
	
	return retorno;
}

/*devolve o proximo caracter do ficheiro != branco*/
int nextChar(FILE *file)
{
	char c = '\a';
	size_t read = 0;
	
	do
	{
		//c = fgetc(file);
		read = fread(&c,nBytes,1,file);

		if(ferror(file))
			return EF_ERROR;

		if(feof(file))
			return EF_EOF;
		
	}while( (c > 6 && c < 14) || (c == ' ') );// || (c == '\t') || (c == '\n') || (c == 13) );

	return c;
}

/*le um comentaro do ficheiro e adiciona-o a uma lista comentarios*/
void * addComment(FILE *file, char ** comentarios, char c)
{
	int i = 0, coluna = 0;
	
	char *comment;
	comment = (char *) malloc(mem*sizeof(char));
	
	
	/*actualizamos o indice onde vai ser colocado o novo comentarios.
	* contamos o numero de bytes presentes ate ao momento na matriz*/
	while(comentarios[i] != NULL)
		i++;
		
	do
	{
		coluna++;
		
		if(coluna%mem == 0)
		{
			comment = (char *) realloc( comment,
					 mem*( (coluna/mem) + 1 ) );
		}
		
		comment[coluna-1] = c;
		
		c = fgetc(file);
	
	}while(c != '\n');

	comment[coluna] = '\0';
	
	/*realocamos memoria para receber o novo comentario, sem perder os que 
	* la estao. 
	* (i+2) para se poder efectuar ...[i+1]=NULL*/
	comentarios = (char **) realloc(comentarios, (i+2)*sizeof(char *));

	/*adicionamos o novo comentarios à matriz*/
	comentarios[i]=strdup(comment);
	
	/*apontamos o proximo para NULL, apenas para uso interno*/
	comentarios[i+1] = NULL;

	free(comment);
	
	return comentarios;
}


/*Esta funcao le e guarda o tipo de um PNM, e caso existam comentarios
* antes deste tambem os guarda.
*/
int leTipo(FILE *file, PNM *pnm)
{
	char c = '\a';
	int done = 0, i = 0;
	
	do
	{
		c = nextChar(file); /*lemos o 1º caracter nao branco*/
		
		if(c == -1) return EXIT_FAILURE;
		
		if(c == '#')
		{	
			/*vamos adicionar esta linha como comentarios, pois
			* comeca por #.*/
			pnm->comentarios = addComment(file,pnm->comentarios,c);	
		}
		else
		{
			/*vejamos se é o identificador. este tem de comecar por
			* um 'P' maiusculo e tem de ser seguido por um digito
			* entre 1 e 6, caso contrario, o PNM nao é valido*/
			if(c == 'P')
			{
				/*como c == P, guardamo-lo...*/
				pnm->tipo[i++] = 'P';
				
				/*quero exactamente o proximo caracter*/
				//c = fgetc(file);
				c = nextChar(file);
				
				/*caso este c esteja entre 1 e 6, detectamos
				* com sucesso o tipo do PNM, caso contrario o
				* PNM nao e valido*/
				if(c >= 49 && c <= 56)
				{
					pnm->tipo[i++]= c;
					pnm->tipo[i]='\0';
					done = 1;
				}
				else
					return PNM_T_ERROR;
				
			}
			else
				return PNM_T_FAIL;
						
		}
		
	}while(done == 0 && feof(file)==0);
	
	
	return EXIT_SUCCESS;
}


int leDim(FILE *file, PNM *pnm)
{
	char c = '\a';
	int done = 0, i = 0;
	char *valor = (char *)malloc(sizeof(char)*mem);
	
	
	
	do
	{
		c = nextChar(file); /*lemos o 1º caracter nao branco*/
		
		if(c == '#')
		{	
			/*vamos adicionar esta linha aos comentarios, pois
			* comeca por #.*/
			pnm->comentarios = addComment(file,pnm->comentarios,c);	
		}
		else
		{
			if(isdigit(c))
			{
				while(isdigit(c))
				{
					i++;
					/*se a condicao for verdadeira 
					* realocamos memoria*/
					if(i%mem == 0)
					{
						valor = (char *) realloc(valor,
							 (sizeof(valor)) + mem );
					}
		
					valor[i-1]=c;
					/*queremos imediatamente o proximo
					* caracter, mesmo que este seja branco*/
					c = fgetc(file);
				}
				valor[i]='\0';

				pnm->colunas = atoi(valor);
				
				i = 0;
				free(valor);
				valor = (char *)malloc(sizeof(char)*mem);
			
				/*as dimensoes podem estar em linhas
				* diferentes...*/
				c = nextChar(file);
		
				if(isdigit(c))
				{
					while(isdigit(c))
					{
						i++;
						
						/*se a condicao for verdadeira
						* realocamos memoria*/
						if(i%mem == 0)
						{
							valor = (char *) realloc(valor,
								 (sizeof(valor)) + mem );	
						}
		
						valor[i-1]=c;
						/*queremos imediatamente o 
						* proximo caracter, mesmo que
						* este seja branco*/
						c = fgetc(file);		
					}
					valor[i]='\0';
			
					pnm->linhas = atoi(valor);
					
					/*se o numero de colunas ou linhas for
					* zero, entao o PNM nao esta num 
					* formato valido*/
					if(pnm->colunas == int_inic 
						|| pnm->linhas == int_inic) 
							return PNM_D_ERROR;
					
					done = 1;
				}
				else
					return PNM_D_INC;
			}
			else
				return PNM_D_FAIL;
			
			free(valor);	
		}
		
	}while(done == 0 && feof(file)==0);
	
	
	return EXIT_SUCCESS;
}

/*le o pixel maximod de PNM*/
int leMPixel(FILE *file, PNM *pnm)
{
	char c = '\a';
	int done = 0, i = 0;
	char *valor = (char *)malloc(sizeof(char)*mem);
	
	
	do
	{
		c = nextChar(file); /*lemos o 1º caracter nao branco*/
		
		if(c == '#')
		{	
			/*vamos adicionar esta linha como comentarios, pois
			* comeca por #.*/
			pnm->comentarios = addComment(file,pnm->comentarios,c);	
		}
		else
		{
			if(isdigit(c))
			{
				while(isdigit(c))
				{
					i++;
					/*se a condicao for verdadeira
					* realocamos memoria*/
					if(i%mem == 0)
					{
						valor = (char *) realloc(valor,
							 (sizeof(valor)) + mem );
					}
		
					valor[i-1]=c;
					/*queremos imediatamente o proximo
					* caracter, mesmo que este seja branco*/
					c = fgetc(file);
				}
				valor[i]='\0';
			
				pnm->max_color = atoi(valor);
				
				done = 1;
			}
			else return PNM_M_FAIL;
		}
	}while(done == 0 && !feof(file));
	
	
	free(valor);

//SERA NECESSARIO???????????????????????????? ESTE IF	
//	if(pnm->max_color == int_inic) return EXIT_FAILURE;
	
	return EXIT_SUCCESS;
}

/*Funcao responsavel por ler a matriz da imagem*/
int leRep(FILE *file, PNM *pnm)
{
	char c = '\a';
	char * tipo = strdup(pnm->tipo);
	int done = 0, cl = 0, lh = 0, colunas = 0, linhas = 0, valor = 0;
	
	
	if( strcmp(tipo,P1) == 0 || strcmp(tipo,P2) == 0 || strcmp(tipo,P3) == 0)
	{	
		do
		{
			c = nextChar(file); /*lemos o 1º caracter nao branco*/
		
			if(c == '#')
			{	
				/*vamos adicionar esta linha como comentarios, pois
				* comeca por #.*/
				pnm->comentarios = addComment(file,pnm->comentarios,c);	
			}
			else
			{				
				if(strcmp(pnm->tipo,P3) == 0)
					colunas = (pnm->colunas)*3;
				else
					colunas = (pnm->colunas);
					
				linhas = pnm->linhas;
			
				pnm->rep = (int **)realloc(pnm->rep, 
								linhas*(sizeof(int *)) );
		
				for(lh = 0; lh < linhas; lh++)
				{
					pnm->rep[lh] = (int *)
						malloc( colunas*sizeof(int) );
			
					for(cl = 0; cl < colunas; cl++)
					{
						if(feof(file)) return EF_EOF;

						valor = retiraVal(file,c);
						pnm->rep[lh][cl] = valor;
						c = nextChar(file);
					}
				}
			
				done = 1;
			}
		}while(done == 0 && !feof(file));
	}
	else
	{
	
		/*se inicialmente nao estamos perante um P1, P2 ou P3
		* entao é provavel que estejamos perante um P4, P5 ou P6.
		* Este algoritmo guarda um PNM binario como se fosse um PNM
		* em texto corrido. É mais facil de calcular a memoria necessaria 
		* para o armazenar. Este sistema consome mais memoria para 
		* armazenar um P1 ou P4, pois sao os unicos que se podem
		* compactar, os formatos P2 P3 P5 e P6 ja estao por defeito
		* compactados*/
		
		c = fgetc(file); /*lemos o 1º caracter nao branco*/
		
		tipo = strdup(pnm->tipo);
		
		
		
		if(strcmp(tipo,P4) == 0)
		{
			colunas = pnm->colunas;
					
			linhas = pnm->linhas;
			
			pnm->rep = (int **)realloc(pnm->rep, 
							linhas*(sizeof(int *)));

			for(lh = 0; lh < linhas; lh++)
			{
				pnm->rep[lh] = (int *)
					malloc( colunas*sizeof(int) );
		
				for(cl = 0; cl < colunas; cl+=8)
				{
					if(feof(file)) return EF_EOF;

					if( c < 0 ) valor = 256 + c;
					else valor = c;
					
					addValores(pnm->rep,lh,cl,
							valor,colunas);
						
					c = fgetc(file);
				}
			}
		
			done = 1;
		
		}
		else if(strcmp(tipo,P5) == 0 || strcmp(tipo,P6) == 0)
		{
			if(strcmp(tipo,P6) == 0 )
				colunas = (pnm->colunas)*3;
			else
				colunas = (pnm->colunas);
					
			linhas = pnm->linhas;
			
			pnm->rep = (int **)realloc(pnm->rep, 
							linhas*(sizeof(int *)) );
							
		
			for(lh = 0; lh < linhas; lh++)
			{
				pnm->rep[lh] = (int *)
					malloc( colunas*sizeof(int) );
		
				for(cl = 0; cl < colunas; cl++)
				{
					if(feof(file)) return EF_EOF;

					if( c < 0 ) valor = 256 + c;
					else valor = c;
					
					pnm->rep[lh][cl]=valor;
						
					c = fgetc(file);
					
				}
			}
		
			done = 1;
		}
		else return PNM_R_FAIL;
	}
	
	free(tipo);
	return EXIT_SUCCESS;
}


/*le um PNM*/
int readPNM(char * path ,PNM *pnm)
{

	int temp = -1;
	
	FILE * file = fopen(path,"rb");	
	
	if(file == NULL)
		return EF_NULL;
	else if( (temp = leTipo(file, pnm)) != EXIT_SUCCESS) 
		return temp;
	else if( (temp = leDim(file, pnm)) != EXIT_SUCCESS)
			return temp;
		else 	
			if(strcmp(pnm->tipo , P1) != 0 
				&& strcmp(pnm->tipo , P4) != 0)
			{
				if( (temp = leMPixel(file, pnm)) != EXIT_SUCCESS)
					return temp;
				else if( (temp = leRep(file, pnm)) != EXIT_SUCCESS)
						return temp;
			}
			else if( (temp = leRep(file, pnm)) != EXIT_SUCCESS)
					return temp;
							
	
	
	fclose(file);
	
	return EXIT_SUCCESS;
}

/*cria uma estrutura PNM e le um PNM para a estrutura
* previamente criada, retornando o apontador para
* a mesma caso tu tenha corrido bem*/
PNM * lePNM(char * path)
{
	PNM * pnm = createEPNM();

	if(readPNM(path, pnm) == EXIT_SUCCESS)
	{
		return pnm;
	}
	else
	{
		return NULL;
	}
}

/*Caso o apontador para o PNM seja diferente de NULL*/
int freePNMandRead(char * path, PNM *pnm)
{
	int temp = -1;

	if(pnm != NULL)
		freePNM(pnm);
		
	pnm = createEPNM();
	
	if( (temp = readPNM(path, pnm)) == EXIT_SUCCESS)
		return EXIT_SUCCESS;
	else
		return temp;
}


/*guarda um PNM em ficheiro*/
int writePNM(PNM *pnm, char * path)
{
	int i = 0, cl = 0, lh = 0, colunas = 0, max_color = 0,
		 linhas = 0, col = 0, max = 0, val = 0;
	
	char ch;
	
	char * tipo;
	
	int max_boolean = 0, ascii = 0, ind = 0; //falso
	
	int * bin;
	
	FILE *fp;
	
	/*Criacao do ficheiro path em modo escrita e binario*/
	fp = fopen(path,"wb");
	
	if(fp == NULL)
		return EF_NULL;
		
	tipo = strdup(pnm->tipo);
	
	colunas = col =  pnm->colunas;
	linhas = pnm->linhas;
	
	/*caso a condicao seja verdadeira, max_boolean = 1*/
	max_boolean = (strcmp(tipo , P1) != 0 && strcmp(tipo , P4) != 0);
	
	ascii = (strcmp(tipo , P1) == 0 || 
		strcmp(tipo , P2) == 0 || strcmp(tipo , P3) == 0);
	
	
	if(max_boolean == 1)
		max_color = pnm->max_color;
	
	
	
	/*Comecemos a escrever o ficheiro*/
	
	/*Tipo*/
	fprintf(fp,"%s\n",tipo);
	
	/*Comentarios*/
	i = 0;
	while(pnm->comentarios[i] != NULL)
	{
		fprintf(fp,"%s\n",pnm->comentarios[i++]);
	}
		
	/*Dimensoes*/
	fprintf(fp,"%d %d\n",colunas,linhas);
	
	/*Max_color*/
	if(max_boolean == 1)
		fprintf(fp,"%d\n",max_color);
		
	
	/*RGB? -> actualizacao das colunas para impressao dos pixeis*/
	if(strcmp(tipo,P6) == 0 || strcmp(tipo,P3) == 0)
		colunas = (pnm->colunas)*3;
		
		
	/*Matriz*/
	if(ascii == 1)
	{			
		for(lh = 0; lh < linhas; lh++)
		{
			for(cl = 0; cl < colunas; cl++)
			{
				val = pnm->rep[lh][cl];
				
				if((max + 4)>70 ) {max = 0; fprintf(fp,"\n");}
				
				fprintf(fp,"%4d",val);
						
				max+=4;
			}
		}
		fprintf(fp,"\n");
	}
	/*formato compactado (binario)*/
	else
	{
		if(strcmp(pnm->tipo,P4) == 0)
		{
			for(lh = 0; lh < linhas; lh++)
			{
				for(cl = 0; cl < colunas; cl+=8)
				{
					if( ((colunas-1) - cl) < 0 )
					{
						cl = colunas - 1;
						ind = colunas - cl;
					}
					else ind = 7;
					
					bin = initBin();
					
					while( ind >= 0 )
					{
						ch = pnm->rep[lh][cl+ind];
						
						if( ch <= 0 ) ch = 0;
						else ch = 1;
						bin[ind] = ch;
						
						
						ind--;
					}
					fprintf(fp,"%c",bin2dec(bin));
					
				}
				
			}
		}
		else
		{
			for(lh = 0; lh < linhas; lh++)
			{
				for(cl = 0; cl < colunas; cl++)
				{
					ch = pnm->rep[lh][cl]; 
				
					if( ch < 0 ) val = 256 + ch;
				
					fprintf(fp,"%c",ch);	
				}
			}
		}
	}

	free(tipo);
	
	fclose(fp);
	
	return EXIT_SUCCESS;
}
