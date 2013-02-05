#include "pnm_manipulation.h"

/*devolve uma copia de um PNM*/
PNM * copia(PNM *pnm)
{
	int lh = 0, cl = 0, colunas = 0, linhas = 0;
	PNM *temp;
	temp = (PNM *)malloc(sizeof(PNM));
	
	inicPNM(temp);
	
	
	temp->tipo = strdup(pnm->tipo);
	temp->colunas = pnm->colunas;
	linhas = temp->linhas = pnm->linhas;
	temp->max_color = pnm->max_color;
	
	if(strcmp(temp->tipo,P3) || strcmp(temp->tipo,P6))
		colunas = (temp->colunas)*3;
	else
		colunas = (temp->colunas);
		
	while(pnm->comentarios[lh]!=NULL)
	{
		temp->comentarios = (char **) realloc(temp->comentarios,
							sizeof(char *)*(lh+1));
							
		temp->comentarios[lh] = strdup(pnm->comentarios[lh]);
		lh++;
		
	}
	
	temp->comentarios[lh] = NULL;
	
	temp->rep = (int **)malloc(sizeof(int *) * linhas);
	
	for(lh = 0; lh < linhas; lh++)
	{
		temp->rep[lh] = (int *)malloc(sizeof(int)*colunas);
		
		for(cl = 0; cl < colunas; cl++)
			temp->rep[lh][cl]=pnm->rep[lh][cl];
	}
	
	return temp;
}

/*devolve uma copia da matriz de comentarios de um PNM*/
char ** copiaComments(char ** comments)
{
	int lh = 0;
	
	char **temp = (char **)malloc(sizeof(char*));
	while(comments[lh]!=NULL)
	{
		temp = (char **) realloc(temp, sizeof(char *) *(lh+1));
							
		temp[lh] = strdup(comments[lh]);
		lh++;
	}
	temp = (char **) realloc(temp, sizeof(char *) *(lh+1));
	temp[lh] = NULL;
	
	return temp;
}


/*devolve uma copia da matriz de pixeis de um PNM*/
int ** copiaRep(int ** rep, int mul_col, int colunas, int linhas)
{
	int lh = 0, cl = 0;
	
	int **temp;
	
	temp = (int **)malloc(sizeof(int *) * linhas);
	for(lh = 0; lh < linhas; lh++)
		for(cl = 0; cl < colunas; cl++)
			temp[lh][cl] = rep[lh][cl];
	
	return temp;
}







/*----------------------------converte----------------------------*/
PNM *converte(PNM *pnm, char*tipoDes)
{

	char tipoOrig = pnm->tipo[1], tipoDest;
	
	PNM *temp = (PNM *)malloc(sizeof(PNM));
	
	if(strlen(tipoDes) == 2) tipoDest = tipoDes[1];
	else return NULL;
	
	
	switch(tipoDest)
	{
		case '1':
			switch(tipoOrig)
			{
				case '1': temp = copia(pnm);
					break;
				case '2': temp = p2_2_p1(pnm);
					break;
				case '3': temp = p3_2_p1(pnm);
					break;
				case '4': temp = p4_2_p1(pnm);		
					break;
				case '5': temp = p5_2_p1(pnm);		
					break;
				case '6': temp = p6_2_p1(pnm);			
					break;
				default: return NULL;
			}
		
			break;
		case '2':
			switch(tipoOrig)
			{
				case '1': temp = p1_2_p2(pnm);
					break;
				case '2': temp = copia(pnm);
					break;
				case '3': temp = p3_2_p2(pnm);
					break;
				case '4': temp = p4_2_p2(pnm);		
					break;
				case '5': temp = p5_2_p2(pnm);		
					break;
				case '6': temp = p6_2_p2(pnm);			
					break;
				default: return NULL;
			}
			break;
		case '3':
			switch(tipoOrig)
			{
				case '1': temp = p1_2_p3(pnm);
					break;
				case '2': temp = p2_2_p3(pnm);
					break;
				case '3': temp = copia(pnm);		
					break;
				case '4': temp = p4_2_p3(pnm);		
					break;
				case '5': temp = p5_2_p3(pnm);			
					break;
				case '6': temp = p6_2_p3(pnm);			
					break;
				default: return NULL;
			}
			break;
		case '4':
			switch(tipoOrig)
			{
				case '1': temp = p1_2_p4(pnm);		
					break;
				case '2': temp = p2_2_p4(pnm);			
					break;
				case '3': temp = p3_2_p4(pnm);		
					break;
				case '4': temp = copia(pnm);		
					break;
				case '5': temp = p5_2_p4(pnm);		
					break;
				case '6': temp = p6_2_p4(pnm);			
					break;
				default: return NULL;
			}
			break;
		case '5':
			switch(tipoOrig)
			{
				case '1': temp = p1_2_p5(pnm);		
					break;
				case '2': temp = p2_2_p5(pnm);			
					break;
				case '3': temp = p3_2_p5(pnm);
					break;
				case '4': temp = p4_2_p5(pnm);
					break;
				case '5': temp = copia(pnm);		
					break;
				case '6': temp = p6_2_p5(pnm);	
					break;
				default: return NULL;
			}
			break;
		case '6':
			switch(tipoOrig)
			{
				case '1': temp = p1_2_p6(pnm);		
					break;
				case '2': temp = p2_2_p6(pnm);			
					break;
				case '3': temp = p3_2_p6(pnm);
					break;
				case '4': temp = p4_2_p6(pnm);		
					break;
				case '5': temp = p5_2_p6(pnm);		
					break;
				case '6': temp = copia(pnm);			
					break;
				default: return NULL;
			}
			break;
		default: return NULL;
	}
	
	if(pnm != NULL)
	{
		freePNM(pnm);
		pnm = NULL;
	}
	
	return temp;
}






