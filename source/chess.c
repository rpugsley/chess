#include "chess.h"

#include <stdio.h>
#include <malloc.h>
#include <conio.h>

#ifndef botaoroqueMaior
 #define botaoroqueMaior                  2        /* RLE  */
 #define botaoroqueMenor                  4        /* RLE  */
#endif


#define OFFSET2 4
#define OFFSET  23


void imprime( const stTabuleiro *pTab, DATAFILE *imagens, BITMAP *db, int fundo )
{
   int i , j, x = 1;
   clear_bitmap(db);

   if(fundo == 0)
      draw_sprite(db, imagens[17].dat, TAM * TAMCASA, 0);
   if(fundo == 1)
      draw_sprite(db, imagens[18].dat, TAM * TAMCASA, 0);
   draw_rle_sprite(db, imagens[29].dat, 0, 0);

   // Imprime Peças
   for(i = 0; i < 8; i++) 
   {
      for(j = 0; j < 8; j++) 
      {
         if(pTab->anima.linini !=  i || pTab->anima.colini != j )
         {
             switch (pTab->tabuleiro[i][j].codPeca) 
             {
                case PEAO:
                   if ( pTab->tabuleiro[i][j].codJogador == BRANCO )
                      draw_rle_sprite(db, PEAOBRANCO, j * TAMCASA + OFFSET, i * TAMCASA + OFFSET2);
                   else
                      draw_rle_sprite(db, PEAOPRETO, j * TAMCASA + OFFSET, i * TAMCASA + OFFSET2);
                   break;
                case TORRE:
                   if ( pTab->tabuleiro[i][j].codJogador == BRANCO )
                    draw_rle_sprite(db, TORREBRANCO, j * TAMCASA + OFFSET, i * TAMCASA + OFFSET2);
                   else
                     draw_rle_sprite(db, TORREPRETO, j * TAMCASA + OFFSET, i * TAMCASA + OFFSET2);
                   break;               
                case CAVALO:
                   if ( pTab->tabuleiro[i][j].codJogador == BRANCO )
                    draw_rle_sprite(db, CAVALOBRANCO, j * TAMCASA + OFFSET, i * TAMCASA + OFFSET2);
                   else
                     draw_rle_sprite(db, CAVALOPRETO, j * TAMCASA + OFFSET, i * TAMCASA + OFFSET2);
                   break;               
                case BISPO:
                   if ( pTab->tabuleiro[i][j].codJogador == BRANCO )
                    draw_rle_sprite(db, BISPOBRANCO, j * TAMCASA + OFFSET, i * TAMCASA + OFFSET2);
                   else
                     draw_rle_sprite(db, BISPOPRETO, j * TAMCASA + OFFSET, i * TAMCASA + OFFSET2);
                   break;               
                case RAINHA:
                   if ( pTab->tabuleiro[i][j].codJogador == BRANCO )
                    draw_rle_sprite(db, RAINHABRANCO, j * TAMCASA + OFFSET, i * TAMCASA + OFFSET2);
                   else
                     draw_rle_sprite(db, RAINHAPRETO, j * TAMCASA + OFFSET, i * TAMCASA + OFFSET2);
                   break;               
                case REI:
                   if ( pTab->tabuleiro[i][j].codJogador == BRANCO )
                    draw_rle_sprite(db, REIBRANCO, j * TAMCASA + OFFSET, i * TAMCASA + OFFSET2);
                   else
                     draw_rle_sprite(db, REIPRETO, j * TAMCASA + OFFSET, i * TAMCASA + OFFSET2);
                   break;                        
            }
        }
      }
   }
}


void inicializa( stTabuleiro **ppTab )
{
   int i;
   int lin, col;
   
   *ppTab = (stTabuleiro *)malloc(sizeof(stTabuleiro));
   stTabuleiro *pTab = *ppTab;
   
	pTab->codJogadorVEZ = BRANCO;
    for( lin = 0; lin <= 7; lin++ )
	{
 		for( col =0; col < TAM; col++ )
		{
           pTab->tabuleiro[lin][col].codPeca = VAZIO;
           pTab->tabuleiro[lin][col].codJogador = VAZIO;
  		}
	}

	for( col=0; col < TAM; col++ )
	{
      pTab->tabuleiro[1][col].codPeca = PEAO;
	  pTab->tabuleiro[1][col].codJogador = BRANCO;
      pTab->tabuleiro[6][col].codPeca = PEAO;
	  pTab->tabuleiro[6][col].codJogador = PRETO;
      pTab->tabuleiro[0][col].codJogador = BRANCO;
      pTab->tabuleiro[7][col].codJogador = PRETO;
   }

    for (i=0; i<=7; i+=7) 
   {
      pTab->tabuleiro[i][0].codPeca = TORRE;
      pTab->tabuleiro[i][1].codPeca = CAVALO;
      pTab->tabuleiro[i][2].codPeca = BISPO;
      pTab->tabuleiro[i][5].codPeca = BISPO;
      pTab->tabuleiro[i][6].codPeca = CAVALO;
      pTab->tabuleiro[i][7].codPeca = TORRE;
   }

    pTab->tabuleiro[0][3].codPeca = RAINHA;
    pTab->tabuleiro[0][4].codPeca = REI;
    pTab->tabuleiro[7][4].codPeca = REI;
    pTab->tabuleiro[7][3].codPeca = RAINHA;

    pTab->roqueGBranco = 1 ;
	pTab->roquePBranco = 1;
	pTab->roqueGPreto = 1;
	pTab->roquePPreto = 1;
    pTab->enPassantBranco = -1 ;
    pTab->enPassantPreto = -1 ;
    pTab->anima.linini = pTab->anima.colini = -1;
    pTab->anima.linfim = pTab->anima.colfim = -1;

}

int joga( stTabuleiro *pTab, int posLinIni, int posColIni, int posLinFim, int posColFim )
{
    pTab->FezroqueGBranco = 0 ;
	pTab->FezroquePBranco = 0;
	pTab->FezroqueGPreto = 0;
	pTab->FezroquePPreto = 0;
    pTab->FezPassantBranco = 0 ;
    pTab->FezPassantPreto = 0 ;
    pTab->FezCoroamentoBranco = 0 ;
    pTab->FezCoroamentoPreto = 0 ;

    if( posLinIni < 0 || posLinIni >= TAM )
		return 0;

	if( posColIni < 0 || posColIni >= TAM )
		return 0;

	if( posLinFim < 0 || posLinFim >= TAM )
		return 0;

	if( posColFim < 0 || posColFim >= TAM )
		return 0;

	if( pTab->tabuleiro[posLinIni][posColIni].codPeca == VAZIO )
		return 0;

	
   if( pTab->tabuleiro[posLinIni][posColIni].codJogador != pTab->codJogadorVEZ )
		return 0;
   if( pTab->tabuleiro[posLinFim][posColFim].codJogador == pTab->codJogadorVEZ)
        return 0;

	int valido = 0;
	switch( pTab->tabuleiro[posLinIni][posColIni].codPeca )
	{
        case PEAO:
			valido = movePeao( pTab, posLinIni, posColIni,  posLinFim,  posColFim );
			break;
        case TORRE:
			valido = moveTorre( pTab, posLinIni, posColIni,  posLinFim,  posColFim );
            pTab->enPassantBranco = -1 ;
            pTab->enPassantPreto = -1 ;
			break;
        case CAVALO:
            valido = moveCavalo( pTab, posLinIni, posColIni, posLinFim, posColFim );
            pTab->enPassantBranco = -1 ;
            pTab->enPassantPreto = -1 ;
            break;
        case BISPO:
            valido = moveBispo( pTab, posLinIni, posColIni, posLinFim, posColFim );
            pTab->enPassantBranco = -1 ;
            pTab->enPassantPreto = -1 ;
           break;
        case RAINHA:
            valido = moveRainha( pTab, posLinIni, posColIni, posLinFim, posColFim );
            pTab->enPassantBranco = -1 ;
            pTab->enPassantPreto = -1 ;
           break;
        case REI:
            valido = moveRei( pTab, posLinIni, posColIni, posLinFim, posColFim );
            pTab->enPassantBranco = -1 ;
            pTab->enPassantPreto = -1 ;
           break;
	}

	if( valido ) 
	{
		if(pTab->codJogadorVEZ == BRANCO)
		{
	     if( pTab->tabuleiro[posLinIni][posColIni].codPeca == TORRE)
	     {
	        if(posLinIni == 0 && posColIni == 0)
	           pTab->roqueGBranco = 0 ;
           if(posLinIni == 0 && posColIni == TAM-1)
      	     pTab->roquePBranco = 0;
	     }
	     if( pTab->tabuleiro[posLinIni][posColIni].codPeca == REI)
	     {
	           pTab->roqueGBranco = 0;
	           pTab->roquePBranco = 0;
	     }
       }
      if(pTab->codJogadorVEZ == PRETO)
		{
	     if( pTab->tabuleiro[posLinIni][posColIni].codPeca == TORRE)
	     {
	       if(posLinIni == 7 && posColIni == 0)
	         pTab->roqueGPreto = 0 ;
           if(posLinIni == 7 && posColIni == TAM-1)
      	     pTab->roquePPreto = 0;
	     }
	     if( pTab->tabuleiro[posLinIni][posColIni].codPeca == REI)
	     {
	           pTab->roqueGPreto = 0;
	           pTab->roquePPreto = 0;
	     }
       }
     pTab->anima.linini = posLinIni;
     pTab->anima.linfim = posLinFim;
     pTab->anima.colini = posColIni; 
     pTab->anima.colfim = posColFim;

	}

	return valido;
}

