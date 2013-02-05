
#include "pauta.h"


void freeMusica(Musica * musica)
{
	if(musica != NULL)
	{
		free(musica->titulo);
		
		Notas * notas = musica->letra, *temp = musica->letra;
		
		if(notas != NULL)
		{
			for(temp = temp->next;temp != NULL;temp = temp->next)
			{
				if(notas->nota != NULL) free(notas->nota);
				if(notas->letra != NULL) free(notas->letra);
				free(notas);
				notas = temp;
			}
			free(notas);
		}
		
		free(musica);
	}
}

/*converte um RGB em Hexadecimal para um array de inteiros (resp. [r g b])*/
int * hexa_2_dec(char * hexa)
{
	int *rgb = (int *)malloc(sizeof(int) * 3);

	char r[3];
	r[0] = hexa[0];
	r[1] = hexa[1];
	r[2] = '\0';

	char g[3];
	g[0] = hexa[2];
	g[1] = hexa[3];
	g[2] = '\0';

	char b[3];
	b[0] = hexa[4];
	b[1] = hexa[5];
	b[2] = '\0';

	sscanf(r,"%x",&rgb[0]);
	sscanf(g,"%x",&rgb[1]);
	sscanf(b,"%x",&rgb[2]);

	return rgb;
}


/*cria um pnm P6 colando a imagem I1 com a I2*/
PNM * colaPNM(PNM *i1, PNM *i2)
{

	if(i1 == NULL)
		return NULL;
	
	int lh = 0, cl1 = 0, cl2 = 0, c_i1 = i1->colunas*3, c_i2 = 0,
	 c_i = 0, l_i1 = i1->linhas,m1 = 0, max = 0, pixel_m = 0;

	PNM * retorno = createEPNM();

	if(i2 == NULL)
	{
		/*cria um espaco de 3pixeis*/
		c_i2 = 27;
		retorno->colunas = i1->colunas + 3;
		pixel_m = pixeis-1;
	}
	else 
	{
		c_i2 = i2->colunas*3;
		retorno->colunas = i1->colunas + i2->colunas;
	}
	
	c_i = c_i1 + c_i2;
	 
	retorno->tipo = strdup(P6);
	retorno->linhas = l_i1;
	
	retorno->rep = (int **)realloc(retorno->rep, l_i1*(sizeof(int *)) );

	for(lh = 0; lh < l_i1; lh++)
	{
		retorno->rep[lh] = (int *) malloc( c_i*sizeof(int) );
		
		for(cl1 = 0; cl1 < c_i1; cl1+=3)
		{
			m1 = retorno->rep[lh][cl1] = i1->rep[lh][cl1];
			if(m1 > max) max = m1;	
			m1 = retorno->rep[lh][cl1+1] = i1->rep[lh][cl1+1];
			if(m1 > max) max = m1;
			m1 = retorno->rep[lh][cl1+2] = i1->rep[lh][cl1+2];
			if(m1 > max) max = m1;
		}
		if(i2 != NULL)
		{
			for(cl2 = c_i1, cl1 = 0; cl2 < c_i2 + c_i1; cl2+=3,cl1+=3)
			{
				m1 = retorno->rep[lh][cl2] = i2->rep[lh][cl1];
				if(m1 > max) max = m1;
				m1 = retorno->rep[lh][cl2+1] = i2->rep[lh][cl1+1];
				if(m1 > max) max = m1;
				m1 = retorno->rep[lh][cl2+2] = i2->rep[lh][cl1+2];
				if(m1 > max) max = m1;
			}
		}
		else
		{
			for(cl2 = c_i1, cl1 = 0; cl2 < c_i2 + c_i1; cl2+=3,cl1+=3)
			{
				m1 = retorno->rep[lh][cl2] = pixel_m;
				m1 = retorno->rep[lh][cl2+1] = pixel_m;
				m1 = retorno->rep[lh][cl2+2] = pixel_m;
				max = pixeis-1;
			}
		}
	}
	
	if(i1 != NULL)
		free(i1);
	if(i2 != NULL)
		free(i2);

	retorno->max_color = max;

	return retorno;
}

