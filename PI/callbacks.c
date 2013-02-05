#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

#include <unistd.h>
//#include "trabalho.h"

void actualizaTitulo(GList * user_data)
{
	GtkWidget * principal = g_list_nth(user_data,j_p)->data;
	
	gtk_window_set_title (GTK_WINDOW (principal), _(g_list_nth(user_data,p_o)->data));
}


void imprime_erro(GList * user_data, char * msm)
{
	GtkWidget * principal = g_list_nth(user_data,j_p)->data;
	GtkWidget * erro = lookup_widget(principal,"fs_erro");
	
        GtkWidget * label_erro = lookup_widget(erro,"e_label");
      
        gtk_label_set_text(GTK_LABEL(label_erro),msm);
      
        char * res = strtok(msm,"\n");
        int len = 0;
        
        while(res != NULL)
        {
        	if(strlen(res) > len) len = strlen(res);
        	res = strtok(NULL,"\n");
        }
        
        gtk_window_set_resizable (GTK_WINDOW (erro), TRUE);
        gtk_window_resize(GTK_WINDOW(erro),len*8 + 60 ,50);
           

        free(msm);


        GtkWidget * menuitem2 = lookup_widget(principal,"menuitem2");
	GtkWidget * hbox1 = lookup_widget(principal,"hbox1");
	GtkWidget * menu_gravar = lookup_widget(principal,"menu_gravar");
	GtkWidget * menu_gravar_como = lookup_widget(principal,"menu_gravar_como");
		
	gtk_widget_set_sensitive(menuitem2,FALSE);
	gtk_widget_set_sensitive(hbox1,FALSE);
	gtk_widget_set_sensitive(menu_gravar,FALSE);
	gtk_widget_set_sensitive(menu_gravar_como,FALSE);

        gtk_widget_show(erro);

}


int actualizaImage(GList * user_data)
{
	GtkWidget * principal = g_list_nth(user_data,j_p)->data;
	
	PNM * temp = g_list_nth(user_data,p_pnm)->data;
	
	
	int linhas = temp->linhas;
	int colunas = temp->colunas;
	int lh = 0; int cl = 0;
	int valores = 0;
	
	if(strcmp(temp->tipo, P6) == 0 || strcmp(temp->tipo, P3) == 0) colunas*=3;
	
	for(lh = 0; lh < linhas; lh++)
		for(cl = 0; cl < colunas; cl++)
		{
			if(temp->rep[lh][cl] < 0 || temp->rep[lh][cl] > 255) printf("%d %d %d\n",lh,cl,temp->rep[lh][cl]);
			else valores++;
		}
			
	gchar * tipo = g_strdup(temp->tipo);
	
	
	GtkWidget * bppm = lookup_widget(principal,"b_to_ppm");
	GtkWidget * bpgm = lookup_widget(principal,"b_to_pgm");
	GtkWidget * bpbm = lookup_widget(principal,"b_to_pbm");
	
	/*como estamos a carregar outra imagem,
	 o destino tem de ser reiniciado*/
	free(g_list_nth(user_data,p_d)->data);
	g_list_nth(user_data,p_d)->data = NULL;

	/*vamos gravar em formato binario, no ficheiro temporario*/
	if(gravarPNM(g_list_nth(user_data,p_pnm)->data,
		g_list_nth(user_data,p_t)->data,0) == 
		EXIT_FAILURE)
	{
		gchar * resto = (gchar *)g_malloc(sizeof(gchar)*1000);
                sprintf(resto,"Ocorreu um erro ao gravar\ntemporariamente a imagem\n");
                
                imprime_erro(user_data,resto);
                
                return EXIT_FAILURE;
	}else
	{
		gboolean active = 
			gtk_check_menu_item_get_active(
				GTK_CHECK_MENU_ITEM(
				lookup_widget(principal,
				"ver_localmente")));
				
		actualizaTitulo(user_data);
		
        	if( active )
		{ 
			GtkWidget *imagem = lookup_widget(principal,"image_central");
			
			gtk_image_clear(GTK_IMAGE(imagem));
			
			gtk_image_set_from_file(GTK_IMAGE(imagem),
				g_list_nth(user_data,p_t)->data);
			
			
			
		}
		
		active = gtk_check_menu_item_get_active(
				GTK_CHECK_MENU_ITEM(
					lookup_widget(principal,
					"usar_imagemagick")));
		
		/*mostrar no imagemagick*/
		if( active )
		{
			active = gtk_check_menu_item_get_active(
				GTK_CHECK_MENU_ITEM(
					lookup_widget(principal,
					"matar_imagemagick")));
			
		
			if( active ) system("killall display");
			
			gchar *magick_exec = (char *)g_malloc(sizeof(char)*200);
			
			sprintf(magick_exec,"display %s &",strdup(g_list_nth(user_data,p_t)->data));

			executa(magick_exec);
		}
		
		gtk_widget_set_sensitive(bppm,TRUE);
		gtk_widget_set_sensitive(bpgm,TRUE);
		gtk_widget_set_sensitive(bpbm,TRUE);
		
		
		if(strcmp(tipo,P1) == 0 || strcmp(tipo,P4) == 0)
			gtk_widget_set_sensitive(bpbm,FALSE);
		else if(strcmp(tipo,P2) == 0 || strcmp(tipo,P5) == 0)
			gtk_widget_set_sensitive(bpgm,FALSE);
		else if(strcmp(tipo,P3) == 0 || strcmp(tipo,P6) == 0)
			gtk_widget_set_sensitive(bppm,FALSE);
		
	}
	
	return EXIT_SUCCESS;
}