int abs(int v) {return (v<0) ? (v * -1) : v;}


int movePeao( stTabuleiro *pTab, int posLinIni, int posColIni, int posLinFim, int posColFim )
{
   if(pTab->codJogadorVEZ == BRANCO)
   {
      if(posLinIni >= posLinFim)
         return 0;
   }
   else
   {
      if(posLinIni <= posLinFim)
         return 0;     
   }

   if( posColIni != posColFim)
   {
      if( abs( posLinIni - posLinFim ) != 1 || abs( posColIni - posColFim ) != 1 )
      {
         return 0;
      }
      if( pTab->tabuleiro[posLinFim][posColFim].codJogador == VAZIO)  // Verifica En Passant
      {
         if(pTab->codJogadorVEZ == BRANCO) 
         {
            if ( pTab->enPassantPreto == posColFim )
            {   
	           	pTab->tabuleiro[posLinFim - 1][posColFim].codPeca = VAZIO;
                pTab->tabuleiro[posLinFim - 1][posColFim].codJogador = VAZIO;
                pTab->FezPassantBranco = 1;
                return 1;
            }
      } 
      else 
      {
          if ( pTab->enPassantBranco == posColFim )
          {
	           	pTab->tabuleiro[posLinFim + 1][posColFim].codPeca = VAZIO;
                pTab->tabuleiro[posLinFim + 1][posColFim].codJogador = VAZIO;
                pTab->FezPassantPreto = 1;
                return 1;
          }
      }
   }
      if( pTab->tabuleiro[posLinFim][posColFim].codJogador != !pTab->codJogadorVEZ)
      {
         return 0;
      }
      else 
      {
         pTab->enPassantBranco = -1 ;
         pTab->enPassantPreto = -1 ;
         return 1; 
      }
   }
   
   if( pTab->tabuleiro[posLinFim][posColFim].codPeca != VAZIO )
      return 0;
      
   if( abs( posLinIni - posLinFim ) > 2)
         return 0;
   
   if( abs( posLinIni - posLinFim ) == 2)
   {
      if( posLinIni != 1 && posLinIni != 6)
         return 0;
      if (pTab->codJogadorVEZ == BRANCO)
         pTab->enPassantBranco = posColIni;
      else
         pTab->enPassantPreto = posColIni;
   }
   else
   {
      pTab->enPassantBranco = -1 ;
      pTab->enPassantPreto = -1 ;
   }
   
   return 1;
}

int moveTorre( stTabuleiro *pTab, int posLinIni, int posColIni, int posLinFim, int posColFim )
{
   int i, ini , fim;
   if( posLinIni != posLinFim && posColIni != posColFim )
      return 0;
   if( posLinIni == posLinFim)
   {
      if( posColIni > posColFim)
         {
            ini = posColFim;
            fim = posColIni;
         }
      else
         {
            ini = posColIni;
            fim = posColFim;
         }
      for(i = ini + 1 ; i < fim; i++)
         if( pTab->tabuleiro[posLinIni][i].codPeca != VAZIO )
            return 0;
   }
   if( posColIni == posColFim)
   {
      if( posLinIni > posLinFim)
         {
            ini = posLinFim;
            fim = posLinIni;
         }
      else
         {
            ini = posLinIni;
            fim = posLinFim;
         }
      for(i = ini + 1 ; i < fim; i++)
         if( pTab->tabuleiro[i][posColFim].codPeca != VAZIO )
            return 0;
   }   

	return 1;
}

int moveCavalo( stTabuleiro *pTab, int posLinIni, int posColIni, int posLinFim, int posColFim )
{
   if( abs( posLinIni - posLinFim ) == 1 && abs( posColIni - posColFim ) == 2 )
      return 1;
   if( abs( posLinIni - posLinFim ) == 2 && abs( posColIni - posColFim ) == 1 )
      return 1;
   
   return 0;
}

int moveBispo( stTabuleiro *pTab, int posLinIni, int posColIni, int posLinFim, int posColFim )
{
   if( abs( posLinIni - posLinFim ) != abs( posColIni - posColFim ))
      return 0;

   int i, j;
   if( posLinIni < posLinFim && posColIni < posColFim )
      for(i = posLinIni +1, j = posColIni +1; i < posLinFim; i++,j++)
         if( pTab->tabuleiro[i][j].codPeca != VAZIO )
            return 0;
   if( posLinIni < posLinFim && posColIni > posColFim )
      for(i = posLinIni +1, j = posColIni -1; i < posLinFim; i++,j--)
         if( pTab->tabuleiro[i][j].codPeca != VAZIO )
            return 0;
   if( posLinIni > posLinFim && posColIni < posColFim )
      for(i = posLinIni -1, j = posColIni +1; i > posLinFim; i--,j++)
         if( pTab->tabuleiro[i][j].codPeca != VAZIO )
            return 0;
   if( posLinIni > posLinFim && posColIni > posColFim )
      for(i = posLinIni -1, j = posColIni -1; i > posLinFim; i--,j--)
         if( pTab->tabuleiro[i][j].codPeca != VAZIO )
            return 0;
   return 1;
}

int moveRainha( stTabuleiro *pTab, int posLinIni, int posColIni, int posLinFim, int posColFim )
{
   if( moveTorre( pTab , posLinIni, posColIni, posLinFim, posColFim) ||
       moveBispo( pTab , posLinIni, posColIni, posLinFim, posColFim) )
  	   return 1;
  	   
   return 0;
}

int moveRei( stTabuleiro *pTab, int posLinIni, int posColIni, int posLinFim, int posColFim )
{  
     if( abs( posLinIni - posLinFim ) > 1 || abs( posColIni - posColFim ) > 1 )
         return 0;
      
   return 1;
}

int reiIsAlive( stTabuleiro *pTab )
{
   int i,j;
   int reiPreto , reiBranco;
   
   reiPreto = reiBranco = 0;
   
   for(i = 0; i < TAM; i++)
   {
      for(j = 0; j < TAM; j++)
      {  
         if( pTab->tabuleiro[i][j].codPeca == REI )
         {
            if( pTab->tabuleiro[i][j].codJogador == BRANCO )
               reiBranco = 1;
            else
               reiPreto = 1;
         }
      }
   }
   if (reiBranco == 1 && reiPreto == 1)
      return 0;
   if (reiBranco == 0)
      return 2;
   
   return 1;
}