/*cria o texto associado a uma nota*/
PNM * criaTexto(char * text)
{
	PNM * retorno = NULL;
	
	if(text == NULL)
		return NULL;
		
	int len = strlen(text), i = 1;
	char * path = (char *)malloc(40);
	PNM * letra = NULL;
	retorno = NULL;

	sprintf(path,"%s%c.ppm",prefixo,text[0]);
	
	retorno = lePNM(path);
	free(path);
	
	for( ;i < len ; i++)
	{
		if(isalnum(text[i]))
		{
			path = (char *)malloc(40);
			sprintf(path,"%s%c.ppm",prefixo,text[i]);
			
			letra = lePNM(path);
			free(path);
		}
		else letra = NULL;
		
		retorno = colaPNM(retorno,letra);
	}
	
	return retorno;
}

/*cria e inicializa uma estrutura Musica*/
Musica * createMusica()
{
	/*apontador para uma estrutura Musica*/
	Musica *musica = (Musica *)malloc(sizeof(Musica));

	musica->acordes = 0;
	musica->notas = 0;
	musica->compassos = 0;
		
	/*Inicializacao*/
//	musica->titulo = (char *)malloc(sizeof(char));
	musica->titulo = NULL;
	
//	musica->letra = (Notas *)malloc(sizeof(Notas));
	musica->letra = NULL;
	
	return musica;
}

/*Insere uma nota (char *) numa estrutura Musica*/
int insertNota(Musica *musica, char *nota, int pausa)
{
	if(checkNota(nota) == EXIT_FAILURE)
		return EXIT_FAILURE;
				

	Notas *notas , *notas_temp;
	notas_temp = notas = (Notas *) malloc(sizeof(Notas));

	/*notas_temp passa apontar para music_temp->notas*/
	notas_temp =  musica->letra;

	/*preenchemos os campos da nova nota*/
	notas->nota = strdup(nota);
	notas->pausa = pausa;
	notas->letra = NULL;
	notas->next = NULL;

	/*Se a estrutura Musica ainda nao tem nenhuma nota guardada,
	* adicionamo-la à cabeca */
	if(notas_temp == NULL)
	{
		musica->letra = notas; //_temp;
	}
	else
	{
		/*posicionar o apontador na ultima posicao*/
		for( ; notas_temp->next != NULL; notas_temp = notas_temp->next) ;
		
		/*como ja esta na ultima posicao, pomo-lo a apontar para a nova "notas"*/
		notas_temp->next = notas;
	}
	
	return EXIT_SUCCESS;
}

/*associa uma "letra" de uma musica a uma nota*/
int insertLetra(Musica * musica, char * letra)
{
	Notas *notas_temp = musica->letra;
	
	if(notas_temp == NULL )
		return EXIT_FAILURE;
	else
	{
		/*posicionar o apontador na ultima posicao*/
		for( ; notas_temp->next != NULL; notas_temp = notas_temp->next) ;
		
		if(letra != NULL)
		{
			notas_temp->letra = strdup(letra);
		}
	}
	
	return EXIT_SUCCESS;
}


/*verifica se str é uma nota ou nao*/
int checkNota(char * str)
{
	if(strcmp(str, Do) == 0)
		return EXIT_SUCCESS;
	else if(strcmp(str, Do_s) == 0)
		return EXIT_SUCCESS;
	else if(strcmp(str, Do_m) == 0)
		return EXIT_SUCCESS;
	else if(strcmp(str, Re) == 0)
		return EXIT_SUCCESS;
	else if(strcmp(str, Re_s) == 0)
		return EXIT_SUCCESS;
	else if(strcmp(str, Re_b) == 0)
		return EXIT_SUCCESS;
	else if(strcmp(str, Mi) == 0)
		return EXIT_SUCCESS;
	else if(strcmp(str, Mi_b) == 0)
		return EXIT_SUCCESS;
	else if(strcmp(str, Fa) == 0)
		return EXIT_SUCCESS;
	else if(strcmp(str, Fa_s) == 0)
		return EXIT_SUCCESS;
	else if(strcmp(str, Sol) == 0)
		return EXIT_SUCCESS;
	else if(strcmp(str, Sol_b) == 0)
		return EXIT_SUCCESS;
	else if(strcmp(str, Sol_s) == 0)
		return EXIT_SUCCESS;
	else if(strcmp(str, La) == 0)
		return EXIT_SUCCESS;
	else if(strcmp(str, La_s) == 0)
		return EXIT_SUCCESS;
	else if(strcmp(str, La_b) == 0)
		return EXIT_SUCCESS;
	else if(strcmp(str, Si) == 0)
		return EXIT_SUCCESS;
	else if(strcmp(str, Si_b) == 0)
		return EXIT_SUCCESS;
	else if(strcmp(str, Comp) == 0)
		return EXIT_SUCCESS;
	else if(strcmp(str, Pausa) == 0)
		return EXIT_SUCCESS;
	else if(strcmp(str, Rep) == 0)
		return EXIT_SUCCESS;
	else if(strcmp(str, acordS) == 0)
		return EXIT_SUCCESS;
	else if(strcmp(str, acordE) == 0)
		return EXIT_SUCCESS;
	else 
	{
		return EXIT_FAILURE;
	}
}