void
on_menu_abrir_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{	
	GtkWidget * principal = g_list_nth(user_data,j_p)->data;
	
	GtkWidget * fs_abrir = lookup_widget(principal,"fs_abrir");
	
	if(fs_abrir != NULL)
	{
		gtk_widget_show(fs_abrir);
		gtk_widget_set_sensitive(principal,FALSE);
	}

}
 

void
on_menu_gravar_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
        
        
        GtkWidget * principal = g_list_nth(user_data,j_p)->data;
        
        gchar *destino = 
                g_strdup(g_list_nth(user_data,p_d)->data);
        
        if(destino == NULL)	
        {       
        	GtkWidget * fs_save = lookup_widget(principal,"fs_save");
        	
        	if(fs_save != NULL)
        	{
        		gtk_widget_show(fs_save);
        		gtk_widget_set_sensitive(principal,FALSE);
        	}
        }
        else
        {
        	GtkWidget * fs_save = lookup_widget(principal,"fs_save");
        
                GtkWidget * formato = lookup_widget(fs_save,"fs_formato");
        
	        GtkWidget * radio_ascii = lookup_widget(formato,"f_ascii");
	        
	        gboolean binario = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(GTK_CHECK_BUTTON(GTK_RADIO_BUTTON(radio_ascii))));
	        
	        if(gravarPNM(g_list_nth(user_data,p_pnm)->data,
			g_list_nth(user_data,p_d)->data,binario) == 
			EXIT_FAILURE)
		{
			gchar * resto = (gchar *)g_malloc(sizeof(gchar)*1000);
	                sprintf(resto,"Ocorreu um erro ao gravar\ntemporariamente a imagem\n");
	                
	                 imprime_erro(user_data,resto);
		}
        }
}


void
on_menu_gravar_como_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
        GtkWidget * principal = g_list_nth(user_data,j_p)->data;

	GtkWidget * fs_save = lookup_widget(principal,"fs_save");
	
	if(fs_save != NULL)
	{
		gtk_widget_show(fs_save);
		gtk_widget_set_sensitive(principal,FALSE);
	}
}


void
on_menu_sair_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	gtk_main_quit();
}


void
on_menu_ih_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_menu_iv_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_menu_rd_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_menu_re_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_ver_localmente_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
        GtkWidget * principal = g_list_nth(user_data,j_p)->data;

        GtkWidget * scroll = lookup_widget(principal,"scrolled_window");
        
        gboolean active = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(menuitem));
        
        if(active == 1)
        {
                gtk_window_set_resizable (GTK_WINDOW (principal), TRUE);
		gtk_window_resize(GTK_WINDOW(principal),430,330);
		gtk_widget_show(scroll);
        }
        else
        {
                gtk_window_unmaximize(GTK_WINDOW(principal));
		gtk_window_set_resizable (GTK_WINDOW (principal), FALSE);
		
		gtk_window_resize(GTK_WINDOW(principal),430,60);
                gtk_widget_hide(scroll);
        }
}