/*void gravaLog( stTabuleiro *pTab, int posLinIni, int posColIni, int posLinFim, int posColFim, int jogada, int jogoAcabou, int comeca )
{
     int lin, col;
     FILE *fLog = fopen( "log.txt", "a+" );
     if(comeca == 0)
        fprintf( fLog, "\n\n\nComecando Novo jogo...\n");

     if(pTab->FezroqueGBranco == 1)
     {
            fprintf(fLog,"Jogador Branco Fez Roque Maior.\n");
            fclose( fLog );
            return;
     }
     if(pTab->FezroquePBranco == 1)
     {
            fprintf(fLog,"Jogador Branco Fez Roque Menor.\n");
            fclose( fLog );
            return;
     }
     if(pTab->FezroqueGPreto == 1)
     {
            fprintf(fLog,"Jogador Preto Fez Roque Maior.\n");
            fclose( fLog );
            return;
     }
     if(pTab->FezroquePPreto == 1)
     {
            fprintf(fLog,"Jogador Preto Fez Roque Menor.\n");
            fclose( fLog );
            return;
     }
     if(pTab->FezCoroamentoBranco > 0)
     {
          switch( pTab->FezCoroamentoBranco )
          {
		      case 1:
		          fprintf(fLog,"Jogador Branco, trocou o peao por uma Torre no coroamento.\n");
                  fclose( fLog );
		          break;
		      case 2:
		          fprintf(fLog,"Jogador Branco, trocou o peao por um Bispo no coroamento.\n");
                  fclose( fLog );
		          break;
              case 3:
		          fprintf(fLog,"Jogador Branco, trocou o peao por uma Rainha no coroamento.\n");
                  fclose( fLog );
		          break;
		      case 4:
		          fprintf(fLog,"Jogador Branco, trocou o peao por um Cavalo no coroamento.\n");
                  fclose( fLog );
		          break;
          }
     }
     if(pTab->FezCoroamentoPreto > 0)
     {
          switch( pTab->FezCoroamentoPreto )
          {
		      case 1:
		          fprintf(fLog,"Jogador Preto, trocou o peao por uma Torre no coroamento.\n");
                  fclose( fLog );
		          break;
		      case 2:
		          fprintf(fLog,"Jogador Preto, trocou o peao por um Bispo no coroamento.\n");
                  fclose( fLog );
		          break;
              case 3:
		          fprintf(fLog,"Jogador Preto, trocou o peao por uma Rainha no coroamento.\n");
                  fclose( fLog );
		          break;
              case 4:
		          fprintf(fLog,"Jogador Preto, trocou o peao por uma Cavalo no coroamento.\n");
                  fclose( fLog );
		          break;
          }     
     }
     
     if(jogada == 0)
         fprintf( fLog, "Jogador %s / LinIni %i- ColIni %i- LinFim %i- ColFim %i",(pTab->codJogadorVEZ == 1) ? "Preto" : "Branco", posLinIni, posColIni, posLinFim, posColFim);
     else
         fprintf( fLog, "Jogador %s / LinIni %i- ColIni %i- LinFim %i- ColFim %i",(pTab->codJogadorVEZ == 1) ? "Branco" : "Preto", posLinIni, posColIni, posLinFim, posColFim);

     if(jogada)
     {
         lin = posLinFim;
         col = posColFim;
      }
      else
      {
         lin = posLinIni;
         col = posColIni;
      }
   	if(lin >= 0 && lin <= TAM && col >= 0 && col <= TAM)
   	{
      switch( pTab->tabuleiro[lin][col].codPeca )
      {
		case PEAO:
		   fprintf(fLog," -- Peca : PEAO\n");
		   break;
		case TORRE:
		   fprintf(fLog," -- Peca : TORRE\n");
		   break;
        case CAVALO:
		   fprintf(fLog," -- Peca : CAVALO\n");
		   break;
        case BISPO:
		   fprintf(fLog," -- Peca : BISPO\n");
		   break;
        case RAINHA:
		   fprintf(fLog," -- Peca : RAINHA\n");
		   break;
        case REI:
		   fprintf(fLog," -- Peca : REI\n");
		   break;
      }
    }
     
     if(pTab->FezPassantBranco == 1)
        fprintf(fLog,"\nJogador Branco Fez En Passant na jogada anterior.");
     if(pTab->FezPassantPreto == 1)
        fprintf(fLog,"\nJogador Preto Fez En Passant na jogada anterior.");
     
     if( jogada == 0)
        fprintf( fLog, "\nA Jogada anterior foi invalida.");
     if (jogoAcabou == 2)
        fprintf( fLog, "\nJogador Preto Ganhou!!!\n\n");
     if (jogoAcabou == 1)
        fprintf( fLog, "\nJogador Branco Ganhou!!!\n\n");
     fprintf(fLog,"\n");
     fclose( fLog );
     
}*/

int podeRoque(stTabuleiro *pTab)
{
   int ret = 0;
   
        if(pTab->codJogadorVEZ == BRANCO)
		{   		
         if( pTab->roqueGBranco == 1 && pTab->tabuleiro[0][0].codJogador == BRANCO)
            if(pTab->tabuleiro[0][1].codPeca == VAZIO && pTab->tabuleiro[0][2].codPeca == VAZIO && pTab->tabuleiro[0][3].codPeca == VAZIO)
                ret+=10;
                     
         if( pTab->roquePBranco == 1 && pTab->tabuleiro[0][7].codJogador == BRANCO)
            if(pTab->tabuleiro[0][5].codPeca == VAZIO && pTab->tabuleiro[0][6].codPeca == VAZIO)
                ret++;
        }

      if(pTab->codJogadorVEZ == PRETO)
	  {   		
         if( pTab->roqueGPreto == 1 && pTab->tabuleiro[7][0].codJogador == PRETO)
            if(pTab->tabuleiro[7][1].codPeca == VAZIO && pTab->tabuleiro[7][2].codPeca == VAZIO && pTab->tabuleiro[7][3].codPeca == VAZIO)
               ret+=10;
        
          if( pTab->roquePPreto == 1 && pTab->tabuleiro[7][7].codJogador == PRETO)
            if(pTab->tabuleiro[7][5].codPeca == VAZIO && pTab->tabuleiro[7][6].codPeca == VAZIO)
               ret++;
      }
      return ret;
}

int fazRoque(stTabuleiro *pTab , int fazroque)
{
        if(pTab->codJogadorVEZ == BRANCO)
		{   		
                  if(fazroque == 1)
                  {
               		pTab->tabuleiro[0][0].codPeca = VAZIO;
                    pTab->tabuleiro[0][0].codJogador = VAZIO;
               		pTab->tabuleiro[0][4].codPeca = VAZIO;
                    pTab->tabuleiro[0][4].codJogador = VAZIO;
               		
                    pTab->tabuleiro[0][2].codPeca = REI;
                    pTab->tabuleiro[0][2].codJogador = BRANCO;
               		pTab->tabuleiro[0][3].codPeca = TORRE;
                    pTab->tabuleiro[0][3].codJogador = BRANCO;

                    pTab->roqueGBranco = 0;
                    pTab->roquePBranco = 0;
                    pTab->FezroqueGBranco = 1 ;

                     return 1;
                  }
                  if(fazroque == 2)
                  {
               		pTab->tabuleiro[0][7].codPeca = VAZIO;
                    pTab->tabuleiro[0][7].codJogador = VAZIO;
               		pTab->tabuleiro[0][4].codPeca = VAZIO;
                    pTab->tabuleiro[0][4].codJogador = VAZIO;
               		
                    pTab->tabuleiro[0][6].codPeca = REI;
                    pTab->tabuleiro[0][6].codJogador = BRANCO;
               		pTab->tabuleiro[0][5].codPeca = TORRE;
                    pTab->tabuleiro[0][5].codJogador = BRANCO;

                    pTab->roqueGBranco = 0;
                    pTab->roquePBranco = 0;
                    pTab->FezroquePBranco = 1 ;
                    return 1;
                 }
      }
      if(pTab->codJogadorVEZ == PRETO)
      {   		
                  if(fazroque == 1)
                  {
               		pTab->tabuleiro[7][0].codPeca = VAZIO;
                    pTab->tabuleiro[7][0].codJogador = VAZIO;
               		pTab->tabuleiro[7][4].codPeca = VAZIO;
                    pTab->tabuleiro[7][4].codJogador = VAZIO;
               		
                    pTab->tabuleiro[7][2].codPeca = REI;
                    pTab->tabuleiro[7][2].codJogador = PRETO;
               		pTab->tabuleiro[7][3].codPeca = TORRE;
                    pTab->tabuleiro[7][3].codJogador = PRETO;

                    pTab->roqueGPreto = 0;
                    pTab->roquePBranco = 0;
                    pTab->FezroqueGPreto = 1 ;
                    return 1;
                  }
                  if(fazroque == 2)
                  {
               		pTab->tabuleiro[7][7].codPeca = VAZIO;
                    pTab->tabuleiro[7][7].codJogador = VAZIO;
               		pTab->tabuleiro[7][4].codPeca = VAZIO;
                    pTab->tabuleiro[7][4].codJogador = VAZIO;
               		
                    pTab->tabuleiro[7][6].codPeca = REI;
                    pTab->tabuleiro[7][6].codJogador = PRETO;
               		pTab->tabuleiro[7][5].codPeca = TORRE;
                    pTab->tabuleiro[7][5].codJogador = PRETO;

                    pTab->roqueGPreto = 0;
                    pTab->roquePBranco = 0;
                    pTab->FezroquePPreto = 1 ;
                    return 1;
                 }
      }
      return 0;
}