/* ____________________________de P1 para ... ______________________________*/
	
/*converte um PNM do formato P1 para o formato P4*/
PNM * p1_2_p4(PNM *pnm)
{
	PNM *temp = copia(pnm);
	
	temp->tipo[1] = '4';
	
	return temp;
}

/*converte um PNM do formato P1 para o formato P2*/
PNM * p1_2_p2(PNM *pnm)
{
	PNM * temp = (PNM *)malloc(sizeof(PNM));
	int lh = 0, cl = 0, colunas = 0, linhas = 0, max_color = 0;
	
	temp->tipo = strdup(pnm->tipo);
	temp->tipo[1] = '2';
	
	colunas = pnm->colunas;
	linhas = pnm->linhas;
	
	temp->colunas = colunas;
	temp->linhas = linhas;
	
	temp->comentarios = copiaComments(pnm->comentarios);
	
	temp->rep = (int **)malloc(sizeof(int *)*linhas);
	
	for(lh = 0; lh < linhas; lh++)
	{
		temp->rep[lh] = (int *)malloc(sizeof(int)*colunas);
		
		for(cl = 0; cl < colunas; cl++)
		{
			if(pnm->rep[lh][cl] == 0) 
			{
				temp->rep[lh][cl] = 255;
				max_color = 255;
			}
			else temp->rep[lh][cl] = 0;
		}
	}
	temp->max_color = max_color;
	
	return temp;
}

/*converte um PNM do formato P1 para o formato P5*/
PNM * p1_2_p5(PNM *pnm)
{
	PNM *temp = p1_2_p2(pnm);
	temp->tipo[1] = '5';
	
	return temp;
}

/*converte um PNM do formato P1 para o formato P3*/
PNM * p1_2_p3(PNM *pnm)
{
	PNM * temp = (PNM *)malloc(sizeof(PNM));
	
	int lh = 0, cl = 0, colunas = 0, linhas = 0, color = 0, max_color = 0,
		cl_p4 = 0;
	
	temp->tipo = strdup(pnm->tipo);
	temp->tipo[1] = '3';
	
	colunas = pnm->colunas;
	linhas = pnm->linhas;
	
	temp->colunas = colunas;
	temp->linhas = linhas;
	
	temp->comentarios = copiaComments(pnm->comentarios);
	
	colunas*=3;
	
	temp->rep = (int **)malloc(sizeof(int *)*linhas);
	
	for(lh = 0; lh < linhas; lh++)
	{
		temp->rep[lh] = (int *)malloc(sizeof(int)*colunas);
		for(cl = 0, cl_p4 = 0; cl < colunas; cl+=3, cl_p4++)
		{
			color = pnm->rep[lh][cl_p4];
			if(color == 0)
				color = max_color =  255;
			temp->rep[lh][cl] = color;
			temp->rep[lh][cl+1] = color;
			temp->rep[lh][cl+2] = color;
		}
	}
	
	temp->max_color = max_color;
	
	return temp;
}