void
on_usar_imagemagick_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget * principal = g_list_nth(user_data,j_p)->data;

	GtkWidget * usar = lookup_widget(principal, "usar_imagemagick");
	
	GtkWidget * matar = lookup_widget(principal, "matar_imagemagick");
	
	if(gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(usar)))
		gtk_widget_set_sensitive(matar,TRUE);
	else
		gtk_widget_set_sensitive(matar,FALSE);
	
}


void
on_matar_imagemagick_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_sobre_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget * principal = g_list_nth(user_data, j_p)->data;
	
        GtkWidget * fs_sobre = lookup_widget(principal,"fs_sobre");
        
        if(fs_sobre != NULL)
	{
                gtk_widget_show(fs_sobre);
	}
}


void
on_b_iv_clicked                        (GtkButton       *button,
                                        gpointer         user_data)
{
	g_list_nth(user_data,p_pnm)->data =
		inVV(g_list_nth(user_data,p_pnm)->data);
		
	gravarPNM(g_list_nth(user_data,p_pnm)->data,
		g_list_nth(user_data,p_t)->data,0);
		
	actualizaImage(user_data);
}


void
on_b_ih_clicked                        (GtkButton       *button,
                                        gpointer         user_data)
{
	g_list_nth(user_data,p_pnm)->data =
		inVH(g_list_nth(user_data,p_pnm)->data);
	
	gravarPNM(g_list_nth(user_data,p_pnm)->data,
		g_list_nth(user_data,p_t)->data,0);
		
	actualizaImage(user_data);
}


void
on_b_rd_clicked                        (GtkButton       *button,
                                        gpointer         user_data)
{
	g_list_nth(user_data,p_pnm)->data =
		rodaRD(g_list_nth(user_data,p_pnm)->data);
	
	gravarPNM(g_list_nth(user_data,p_pnm)->data,
		g_list_nth(user_data,p_t)->data,0);
		
	actualizaImage(user_data);
}


void
on_b_re_clicked                        (GtkButton       *button,
                                        gpointer         user_data)
{
	g_list_nth(user_data,p_pnm)->data =
		rodaRE(g_list_nth(user_data,p_pnm)->data);
		
	gravarPNM(g_list_nth(user_data,p_pnm)->data,
		g_list_nth(user_data,p_t)->data,0);
		
	actualizaImage(user_data);
}

void
on_b_act_clicked                        (GtkButton       *button,
                                        gpointer         user_data)
{
	if( (g_list_nth(user_data,p_pnm)->data = 
        	leFich(g_list_nth(user_data,p_o)->data,
        		g_list_nth(user_data,p_pnm)->data,
        		NULL)) == NULL )
        {
        	gchar * resto = (gchar *)g_malloc(sizeof(gchar)*1000);
                sprintf(resto,"Ocorreu um erro ao ler:\n[%s]", (char *)
                	g_list_nth(user_data,p_o)->data);
                
                imprime_erro(user_data,resto);
        }
        else
        {
		actualizaImage(user_data);
        }
}

void
on_b_to_pbm_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	g_list_nth(user_data,p_pnm)->data =
		converte(g_list_nth(user_data,p_pnm)->data,P4);
		
	gravarPNM(g_list_nth(user_data,p_pnm)->data,
		g_list_nth(user_data,p_t)->data,0);
		
	actualizaImage(user_data);
}

void
on_b_to_pgm_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	g_list_nth(user_data,p_pnm)->data =
		converte(g_list_nth(user_data,p_pnm)->data,P5);
		
	gravarPNM(g_list_nth(user_data,p_pnm)->data,
		g_list_nth(user_data,p_t)->data,0);
		
	actualizaImage(user_data);
}