int podeCoroamento(stTabuleiro *pTab)
{
    int contRainha = 0, contCavalo = 0, contTorre = 0, contBispo = 0, pode = 0;
    int lin, col, i;
    for(i = 0; i < TAM; i++)
    {
            if(pTab->tabuleiro[0][i].codPeca == PEAO && pTab->tabuleiro[0][i].codJogador == pTab->codJogadorVEZ)
            {
                    for( lin = 0; lin <= 7; lin++ )
	                {
 		               for( col = 0; col < TAM; col++ )
		               {
                            if(pTab->tabuleiro[lin][col].codPeca == RAINHA && pTab->tabuleiro[lin][col].codJogador == PRETO)
                                contRainha++;
  		               }
	                }
	                if(contRainha == 0)
		      	         pode += 1;


                    for( lin = 0; lin <= 7; lin++ )
	                {
 		               for( col =0; col < TAM; col++ )
		               {
                            if(pTab->tabuleiro[lin][col].codPeca == CAVALO && pTab->tabuleiro[lin][col].codJogador == PRETO)
                              contCavalo++;
  		               }
	                }
	                if(contCavalo == 0 || contCavalo == 1)
		      	         pode += 10;


                    for( lin = 0; lin <= 7; lin++ )
	                {
 		               for( col =0; col < TAM; col++ )
		               {
                            if(pTab->tabuleiro[lin][col].codPeca == BISPO && pTab->tabuleiro[lin][col].codJogador == PRETO)
                              contBispo++;
  		               }
	                }
	                if(contBispo == 0 || contBispo == 1)
		      	         pode += 100;
                    
                    
                    for( lin = 0; lin <= 7; lin++ )
	                {
 		               for( col =0; col < TAM; col++ )
		               {
                            if(pTab->tabuleiro[lin][col].codPeca == TORRE && pTab->tabuleiro[lin][col].codJogador == PRETO)
                              contTorre++;
  		               }
	                }
	                if(contTorre == 0 || contTorre == 1)
		      	         pode += 1000;
           }


           if(pTab->tabuleiro[7][i].codPeca == PEAO && pTab->tabuleiro[7][i].codJogador == pTab->codJogadorVEZ)
           {
                    for( lin = 0; lin <= 7; lin++ )
	                {
 		               for( col = 0; col < TAM; col++ )
		               {
                            if(pTab->tabuleiro[lin][col].codPeca == RAINHA && pTab->tabuleiro[lin][col].codJogador == BRANCO)
                                contRainha++;
  		               }
	                }
	                if(contRainha == 0)
		      	         pode += 1;


                    for( lin = 0; lin <= 7; lin++ )
	                {
 		               for( col =0; col < TAM; col++ )
		               {
                            if(pTab->tabuleiro[lin][col].codPeca == CAVALO && pTab->tabuleiro[lin][col].codJogador == BRANCO)
                              contCavalo++;
  		               }
	                }
	                if(contCavalo == 0 || contCavalo == 1)
		      	         pode += 10;


                    for( lin = 0; lin <= 7; lin++ )
	                {
 		               for( col =0; col < TAM; col++ )
		               {
                            if(pTab->tabuleiro[lin][col].codPeca == BISPO && pTab->tabuleiro[lin][col].codJogador == BRANCO)
                              contBispo++;
  		               }
	                }
	                if(contBispo == 0 || contBispo == 1)
		      	         pode += 100;
                    
                    
                    for( lin = 0; lin <= 7; lin++ )
	                {
 		               for( col =0; col < TAM; col++ )
		               {
                            if(pTab->tabuleiro[lin][col].codPeca == TORRE && pTab->tabuleiro[lin][col].codJogador == BRANCO)
                              contTorre++;
  		               }
	                }
	                if(contTorre == 0 || contTorre == 1)
		      	         pode += 1000;
		      	         
           }
    }    
    return (pode);
}