/*Dado um ficheiro com as notas da musica e uma Estrutura Musica, o conteudo
* do ficheiro é armazenado na estrutura*/
int readMusic(char * path, Musica * musica)
{

	FILE * file = NULL;
	file = fopen(path,"r");
	
	if(file == NULL)
		return EF_NULL;
	
	char ch = fgetc(file);
	int i = 0, j = 0, pausa = 0;
	
	char * notas = (char *)malloc(sizeof(char)*10);
	notas[0] = '\0';

	char * letra = NULL;(char *)malloc(sizeof(char)*10);	
	
	/*vamos ler o titulo, partimos do principio que o titulo
	* nao tem mais que 100caracteres*/
	musica->titulo = (char *) realloc(musica->titulo,sizeof(char)*100);
	
	/*leitura do titulo*/
	for(i = 0; ch != '\n' ; i++, ch = fgetc(file))
		musica->titulo[i] = ch;
	musica->titulo[i] = '\0';
	
	
	/*vamos ler a letra da musica*/
	i = 0;
	ch = '\a';
	
	/*cada nova linha representa um compasso novo, isto até ao final do file*/
	while(!feof(file))
	{
	
		ch = fgetc(file);
		
		/*nova nota com sua pausa*/
		if(ch == '$')
		{
			/*reinicializar valores*/	
			if( strlen(notas) > 0)
			{
				free(notas);
				notas = (char *)malloc(sizeof(char)*10);
			}
			
			i = 0;
			pausa = 0;
			notas[i] = '\0';
		
			ch = fgetc(file);
			
			/*ler a nota*/
			for(; ch != ','; ch = fgetc(file))
			{
				if(! isspace(ch))
				{
					notas[i] = toupper(ch);
					i++;
				}
			}
			
			notas[i] = '\0';
			
			/*ler a pausa*/
			while(isdigit(ch) == 0)
				ch = fgetc(file);
			
			pausa = ch - 48;
			
			/*ler o resto*/
			while(ch != '%')
				ch = fgetc(file);
			
			
			/*adicionar a nota e a pausa*/
			if( insertNota(musica,notas,pausa) == EXIT_FAILURE)
				return EXIT_FAILURE;
			
			musica->notas++;
			
		}
		/*novo acorde*/
		if(ch == '(')
		{
			/*insercao de inicio de acorde*/
			if( insertNota(musica,acordS, 0) == EXIT_FAILURE)
				return EXIT_FAILURE;
			
			musica->acordes++;
		} 		
		/*fim acorde*/
		else if(ch == ')')
		{
			if( insertNota(musica,acordE, 0) == EXIT_FAILURE)
				return EXIT_FAILURE;
		} 		
		/*novo compasso*/
		else if(ch == '\n')
		{
			if( insertNota(musica,Comp, 0)  == EXIT_FAILURE)
				return EXIT_FAILURE;
			musica->compassos++;
		}else if(ch == '<')
		{
			j = 0;
			if(letra != NULL)
			{
				free(letra);
			}
			
			letra = (char *) malloc(mem);
			
			for(j = 0 ; (ch = fgetc(file)) != '>' ; j++)
			{
				if(j%mem == 0) letra = 
					(char *)realloc(letra,((j/mem)+1)*mem);
				letra[j] = ch;
			}
			
			letra[j] = '\0';
			
			if(letra != NULL)
			{
				insertLetra(musica,letra);
				free(letra);
				letra = NULL;
			}	
		}
	}
	
	fclose(file);
	
	return EXIT_SUCCESS;
}