/*converte um PNM do formato P1 para o formato P6*/
PNM * p1_2_p6(PNM *pnm)
{
	PNM *temp = p1_2_p3(pnm);
	temp->tipo[1] = '6';
	
	return temp;
}




/*_______________________________de P2 para ...______________________________*/

/*converte um PNM do formato P4 para o formato P1*/
PNM * p2_2_p5(PNM *pnm)
{
	PNM *temp = copia(pnm);
	
	temp->tipo[1] = '5';
	
	return temp;
}

/*converte de P2 para P1*/
PNM * p2_2_p1(PNM * pnm)
{

	PNM * temp = (PNM *)malloc(sizeof(PNM));
	
	int lh = 0, cl = 0, colunas = 0, linhas = 0, resto = 0, quoc = 0,
		 color = 0, pnm_pixel_meio = 0, pnm_maximo = 0;
	
	temp->tipo = strdup(pnm->tipo);
	temp->tipo[1] = '1';
	
	colunas = pnm->colunas;
	linhas = pnm->linhas;
	
	temp->colunas = colunas;
	temp->linhas = linhas;
	
	temp->comentarios = copiaComments(pnm->comentarios);
	
	temp->rep = (int **)malloc(sizeof(int *)*linhas);

	pnm_maximo = pnm->max_color;

	pnm_pixel_meio = pixeis/2;//pnm_maximo/2;

	for(lh = 0; lh < linhas; lh++)
	{
		temp->rep[lh] = (int *)malloc(sizeof(int)*colunas);
		
		for(cl = 0; cl < colunas; cl++)
		{
			color = pnm->rep[lh][cl];
			
			resto = color%1;
			quoc = color/1;
			
			//if(resto > pnm_pixel_meio) color = quoc+1;
			//else color = quoc;
			
			if(color > pnm_pixel_meio) color = 0;
			else color = 1;
			
			
			temp->rep[lh][cl] = color;
			
			
		}
	}

	return temp;
}

/*converte de P2 para o formato P4*/
PNM * p2_2_p4(PNM * pnm)
{
	PNM *temp = p2_2_p1(pnm);
	
	temp->tipo[1] = '4';
	
	return temp;
}

/*converte de P2 para P3*/
PNM * p2_2_p3(PNM *pnm)
{
	PNM * temp = (PNM *)malloc(sizeof(PNM));
	
	int lh = 0, cl = 0, colunas = 0, linhas = 0, color = 0, cl_p4 = 0,
		resto = 0, quoc = 0, pixel_meio = 0, branco = 0, max_color = 0,
		maximo = 0;
	
	temp->tipo = strdup(pnm->tipo);
	temp->tipo[1] = '3';
	
	colunas = pnm->colunas;
	linhas = pnm->linhas;
	
	temp->colunas = colunas;
	temp->linhas = linhas;
	
	temp->comentarios = copiaComments(pnm->comentarios);
	
	colunas*=3;
	
	temp->rep = (int **)malloc(sizeof(int *)*linhas);
	
	maximo = pnm->max_color;
		
	branco = pixeis-1;
	
	for(lh = 0; lh < linhas; lh++)
	{
		temp->rep[lh] = (int *)malloc(sizeof(int)*colunas);
		for(cl = 0, cl_p4 = 0; cl < colunas; cl+=3, cl_p4++)
		{
			color = pnm->rep[lh][cl_p4];
			
			resto = (color*branco)%maximo;
			quoc = (color*branco)/maximo;
			
			color = 0;
			
			if(resto > pixel_meio) color = quoc+1;
			else color = quoc;
			
			
			
			if(color > max_color) max_color = color;
			
			temp->rep[lh][cl] = color;
			temp->rep[lh][cl+1] = color;
			temp->rep[lh][cl+2] = color;
		}
	}
	
	temp->max_color = max_color;
	
	return temp;
}

/*converte de P2 para o formato P6*/
PNM * p2_2_p6(PNM *pnm)
{
	PNM *temp = p2_2_p3(pnm);
	
	temp->tipo[1] = '6';
	
	return temp;
}




/*______________________________de P3 para ... _____________________________*/

/*converte um PNM do formato P4 para o formato P1*/
PNM * p3_2_p6(PNM *pnm)
{
	PNM *temp = copia(pnm);
		
	temp->tipo[1] = '6';
	
	return temp;
}