void Animacao(stTabuleiro *pTab, BITMAP *db, DATAFILE *imagens,int fundo)
{
   int i, j , linini , colini , linfim, colfim;

   if ( pTab->anima.linini != -1 )
   {
       linini = pTab->anima.linini * TAMCASA;
       linfim = pTab->anima.linfim * TAMCASA;
       colini = pTab->anima.colini * TAMCASA;
       colfim = pTab->anima.colfim * TAMCASA;
    
       show_mouse(NULL);
         switch (pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codPeca) 
         {
            case PEAO:
               if((pTab->anima.linini != pTab->anima.linfim) && (pTab->anima.colini == pTab->anima.colfim))
               {
                    for(i = linini; i >= linfim; i-=VEL )
                    {
                      imprime( pTab, imagens, db, fundo );
                      if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                        draw_rle_sprite(db, PEAOBRANCO, colini + OFFSET, i + OFFSET2);
                      else
                        draw_rle_sprite(db, PEAOPRETO, colini + OFFSET, i + OFFSET2);
                      blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                    }      
                    for(i = linini; i <= linfim; i+=VEL )
                    {
                      imprime( pTab, imagens, db, fundo);
                      if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                        draw_rle_sprite(db, PEAOBRANCO, colini + OFFSET, i + OFFSET2);
                      else
                        draw_rle_sprite(db, PEAOPRETO, colini + OFFSET, i + OFFSET2);
                      blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                    }      
                }
               if(pTab->anima.colini != pTab->anima.colfim)
               {
                       if( pTab->anima.linini < pTab->anima.linfim && pTab->anima.colini < pTab->anima.colfim )
                       {
                          for(i = linini , j = colini +1; i < linfim; i+=VEL,j+=VEL)
                          {
                              imprime( pTab, imagens, db, fundo );
                              if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                                draw_rle_sprite(db, PEAOBRANCO, j + OFFSET, i + OFFSET2);
                              else
                                draw_rle_sprite(db, PEAOPRETO, j + OFFSET, i + OFFSET2);
                              blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                          }
                       }
                       if( pTab->anima.linini < pTab->anima.linfim && pTab->anima.colini > pTab->anima.colfim )
                       {
                          for(i = linini , j = colini ; i < linfim; i+=VEL,j-=VEL)
                          {
                              imprime( pTab, imagens, db, fundo );
                              if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                                draw_rle_sprite(db, PEAOBRANCO, j + OFFSET, i + OFFSET2);
                              else
                                draw_rle_sprite(db, PEAOPRETO, j + OFFSET, i + OFFSET2);
                              blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                          }
                       }
                       if( pTab->anima.linini > pTab->anima.linfim && pTab->anima.colini < pTab->anima.colfim )
                       {
                          for(i = linini , j = colini ; i > linfim; i-=VEL,j+=VEL)
                          {
                              imprime( pTab, imagens, db, fundo );
                              if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                                draw_rle_sprite(db, PEAOBRANCO, j + OFFSET, i + OFFSET2);
                              else
                                draw_rle_sprite(db, PEAOPRETO, j + OFFSET, i + OFFSET2);
                              blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                          }
                       }
                       if( pTab->anima.linini > pTab->anima.linfim && pTab->anima.colini > pTab->anima.colfim )
                       {
                          for(i = linini , j = colini ; i > linfim; i-=VEL,j-=VEL)
                          {
                              imprime( pTab, imagens, db, fundo );
                              if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                                draw_rle_sprite(db, PEAOBRANCO, j + OFFSET, i + OFFSET2);
                              else
                                draw_rle_sprite(db, PEAOPRETO, j + OFFSET, i + OFFSET2);
                              blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                          }
                       }
                }
               break;
            case TORRE:
                if(pTab->anima.colini == pTab->anima.colfim)
                {
                   for(i = linini; i <= linfim; i+=VEL )
                   {
                      imprime( pTab, imagens, db, fundo);
                      if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                        draw_rle_sprite(db, TORREBRANCO, colini + OFFSET, i + OFFSET2);
                      else
                        draw_rle_sprite(db, TORREPRETO, colini + OFFSET, i + OFFSET2);
                      blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                   }      
                   for(i = linini; i >= linfim; i-=VEL )
                   {
                      imprime( pTab, imagens, db, fundo );
                      if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                        draw_rle_sprite(db, TORREBRANCO, colini + OFFSET, i + OFFSET2);
                      else
                        draw_rle_sprite(db, TORREPRETO, colini + OFFSET, i + OFFSET2);
                      blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                   }
                }
                if(pTab->anima.colini != pTab->anima.colfim)
                {
                   for(i = colini; i <= colfim; i+=VEL )
                   {
                      imprime( pTab, imagens, db, fundo);
                      if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                        draw_rle_sprite(db, TORREBRANCO, i + OFFSET, linini + OFFSET2);
                      else
                        draw_rle_sprite(db, TORREPRETO, i + OFFSET, linini + OFFSET2);
                      blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                   }      
                   for(i = colini; i >= colfim; i-=VEL )
                   {
                      imprime( pTab, imagens, db, fundo );
                      if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                        draw_rle_sprite(db, TORREBRANCO, i + OFFSET, linini + OFFSET2);
                      else
                        draw_rle_sprite(db, TORREPRETO, i + OFFSET, linini + OFFSET2);
                      blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                   }      
                }
                   break;               
            case CAVALO:/*
                if(pTab->anima.linfim - pTab->anima.linini == 2 )
                {
                    if(pTab->anima.colini - pTab->anima.colfim == 1)
                    {
                           for(i = linini; i <= linfim; i+=VEL )
                           {
                              imprime( pTab, imagens, db, fundo);
                              if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                                draw_rle_sprite(db, TORREBRANCO, colini + OFFSET, i + OFFSET2);
                              else
                                draw_rle_sprite(db, TORREPRETO, colini + OFFSET, i + OFFSET2);
                              blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                            }
                           for(j = colini; j >= colfim; i+=VEL)
                           {
                              imprime( pTab, imagens, db, fundo);
                              if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                                draw_rle_sprite(db, TORREBRANCO, j + OFFSET, linfim + OFFSET2);
                              else
                                draw_rle_sprite(db, TORREPRETO, j + OFFSET, linfim + OFFSET2);
                              blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                            }
                    }
                    if(pTab->anima.colfim - pTab->anima.colini == -1)
                    {
                           for(i = linini; i <= linfim; i+=VEL )
                           {
                              imprime( pTab, imagens, db, fundo);
                              if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                                draw_rle_sprite(db, TORREBRANCO, colini + OFFSET, i + OFFSET2);
                              else
                                draw_rle_sprite(db, TORREPRETO, colini + OFFSET, i + OFFSET2);
                              blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                           }
                           for(j = colini; j <= colfim; i-=VEL)
                           {
                              imprime( pTab, imagens, db, fundo);
                              if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                                draw_rle_sprite(db, TORREBRANCO, j + OFFSET, linfim + OFFSET2);
                              else
                                draw_rle_sprite(db, TORREPRETO, j + OFFSET, linfim + OFFSET2);
                              blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                           }
                    }
                }
                if(pTab->anima.linini - pTab->anima.linfim == 2 )
                {
                    if(pTab->anima.colini - pTab->anima.colfim == 1)
                    {
                           for(i = linini; i >= linfim; i-=VEL )
                           {
                              imprime( pTab, imagens, db, fundo);
                              if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                                draw_rle_sprite(db, TORREBRANCO, colini + OFFSET, i + OFFSET2);
                              else
                                draw_rle_sprite(db, TORREPRETO, colini + OFFSET, i + OFFSET2);
                              blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                           }
                           for(j = colini; j >= colfim; i-=VEL)
                           {
                              imprime( pTab, imagens, db, fundo);
                              if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                                draw_rle_sprite(db, TORREBRANCO, j + OFFSET, linfim + OFFSET2);
                              else
                                draw_rle_sprite(db, TORREPRETO, j + OFFSET, linfim + OFFSET2);
                              blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                           }
                    }
                    if(pTab->anima.colfim - pTab->anima.colini == 1)
                    {
                           for(i = linini; i >= linfim; i-=VEL )
                           {
                              imprime( pTab, imagens, db, fundo);
                              if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                                draw_rle_sprite(db, TORREBRANCO, colini + OFFSET, i + OFFSET2);
                              else
                                draw_rle_sprite(db, TORREPRETO, colini + OFFSET, i + OFFSET2);
                              blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                           }
                           for(j = colini; j <= colfim; i+=VEL)
                           {
                              imprime( pTab, imagens, db, fundo);
                              if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                                draw_rle_sprite(db, TORREBRANCO, j + OFFSET, linfim + OFFSET2);
                              else
                                draw_rle_sprite(db, TORREPRETO, j + OFFSET, linfim + OFFSET2);
                              blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                           }
                    }
                }
                if(pTab->anima.colfim - pTab->anima.colini == 2 )
                {
                    if(pTab->anima.linini - pTab->anima.linfim == 1)
                    {
                           for(j = colini; j <= colfim; j+=VEL )
                           {
                              imprime( pTab, imagens, db, fundo);
                              if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                                draw_rle_sprite(db, TORREBRANCO, j + OFFSET, linini + OFFSET2);
                              else
                                draw_rle_sprite(db, TORREPRETO, j + OFFSET, linini + OFFSET2);
                              blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                           }
                           for(i = linini; i >= linfim; i-=VEL)
                           {
                              imprime( pTab, imagens, db, fundo);
                              if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                                draw_rle_sprite(db, TORREBRANCO, colfim + OFFSET, i + OFFSET2);
                              else
                                draw_rle_sprite(db, TORREPRETO, colfim + OFFSET, i + OFFSET2);
                              blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                           }
                    }
                    if(pTab->anima.colfim - pTab->anima.colini == 1)
                    {
                           for(j = colini; j <= colfim; i+=VEL )
                           {
                              imprime( pTab, imagens, db, fundo);
                              if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                                draw_rle_sprite(db, TORREBRANCO, j + OFFSET, linini + OFFSET2);
                              else
                                draw_rle_sprite(db, TORREPRETO, j + OFFSET, linini + OFFSET2);
                              blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                           }
                           for(i = linini; i <= linfim; i+=VEL)
                           {
                              imprime( pTab, imagens, db, fundo);
                              if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                                draw_rle_sprite(db, TORREBRANCO, colfim + OFFSET, i + OFFSET2);
                              else
                                draw_rle_sprite(db, TORREPRETO, colfim + OFFSET, i + OFFSET2);
                              blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                           }
                    }               
                }
                if(pTab->anima.colini - pTab->anima.colfim == -2 )
                {
                    if(pTab->anima.linini - pTab->anima.linfim == 1)
                    {
                           for(j = colini; j >= colfim; j-=VEL )
                           {
                              imprime( pTab, imagens, db, fundo);
                              if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                                draw_rle_sprite(db, TORREBRANCO, j + OFFSET, linini + OFFSET2);
                              else
                                draw_rle_sprite(db, TORREPRETO, j + OFFSET, linini + OFFSET2);
                              blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                           }
                           for(i = linini; i >= linfim; i-=VEL)
                           {
                              imprime( pTab, imagens, db, fundo);
                              if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                                draw_rle_sprite(db, TORREBRANCO, colfim + OFFSET, i + OFFSET2);
                              else
                                draw_rle_sprite(db, TORREPRETO, colfim + OFFSET, i + OFFSET2);
                              blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                           }
                    }
                    if(pTab->anima.colfim - pTab->anima.colini == -1)
                    {
                           for(j = colini; j >= colfim; i-=VEL )
                           {
                              imprime( pTab, imagens, db, fundo);
                              if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                                draw_rle_sprite(db, TORREBRANCO, j + OFFSET, linini + OFFSET2);
                              else
                                draw_rle_sprite(db, TORREPRETO, j + OFFSET, linini + OFFSET2);
                              blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                           }
                           for(i = linini; i <= linfim; i+=VEL)
                           {
                              imprime( pTab, imagens, db, fundo);
                              if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                                draw_rle_sprite(db, TORREBRANCO, colfim + OFFSET, i + OFFSET2);
                              else
                                draw_rle_sprite(db, TORREPRETO, colfim + OFFSET, i + OFFSET2);
                              blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                           }
                    }               
                }*/
               break;               
               
            case BISPO:
                       if( pTab->anima.linini < pTab->anima.linfim && pTab->anima.colini < pTab->anima.colfim )
                       {
                          for(i = linini , j = colini +1; i < linfim; i+=VEL,j+=VEL)
                          {
                              imprime( pTab, imagens, db, fundo );
                              if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                                draw_rle_sprite(db, BISPOBRANCO, j + OFFSET, i + OFFSET2);
                              else
                                draw_rle_sprite(db, BISPOPRETO, j + OFFSET, i + OFFSET2);
                              blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                          }
                       }
                       if( pTab->anima.linini < pTab->anima.linfim && pTab->anima.colini > pTab->anima.colfim )
                       {
                          for(i = linini , j = colini ; i < linfim; i+=VEL,j-=VEL)
                          {
                              imprime( pTab, imagens, db, fundo );
                              if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                                draw_rle_sprite(db, BISPOBRANCO, j + OFFSET, i + OFFSET2);
                              else
                                draw_rle_sprite(db, BISPOPRETO, j + OFFSET, i + OFFSET2);
                              blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                          }
                       }
                       if( pTab->anima.linini > pTab->anima.linfim && pTab->anima.colini < pTab->anima.colfim )
                       {
                          for(i = linini , j = colini ; i > linfim; i-=VEL,j+=VEL)
                          {
                              imprime( pTab, imagens, db, fundo );
                              if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                                draw_rle_sprite(db, BISPOBRANCO, j + OFFSET, i + OFFSET2);
                              else
                                draw_rle_sprite(db, BISPOPRETO, j + OFFSET, i + OFFSET2);
                              blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                          }
                       }
                       if( pTab->anima.linini > pTab->anima.linfim && pTab->anima.colini > pTab->anima.colfim )
                       {
                          for(i = linini , j = colini ; i > linfim; i-=VEL,j-=VEL)
                          {
                              imprime( pTab, imagens, db, fundo );
                              if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                                draw_rle_sprite(db, BISPOBRANCO, j + OFFSET, i + OFFSET2);
                              else
                                draw_rle_sprite(db, BISPOPRETO, j + OFFSET, i + OFFSET2);
                              blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                          }
                       }
               break;               
            case RAINHA:
                if((pTab->anima.colini == pTab->anima.colfim) && (pTab->anima.linini != pTab->anima.linfim))
                {
                   for(i = linini; i <= linfim; i+=VEL )
                   {
                      imprime( pTab, imagens, db, fundo);
                      if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                        draw_rle_sprite(db, RAINHABRANCO, colini + OFFSET, i + OFFSET2);
                      else
                        draw_rle_sprite(db, RAINHAPRETO, colini + OFFSET, i + OFFSET2);
                      blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                   }      
                   for(i = linini; i >= linfim; i-=VEL )
                   {
                      imprime( pTab, imagens, db, fundo );
                      if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                        draw_rle_sprite(db, RAINHABRANCO, colini + OFFSET, i + OFFSET2);
                      else
                        draw_rle_sprite(db, RAINHAPRETO, colini + OFFSET, i + OFFSET2);
                      blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                   }
                }
                if((pTab->anima.colini != pTab->anima.colfim) && (pTab->anima.linini == pTab->anima.linfim))
                {
                   for(i = colini; i <= colfim; i+=VEL )
                   {
                      imprime( pTab, imagens, db, fundo);
                      if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                        draw_rle_sprite(db, RAINHABRANCO, i + OFFSET, linini + OFFSET2);
                      else
                        draw_rle_sprite(db, RAINHAPRETO, i + OFFSET, linini + OFFSET2);
                      blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                   }      
                   for(i = colini; i >= colfim; i-=VEL )
                   {
                      imprime( pTab, imagens, db, fundo );
                      if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                        draw_rle_sprite(db, RAINHABRANCO, i + OFFSET, linini + OFFSET2);
                      else
                        draw_rle_sprite(db, RAINHAPRETO, i + OFFSET, linini + OFFSET2);
                      blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                   }      
                }
               if(pTab->anima.colini != pTab->anima.colfim)
               {
                       if( pTab->anima.linini < pTab->anima.linfim && pTab->anima.colini < pTab->anima.colfim )
                       {
                          for(i = linini , j = colini +1; i < linfim; i+=VEL,j+=VEL)
                          {
                              imprime( pTab, imagens, db, fundo );
                              if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                                draw_rle_sprite(db, RAINHABRANCO, j + OFFSET, i + OFFSET2);
                              else
                                draw_rle_sprite(db, RAINHAPRETO, j + OFFSET, i + OFFSET2);
                              blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                          }
                       }
                       if( pTab->anima.linini < pTab->anima.linfim && pTab->anima.colini > pTab->anima.colfim )
                       {
                          for(i = linini , j = colini ; i < linfim; i+=VEL,j-=VEL)
                          {
                              imprime( pTab, imagens, db, fundo );
                              if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                                draw_rle_sprite(db, RAINHABRANCO, j + OFFSET, i + OFFSET2);
                              else
                                draw_rle_sprite(db, RAINHAPRETO, j + OFFSET, i + OFFSET2);
                              blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                          }
                       }
                       if( pTab->anima.linini > pTab->anima.linfim && pTab->anima.colini < pTab->anima.colfim )
                       {
                          for(i = linini , j = colini ; i > linfim; i-=VEL,j+=VEL)
                          {
                              imprime( pTab, imagens, db, fundo );
                              if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                                draw_rle_sprite(db, RAINHABRANCO, j + OFFSET, i + OFFSET2);
                              else
                                draw_rle_sprite(db, RAINHAPRETO, j + OFFSET, i + OFFSET2);
                              blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                          }
                       }
                       if( pTab->anima.linini > pTab->anima.linfim && pTab->anima.colini > pTab->anima.colfim )
                       {
                          for(i = linini , j = colini ; i > linfim; i-=VEL,j-=VEL)
                          {
                              imprime( pTab, imagens, db, fundo );
                              if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                                draw_rle_sprite(db, RAINHABRANCO, j + OFFSET, i + OFFSET2);
                              else
                                draw_rle_sprite(db, RAINHAPRETO, j + OFFSET, i + OFFSET2);
                              blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                          }
                       }
                }

               break;               
            case REI:
                if((pTab->anima.colini == pTab->anima.colfim) && (pTab->anima.linini != pTab->anima.linfim))
                {
                   for(i = linini; i <= linfim; i+=VEL )
                   {
                      imprime( pTab, imagens, db, fundo);
                      if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                        draw_rle_sprite(db, REIBRANCO, colini + OFFSET, i + OFFSET2);
                      else
                        draw_rle_sprite(db, REIPRETO, colini + OFFSET, i + OFFSET2);
                      blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                   }      
                   for(i = linini; i >= linfim; i-=VEL )
                   {
                      imprime( pTab, imagens, db, fundo );
                      if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                        draw_rle_sprite(db, REIBRANCO, colini + OFFSET, i + OFFSET2);
                      else
                        draw_rle_sprite(db, REIPRETO, colini + OFFSET, i + OFFSET2);
                      blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                   }
                }
                if((pTab->anima.colini != pTab->anima.colfim) && (pTab->anima.linini == pTab->anima.linfim))
                {
                   for(i = colini; i <= colfim; i+=VEL )
                   {
                      imprime( pTab, imagens, db, fundo);
                      if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                        draw_rle_sprite(db, REIBRANCO, i + OFFSET, linini + OFFSET2);
                      else
                        draw_rle_sprite(db, REIPRETO, i + OFFSET, linini + OFFSET2);
                      blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                   }      
                   for(i = colini; i >= colfim; i-=VEL )
                   {
                      imprime( pTab, imagens, db, fundo );
                      if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                        draw_rle_sprite(db, REIBRANCO, i + OFFSET, linini + OFFSET2);
                      else
                        draw_rle_sprite(db, REIPRETO, i + OFFSET, linini + OFFSET2);
                      blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                   }      
                }
               if(pTab->anima.colini != pTab->anima.colfim)
               {
                       if( pTab->anima.linini < pTab->anima.linfim && pTab->anima.colini < pTab->anima.colfim )
                       {
                          for(i = linini , j = colini +1; i < linfim; i+=VEL,j+=VEL)
                          {
                              imprime( pTab, imagens, db, fundo );
                              if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                                draw_rle_sprite(db, REIBRANCO, j + OFFSET, i + OFFSET2);
                              else
                                draw_rle_sprite(db, REIPRETO, j + OFFSET, i + OFFSET2);
                              blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                          }
                       }
                       if( pTab->anima.linini < pTab->anima.linfim && pTab->anima.colini > pTab->anima.colfim )
                       {
                          for(i = linini , j = colini ; i < linfim; i+=VEL,j-=VEL)
                          {
                              imprime( pTab, imagens, db, fundo );
                              if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                                draw_rle_sprite(db, REIBRANCO, j + OFFSET, i + OFFSET2);
                              else
                                draw_rle_sprite(db, REIPRETO, j + OFFSET, i + OFFSET2);
                              blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                          }
                       }
                       if( pTab->anima.linini > pTab->anima.linfim && pTab->anima.colini < pTab->anima.colfim )
                       {
                          for(i = linini , j = colini ; i > linfim; i-=VEL,j+=VEL)
                          {
                              imprime( pTab, imagens, db, fundo );
                              if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                                draw_rle_sprite(db, REIBRANCO, j + OFFSET, i + OFFSET2);
                              else
                                draw_rle_sprite(db, REIPRETO, j + OFFSET, i + OFFSET2);
                              blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                          }
                       }
                       if( pTab->anima.linini > pTab->anima.linfim && pTab->anima.colini > pTab->anima.colfim )
                       {
                          for(i = linini , j = colini ; i > linfim; i-=VEL,j-=VEL)
                          {
                              imprime( pTab, imagens, db, fundo );
                              if ( pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador == BRANCO )
                                draw_rle_sprite(db, REIBRANCO, j + OFFSET, i + OFFSET2);
                              else
                                draw_rle_sprite(db, REIPRETO, j + OFFSET, i + OFFSET2);
                              blit( db, screen, 0, 0, 0, 0 , db->w , db->h);
                          }
                       }
                }

               break;      
    }         
    

   show_mouse(db);

   // copiando destindo da origem
   pTab->tabuleiro[pTab->anima.linfim][pTab->anima.colfim].codJogador = pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador;
	pTab->tabuleiro[pTab->anima.linfim][pTab->anima.colfim].codPeca = pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codPeca;
	// e o início, agora é vazio
	pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codPeca = VAZIO;
   pTab->tabuleiro[pTab->anima.linini][pTab->anima.colini].codJogador = VAZIO;

      pTab->anima.linini = pTab->anima.linfim = -1;
      pTab->anima.colini = pTab->anima.colfim = -1;
   }
}


