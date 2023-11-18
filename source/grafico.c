#include "grafico.h"
#include <allegro.h>

int IniciaAllegro()
{
   DATAFILE *imagens;

   if (allegro_init() != 0)
      return 0;

   install_keyboard();
   install_mouse();
   install_timer();
   set_color_depth(24);
   
   imagens = load_datafile("imagens.dat");
   if (!imagens) 
   {
    allegro_exit();
    allegro_message("Nao foi possivel carregar imagens.");
    return 0;
    }


   if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1024, 768, 0, 0) != 0) 
   {
      set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
      allegro_message("Nao foi possivel selecionar modo grafico algum.\n%s\n", allegro_error);
      return 0;
   }
   set_pallete(imagens[21].dat);
   set_window_title("Jogo de Xadrez(ESC para SAIR)");

   return 1;
}

int pegamouse(int *lin, int *col)
{
   if (mouse_b & 1)
   {
      *lin = mouse_y;
      *col = mouse_x;
      return 1;
   }
   return 0;
}