/*calcula o numero de segmentos de uma pauta que serão necessarios criar 
* para que se possam colocar todas as notas na pauta*/
int calcula_n_pauta(Musica *musica)
{
	int retorno = 0;
	int resto = 0;
	int ret_aux = 0;

	retorno+=(musica->acordes*(x_nota<<1));
	retorno+=(musica->notas*(x_nota<<1));
	retorno+=(musica->compassos*x_compasso);
		
	if( (resto = retorno%comprimento) > 0)
		ret_aux = (retorno/comprimento) + 1;
	else 
		ret_aux = (retorno/comprimento);
	
	retorno += ret_aux * inicial_spc;
		
	if( (resto = retorno%comprimento) > 0)
		retorno = (retorno/comprimento) + 1;
	else 
		retorno = (retorno/comprimento);
		
	return retorno;
}

/*cria uma pauta (sem as notas, somente linhas) no formato P6*/
PNM * createPauta(Musica *musica,int mul_altura) 
{
	int lh = 0, cl = 0, colunas = 0, i = 0, linhas = 0;
	int branco = pixeis-1;
	int preto = 0;
	int topo = 2*dbl;
	int fundo = altura-2*dbl;
	int limInf = 0;
	int limSup = 0;
	int segmento = altura;
	
	/*criacao do PNM pauta, alocacao de memoria e inicializacao*/
	PNM *pauta = createEPNM();
	
	/*identificacao do tipo do PNM*/
	pauta->tipo = strdup(P6);
	
	/*insercao do titulo da musica como comentario do PNM (Pauta)*/
	insertComment(pauta->comentarios,musica->titulo);
	
	/*numero de colunas e linhas do PNM*/
	pauta->colunas = comprimento;
	linhas = pauta->linhas = altura*mul_altura;
	
	/*cor maxima do PNM*/
	pauta->max_color = pixeis-1;
	
	/*por ser um P6*/
	colunas = comprimento*3;
	
	pauta->rep = (int **)realloc(pauta->rep,sizeof(int *)*linhas);
	
	/*carregamento da imagem representativa da clave de sol*/
	PNM *clave = lePNM(clave_de_sol_ppm);
	
	for(i = 0; i < linhas; i+=altura)
	{
		topo=i+2*dbl;
		fundo=i+(altura-2*dbl);
		segmento=altura+i;
		limInf = topo;
		limSup = topo+ espl;
		
		/*cria o topo da pauta: 20linhas brancas*/
		for(lh = i; lh < topo; lh++)
		{
			pauta->rep[lh] = (int *)malloc(sizeof(int)*colunas);
		
			for(cl = 0; cl < colunas; cl++)
			{
				pauta->rep[lh][cl] = branco;
			}
		}
	
	
		/*cria as linhas da pauta*/
		for(lh = topo; lh < fundo; lh++)
		{
			pauta->rep[lh] = (int *)malloc(sizeof(int)*colunas);
		
			for(cl = 0; cl < colunas; cl++)
			{
				if(lh >= limInf && lh <= limSup)
				{
					if(lh+1 > limSup)
					{
						limInf+=dbl+espl;
						limSup+=dbl+espl;
					
						/*a comparacao é feita ja na coluna 0 
						* da linha que deve de ser branca*/
						pauta->rep[lh][cl] = branco;
					}
					else
						pauta->rep[lh][cl] = preto;
				}
				else
					pauta->rep[lh][cl] = branco;
			}
		}
	
		/*cria o fundo da pauta: 20linhas brancas*/
		for(lh = fundo; lh < segmento; lh++)
		{
			pauta->rep[lh] = (int *)malloc(sizeof(int)*colunas);
			
			for(cl = 0; cl < colunas; cl++)
			{
				pauta->rep[lh][cl] = branco;
			}
		}
	}
	
	for(lh = 0; lh < mul_altura; lh++)
		insereCSol(clave,pauta,altura*lh);
		
	if(clave != NULL)
		free(clave);

	
	return pauta;
}

/*Uma pauta por defeito é escrita no format P6 ou P3, dai que, apenas
* tratemos a troca de cores para este formato*/
int swapColor(PNM * pnm, int oldcolor, int borda, int *newcolor)
{
	int lh = 0, cl = 0, linhas = 0, colunas = 0;
	
	colunas = pnm->colunas * 3;
	linhas = pnm->linhas;
		
	for(lh = 0; lh < linhas; lh++)
		for(cl = 0; cl < colunas; cl+=3)
			if(
				(pnm->rep[lh][cl] != oldcolor || 
				pnm->rep[lh][cl+1] != oldcolor || 
				pnm->rep[lh][cl+2] != oldcolor) 
				&&
				(pnm->rep[lh][cl] != borda ||
				pnm->rep[lh][cl+1] != borda|| 
				pnm->rep[lh][cl+2] != borda)
				)
			{
				
				pnm->rep[lh][cl] = newcolor[0];
				pnm->rep[lh][cl+1] = newcolor[1];
				pnm->rep[lh][cl+2] = newcolor[2];
			}
				
	return EXIT_SUCCESS;
}