void imprimeCoroamento(stTabuleiro *pTab, BITMAP *db, DATAFILE *imagens, int coroamento)
{
     if(pTab->codJogadorVEZ == BRANCO)
     {
         if(coroamento == 1)
         {
            draw_rle_sprite(db, RAINHABRANCO, 840, 220);
         }
         if(coroamento == 10)
         {
            draw_rle_sprite(db, CAVALOBRANCO, 840, 220);
         }
         if(coroamento == 11)
         {
            draw_rle_sprite(db, RAINHABRANCO, 830, 220);
            draw_rle_sprite(db, CAVALOBRANCO, 905, 220);
         }
         if(coroamento == 100)
         {
            draw_rle_sprite(db, BISPOBRANCO, 840, 220);
         }
         if(coroamento == 101)
         {
            draw_rle_sprite(db, BISPOBRANCO, 830, 220);
            draw_rle_sprite(db, RAINHABRANCO, 905, 220);
         }
         if(coroamento == 110)
         {
            draw_rle_sprite(db, BISPOBRANCO , 830, 220);
            draw_rle_sprite(db, CAVALOBRANCO , 905, 220);
         }
         if(coroamento == 111)
         {
            draw_rle_sprite(db, BISPOBRANCO , 830, 220);
            draw_rle_sprite(db, CAVALOBRANCO , 905, 220);
            draw_rle_sprite(db, RAINHABRANCO , 830, 315);
         }
         if(coroamento == 1000)
         {
            draw_rle_sprite(db, TORREBRANCO, 840, 220);
         }
         if(coroamento == 1001)
         {
            draw_rle_sprite(db, TORREBRANCO, 830, 220);
            draw_rle_sprite(db, RAINHABRANCO, 905, 220);
         }
         if(coroamento == 1010)
         {
            draw_rle_sprite(db, TORREBRANCO , 830, 220);
            draw_rle_sprite(db, CAVALOBRANCO , 905, 220);
         }
         if(coroamento == 1011)
         {
            draw_rle_sprite(db, TORREBRANCO , 830, 220);
            draw_rle_sprite(db, CAVALOBRANCO , 905, 220);
            draw_rle_sprite(db, RAINHABRANCO , 830, 315);
         }
         if(coroamento == 1100)
         {
            draw_rle_sprite(db, TORREBRANCO, 830, 220);
            draw_rle_sprite(db, BISPOBRANCO, 905, 220);
         }
         if(coroamento == 1101)
         {
            draw_rle_sprite(db, TORREBRANCO , 830, 220);
            draw_rle_sprite(db, BISPOBRANCO , 905, 220);
            draw_rle_sprite(db, RAINHABRANCO , 830, 315);
         }
         if(coroamento == 1110)
         {
            draw_rle_sprite(db, TORREBRANCO, 830, 220);
            draw_rle_sprite(db, BISPOBRANCO , 905, 220);
            draw_rle_sprite(db, CAVALOBRANCO , 830, 315);
         }
         if(coroamento == 1111)
         {
            draw_rle_sprite(db, TORREBRANCO, 830, 220);
            draw_rle_sprite(db, BISPOBRANCO , 905, 220);
            draw_rle_sprite(db, CAVALOBRANCO , 830, 315);
            draw_rle_sprite(db, RAINHABRANCO , 905 , 315);
         }
     }
     if(pTab->codJogadorVEZ == PRETO)
     {
         if(coroamento == 1)
         {
            draw_rle_sprite(db, RAINHAPRETO, 840, 220);
         }
         if(coroamento == 10)
         {
            draw_rle_sprite(db, CAVALOPRETO, 840, 220);
         }
         if(coroamento == 11)
         {
            draw_rle_sprite(db, RAINHAPRETO, 830, 220);
            draw_rle_sprite(db, CAVALOPRETO, 905, 220);
         }
         if(coroamento == 100)
         {
            draw_rle_sprite(db, BISPOPRETO, 840, 220);
         }
         if(coroamento == 101)
         {
            draw_rle_sprite(db, BISPOPRETO, 830, 220);
            draw_rle_sprite(db, RAINHAPRETO, 905, 220);
         }
         if(coroamento == 110)
         {
            draw_rle_sprite(db, BISPOPRETO , 830, 220);
            draw_rle_sprite(db, CAVALOPRETO , 905, 220);
         }
         if(coroamento == 111)
         {
            draw_rle_sprite(db, BISPOPRETO , 830, 220);
            draw_rle_sprite(db, CAVALOPRETO , 905, 220);
            draw_rle_sprite(db, RAINHAPRETO , 830, 315);
         }
         if(coroamento == 1000)
         {
            draw_rle_sprite(db, TORREPRETO, 840, 220);
         }
         if(coroamento == 1001)
         {
            draw_rle_sprite(db, TORREPRETO, 830, 220);
            draw_rle_sprite(db, RAINHAPRETO, 905, 220);
         }
         if(coroamento == 1010)
         {
            draw_rle_sprite(db, TORREPRETO , 830, 220);
            draw_rle_sprite(db, CAVALOPRETO , 905, 220);
         }
         if(coroamento == 1011)
         {
            draw_rle_sprite(db, TORREPRETO , 830, 220);
            draw_rle_sprite(db, CAVALOPRETO , 905, 220);
            draw_rle_sprite(db, RAINHAPRETO , 830, 315);
         }
         if(coroamento == 1100)
         {
            draw_rle_sprite(db, TORREPRETO, 830, 220);
            draw_rle_sprite(db, BISPOBRANCO , 905, 220);
         }
         if(coroamento == 1101)
         {
            draw_rle_sprite(db, TORREPRETO , 830, 220);
            draw_rle_sprite(db, BISPOPRETO , 905, 220);
            draw_rle_sprite(db, RAINHAPRETO , 830, 315);
         }
         if(coroamento == 1110)
         {
            draw_rle_sprite(db, TORREPRETO, 830, 220);
            draw_rle_sprite(db, BISPOPRETO , 905, 220);
            draw_rle_sprite(db, CAVALOPRETO , 830, 315);
         }
         if(coroamento == 1111)
         {
            draw_rle_sprite(db, TORREPRETO, 830, 220);
            draw_rle_sprite(db, BISPOPRETO , 905, 220);
            draw_rle_sprite(db, CAVALOPRETO , 830, 315);
            draw_rle_sprite(db, RAINHAPRETO , 905 , 315);
         }
     }
}