/*converte de P3 para P1*/
PNM * p3_2_p1(PNM *pnm)
{
	PNM * temp = (PNM *)malloc(sizeof(PNM));
	
	int lh = 0, cl = 0, colunas = 0, linhas = 0, color = 0, cl_p3 = 0,
		resto = 0, quoc = 0, pixel_meio = 0;
		
	temp->tipo = strdup(pnm->tipo);
	temp->tipo[1] = '1';
	
	colunas = pnm->colunas;
	linhas = pnm->linhas;
	
	temp->colunas = colunas;
	temp->linhas = linhas;
	
	temp->comentarios = copiaComments(pnm->comentarios);
	
	pixel_meio = pixeis/2;
	
	temp->rep = (int **)malloc(sizeof(int *)*linhas);

	for(lh = 0; lh < linhas; lh++)
	{
		temp->rep[lh] = (int *)malloc(sizeof(int)*colunas);
		for(cl = 0, cl_p3 = 0; cl < colunas; cl++, cl_p3+=3)
		{
			color = pnm->rep[lh][cl_p3]
				+ pnm->rep[lh][cl_p3+1]
				+ pnm->rep[lh][cl_p3+2];
			
			resto = color%3;
			quoc = color/3;
			
			if(resto > pixel_meio) color = quoc+1;
			else color = quoc;
			
			if(color > pixel_meio) color = 0;
			else color = 1;
			
			temp->rep[lh][cl] = color;
		}
	}
	
	return temp;
}

/*converte de P3 para o formato P4*/
PNM * p3_2_p4(PNM *pnm)
{
	PNM *temp = p3_2_p1(pnm);
	
	temp->tipo[1] = '4';
	
	return temp;
}

/*converte de P3 para P2*/
PNM * p3_2_p2(PNM *pnm)
{
	PNM * temp = (PNM *)malloc(sizeof(PNM));
	
	int lh = 0, cl = 0, colunas = 0, linhas = 0, color = 0, cl_p3 = 0,
		resto = 0, quoc = 0, pixel_meio = 0, max_color = 0;
		
	temp->tipo = strdup(pnm->tipo);
	temp->tipo[1] = '2';
	
	colunas = pnm->colunas;
	linhas = pnm->linhas;
	
	temp->colunas = colunas;
	temp->linhas = linhas;
	
	temp->comentarios = copiaComments(pnm->comentarios);

	temp->rep = (int **)malloc(sizeof(int *)*linhas);

	for(lh = 0; lh < linhas; lh++)
	{
		temp->rep[lh] = (int *)malloc(sizeof(int)*colunas);
		for(cl = 0, cl_p3 = 0; cl < colunas; cl++, cl_p3+=3)
		{
			color = pnm->rep[lh][cl_p3]
				+ pnm->rep[lh][cl_p3+1]
				+ pnm->rep[lh][cl_p3+2];
			
			resto = color%3;
			quoc = color/3;
			
			if(resto > pixel_meio) color = quoc+1;
			else color = quoc;
			
			if(color > max_color) max_color = color;

			temp->rep[lh][cl] = color;
		}
	}
	
	temp->max_color = max_color;

	return temp;
}

/*converte de P3 para o formato P5*/
PNM * p3_2_p5(PNM *pnm)
{
	PNM *temp = p3_2_p2(pnm);
	
	temp->tipo[1] = '5';
	
	return temp;
}




/*________________________________de P4 para ... ____________________________*/

/*converte um PNM do formato P4 para o formato P1*/
PNM * p4_2_p1(PNM *pnm)
{
	PNM *temp = copia(pnm);
	
	temp->tipo[1] = '1';
	
	return temp;
}



/*______________________________de P5 para ... _____________________________*/

/*converte um PNM do formato P5 para o formato P2*/
PNM * p5_2_p2(PNM *pnm)
{
	PNM *temp = copia(pnm);
	
	temp->tipo[1] = '2';
	
	return temp;
}



/*______________________________de P6 para ... _____________________________*/

/*converte um PNM do formato P6 para o formato P3*/
PNM * p6_2_p3(PNM *pnm)
{
	PNM *temp = copia(pnm);
	
	temp->tipo[1] = '3';
		
	return temp;
	
}






/*___________________________Rodar 90º p/ direita___________________________*/