/*funcao que dada uma nota (PNM), uma pauta (PNM), introduz a nota na pauta,
* sobrepondo a nota à pauta na posicao correcta.
* y_nota é a posicao onde a nota vai ficar e nao a coordenada; a coordenada
* é dada por (y_nota -(x_nota/2))*/
int sbrPNM(PNM *nota, PNM * pauta,int y_nota , int xx_nota)
{
	int branco = pixeis-1;
	
	int cl = xx_nota*3, cl_n = 0, lh = y_nota-nota->linhas+x_nota/2, lh_n = 0;
	int linhas = lh + nota->linhas,//x_nota,
	    linhas_n = nota->linhas,
            colunas_n = nota->colunas*3,
	    colunas = cl + colunas_n,
	    colunas_p = pauta->colunas*3;
	
	if( 
		(colunas < colunas_p) 
		&& (cl >= 0)
		&& (lh >= 0)
		&& (linhas < pauta->linhas)
	  )
	{
		for(lh_n = 0, lh;
			 lh_n < linhas_n && lh < linhas ;
			 lh++, lh_n++
		)
		{
			for(cl_n = 0, cl = xx_nota*3; 
				cl_n < colunas_n && cl < colunas ; 
				cl+=3, cl_n+=3
			)
			{
				if(nota->rep[lh_n][cl_n] != branco ||
					 nota->rep[lh_n][cl_n+1] != branco ||
					  nota->rep[lh_n][cl_n+2] != branco
				)
				{
					pauta->rep[lh][cl] = nota->rep[lh_n][cl_n];
					pauta->rep[lh][cl+1] = nota->rep[lh_n][cl_n+1];
					pauta->rep[lh][cl+2] = nota->rep[lh_n][cl_n+2];
				}
			}
		}
		
	}
	else
		return EXIT_FAILURE;
		
	return EXIT_SUCCESS;
}

/*insere a clave de sol (PNM) na pauta (PNM)*/
int insereCSol(PNM *clave, PNM *pauta, int y_inic)
{
	int lh = 0, lh1 = y_inic, cl = 0, linhas = 0, colunas =0;
	
	int branco = pixeis-1;
	
	linhas = clave->linhas;
	colunas = clave->colunas*3;
		
	for(lh1 = y_inic, lh = 0; lh < linhas; lh++, lh1++)
	{
		for(cl = 0; cl < colunas; cl++)
		{
			if(clave->rep[lh][cl] != branco)
				pauta->rep[lh1][cl] = clave->rep[lh][cl];
				
		}
	}
	
	return EXIT_SUCCESS;
}

/*Insere na pauta uma linha de separacao de compassos e/ou a linha 
* suplementar do Do.
* PNM onde a alterar, coordenadas x e y de inicializacao e finalizacao
* de insercao do segmento de recta*/
int insereSegLinha(PNM *pauta,int yy_start, int yy_end, int xx_start, int xx_end)
{
	int preto = 0, branco = pixeis-1;;
	
	int lh = 0, cl = 0;
		
	for( lh = yy_start ; lh < yy_end; lh++)
		for( cl = xx_start; cl < xx_end; cl+=3)
			if(pauta->rep[lh][cl] == branco && 
				pauta->rep[lh][cl+1] == branco && 
				pauta->rep[lh][cl+2] == branco
			)
			{
				pauta->rep[lh][cl] = preto;
				pauta->rep[lh][cl+1] = preto;
				pauta->rep[lh][cl+2] = preto;
			}
	
	return EXIT_SUCCESS;
}

/*funcao utilizada em criaPMusical para colocar os objectos 
* (linhas, notas, etc) nas pautas*/
int sbr(PNM * objecto,
	PNM * pauta,
	int yy_nota,
	int xx_inc,
	int *xx_nota,
	int *i,
	int _do)