void
on_b_to_ppm_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	g_list_nth(user_data,p_pnm)->data =
		converte(g_list_nth(user_data,p_pnm)->data,P6);
		
	gravarPNM(g_list_nth(user_data,p_pnm)->data,
		g_list_nth(user_data,p_t)->data,0);
		
	actualizaImage(user_data);
}


void
on_ja_abrir_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
        GtkWidget * principal = g_list_nth(user_data,j_p)->data;

	GtkWidget * fs_abrir = lookup_widget(principal,"fs_abrir");
	
	gchar *origem = NULL;
	
	int tipo = 0;
	
	
	if(fs_abrir != NULL)
	{
                origem = g_strdup(gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(fs_abrir)));
                
                /*caso ficheiro seleccionado seja uma pasta*/
                if(origem == NULL)
                	gtk_file_chooser_get_current_folder (GTK_FILE_CHOOSER(fs_abrir));
                /*se for um ficheiro*/
                else
                {
                	tipo = fileType(origem);
                	
                	g_list_nth(user_data,p_o)->data = strdup(origem);
                	
                	if(tipo == FGIF_JPG)
                	{
   				gtk_widget_hide(fs_abrir);
   				
				GtkWidget * conv = lookup_widget(principal,"fs_conv");
				gtk_widget_show(conv);
				
				origem = strdup(g_list_nth(user_data,p_o)->data);
                	}
                	else
			{	
				gtk_widget_hide(fs_abrir);

				gtk_widget_set_sensitive(principal,TRUE);

			        if( (g_list_nth(user_data,p_pnm)->data = 
			        	leFich(origem,g_list_nth(user_data,p_pnm)->data,
			        	NULL)) == NULL )
			        {
			        	char * resto = (char *)malloc(sizeof(char)*(30+strlen(origem)));
			        	
			                sprintf(resto,"Ocorreu um erro ao ler: \n[%s]",origem);
			                
			               
			                imprime_erro(user_data,origem);
			              
			        }
			        else
			        {
			        
					if( actualizaImage(user_data) == EXIT_SUCCESS)
					{
						GtkWidget * menuitem2 = lookup_widget(principal,"menuitem2");
						GtkWidget * hbox1 = lookup_widget(principal,"hbox1");
						GtkWidget * menu_gravar = lookup_widget(principal,"menu_gravar");
						GtkWidget * menu_gravar_como = lookup_widget(principal,"menu_gravar_como");
							
						gtk_widget_set_sensitive(menuitem2,TRUE);
						gtk_widget_set_sensitive(hbox1,TRUE);
						gtk_widget_set_sensitive(menu_gravar,TRUE);
						gtk_widget_set_sensitive(menu_gravar_como,TRUE);
					}
			        }
			}
	        }
	}
}


void
on_ja_cancelar_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
        GtkWidget * principal = g_list_nth(user_data,j_p)->data;

        GtkWidget * fs_abrir = lookup_widget(principal,"fs_abrir");
        
        if(fs_abrir != NULL)
	{
                gtk_file_chooser_unselect_all (GTK_FILE_CHOOSER(fs_abrir));
                gtk_widget_hide(fs_abrir);
                gtk_widget_set_sensitive(principal,TRUE);	
	}
}

void
on_js_cancelar_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
        GtkWidget * principal = g_list_nth(user_data,j_p)->data;

        GtkWidget * fs_save = lookup_widget(principal,"fs_save");
        
        if(fs_save != NULL)
	{
               gtk_widget_hide(fs_save);
               gtk_widget_set_sensitive(principal,TRUE);	
	}

}


void
on_js_avancar_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
        GtkWidget * principal = g_list_nth(user_data,j_p)->data;
        GtkWidget * fs_save = lookup_widget(principal,"fs_save");
        GtkWidget * formato = lookup_widget(fs_save,"fs_formato");

	if(fs_save != NULL && gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(fs_save)))
	{
	        g_list_nth(user_data,p_d)->data =
	               g_strdup(gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(fs_save)));
				
		gtk_widget_hide(fs_save);
		
		gtk_widget_show(formato);
	}
}


