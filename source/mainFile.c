#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#include "chess.h"
#include "grafico.h" 

/*    Robson Pugsley (rpugsley@gmail.com) -  Tecnicas de Programacao II
      Este jogo foi desenvolvido usando o Dev-C++ 4.9.9.2 e Allegro 4.2.0 rc2.
*/



int main()
{    

    int linIni, colIni, linFim, colFim;
    int lin, col;
    int jogoAcabou = 0, jogada, comeca = 0;
    int clicou = 0, podeclicar = 1  ;
    int roque, fazro = 0; 
    int coroamento, fazcor = 0;
    int fundo;
    int opfinal, gReplay;
    DATAFILE *imagens;
    int nJog = 0;
    int (*pFuncao)(); //ponteiro para funcao
    
    if(!IniciaAllegro())
      return 0;
	srand( (unsigned) time ( NULL ) );
	fundo = rand () % 2;

    imagens = load_datafile("imagens.dat");
    stTabuleiro *tab;
    inicializa( &tab );

    stTabuleiro *tabReplay;
    gravaReplay(&tabReplay, tab, &nJog);

	BITMAP *db = create_bitmap(SCREEN_W, SCREEN_H);

   do
	{
        if(key[KEY_ESC])
            break;
        jogada = 1;
        gReplay = 0;
        show_mouse(NULL);
        imprime( tab, imagens, db, fundo);


		if(tab->codJogadorVEZ == BRANCO)
            draw_rle_sprite(db, imagens[19].dat, 9 * TAMCASA-30 , 60);
        else 
            draw_rle_sprite(db, imagens[20].dat, 9 * TAMCASA-30 , 708);
        roque = podeRoque(tab);
        imprimeRoque(tab ,db, imagens , roque);
        coroamento = podeCoroamento(tab);
        imprimeCoroamento(tab, db , imagens, coroamento);
        animaBotao(tab, roque , imagens, db);

        if (coroamento)
           fazcor = checaCoroamento(coroamento);

        if (roque)
           fazro = checaRoque(tab, roque);

        if (fazcor != 0 || fazro != 0)
        {
            clicou = 2;
        }
		if(podeclicar)
		{
   	    	if(pegamouse(&lin, &col))
            { 
               clicou++;
               podeclicar = 0;
            }
        }

       if (!(mouse_b & 1))
       {  
         podeclicar = 1;
       }
       if(clicou == 1)
       {
         linIni = lin;
         colIni = col;
       }

      if(clicou == 2)
      {
         linFim = lin;
         colFim = col;

         if(!fazCoroamento( tab, db, imagens, fazcor ))
          {
               if(!fazRoque(tab, fazro))
                  jogada = joga( tab, linIni / TAMCASA, colIni / TAMCASA, linFim / TAMCASA, colFim / TAMCASA);
               else fazro = 0;
               if( jogada ) 
               {
                   tab->codJogadorVEZ = !tab->codJogadorVEZ;
                   mouse_b = 0;
                   gReplay = 1;
               }
          }
          else
          {
             jogada = 1;
             fazcor = 0;
             gReplay = 1;
          }
          clicou = 0;


      }//clicou =2
        jogoAcabou = reiIsAlive(tab);
        comeca = 1;
      if( !jogada )
      {
        blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
        alert("Movimento invalido", NULL, NULL, "OK", NULL,0,0);
	  } 
      else 
      {
        Animacao(tab, db, imagens, fundo);
        if (gReplay)
           gravaReplay(&tabReplay, tab, &nJog);
        set_mouse_sprite(imagens[16].dat);
        set_mouse_sprite_focus(17, 7);
        show_mouse(db);
        blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
        show_mouse(NULL);
      }

	}while(!jogoAcabou);

    if( jogoAcabou == 1)
    {
        show_mouse(NULL);
        pFuncao = jogBrancoshow;
        
    }
    else if(jogoAcabou == 2)
    {
        show_mouse(NULL);
        pFuncao = jogPretoshow;
    }
    else 
    {
        opfinal = alert("Sair?", NULL, NULL, "OK", "Replay",0,2);
    }
    if(jogoAcabou == 1 || jogoAcabou == 2)
    {
        opfinal = ExecutaShow(pFuncao);
    }
    if(opfinal == 2)
        tocaReplay(tabReplay, imagens, nJog);

    free(tab);

    allegro_exit();

	return(0);
}

END_OF_MAIN()