/*(objecto a inserir, inserir em..., altura do objecto, incrmento da posicao x,
incremento em y, linha extra para o do)*/
{
	if(sbrPNM(objecto, pauta, yy_nota, *xx_nota) == EXIT_SUCCESS)
	{
		/*caso em que o do necessita de uma linha extra*/
		if(_do == 1)
		{
			
			insereSegLinha(
				pauta,
				yy_nota - 1,
				yy_nota+ espl-1,
				(*xx_nota)*3-x_nota,
				(*xx_nota)*3+(x_nota<<2) /* + x_nota*4*/
				);
		}
		
		*xx_nota += (xx_inc<<1); /*+ xx_inc*2*/
	}
	else
	{
		*i+=altura;
		*xx_nota = inicial_spc;
		
		if(_do == 1)
		{
			insereSegLinha(
				pauta,
				yy_nota + altura - 1,
				yy_nota + altura + espl-1,
				(*xx_nota)*3-x_nota,
				(*xx_nota)*3+(x_nota<<2) /* + x_nota*4*/
				);
		}
		
		
		if(sbrPNM(objecto, pauta, yy_nota+altura, *xx_nota) != EXIT_SUCCESS)
		{
			return EXIT_FAILURE;
		}
		
		*xx_nota += (xx_inc<<1); /*+ xx_inc*2*/
	}
	
	return EXIT_SUCCESS;
}

/*funcao que determina a nota a inserir e actualiza o yy_nota, o xx_inc e
* ainda troca a cor da nota*/
int detNota(PNM * nota, char * objecto, int *yy_nota,
	 int *xx_inc, int acorde, int i, int *_do)
{
	
	*_do = 0;

	if(strcmp(objecto,Do) == 0)
	{
		*yy_nota = yDo+i;
		*xx_inc = x_nota*acorde;
		swapColor(nota,255,0,hexa_2_dec(cDo));
		*_do = 1;
	}
	else if (strcmp(objecto,Do_s) == 0)
	{
		*yy_nota = yDo_s+i;
		*xx_inc = x_nota*acorde;
		swapColor(nota,255,0,hexa_2_dec(cDo_s));
	}
	else if (strcmp(objecto,Re) == 0)
	{
		*yy_nota = yRe+i;
		*xx_inc = x_nota*acorde;
		swapColor(nota,255,0,hexa_2_dec(cRe));
	}
	else if (strcmp(objecto,Re_s) == 0)
	{
		*yy_nota = yRe_s+i;
		*xx_inc = x_nota*acorde;
		swapColor(nota,255,0,hexa_2_dec(cRe_s));
	}
	else if (strcmp(objecto,Mi) == 0)
	{
		*yy_nota = yMi+i;
		*xx_inc = x_nota*acorde;
		swapColor(nota,255,0,hexa_2_dec(cMi));
	}
	else if (strcmp(objecto,Fa) == 0)
	{
		*yy_nota = yFa+i;
		*xx_inc = x_nota*acorde;
		swapColor(nota,255,0,hexa_2_dec(cFa));
	}
	else if (strcmp(objecto,Fa_s) == 0)
	{
		*yy_nota = yFa_s+i;
		*xx_inc = x_nota*acorde;
		swapColor(nota,255,0,hexa_2_dec(cFa_s));
	}
	else if (strcmp(objecto,Sol) == 0)
	{
		*yy_nota = ySol+i;
		*xx_inc = x_nota*acorde;
		swapColor(nota,255,0,hexa_2_dec(cSol));
	}
	else if (strcmp(objecto,Sol_s) == 0)
	{
		*yy_nota = ySol_s+i;
		*xx_inc = x_nota*acorde;
		swapColor(nota,255,0,hexa_2_dec(cSol_s));
	}
	else if (strcmp(objecto,La) == 0)
	{
		*yy_nota = yLa+i;
		*xx_inc = x_nota*acorde;
		swapColor(nota,255,0,hexa_2_dec(cLa));
	}
	else if (strcmp(objecto,La_s) == 0)
	{
		*yy_nota = yLa_s+i;
		*xx_inc = x_nota*acorde;
		swapColor(nota,255,0,hexa_2_dec(cLa_s));
	}
	else if (strcmp(objecto,Si) == 0)
	{
		*yy_nota = ySi+i;
		*xx_inc = x_nota*acorde;
		swapColor(nota,255,0,hexa_2_dec(cSi));
	}
	else if (strcmp(objecto,Do_m) == 0)
	{
		*yy_nota = yDo_m+i;
		*xx_inc = x_nota*acorde;
		swapColor(nota,255,0,hexa_2_dec(cDo_m));
	}
	else if (strcmp(objecto,Re_b) == 0)
	{
		*yy_nota = yRe_b+i;
		swapColor(nota,255,0,hexa_2_dec(cRe_b));
		*xx_inc = x_nota*acorde;
	}
	else if (strcmp(objecto,Mi_b) == 0)
	{
		*yy_nota = yMi_b+i;
		*xx_inc = x_nota*acorde;
		swapColor(nota,255,0,hexa_2_dec(cMi_b));
	}
	else if (strcmp(objecto,Sol_b) == 0)
	{
		*yy_nota = ySol_b+i;
		*xx_inc = x_nota*acorde;
		swapColor(nota,255,0,hexa_2_dec(cSol_b));
	}
	else if (strcmp(objecto,La_b) == 0)
	{
		*yy_nota = yLa_b+i;
		*xx_inc = x_nota*acorde;
		swapColor(nota,255,0,hexa_2_dec(cLa_b));
	}
	else if (strcmp(objecto,Si_b) == 0)
	{
		*yy_nota = ySi_b+i;
		*xx_inc = x_nota*acorde;
		swapColor(nota,255,0,hexa_2_dec(cSi_b));
	}
	
	return EXIT_SUCCESS;
}