PNM * rodarD(PNM *pnm)
{
	PNM *temp = (PNM *)malloc(sizeof(PNM));
	
	int cl = 0, lh = 0, colunas = 0 , linhas = 0, colunasW = 0,
		 colunasR = 0, r = 0, t = 0;
	
	/*copia o tipo da imagem*/
	temp->tipo = strdup(pnm->tipo);
	
	/*uma rotacao de 90º implica trocar os valores
	* das linhas e colunas em Temp
	*/
	linhas = temp->colunas = pnm->linhas;
	colunas = temp->linhas = pnm->colunas;
	
	/*O pixel maximo é o mesmo*/
	temp->max_color = pnm->max_color;
	
	/*os comentarios também são*/
	temp->comentarios = copiaComments(pnm->comentarios);
	
	
	colunas--;
	linhas--;
	
	if(strcmp(temp->tipo,P3) == 0 || strcmp(temp->tipo,P6 ) == 0)
	{
		colunasW = (linhas+1)*3;
		colunasW--;
		colunasR = (colunas+1)*3;
		colunasR--;
		
		/*O numero de linhas em Temp passou a ser igual ao numero
		* de colunas em pnm, dai que:
		*/
		temp->rep = (int **)malloc(sizeof(int *)*(colunas+1));
		
		for(lh = 0, r = 0; lh <= colunas && r <= colunasR; lh++, r+=3)
		{
			temp->rep[lh] = (int *)malloc(sizeof(int)*(colunasW+1));
			
			/*colunasW é o triplo de linhas*/
			for(cl = 0, t = linhas; cl <= colunasW && t >= 0; cl+=3, t--)
			{
				/*No primeiro caso, a posicao (0,0) em temp 
				* corresponde à posicao (ultLinha,0) no pnm
				* original. Como estamos perante um PPM, cada 
				* pixel é composto por 3 cores (rgb), dai que:
				*/
				temp->rep[lh][cl] = pnm->rep[t][r];
				temp->rep[lh][cl+1] = pnm->rep[t][r+1];
				temp->rep[lh][cl+2] = pnm->rep[t][r+2];
			}
		}
	
	}else
	{
		/*caso estejamos perante um P1,P2,P4 ou P5*/
	
		temp->rep = (int **)malloc(sizeof(int *)*(colunas+1));
	
		for( lh = 0 ; lh <= colunas ; lh++ )
		{
			temp->rep[lh] = (int *)malloc(sizeof(int)*(linhas+1));
		
			for( cl = 0 ; cl <= linhas ; cl++ )
			{
				/*Neste, a posicao (0,0) em temp 
				* corresponde à posicao (ultLinha,0) no pnm
				* original; a posicao (0,1) correspinde à 
				* posicao (ultLinha-1,0) e por em diante,
				* dai que:
				*/
				temp->rep[lh][cl] = pnm->rep[linhas-cl][lh];
			}
		}
	}

	return temp;
}


/*___________________________Rodar 90º p/ esquerda__________________________*/

PNM * rodarE(PNM *pnm)
{
	PNM *temp = (PNM *)malloc(sizeof(PNM));
	
	int cl = 0, lh = 0, colunas = 0 , linhas = 0, colunasW = 0,
		 colunasR = 0, r = 0, t = 0;
	
	/*copia o tipo da imagem*/
	temp->tipo = strdup(pnm->tipo);
	
	/*uma rotacao de 90º implica trocar os valores
	* das linhas e colunas em Temp
	*/
	linhas = temp->colunas = pnm->linhas;
	colunas = temp->linhas = pnm->colunas;
	
	/*O pixel maximo é o mesmo*/
	temp->max_color = pnm->max_color;
	
	/*os comentarios também são*/
	temp->comentarios = copiaComments(pnm->comentarios);
	
	
	colunas--;
	linhas--;
	
	if(strcmp(temp->tipo,P3) == 0 || strcmp(temp->tipo,P6) == 0)
	{
		colunasW = (linhas+1)*3;
		colunasW--;
		colunasR = (colunas+1)*3;
		colunasR--;
		
		/*O numero de linhas em Temp passou a ser igual ao numero
		* de colunas em pnm, dai que:
		*/
		temp->rep = (int **)malloc(sizeof(int *)*(colunas+1));
		
		for(lh = 0, r = colunasR; lh <= colunas && r >= 0; lh++, r-=3)
		{
			temp->rep[lh] = (int *)malloc(sizeof(int)*(colunasW+1));
			
			/*colunasW é o triplo de linhas*/
			for(cl = 0, t = 0; cl <= colunasW && t <= linhas; cl+=3, t++)
			{
				/*No primeiro caso, a posicao (0,0) em temp 
				* corresponde à posicao (0,ultColuna) no pnm
				* original. Como estamos perante um PPM, cada 
				* pixel é composto por 3 cores (rgb), dai que:
				*/
				temp->rep[lh][cl] = pnm->rep[t][r-2];
				temp->rep[lh][cl+1] = pnm->rep[t][r-1];
				temp->rep[lh][cl+2] = pnm->rep[t][r];
			}
		}
	
	}else
	{
		/*caso estejamos perante um P1,P2,P4 ou P5*/
	
		temp->rep = (int **)malloc(sizeof(int *)*(colunas+1));
	
		for( lh = 0 ; lh <= colunas ; lh++ )
		{
			temp->rep[lh] = (int *)malloc(sizeof(int)*(linhas+1));
		
			for( cl = 0 ; cl <= linhas ; cl++ )
			{
				/*Neste, a posicao (0,0) em temp 
				* corresponde à posicao (0,ultColuna) no pnm
				* original; a posicao (0,1) correspinde à 
				* posicao (ultLinha-1,0) e por em diante,
				* dai que:
				*/
				temp->rep[lh][cl] = pnm->rep[cl][colunas-lh];
			}
		}
	}
	

	return temp;
}