void imprimeRoque(stTabuleiro *pTab, BITMAP *db, DATAFILE *imagens, int roque)
{
    if(pTab->codJogadorVEZ == BRANCO)
    {
         if(roque == 10)
        {
             draw_rle_sprite(db, ROQUEMAIOR, 800, 100);
        }
        else if (roque == 1)
        {
             draw_rle_sprite(db, ROQUEMENOR, 925, 100);
        }
        else if(roque == 11)
        {
             draw_rle_sprite(db, ROQUEMAIOR, 800, 100);
             draw_rle_sprite(db, ROQUEMENOR, 925,  100);
        }
    }
    if(pTab->codJogadorVEZ == PRETO)
    {
         if(roque == 10)
        {
             draw_rle_sprite(db, ROQUEMAIOR, 800, 650);
        }
        else if (roque == 1)
        {
             draw_rle_sprite(db, ROQUEMENOR, 925, 650);
        }
        else if(roque == 11)
        {
             draw_rle_sprite(db, ROQUEMAIOR, 800, 650);
             draw_rle_sprite(db, ROQUEMENOR, 925,  650);
        }
    }
    
}

int checaCoroamento(int coroamento)
{
   int mx, my;

   mx = my = 0;

   if(pegamouse(&my, &mx))
   {

         if(coroamento == 1)
           if( 830 < mx && mx < 875 && 220 < my && my < 295)
              return 1;

         if(coroamento == 10)
           if( 830 < mx && mx < 875 && 220 < my && my < 295)
              return 2;

         if(coroamento == 11)
         {
                       if( 830 < mx && mx < 875 && 220 < my && my < 295)
                       {
                           return 1;
                       }
                       if( 905 < mx && mx < 950 && 220 < my && my < 295)
                       {
                           return 2;
                       }
         }
         if(coroamento == 100)
         {
                       if( 830 < mx && mx < 875 && 220 < my && my < 295)
                       {
                           return 3;
                       }
         }
         if(coroamento == 101)
         {
                       if( 830 < mx && mx < 875 && 220 < my && my < 295)
                       {
                           return 3;
                       }
                       if( 905 < mx && mx < 950 && 220 < my && my < 295)
                       {
                           return 1;
                       }
         }
         if(coroamento == 110)
         {
                       if( 830 < mx && mx < 875 && 220 < my && my < 295)
                       {
                           return 3;
                       }
                       if( 905 < mx && mx < 950 && 220 < my && my < 295)
                       {
                           return 2;
                       }
         }
         if(coroamento == 111)
         {
                       if( 830 < mx && mx < 875 && 220 < my && my < 295)
                       {
                           return 3;
                       }
                       if( 905 < mx && mx < 950 && 220 < my && my < 295)
                       {
                           return 2;
                       }
                       if( 830 < mx && mx < 875 && 315 < my && my < 390)
                       {
                           return 1;
                       }
         }
         if(coroamento == 1000)
         {
                       if( 830 < mx && mx < 875 && 220 < my && my < 295)
                       {
                           return 4;
                       }
         }
         if(coroamento == 1001)
         {
                       if( 830 < mx && mx < 875 && 220 < my && my < 295)
                       {
                           return 4;
                       }
                       if( 905 < mx && mx < 950 && 220 < my && my < 295)
                       {
                           return 1;
                       }
         }
         if(coroamento == 1010)
         {
                       if( 830 < mx && mx < 875 && 220 < my && my < 295)
                       {
                           return 4;
                       }
                       if( 905 < mx && mx < 950 && 220 < my && my < 295)
                       {
                           return 2;
                       }
         }
         if(coroamento == 1011)
         {
                       if( 830 < mx && mx < 875 && 220 < my && my < 295)
                       {
                           return 4;
                       }
                       if( 905 < mx && mx < 950 && 220 < my && my < 295)
                       {
                           return 2;
                       }
                       if( 830 < mx && mx < 875 && 315 < my && my < 390)
                       {
                           return 1;
                       }
         }
         if(coroamento == 1100)
         {
                       if( 830 < mx && mx < 875 && 220 < my && my < 295)
                       {
                           return 4;
                       }
                       if( 905 < mx && mx < 950 && 220 < my && my < 295)
                       {
                           return 3;
                       }
         }
         if(coroamento == 1101)
         {
                       if( 830 < mx && mx < 875 && 220 < my && my < 295)
                       {
                           return 4;
                       }
                       if( 905 < mx && mx < 950 && 220 < my && my < 295)
                       {
                           return 3;
                       }
                       if( 830 < mx && mx < 875 && 315 < my && my < 390)
                       {
                           return 1;
                       }
         }
         if(coroamento == 1110)
         {
                       if( 830 < mx && mx < 875 && 220 < my && my < 295)
                       {
                           return 4;
                       }
                       if( 905 < mx && mx < 950 && 220 < my && my < 295)
                       {
                           return 3;
                       }
                       if( 830 < mx && mx < 875 && 315 < my && my < 390)
                       {
                           return 2;
                       }
         }
         if(coroamento == 1111)
         {
                       if( 830 < mx && mx < 875 && 220 < my && my < 295)
                       {
                           return 4;
                       }
                       if( 905 < mx && mx < 950 && 220 < my && my < 295)
                       {
                           return 3;
                       }
                       if( 830 < mx && mx < 875 && 315 < my && my < 390)
                       {
                           return 2;
                       }
                       if( 905 < mx && mx < 950 && 315 < my && my < 390)
                       {
                           return 1;
                       }
         }
     }

   return 0;
}