/*cria a pauta musical referente à musica "path"*/
PNM * criaPMusical(char *path)
{
	int yy_nota = 0;
	int xx_nota = inicial_spc, xx_inc = 0, _do = 0, xx_nota_ant = 0;
	int i = 0;
	char * objecto = strdup("vazio");
	char * textoM;
	int temp = 0, temp2 = 0;

	Musica *musica = createMusica();
	
	int control = readMusic(path,musica);
	
	if(control == EXIT_FAILURE || control == EF_NULL)
		return NULL;
	
	int n_seg = calcula_n_pauta(musica);
	
	/*criacao da pauta musical*/
	PNM *pauta = createPauta(musica,n_seg);
	
	Notas *notas = musica->letra;
	/*carregamento da imagem representativa de uma nota musical*/
	PNM *semibreve = lePNM(semibreve_path_ppm);
	PNM *minima = lePNM(minima_path_ppm);
	PNM *colcheia = lePNM(colcheia_path_ppm);
	PNM *semicolcheia = lePNM(semicolcheia_path_ppm);
	PNM *seminima = lePNM(seminima_path_ppm);
	PNM *fusa = lePNM(fusa_path_ppm);
	
	
	/*carregamento da imagem representativa de uma pausa musical*/
	PNM *p_semibreve = lePNM(p_semibreve_path_ppm);
	PNM *p_minima = lePNM(p_minima_path_ppm);
	PNM *p_colcheia = lePNM(p_colcheia_path_ppm);
	PNM *p_semicolcheia = lePNM(p_semicolcheia_path_ppm);
	PNM *p_seminima = lePNM(p_seminima_path_ppm);
	PNM *p_fusa = lePNM(p_fusa_path_ppm);
	
	
	PNM **pausas = (PNM **) malloc(sizeof(PNM *) * 6);
	PNM **notas_ = (PNM **) malloc(sizeof(PNM *) * 6);
	
	
	pausas[0] = p_semibreve;
	pausas[1] = p_minima;
	pausas[2] = p_colcheia;
	pausas[3] = p_semicolcheia;
	pausas[4] = p_seminima;
	pausas[5] = p_fusa;
	
	
	notas_[0] = semibreve;
	notas_[1] = minima;
	notas_[2] = colcheia;
	notas_[3] = semicolcheia;
	notas_[4] = seminima;
	notas_[5] = fusa;
	
	/*carregamento da imagem representativa de uma repeticao musical*/
	PNM *repeticao = lePNM(repeticao_ppm);
	
	/*texto a inserir na pauta*/
	PNM *texto = criaTexto(musica->titulo);
	
	/*insercao do titulo da musica na imagem*/
	temp = (pauta->colunas/2) - (texto->colunas/2);
	
	if(texto != NULL)
		sbr(texto,pauta,x_nota,x_nota,&temp,&temp2,0);


	/*preenchimento da pauta com notas, repeticoes, pausas e afins*/	
	for(;notas != NULL; notas = notas->next)
	{
		free(objecto);
		objecto = strdup(notas->nota);
		textoM = notas->letra;
		
		
		/* caso objecto seja uma pausa */
		if( strcmp(objecto, Pausa) == 0 )
		{
			if(notas->pausa >= 1 && notas->pausa <= 6)
				sbr(pausas[notas->pausa-1],pauta,y_pausa+i,x_nota,&xx_nota,&i,0);
			
		}
		/* caso objecto seja um novo compasso */
		else if(strcmp(objecto, Comp) == 0)
		{
			/*insercao da linha divisoria de um compasso*/
			insereSegLinha(pauta,i+2*dbl,i+altura-3*dbl,
				(xx_nota-10)*3,(xx_nota-10)*3+(espCl*3));
			/*actualizacao do xx_nota*/
		}
		/* caso objecto seja um conjunto de acordes */
		else if( strcmp(objecto,acordS) == 0 )
		{
			for(notas = notas->next ; strcmp(notas->nota,acordE) != 0 ; notas = notas->next)
			{
				/*determina qual a nota sem actualizar xx_nota, 
				* tb pinta a nota com a sua respectiva cor*/
				if(notas->pausa >= 1 && notas->pausa <= 6)
				{
				    detNota(notas_[notas->pausa-1], notas->nota,&yy_nota,&xx_inc,0,i,&_do);
				    /*insere a nota no sitio correspondente*/
							
				    sbr(notas_[notas->pausa-1],pauta,yy_nota,xx_inc,&xx_nota,&i,_do);
				}
				
				textoM = notas->letra;
				/*adiciona o texto*/
				if(textoM != NULL)
				{
					temp2 = i;
					temp = yText+i;
					
					texto = criaTexto(textoM);
					temp = texto->colunas/2;
					xx_nota_ant = xx_nota + 9 - temp;
					sbr(texto, pauta, yText+i ,xx_inc,&xx_nota_ant,&temp2,0);
				}
			}
			
			xx_nota+=x_nota<<1;
		}
		/* caso objecto seja uma repeticao */
		else if( strcmp(objecto, Rep) == 0 )
		{
			/*insere o sinal de repeticao */
			xx_inc = 0;
			sbr(repeticao,pauta,yrep1+i,xx_inc,&xx_nota,&i,0);
			sbr(repeticao,pauta,yrep2+i,xx_inc,&xx_nota,&i,0);
			xx_nota += x_nota<<1;
		}
		/* se apenas for uma nota */
		else
		{
			/*determina qual a nota actualizando xx_nota, 
			* tb pinta a nota com a sua respectiva cor*/
			if(notas->pausa >= 1 && notas->pausa <= 6)
        			detNota(notas_[notas->pausa-1], notas->nota,&yy_nota,&xx_inc,1,i,&_do);
			/*insere a nota no sitio correspondente*/

			if(textoM != NULL)
			{
				if(notas->pausa >= 1 && notas->pausa <= 6)
					sbr(notas_[notas->pausa-1],pauta,yy_nota,0,&xx_nota,&i,_do);
				texto = criaTexto(textoM);
				
				temp = texto->colunas/2;
				
				xx_nota_ant = xx_nota + 9 - temp;
				sbr(texto, pauta, yText+i,0,&xx_nota_ant,&i,0);
				
				xx_nota+=xx_inc<<1;
			}
			else
			{
				if(notas->pausa >= 1 && notas->pausa <= 6)
					sbr(notas_[notas->pausa-1],pauta,yy_nota,xx_inc,&xx_nota,&i,_do);
			}
			
			if(textoM != NULL)
			{
				if(texto != NULL) free(texto);
				texto = NULL;
			
			}
		}
		
	}
	/*fim do for*/
	

	freeMusica(musica);
	if(texto != NULL) free(texto);
	
	freePNM(p_semibreve);
	freePNM(p_seminima);
	freePNM(p_minima);
	freePNM(p_colcheia);
	freePNM(p_semicolcheia);
	freePNM(p_fusa);
	
	freePNM(semibreve);
	freePNM(seminima);
	freePNM(minima);
	freePNM(colcheia);
	freePNM(semicolcheia);
	freePNM(fusa);
	
	free(notas_);
	free(pausas);
	
	freePNM(repeticao);
	
	return pauta;
}
