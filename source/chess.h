#ifndef __CHESS_H__
#define __CHESS_H__

#include <allegro.h>

#define VEL     5
#define VAZIO	 -1
#define PEAO	 0
#define TORRE	 1
#define CAVALO  2
#define BISPO   3
#define RAINHA  4
#define REI     5

#define BT_ROQUEMAIOR_L                  220
#define BT_ROQUEMAIOR_C                  840
#define BT_ROQUEMENOR_L                  120
#define BT_ROQUEMENOR_C                  840


#define PEAOBRANCO imagens[22].dat
#define PEAOPRETO  imagens[23].dat
#define TORREBRANCO imagens[30].dat
#define TORREPRETO  imagens[31].dat
#define CAVALOBRANCO imagens[6].dat
#define CAVALOBRANCO2 imagens[7].dat
#define CAVALOBRANCO3 imagens[8].dat
#define CAVALOPRETO   imagens[9].dat
#define CAVALOPRETO2  imagens[10].dat
#define CAVALOPRETO3  imagens[11].dat
#define BISPOBRANCO imagens[0].dat
#define BISPOPRETO  imagens[1].dat
#define RAINHABRANCO imagens[24].dat
#define RAINHAPRETO imagens[25].dat
#define REIBRANCO  imagens[26].dat
#define REIPRETO  imagens[27].dat
#define ROQUEMAIOR  imagens[2].dat
#define ROQUEMENOR  imagens[4].dat

#define BRANCO  0
#define PRETO   1

#define TAM 8
#define TAMCASA  96



typedef struct tagstAnimacao
{
   int linini;
   int colini;
   int linfim;
   int colfim;

} stAnimacao;


typedef struct tagstCasa
{
	int codPeca;
	int codJogador;
} stCasa;



typedef struct tagstTabuleiro
{
	int		codJogadorVEZ;
	stCasa	tabuleiro[TAM][TAM];
   stAnimacao anima;
	int      roqueGBranco;
	int      roquePBranco;
	int      roqueGPreto;
	int      roquePPreto;
	int      enPassantBranco;
	int      enPassantPreto;
	int      FezroqueGBranco;
	int      FezroquePBranco;
	int      FezroqueGPreto;
	int      FezroquePPreto;
	int      FezCoroamentoBranco;
	int      FezCoroamentoPreto;
	int      FezPassantBranco;
	int      FezPassantPreto;
	
} stTabuleiro;

void inicializa( stTabuleiro **ppTab );
void imprime( const stTabuleiro *pTab, DATAFILE *imagens, BITMAP *db, int fundo);
//void gravaLog( stTabuleiro *pTab, int posLinIni, int posColIni, int posLinFim, int posColFim, int jogada, int jogoAcabou, int comeca );
void Animacao(stTabuleiro *pTab, BITMAP *db, DATAFILE *imagens, int fundo);
void imprimeCoroamento(stTabuleiro *pTab, BITMAP *db, DATAFILE *imagens, int coroamento);
void imprimeRoque(stTabuleiro *pTab, BITMAP *db, DATAFILE *imagens, int roque);

int joga( stTabuleiro *pTab, int posLinIni, int posColIni, int posLinFin, int posColFin );
int reiIsAlive( stTabuleiro *pTab);
int checaRoque(stTabuleiro *pTab ,int roque);
int checaCoroamento(int coroamento);
int fazCoroamento( stTabuleiro *pTab ,BITMAP *db, DATAFILE *imagens , int fazcoroamento );
int fazRoque(stTabuleiro *pTab , int fazroque);
int podeRoque(stTabuleiro *pTab);
int podeCoroamento(stTabuleiro *pTab);

int jogPretoshow();
int jogBrancoshow();
int ExecutaShow( int (*pFuncaoLocal)() );


void gravaReplay( stTabuleiro **ppTab, stTabuleiro *pTabOrig, int *n);
void tocaReplay( const stTabuleiro *pTab, DATAFILE *imagens, int n);
void animaBotao(stTabuleiro *pTab ,int roque,DATAFILE *imagens, BITMAP *db );

int movePeao( stTabuleiro *pTab, int posLinIni, int posColIni, int posLinFin, int posColFin );
int moveTorre( stTabuleiro *pTab, int posLinIni, int posColIni, int posLinFin, int posColFin );
int moveCavalo( stTabuleiro *pTab, int posLinIni, int posColIni, int posLinFim, int posColFim );
int moveBispo( stTabuleiro *pTab, int posLinIni, int posColIni, int posLinFim, int posColFim );
int moveRainha( stTabuleiro *pTab, int posLinIni, int posColIni, int posLinFim, int posColFim );
int moveRei( stTabuleiro *pTab, int posLinIni, int posColIni, int posLinFim, int posColFim );




#endif