int checaRoque(stTabuleiro *pTab ,int roque)
{
   int ret = 0;
   int mx, my;

   mx = my = 0;
   
   
   if(pTab->codJogadorVEZ == BRANCO)
   {
       if ( pegamouse(&my, &mx) )
       {
          if( 800 < mx && mx < 895 && 100 < my && my < 120 )
             if(roque == 10 || roque == 11  )
                 ret = 1;
    
          if( 925 < mx && mx < 1020 && 100 < my && my < 120 )
             if(roque == 1 || roque == 11  )
                 ret = 2;
       }
    }
   if(pTab->codJogadorVEZ == PRETO)
   {
       if ( pegamouse(&my, &mx) )
       {
          if( 800 < mx && mx < 895 && 650 < my && my < 670 )
             if(roque == 10 || roque == 11  )
                 ret = 1;
    
          if( 925 < mx && mx < 1020 && 650 < my && my < 670 )
             if(roque == 1 || roque == 11  )
                 ret = 2;
       }
    }


   return ret;
}

int fazCoroamento(stTabuleiro *pTab ,BITMAP *db, DATAFILE *imagens , int fazcoroamento)
{
    int i;

    for(i = 0; i < TAM; i++)
    {
        if(pTab->tabuleiro[7][i].codPeca == PEAO && pTab->tabuleiro[7][i].codJogador == pTab->codJogadorVEZ)
        {
            if(fazcoroamento == 1)
            {
                pTab->tabuleiro[7][i].codPeca = RAINHA;
                pTab->FezCoroamentoBranco = 1;
		      	return 1;
            }
            if(fazcoroamento == 2)
            {
                pTab->tabuleiro[7][i].codPeca = CAVALO;
                pTab->FezCoroamentoBranco = 2;
		      	return 1;
            }
            if(fazcoroamento == 3)
            {
                pTab->tabuleiro[7][i].codPeca = BISPO;
                pTab->FezCoroamentoBranco = 3;
		      	return 1;
            }
            if(fazcoroamento == 4)
            {
                pTab->tabuleiro[7][i].codPeca = TORRE;
                pTab->FezCoroamentoBranco = 4;
		      	return 1;
            }
        }                
        if(pTab->tabuleiro[0][i].codPeca == PEAO && pTab->tabuleiro[0][i].codJogador == pTab->codJogadorVEZ)
        {
            if(fazcoroamento == 1)
            {
                pTab->tabuleiro[0][i].codPeca = RAINHA;
                pTab->FezCoroamentoPreto = 1;
		      	return 1;
            }
            if(fazcoroamento == 2)
            {
                pTab->tabuleiro[0][i].codPeca = CAVALO;
                pTab->FezCoroamentoPreto = 2;
		      	return 1;
            }
            if(fazcoroamento == 3)
            {
                pTab->tabuleiro[0][i].codPeca = BISPO;
                pTab->FezCoroamentoPreto = 3;
		      	return 1;
            }
            if(fazcoroamento == 4)
            {
                pTab->tabuleiro[0][i].codPeca = TORRE;
                pTab->FezCoroamentoBranco = 4;
		      	return 1;
            }
        }                
    }
    return 0;
}

void gravaReplay( stTabuleiro **ppTab, stTabuleiro *pTabOrig, int *n)
{
    int lin, col;

    if ((*n) == 0)
       *ppTab = (stTabuleiro *) malloc (sizeof(stTabuleiro));
    else
       *ppTab = (stTabuleiro *) realloc(*ppTab, ((*n)+1) * sizeof(stTabuleiro));

    stTabuleiro *pTab = &((*ppTab)[*n]);

    for( lin = 0; lin < TAM; lin++ )
    {
 		for( col = 0; col < TAM; col++ )
		{
           pTab->tabuleiro[lin][col].codPeca = pTabOrig->tabuleiro[lin][col].codPeca;
           pTab->tabuleiro[lin][col].codJogador = pTabOrig->tabuleiro[lin][col].codJogador;
  		}
	}

    pTab->roqueGBranco = 1 ;
 	 pTab->roquePBranco = 1;
	 pTab->roqueGPreto = 1;
	 pTab->roquePPreto = 1;
    pTab->enPassantBranco = -1 ;
    pTab->enPassantPreto = -1 ;
    pTab->anima.linini = pTab->anima.colini = -1;
    pTab->anima.linfim = pTab->anima.colfim = -1;

   (*n)++;
}

void tocaReplay( const stTabuleiro *pTab, DATAFILE *imagens, int n)
{
   int op, cont = 0;

	BITMAP *db = create_bitmap(SCREEN_W, SCREEN_H);

   show_mouse(NULL);
   alert("Use o as setas do teclado para ver as jogadas.", NULL, NULL, "OK", NULL ,0,1);

   do {

      imprime( &(pTab[cont]), imagens, db, 1);
      blit( db, screen, 0, 0, 0, 0 , db->w , db->h);

      clear_keybuf();

      op = readkey();

      if ((op >> 8) == KEY_LEFT) cont--;
      if ((op >> 8) == KEY_RIGHT) cont++;

      if (cont <=0) cont = 0;
      if (cont >= n) cont = n - 1;

   } while ((op >> 8) != KEY_ESC);
}
void animaBotao(stTabuleiro *pTab ,int roque,DATAFILE *imagens, BITMAP *db )
{
   if (mouse_b & 1)
   {
        if(roque > 0)
        {
            if(pTab->codJogadorVEZ == BRANCO)
            {
                  if( 800 < mouse_x && mouse_x < 895 && 100 < mouse_y && mouse_y <120  )
                      if(roque == 10 || roque == 11  )
                          draw_rle_sprite(db, imagens[3].dat, 800, 100);
                  if( 925 < mouse_x && mouse_x < 1020 && 100 < mouse_y && mouse_y < 120  )
                       if(roque == 1 || roque == 11  )
                           draw_rle_sprite(db, imagens[5].dat, 925, 100);                        
            }
            if(pTab->codJogadorVEZ == PRETO)
            {
                  if( 800 < mouse_x && mouse_x < 895 && 650 < mouse_y && mouse_y < 670  )
                      if(roque == 10 || roque == 11  )
                           draw_rle_sprite(db, imagens[3].dat, 800, 650);                        
                  if( 925 < mouse_x && mouse_x < 1020 && 650 < mouse_y && mouse_y < 670  )
                      if(roque == 1 || roque == 11  )
                          draw_rle_sprite(db, imagens[5].dat, 925, 650);                        
            }
        }
   }
   return;
    
}

int jogPretoshow()
{
    int op;
    
    op = alert("Parabens jogador Preto, voce ganhou!!!!", NULL, NULL, "OK","Replay",0,2);
    return op;
}

int jogBrancoshow()
{
    int op;

    op = alert("Parabens jogador Branco, voce ganhou!!!!", NULL, NULL, "OK","Replay",0,2);
    return op;
}

int ExecutaShow( int (*pFuncaoLocal)() )
{
   (*pFuncaoLocal)();
}