void
on_f_cancelar_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
        GtkWidget * principal = g_list_nth(user_data,j_p)->data;
        GtkWidget * fs_save = lookup_widget(principal,"fs_save");
        GtkWidget * formato = lookup_widget(fs_save,"fs_formato");
        
        if(fs_save != NULL && formato != NULL)
	{
                gtk_file_chooser_unselect_all (GTK_FILE_CHOOSER(fs_save));
                gtk_widget_hide(formato);
                gtk_widget_set_sensitive(principal,TRUE);	
	}

}


void
on_f_gravar_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
        GtkWidget * principal = g_list_nth(user_data,j_p)->data;
        GtkWidget * fs_save = lookup_widget(principal,"fs_save");
        GtkWidget * formato = lookup_widget(fs_save,"fs_formato");
        
        GtkWidget * radio_ascii = lookup_widget(formato,"f_ascii");
        
        gboolean binario = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(GTK_CHECK_BUTTON(GTK_RADIO_BUTTON(radio_ascii))));
        
        if(gravarPNM(g_list_nth(user_data,p_pnm)->data,
		g_list_nth(user_data,p_d)->data,binario) == 
		EXIT_FAILURE)
	{
		gchar * resto = (gchar *)g_malloc(sizeof(gchar)*1000);
                sprintf(resto,"Ocorreu um erro ao gravar a imagem\npara o ficheiro:\n[%s]",(char *)g_list_nth(user_data,p_d)->data);
                
                 imprime_erro(user_data,resto);
	}
        
        gtk_widget_hide(formato);
        
        gtk_widget_set_sensitive(principal,TRUE);
}


void
on_s_ok_clicked                        (GtkButton       *button,
                                        gpointer         user_data)
{
        gtk_widget_hide(user_data);
}


void
on_e_ok_clicked                        (GtkButton       *button,
                                        gpointer         user_data)
{
        GtkWidget *label = lookup_widget(user_data,"e_label");
        
        gtk_label_set_text(GTK_LABEL(label),"");
        gtk_widget_hide(user_data);
}

void
on_cp_cancelar_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget * principal = g_list_nth(user_data,j_p)->data;
	GtkWidget * fs_conv = lookup_widget(principal,"fs_conv");
	
	gtk_widget_hide(fs_conv);
	gtk_widget_set_sensitive(principal, TRUE);
}


void
on_cp_ok_clicked                       (GtkButton       *button,
                                        gpointer         user_data)
{

	GtkWidget * principal = g_list_nth(user_data,j_p)->data;
	GtkWidget * fs_conv = lookup_widget(principal,"fs_conv");
	GtkWidget * familia = lookup_widget(fs_conv,"combobox1");
	
	
	char * activo = (char *) gtk_combo_box_get_active_text(GTK_COMBO_BOX(familia));
	char * origem = g_list_nth_data(user_data,p_o);

	origem = conv_gif_jpg(origem,activo);
	
	gtk_widget_set_sensitive(principal,TRUE);

			        if( (g_list_nth(user_data,p_pnm)->data = 
			        	leFich(origem,g_list_nth(user_data,p_pnm)->data,
			        	NULL)) == NULL )
			        {
			        	char * resto = (char *)malloc(sizeof(char)*(30+strlen(origem)));
			        	
			                sprintf(resto,"Ocorreu um erro ao ler: \n[%s]",origem);
			                
			               
			                imprime_erro(user_data,origem);
			              
			        }
			        else
			        {
			        
					if( actualizaImage(user_data) == EXIT_SUCCESS)
					{
						GtkWidget * menuitem2 = lookup_widget(principal,"menuitem2");
						GtkWidget * hbox1 = lookup_widget(principal,"hbox1");
						GtkWidget * menu_gravar = lookup_widget(principal,"menu_gravar");
						GtkWidget * menu_gravar_como = lookup_widget(principal,"menu_gravar_como");
							
						gtk_widget_set_sensitive(menuitem2,TRUE);
						gtk_widget_set_sensitive(hbox1,TRUE);
						gtk_widget_set_sensitive(menu_gravar,TRUE);
						gtk_widget_set_sensitive(menu_gravar_como,TRUE);
					}
			        }
	
	g_list_nth(user_data,p_o)->data = strdup(origem);

	free(origem);

	gtk_widget_hide(fs_conv);
	
	
}