/*__________________________Inverter Horizontalmente________________________*/

PNM * invH(PNM *pnm)
{
	PNM *temp = (PNM *)malloc(sizeof(PNM));
	
	int cl = 0, lh = 0, colunas = 0 , linhas = 0, r = 0;
	
	temp->tipo = strdup(pnm->tipo);
	
	colunas = temp->colunas = pnm->colunas;
	linhas = temp->linhas = pnm->linhas;
	
	temp->max_color = pnm->max_color;
	
	temp->comentarios = copiaComments(pnm->comentarios);
	
	linhas--;
	colunas--;
	
	temp->rep = (int **)malloc(sizeof(int *)*(linhas+1));
	
	if(strcmp(temp->tipo,P3) == 0 || strcmp(temp->tipo,P6) == 0)
	{
		colunas = (colunas+1)*3;
		colunas--;
		
		
		
		for(lh = 0; lh <= linhas; lh++)
		{
			temp->rep[lh] = (int *)malloc(sizeof(int)*(colunas+1));
			
			for(cl = 0, r = colunas; cl <= colunas; cl+=3, r-=3)
			{
				/*No primeiro caso, a posicao (0,0) em temp 
				* corresponde à posicao (0,ultColuna) no pnm
				* original. Como estamos perante um PPM, cada 
				* pixel é composto por 3 cores (rgb), dai que:
				*/
				temp->rep[lh][cl] = pnm->rep[lh][r-2];
				temp->rep[lh][cl+1] = pnm->rep[lh][r-1];
				temp->rep[lh][cl+2] = pnm->rep[lh][r];
			}
		}
	
	}else
	{	
		for( lh = 0 ; lh <= linhas ; lh++ )
		{
			temp->rep[lh] = (int *)malloc(sizeof(int)*(colunas+1));
		
			for( cl = 0 ; cl <= colunas ; cl++ )
			{
				temp->rep[lh][cl] = pnm->rep[lh][colunas-cl];
			}
		}
	}

		return temp;
}

/*__________________________Inverter Verticalmente__________________________*/
PNM * invV(PNM *pnm)
{
	PNM *temp = (PNM *)malloc(sizeof(PNM));
	
	int cl = 0, lh = 0, colunas = 0 , linhas = 0;
	
	temp->tipo = strdup(pnm->tipo);
	
	colunas = temp->colunas = pnm->colunas;
	linhas = temp->linhas = pnm->linhas;
	
	temp->max_color = pnm->max_color;
	
	temp->comentarios = copiaComments(pnm->comentarios);
	
	if(strcmp(pnm->tipo,P3) == 0 || strcmp(pnm->tipo,P6) == 0)
		colunas*=3;
		
	linhas--;
	colunas--;
	
	temp->rep = (int **)malloc(sizeof(int *)*(linhas+1));
	
	for( lh = 0 ; lh <= linhas ; lh++ )
	{
		temp->rep[lh] = (int *)malloc(sizeof(int)*(colunas+1));
	
		for( cl = 0 ; cl <= colunas ; cl++ )
		{
			temp->rep[lh][cl] = pnm->rep[linhas-lh][cl];
		}
	}

	return temp;
}
