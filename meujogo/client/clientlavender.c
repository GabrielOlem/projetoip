#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <stdlib.h>
#include <stdio.h>
#include "client.h"
#include <ctype.h>

#define w 1200
#define h 900
#define width 400
#define height 300
#define monstro 4

typedef struct{
    int x,y;
    int vivo;
    int plano;
}chave;
typedef struct{
    int x,y;
    int inicialx,inicialy,planoi;
    double speed;
    int plano;
    int lado;
    int id;
    int vidas;
    int vivo;
    int key;
    char tecla;
    chave chaves[5];
    int acabou;
    int abriu;
    int monstrowin;
    int monstroloose;
}jogador;
/*
lado 0 == frente
lado 1 == costa
lado 2 == esquerda
lado 3 == direita
*/
enum KEYS{UP,DOWN,LEFT,RIGHT};
const int fps=60;
bool done = false;
bool redraw = true;
int tjogo = 150;

ALLEGRO_DISPLAY *menudisplay = NULL;
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_DISPLAY *end = NULL;
ALLEGRO_EVENT menu;
ALLEGRO_EVENT ev;
ALLEGRO_EVENT_QUEUE *fila = NULL, *filamenu = NULL;
ALLEGRO_BITMAP *gbutton = NULL, *bbutton = NULL, *background = NULL;
ALLEGRO_BITMAP *nfrente=NULL;
ALLEGRO_BITMAP *ncosta=NULL;
ALLEGRO_BITMAP *ndireita=NULL;
ALLEGRO_BITMAP *nesquerda=NULL;
ALLEGRO_BITMAP *mapa=NULL;
ALLEGRO_BITMAP *mapaClosedGate=NULL;
ALLEGRO_BITMAP *mapaOpenGate=NULL;
ALLEGRO_BITMAP *hearta=NULL;
ALLEGRO_BITMAP *heartb=NULL;
ALLEGRO_BITMAP *key=NULL;
ALLEGRO_BITMAP *Win=NULL;
ALLEGRO_BITMAP *Defeat=NULL;
ALLEGRO_BITMAP *mao=NULL;
ALLEGRO_BITMAP *frontm = NULL, *frontp1 = NULL, *frontp2 = NULL, *frontp3 = NULL, *frontp4 = NULL;
ALLEGRO_TIMER *timer=NULL;
ALLEGRO_FONT *fonte=NULL, *fontemenor=NULL, *fonteportao=NULL;
ALLEGRO_FONT *sans = NULL, *titlesans = NULL, *mediumsans = NULL, *sansmenor = NULL;
ALLEGRO_AUDIO_STREAM *bg = NULL, *gamesong = NULL;
ALLEGRO_SAMPLE *button = NULL;
ALLEGRO_SAMPLE *svictory = NULL, *sdefeat = NULL, *sdead = NULL, *skill = NULL;

void cal_destroy_audio()
{
    al_destroy_sample(svictory);
    al_destroy_sample(sdefeat);
    al_destroy_sample(sdead);
    al_destroy_sample(skill);
    al_destroy_sample(button);
    al_destroy_audio_stream(bg);
    al_destroy_audio_stream(gamesong);
}

void cal_import_bitmap (char *file, ALLEGRO_BITMAP **name)
{
   *name = al_load_bitmap(file);

   if(!*name)
    {
      *name = al_load_bitmap("menu/error.png");
      if(!*name)
      {
          al_destroy_display(menudisplay);
          al_destroy_display(display);
          al_destroy_display(end);
          cal_destroy_audio();
          printf("\n\nIMAGE ERROR!\n\n");
          exit(1);
      }
    }
}

void cal_mass_destroy()
{
    al_destroy_bitmap(gbutton);
    al_destroy_bitmap(bbutton);
    al_destroy_bitmap(background);
    al_destroy_bitmap(mapa);
    al_destroy_bitmap(mapaOpenGate);
    al_destroy_bitmap(mapaClosedGate);
    al_destroy_bitmap(hearta);
    al_destroy_bitmap(heartb);
    al_destroy_bitmap(key);
    al_destroy_bitmap(Win);
    al_destroy_bitmap(Defeat);
    al_destroy_bitmap(mao);
    al_destroy_bitmap(frontm);
    al_destroy_bitmap(frontp1);
    al_destroy_bitmap(frontp2);
    al_destroy_bitmap(frontp3);
    al_destroy_bitmap(frontp4);
    al_destroy_bitmap(nfrente);
    al_destroy_bitmap(ncosta);
    al_destroy_bitmap(ndireita);
    al_destroy_bitmap(nesquerda);
    al_destroy_timer(timer);
    al_destroy_font(sans);
    al_destroy_font(titlesans);
    al_destroy_font(sansmenor);
    al_destroy_font(fonte);
    al_destroy_font(fontemenor);
    al_destroy_display(menudisplay);
    al_destroy_display(display);
    al_destroy_display(end);
    cal_destroy_audio();
}

void cal_init_event_queues()
{
    filamenu = al_create_event_queue();
    if (!filamenu)
    {
        al_destroy_display(menudisplay);
        exit(1);
    }

    fila = al_create_event_queue();
    if (!fila)
    {
        al_destroy_display(menudisplay);
        exit(1);
    }

    al_register_event_source(filamenu, al_get_mouse_event_source());
    al_register_event_source(filamenu, al_get_keyboard_event_source());
    al_register_event_source(fila, al_get_keyboard_event_source());
    al_register_event_source(fila, al_get_timer_event_source(timer));
}

//Funcao para quando receber do server no inicio
void printRec(jogador pessoa){
    switch(pessoa.lado){
        case 0:
            al_draw_bitmap(nfrente,pessoa.x,pessoa.y,0);
            break;
        case 1:
            al_draw_bitmap(ncosta,pessoa.x,pessoa.y,0);
            break;
        case 2:
            al_draw_bitmap(nesquerda,pessoa.x,pessoa.y,0);
            break;
        case 3:
            al_draw_bitmap(ndireita,pessoa.x,pessoa.y,0);
            break;
    }
}
void printTela(int plano){//Fun��o para printar a tela
    switch(plano){
        case 0:
            al_draw_bitmap(mapa,0,0,0);
            break;
        case 1:
            al_draw_bitmap(mapa,-width,0,0);
            break;
        case 2:
            al_draw_bitmap(mapa,-2*width,0,0);
            break;
        case 3:
            al_draw_bitmap(mapa,0,-height,0);
            break;
        case 4:
            al_draw_bitmap(mapa,-width,-height,0);
            break;
        case 5:
            al_draw_bitmap(mapa,-2*width,-height,0);
            break;
        case 6:
            al_draw_bitmap(mapa,0,-2*height,0);
            break;
        case 7:
            al_draw_bitmap(mapa,-width,-2*height,0);
            break;
        case 8:
            al_draw_bitmap(mapa,-2*width,-2*height,0);
    }
}
void leSkin(int numero){
    switch(numero){
        case 0:
            nfrente=al_load_bitmap("Personagens/p1/p1frente.png");
            ncosta=al_load_bitmap("Personagens/p1/p1costas.png");
            ndireita=al_load_bitmap("Personagens/p1/p1direita.png");
            nesquerda=al_load_bitmap("Personagens/p1/p1esquerda.png");
            break;
        case 1:
            nfrente=al_load_bitmap("Personagens/p2/p2frente.png");
            ncosta=al_load_bitmap("Personagens/p2/p2costas.png");
            ndireita=al_load_bitmap("Personagens/p2/p2direita.png");
            nesquerda=al_load_bitmap("Personagens/p2/p2esquerda.png");
            break;
        case 2:
            nfrente=al_load_bitmap("Personagens/p3/p3frente.png");
            ncosta=al_load_bitmap("Personagens/p3/p3costas.png");
            ndireita=al_load_bitmap("Personagens/p3/p3direita.png");
            nesquerda=al_load_bitmap("Personagens/p3/p3esquerda.png");
            break;
        case 3:
            nfrente=al_load_bitmap("Personagens/p4/p4frente.png");
            ncosta=al_load_bitmap("Personagens/p4/p4costas.png");
            ndireita=al_load_bitmap("Personagens/p4/p4direita.png");
            nesquerda=al_load_bitmap("Personagens/p4/p4esquerda.png");
            break;
        case 4://Monstro
            nfrente=al_load_bitmap("Personagens/monstro/mfrente.png");
            ncosta=al_load_bitmap("Personagens/monstro/mcostas.png");
            ndireita=al_load_bitmap("Personagens/monstro/mdireita.png");
            nesquerda=al_load_bitmap("Personagens/monstro/mesquerda.png");
            break;
    }
}
void printStatusm(jogador pessoa[5]){
    int c1,c2=1,c3=1,c4=1,c5=1;
    for(c1=0;c1<5;c1++){
        if(pessoa[0].vidas==0){
            c2=0;
        }
        if(pessoa[1].vidas==0){
            c3=0;
        }
        if(pessoa[2].vidas==0){
            c4=0;
        }
        if(pessoa[3].vidas==0){
            c5=0;
        }
        if(pessoa[c1].id==monstro){
            if(c2!=0){
                al_draw_line(17,15,22,15,al_map_rgb(0,184,230),10);
            }
            if(c3!=0){
                al_draw_line(24,15,29,15,al_map_rgb(153,153,255),10);
            }
            if(c4!=0){
                al_draw_line(31,15,36,15,al_map_rgb(255,102,204),10);
            }
            if(c5!=0){
                al_draw_line(38,15,43,15,al_map_rgb(102,255,102),10);
            }
        }
    }
    int min;
    int seg;
    min=tjogo/60;
    seg=tjogo%60;
    if(seg<10){
        al_draw_textf(fonte,al_map_rgb(226,223,20),width/2,10,ALLEGRO_ALIGN_CENTRE,"%i 0%i",min,seg);
    }
    else{
        al_draw_textf(fonte,al_map_rgb(226,223,20),width/2,10,ALLEGRO_ALIGN_CENTRE,"%i %i",min,seg);
    }
}
void printStatus(jogador pessoa){
    switch (pessoa.vidas){
        case 1:
            al_draw_bitmap(hearta,10,10,0);
            al_draw_bitmap(heartb,30,10,0);
            al_draw_bitmap(heartb,50,10,0);
            break;
        case 2:
            al_draw_bitmap(hearta,10,10,0);
            al_draw_bitmap(hearta,30,10,0);
            al_draw_bitmap(heartb,50,10,0);
            break;
        case 3:
            al_draw_bitmap(hearta,10,10,0);
            al_draw_bitmap(hearta,30,10,0);
            al_draw_bitmap(hearta,50,10,0);
            break;
    }
    switch(pessoa.key){
        case 1:
            al_draw_text(fontemenor,al_map_rgb(226,223,20),45,32,0,"1");
            al_draw_bitmap(key,20,30,0);
            break;
        case 2:
            al_draw_text(fontemenor,al_map_rgb(226,223,20),45,32,0,"2");
            al_draw_bitmap(key,20,30,0);
            break;
        case 3:
            al_draw_text(fontemenor,al_map_rgb(226,223,20),45,32,0,"3");
            al_draw_bitmap(key,20,30,0);
            break;
        case 4:
            al_draw_text(fontemenor,al_map_rgb(226,223,20),45,32,0,"4");
            al_draw_bitmap(key,20,30,0);
            break;
        case 5:
            al_draw_text(fontemenor,al_map_rgb(226,223,20),45,32,0,"5");
            al_draw_bitmap(key,20,30,0);
            break;
        default:
            al_draw_text(fontemenor,al_map_rgb(226,223,20),45,32,0,"0");
            al_draw_bitmap(key,20,30,0);
            break;
    }
    int min;
    int seg;
    min=tjogo/60;
    seg=tjogo%60;
    if(seg<10){
        al_draw_textf(fonte,al_map_rgb(226,223,20),width/2,10,ALLEGRO_ALIGN_CENTRE,"%i 0%i",min,seg);
    }
    else{
        al_draw_textf(fonte,al_map_rgb(226,223,20),width/2,10,ALLEGRO_ALIGN_CENTRE,"%i %i",min,seg);
    }
}
void defaultbg()
{
    al_clear_to_color(al_map_rgb(60, 60, 60));
    al_draw_bitmap_region (background, 0, 0, w, h, 0, 0, 0);
}

void defaultgamebg()
{
    al_clear_to_color(al_map_rgb(60, 60, 60));
    al_draw_bitmap_region (background, 0, 0, width, height, 0, 0, 0);
}

enum conn_ret_t tryConnect(bool *done){
    int i = 0, f = 0, g = 0;
    int enter = 0;
    char server_ip[14]= "127.0.0.1";
    printf("Please enter the server IP: ");
    while (1)
    {
        if (f == 0 && (menu.keyboard.keycode != ALLEGRO_KEY_ENTER || (menu.keyboard.keycode != ALLEGRO_KEY_PAD_ENTER)))
        {
            f = 1;
            for (g = 0; g < 14; g++)
                server_ip[g] == '\0';
        }
        if(!al_is_event_queue_empty(filamenu)){
            if (*done != 1)
                al_wait_for_event(filamenu, &menu);
            if(menu.type==ALLEGRO_EVENT_KEY_DOWN){
                        switch(menu.keyboard.keycode){
                            case ALLEGRO_KEY_PAD_0:
                            case ALLEGRO_KEY_0:
                                if (i < 14){
                                    server_ip[i] = '0';
                                    server_ip[i+1] = '\0';
                                    i++;
                                }
                                break;
                            case ALLEGRO_KEY_PAD_1:
                            case ALLEGRO_KEY_1:
                                if (i < 14){
                                    server_ip[i] = '1';
                                    server_ip[i+1] = '\0';
                                    i++;
                                }
                                break;
                            case ALLEGRO_KEY_PAD_2:
                            case ALLEGRO_KEY_2:
                                if (i < 14){
                                    server_ip[i] = '2';
                                    server_ip[i+1] = '\0';
                                    i++;
                                }
                                break;
                            case ALLEGRO_KEY_PAD_3:
                            case ALLEGRO_KEY_3:
                                if (i < 14){
                                    server_ip[i] = '3';
                                    server_ip[i+1] = '\0';
                                    i++;
                                }
                                break;
                            case ALLEGRO_KEY_PAD_4:
                            case ALLEGRO_KEY_4:
                                if (i < 14){
                                    server_ip[i] = '4';
                                    server_ip[i+1] = '\0';
                                    i++;
                                }
                                break;
                            case ALLEGRO_KEY_PAD_5:
                            case ALLEGRO_KEY_5:
                                if (i < 14){
                                    server_ip[i] = '5';
                                    server_ip[i+1] = '\0';
                                    i++;
                                }
                                break;
                            case ALLEGRO_KEY_PAD_6:
                            case ALLEGRO_KEY_6:
                                if (i < 14){
                                    server_ip[i] = '6';
                                    server_ip[i+1] = '\0';
                                    i++;
                                }
                                break;
                            case ALLEGRO_KEY_PAD_7:
                            case ALLEGRO_KEY_7:
                                if (i < 14){
                                    server_ip[i] = '7';
                                    server_ip[i+1] = '\0';
                                    i++;
                                }
                                break;
                            case ALLEGRO_KEY_PAD_8:
                            case ALLEGRO_KEY_8:
                                if (i < 14){
                                    server_ip[i] = '8';
                                    server_ip[i+1] = '\0';
                                    i++;
                                }
                                break;
                            case ALLEGRO_KEY_PAD_9:
                            case ALLEGRO_KEY_9:
                                if (i < 14){
                                    server_ip[i] = '9';
                                    server_ip[i+1] = '\0';
                                    i++;
                                }
                                break;
                            case ALLEGRO_KEY_PAD_DELETE:
                            case ALLEGRO_KEY_FULLSTOP:
                                if (i < 14){
                                    server_ip[i] = '.';
                                    server_ip[i+1] = '\0';
                                    i++;
                                }
                                break;
                            case ALLEGRO_KEY_ESCAPE:
                                cal_mass_destroy();
                                *done = true;
                                return 0;
                            case ALLEGRO_KEY_BACKSPACE:
                            case ALLEGRO_KEY_DELETE:
                                if(i>0){
                                    i--;
                                }
                                server_ip[i] = '\0';
                                break;
                            case ALLEGRO_KEY_PAD_ENTER:
                            case ALLEGRO_KEY_ENTER:
                                printf ("\nConnecting to %s...\n", server_ip);
                                return connectToServer(server_ip);
                            default:
                                break;
                        }
                }
        }
        else
        {
            defaultgamebg();
            al_draw_textf(sans, al_map_rgb(0, 0, 0), 200, 120, ALLEGRO_ALIGN_CENTER,"IP: %s",server_ip);
            al_flip_display();
        }
    }
}
void leConnection(bool *done, int *repeat){
    int denial = 0;
    enum conn_ret_t ans=tryConnect(done);
    while (ans!=SERVER_UP && denial == 0){
        if(ans==SERVER_DOWN){
            defaultgamebg();
            al_draw_text(sansmenor, al_map_rgb(0, 0, 0), 200, 130, ALLEGRO_ALIGN_CENTER, "The server is down!");
            al_flip_display();
            puts("O server caiu!");
        }
        else if(ans==SERVER_FULL){
            defaultgamebg();
            al_draw_text(sansmenor, al_map_rgb(0, 0, 0), 200, 130, ALLEGRO_ALIGN_CENTER, "The server is full!");
            al_flip_display();
            puts("O server esta cheio!");
        }
        else if(ans==SERVER_CLOSED){
            defaultgamebg();
            al_draw_text(sansmenor, al_map_rgb(0, 0, 0), 200, 130, ALLEGRO_ALIGN_CENTER, "The server is not available!");
            al_flip_display();
            puts("O server esta fechado para novas conexoes");
        }
        al_rest(2);
        defaultgamebg();
        al_draw_text(sansmenor, al_map_rgb(0, 0, 0), 200, 130, ALLEGRO_ALIGN_CENTER, "Would you like to try again?(Y/N):");
        al_flip_display();
        int res;
        do{
            al_wait_for_event(filamenu,&menu);
            if(menu.type==ALLEGRO_EVENT_KEY_DOWN){
                switch(menu.keyboard.keycode){
                    case ALLEGRO_KEY_Y:
                        res='y';
                        break;
                    case ALLEGRO_KEY_N:
                        res='n';
                        break;
                    default:
                        res='g';
                        break;
                }
            }
        }while(res!='n'&&res!='y');
        if(res=='n'){
            (*repeat) = 1;
            denial = 1;
        }
        else
        {
            res='g';
            ans=tryConnect(done);
        }
    }
}

int inicializar()
{
    if (!al_init())
    {
        exit(1);
    }

    if (!al_init_image_addon())
    {
        exit(1);
    }

    if (!al_install_mouse())
    {
        exit(1);
    }

    if (!al_install_keyboard())
    {
        exit(1);
    }
    if(!al_init_primitives_addon()){
        exit(1);
    }
    menudisplay = al_create_display(w,h);

    if (!menudisplay)
    {
        exit(1);
    }

    al_set_window_title(menudisplay, "Dark Dwellers");
    al_set_new_display_flags(ALLEGRO_NOFRAME);
    al_toggle_display_flag(menudisplay, ALLEGRO_NOFRAME, false);

    al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);

    al_init_font_addon();

    if (!al_init_ttf_addon())
    {
        al_destroy_display(menudisplay);
        exit(1);
    }

    sans = al_load_font("Fonts/PixelOperator-Bold.ttf", 48, 0);

    if (!sans)
    {
        al_destroy_display(menudisplay);
        exit(1);
    }

    titlesans = al_load_font("Fonts/PixelOperator-Bold.ttf", 108, 0);

    if (!titlesans)
    {
        al_destroy_display(menudisplay);
        exit(1);
    }

    sansmenor = al_load_font("Fonts/PixelOperator-Bold.ttf", 24, 0);

    if (!sansmenor)
    {
        al_destroy_display(menudisplay);
        exit(1);
    }

    mediumsans = al_load_font("Fonts/PixelOperator-Bold.ttf", 40, 0);

    if (!mediumsans)
    {
        al_destroy_display(menudisplay);
        exit(1);
    }

    fonte = al_load_font("Fonts/segment.otf",30,0);
    fontemenor = al_load_font("Fonts/segment.otf",20,0);
    fonteportao = al_load_font("Fonts/scary.ttf",48,0);
    if (!fonte)
    {
        al_destroy_display(menudisplay);
        exit(1);
    }

    cal_import_bitmap("menu/graybutton.png", &gbutton);
    cal_import_bitmap("menu/bluebutton.png", &bbutton);
    cal_import_bitmap("menu/spookybg.jpg", &background);
    cal_import_bitmap("Mapa/mapa.jpg", &mapa);
    cal_import_bitmap("Mapa/mapawin1.jpg", &mapaOpenGate);
    cal_import_bitmap("Mapa/mapa.jpg", &mapaClosedGate);
    cal_import_bitmap("status/hearta.png", &hearta);
    cal_import_bitmap("status/heartb.png", &heartb);
    cal_import_bitmap("status/key.png", &key);
    cal_import_bitmap("Mapa/victory.png", &Win);
    cal_import_bitmap("Mapa/defeat.png", &Defeat);
    cal_import_bitmap("status/mao.png", &mao);
    cal_import_bitmap("Personagens/monstro/mfrente.png", &frontm);
    cal_import_bitmap("Personagens/p1/p1frente.png", &frontp1);
    cal_import_bitmap("Personagens/p2/p2frente.png", &frontp2);
    cal_import_bitmap("Personagens/p3/p3frente.png", &frontp3);
    cal_import_bitmap("Personagens/p4/p4frente.png", &frontp4);

    timer = al_create_timer(1.0/fps);
    if (!timer)
    {
        al_destroy_display(menudisplay);
        exit(1);
    }

    cal_init_event_queues();

    if(!al_install_audio())
    {
        al_destroy_display(menudisplay);
        exit(1);
    }

    if(!al_init_acodec_addon())
    {
        al_destroy_display(menudisplay);
        exit(1);
    }

    if (!al_reserve_samples(1))
    {
        al_destroy_display(menudisplay);
        exit(1);
    }

    bg = al_load_audio_stream("Sounds/bgsong.wav", 4, 1024);
    if (!bg)
    {
        al_destroy_display(menudisplay);
        exit(1);
    }

    gamesong = al_load_audio_stream("Sounds/gamesong.wav", 4, 1024);
    if (!gamesong)
    {
        al_destroy_display(menudisplay);
        cal_destroy_audio();
        exit(1);
    }

    al_attach_audio_stream_to_mixer(bg, al_get_default_mixer());
    al_attach_audio_stream_to_mixer(gamesong, al_get_default_mixer());
    al_set_audio_stream_playmode(bg, ALLEGRO_PLAYMODE_LOOP);
    al_set_audio_stream_playmode(gamesong, ALLEGRO_PLAYMODE_LOOP);
    al_set_audio_stream_playing(bg, false);
    al_set_audio_stream_playing(gamesong, false);

    button = al_load_sample("Sounds/button.wav");
    if (!button)
    {
        al_destroy_display(menudisplay);
        cal_destroy_audio();
        exit(1);
    }

    svictory = al_load_sample("Sounds/victory.wav");
    if (!svictory)
    {
        al_destroy_display(menudisplay);
        cal_destroy_audio();
        exit(1);
    }

    sdefeat = al_load_sample("Sounds/defeat.wav");
    if (!sdefeat)
    {
        al_destroy_display(menudisplay);
        cal_destroy_audio();
        exit(1);
    }

    sdead = al_load_sample("Sounds/dead.wav");
    if (!sdead)
    {
        al_destroy_display(menudisplay);
        cal_destroy_audio();
        exit(1);
    }

    skill = al_load_sample("Sounds/kill.wav");
    if (!skill)
    {
        al_destroy_display(menudisplay);
        cal_destroy_audio();
        exit(1);
    }

    return 1;
}
int main(void)
{
    int life[5]={0};
    int start=0,menuval=0;
    int hover[4]={0};
    int c1,c2,c3;
    int aux = 0;
    int temp=0;
    int repeat = 0;
    int colisoes[10][15][20];
    FILE **arq = NULL;
    char plano[15][20];
    arq=(FILE**)malloc(10*(sizeof(FILE *)));
    arq[0]=fopen("matrizes/plano1.txt","rt");
    arq[1]=fopen("matrizes/plano2.txt","rt");
    arq[2]=fopen("matrizes/plano3.txt","rt");
    arq[3]=fopen("matrizes/plano4.txt","rt");
    arq[4]=fopen("matrizes/plano5.txt","rt");
    arq[5]=fopen("matrizes/plano6.txt","rt");
    arq[6]=fopen("matrizes/plano7.txt","rt");
    arq[7]=fopen("matrizes/plano8.txt","rt");
    arq[8]=fopen("matrizes/plano9.txt","rt");
    arq[9]=fopen("matrizes/plano2m.txt","rt");
    for(c1=0;c1<10;c1++){
        for(c2=0;c2<15;c2++){
            fscanf(arq[c1]," %[^\n]",plano[c2]);
        }
        for(c2=0;c2<15;c2++){
            for(c3=0;c3<20;c3++){
                if(plano[c2][c3]=='.'){
                    colisoes[c1][c2][c3]=0;
                }else{
                    colisoes[c1][c2][c3]=1;
                }
            }
        }
    }
    for(c1=0;c1<10;c1++){
        fclose(arq[c1]);
    }
    free(arq);
    colisoes[0][0][0]=1;
    int contador=0;
    int meuId;
    int retorno;
    jogador *pessoa=(jogador*)malloc(5*sizeof(jogador));
    if(!inicializar()){
        exit(EXIT_SUCCESS);
    }
    menustart:
    al_register_event_source(filamenu, al_get_display_event_source(menudisplay));
    defaultbg();
    al_set_audio_stream_playing(gamesong, false);
    al_rewind_audio_stream(bg);
    al_rewind_audio_stream(gamesong);
    al_set_audio_stream_playing(bg, true);
    while (!done){
        while (!start && !done)
            {
                if (menuval == 0)
                {
                    defaultbg();
                    al_draw_text(titlesans, al_map_rgb(65, 65, 65), 600, 154, ALLEGRO_ALIGN_CENTER, "DARK DWELLERS");
                    al_draw_text(titlesans, al_map_rgb(0, 0, 0), 600, 150, ALLEGRO_ALIGN_CENTER, "DARK DWELLERS");
                    if (hover[0] == 0)
                        al_draw_bitmap_region(gbutton, 0, 0, 500, 60, 350, 330, 0);
                    else
                        al_draw_bitmap_region(bbutton, 0, 0, 500, 60, 350, 330, 0);
                    if (hover[1] == 0)
                        al_draw_bitmap_region(gbutton, 0, 0, 500, 60, 350, 450, 0);
                    else
                        al_draw_bitmap_region(bbutton, 0, 0, 500, 60, 350, 450, 0);
                    if (hover[2] == 0)
                        al_draw_bitmap_region(gbutton, 0, 0, 500, 60, 350, 570, 0);
                    else
                        al_draw_bitmap_region(bbutton, 0, 0, 500, 60, 350, 570, 0);
                    if (hover[3] == 0)
                        al_draw_bitmap_region(gbutton, 0, 0, 500, 60, 350, 690, 0);
                    else
                        al_draw_bitmap_region(bbutton, 0, 0, 500, 60, 350, 690, 0);
                    if(!al_is_event_queue_empty(filamenu))
                    {
                        if (!done || !start)
                        {
                            al_wait_for_event(filamenu, &menu);
                        }
                        if (menu.type == ALLEGRO_EVENT_MOUSE_AXES)
                        {
                            if (menu.mouse.x >= 350 && menu.mouse.x <= 850 && menu.mouse.y > 330 && menu.mouse.y <= 390)
                            {
                                al_draw_bitmap_region(bbutton, 0, 0, 500, 60, 350, 330, 0);
                                hover[0] = 1;
                            }
                            else
                                hover[0] = 0;
                            if (menu.mouse.x >= 350 && menu.mouse.x <= 850 && menu.mouse.y > 450 && menu.mouse.y <= 510)
                            {
                                al_draw_bitmap_region(bbutton, 0, 0, 500, 60, 350, 450, 0);
                                hover[1] = 1;
                            }
                            else
                                hover[1] = 0;
                            if (menu.mouse.x >= 350 && menu.mouse.x <= 850 && menu.mouse.y > 570 && menu.mouse.y <= 630)
                            {
                                al_draw_bitmap_region(bbutton, 0, 0, 500, 60, 350, 570, 0);
                                hover[2] = 1;
                            }
                            else
                                hover[2] = 0;
                            if (menu.mouse.x >= 350 && menu.mouse.x <= 850 && menu.mouse.y > 690 && menu.mouse.y <= 750)
                            {
                                al_draw_bitmap_region(bbutton, 0, 0, 500, 60, 350, 690, 0);
                                hover[3] = 1;
                            }
                            else
                                hover[3] = 0;
                        }
                        else if (menu.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                        {
                            if (hover[0] == 1)
                            {
                                //GAME START
                                start = 1;  hover[0] = 0; al_play_sample(button, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);

                                al_destroy_display(menudisplay);
                                menudisplay = al_create_display(width, height);
                                al_set_window_position(menudisplay, 633, 300);
                                al_toggle_display_flag(menudisplay, ALLEGRO_NOFRAME, true);

                                defaultgamebg();
                                al_draw_text(sans, al_map_rgb(0, 0, 0), 200, 120, ALLEGRO_ALIGN_CENTER, "AWAITING IP...");
                                al_flip_display();

                                leConnection(&done, &repeat);
                                if (repeat == 1)
                                {
                                    repeat = 0;
                                    al_destroy_display(menudisplay);
                                    menudisplay = al_create_display(w, h);
                                    al_toggle_display_flag(menudisplay, ALLEGRO_NOFRAME, false);
                                    al_set_window_title(menudisplay, "Dark Dwellers");
                                    start = 0;
                                    goto menustart; //ESSE GOTO VOLTA PARA O MENU, NO INÍCIO DO INT MAIN()
                                }
                                recvMsgFromServer(&meuId, WAIT_FOR_IT);
                                printf("%i\n", meuId);

                                defaultgamebg();
                                al_draw_text(sans, al_map_rgb(0, 0, 0), 200, 120, ALLEGRO_ALIGN_CENTER, "WAITING PLAYERS...");
                                al_flip_display();

                                retorno = recvMsgFromServer(pessoa, WAIT_FOR_IT);
                                if(retorno == SERVER_DISCONNECTED){
                                    cal_mass_destroy();
                                    exit(EXIT_SUCCESS);
                                }

                                for(c1=0;c1<5;c1++){
                                    printf("Key - %i %i %i\n",pessoa[0].chaves[c1].plano,pessoa[0].chaves[c1].x,pessoa[0].chaves[c1].y);
                                }

                                for(c1=0;c1<3;c1++){
                                    printf("%i %i %i\n",pessoa[c1].id,pessoa[c1].x,pessoa[c1].y);
                                }

                                al_destroy_display(menudisplay);

                                display = al_create_display(width,height);
                                if(!display){
                                    return -1;
                                }
                                al_toggle_display_flag(display, ALLEGRO_NOFRAME, true);

                                printTela(pessoa[meuId].plano);
                                leSkin(pessoa[meuId].id);
                                printRec(pessoa[meuId]);
                                al_start_timer(timer);
                                al_set_audio_stream_playing(bg, false);
                                al_set_audio_stream_playing(gamesong, true);
                                al_flip_display();
                            }
                            if (hover[1] == 1)
                            {
                                menuval = 1; hover[1] = 0; hover[4] = 0; al_play_sample(button, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                            }
                            if (hover[2] == 1)
                            {
                                menuval = 2; hover[2] = 0; hover[4] = 0; al_play_sample(button, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                            }
                            if (hover[3] == 1)
                            {
                                done = 1;  hover[3] = 0; al_play_sample(button, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                            }
                        }
                        else if (menu.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                            done = 1;
                    }
                    al_draw_text(sans, al_map_rgb(0, 0, 0), 600, 335, ALLEGRO_ALIGN_CENTER, "START");
                    al_draw_text(sans, al_map_rgb(0, 0, 0), 600, 455, ALLEGRO_ALIGN_CENTER, "HOW TO PLAY");
                    al_draw_text(sans, al_map_rgb(0, 0, 0), 600, 575, ALLEGRO_ALIGN_CENTER, "CREDITS");
                    al_draw_text(sans, al_map_rgb(0, 0, 0), 600, 695, ALLEGRO_ALIGN_CENTER, "EXIT");
                }
                else if (menuval == 1)
                {
                    defaultbg();
                    al_draw_text(mediumsans, al_map_rgb(0, 0, 0), 600, 30, ALLEGRO_ALIGN_CENTER, "Como um fantasma, fuja do monstro e colete");
                    al_draw_text(mediumsans, al_map_rgb(0, 0, 0), 600, 90, ALLEGRO_ALIGN_CENTER, "as chaves. Após coletivamente coletarem 5 de 6,");
                    al_draw_text(mediumsans, al_map_rgb(0, 0, 0), 600, 150, ALLEGRO_ALIGN_CENTER, "o portão se abrirá no topo do mapa para fugirem.");
                    al_draw_text(mediumsans, al_map_rgb(0, 0, 0), 600, 210, ALLEGRO_ALIGN_CENTER, "Todos tem 3 vidas, e quando perderem uma, perdem");
                    al_draw_text(mediumsans, al_map_rgb(0, 0, 0), 600, 270, ALLEGRO_ALIGN_CENTER, "as chaves coletadas individualmente.");
                    al_draw_text(mediumsans, al_map_rgb(0, 0, 0), 600, 350, ALLEGRO_ALIGN_CENTER, "Como o monstro, mate os fantasmas encostando neles.");
                    al_draw_text(mediumsans, al_map_rgb(0, 0, 0), 600, 410, ALLEGRO_ALIGN_CENTER, "O monstro ganha se o tempo acabar ou se todos os");
                    al_draw_text(mediumsans, al_map_rgb(0, 0, 0), 600, 470, ALLEGRO_ALIGN_CENTER, "fantasmas morrerem. Os fantasmas ganham se");
                    al_draw_text(mediumsans, al_map_rgb(0, 0, 0), 600, 530, ALLEGRO_ALIGN_CENTER, "conseguirem fugir com sucesso.");
                    al_draw_bitmap(frontm, 390, 620, 0);
                    al_draw_bitmap(frontp1, 760, 600, 0);
                    al_draw_bitmap(frontp2, 790, 600, 0);
                    al_draw_bitmap(frontp3, 760, 640, 0);
                    al_draw_bitmap(frontp4, 790, 640, 0);
                    al_draw_text(sansmenor, al_map_rgb(0, 0, 0), 470, 616, ALLEGRO_ALIGN_CENTER, "< Monstro");
                    al_draw_text(sansmenor, al_map_rgb(0, 0, 0), 680, 616, ALLEGRO_ALIGN_CENTER, "Fantasmas >");
                    if (hover[4] == 0)
                        al_draw_bitmap_region(gbutton, 0, 0, 500, 60, 350, 690, 0);
                    else
                        al_draw_bitmap_region(bbutton, 0, 0, 500, 60, 350, 690, 0);
                    if(!al_is_event_queue_empty(filamenu))
                    {
                        if (done != 1 || start != 1)
                            al_wait_for_event(filamenu, &menu);
                        if (menu.type == ALLEGRO_EVENT_MOUSE_AXES)
                        {
                            if (menu.mouse.x >= 350 && menu.mouse.x <= 850 && menu.mouse.y > 690 && menu.mouse.y <= 750)
                            {
                                al_draw_bitmap_region(bbutton, 0, 0, 500, 60, 350, 690, 0);
                                hover[4] = 1;
                            }
                            else
                                hover[4] = 0;
                        }
                        else if (menu.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                        {
                            if (hover[4] == 1)
                        {
                                menuval = 0; al_play_sample(button, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                    }
                        }
                        else if (menu.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                            done = 1;
                    }
                    al_draw_text(sans, al_map_rgb(0, 0, 0), 600, 695, ALLEGRO_ALIGN_CENTER, "BACK");
                }
                else if (menuval == 2)
                {
                    defaultbg();
                    al_draw_text(titlesans, al_map_rgb(0, 0, 0), 600, 120, ALLEGRO_ALIGN_CENTER, "EQUIPE:");
                    al_draw_text(sans, al_map_rgb(0, 0, 0), 600, 250, ALLEGRO_ALIGN_CENTER, "FELIPE NUNES");//
                    al_draw_text(sans, al_map_rgb(0, 0, 0), 600, 310, ALLEGRO_ALIGN_CENTER, "GABRIEL 'GME' DE MELO EVANGELISTA");
                    al_draw_text(sans, al_map_rgb(0, 0, 0), 600, 370, ALLEGRO_ALIGN_CENTER, "GABRIEL 'SONIKKU' FERREIRA ROCHA");
                    al_draw_text(sans, al_map_rgb(0, 0, 0), 600, 430, ALLEGRO_ALIGN_CENTER, "JOSE 'ZECA' CARLOS");
                    al_draw_text(sans, al_map_rgb(0, 0, 0), 600, 490, ALLEGRO_ALIGN_CENTER, "LUCAS 'L' DOS REIS");
                    al_draw_text(sans, al_map_rgb(0, 0, 0), 600, 550, ALLEGRO_ALIGN_CENTER, "MARCELO VALOIS");
                    if (hover[4] == 0)
                        al_draw_bitmap_region(gbutton, 0, 0, 500, 60, 350, 690, 0);
                    else
                        al_draw_bitmap_region(bbutton, 0, 0, 500, 60, 350, 690, 0);
                    if(!al_is_event_queue_empty(filamenu))
                    {
                        if (done != 1 || start != 1)
                            al_wait_for_event(filamenu, &menu);
                        if (menu.type == ALLEGRO_EVENT_MOUSE_AXES)
                        {
                            if (menu.mouse.x >= 350 && menu.mouse.x <= 850 && menu.mouse.y > 690 && menu.mouse.y <= 750)
                            {
                                al_draw_bitmap_region(bbutton, 0, 0, 500, 60, 350, 690, 0);
                                hover[4] = 1;
                            }
                            else
                                hover[4] = 0;
                        }
                        else if (menu.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                        {
                            if (hover[4] == 1)
                        {
                                menuval = 0; al_play_sample(button, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                        }
                        }
                        else if (menu.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                            done = 1;
                    }
                    al_draw_text(sans, al_map_rgb(0, 0, 0), 600, 695, ALLEGRO_ALIGN_CENTER, "BACK");
                }
                al_flip_display();
            }
        al_destroy_event_queue(filamenu);
        int teste=0;
        while(start && !done){
            al_wait_for_event(fila, &ev);
            if(ev.type==ALLEGRO_EVENT_TIMER){
                aux++;
                if(aux==fps){
                    tjogo--;
                    aux=0;
                }
                redraw=true;
                retorno=recvMsgFromServer(pessoa,DONT_WAIT);
                if(retorno==SERVER_DISCONNECTED){
                    cal_mass_destroy();
                    exit(1);
                }
                if(pessoa[meuId].vidas == 0){
                    end = al_create_display(width*3,height*3);
                    al_toggle_display_flag(end, ALLEGRO_NOFRAME, true);
                    al_draw_bitmap(Defeat,0,0,0);
                    al_flip_display();
                    al_play_sample(sdefeat, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                    al_destroy_event_queue(fila);
                    al_destroy_display(display);
                    al_rest(2);
                    al_destroy_display(end);
                    menudisplay = al_create_display(w, h);
                    al_toggle_display_flag(menudisplay, ALLEGRO_NOFRAME, false);
                    al_set_window_title(menudisplay, "Dark Dwellers");
                    start = 0;
                    cal_init_event_queues();
                    goto menustart;
                }
                if(tjogo==0){
                    end = al_create_display(width*3, height*3);
                    al_toggle_display_flag(end, ALLEGRO_NOFRAME, true);
                    if(pessoa[meuId].id == monstro){
                        al_draw_bitmap(Win,0,0,0);
                        al_flip_display();
                        al_play_sample(svictory, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                    }
                    else{
                        al_draw_bitmap(Defeat,0,0,0);
                        al_flip_display();
                        al_play_sample(sdefeat, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                    }
                    al_destroy_event_queue(fila);
                    al_destroy_display(display);
                    al_rest(2);
                    al_destroy_display(end);
                    menudisplay = al_create_display(w, h);
                    al_toggle_display_flag(menudisplay, ALLEGRO_NOFRAME, false);
                    al_set_window_title(menudisplay, "Dark Dwellers");
                    start = 0;
                    cal_init_event_queues();
                    goto menustart;
                }
                if(pessoa[meuId].acabou==1){
                    end = al_create_display(width*3, height*3);
                    al_toggle_display_flag(end, ALLEGRO_NOFRAME, true);
                    al_draw_bitmap(Win,0,0,0);
                    al_flip_display();
                    al_play_sample(svictory, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                    al_destroy_event_queue(fila);
                    al_destroy_display(display);
                    al_rest(2);
                    al_destroy_display(end);
                    menudisplay = al_create_display(w, h);
                    al_toggle_display_flag(menudisplay, ALLEGRO_NOFRAME, false);
                    al_set_window_title(menudisplay, "Dark Dwellers");
                    start = 0;
                    cal_init_event_queues();
                    goto menustart;
                }
                if(pessoa[0].monstroloose==1){
                    end = al_create_display(width*3,height*3);
                    al_toggle_display_flag(end, ALLEGRO_NOFRAME, true);
                    if(pessoa[meuId].id!=monstro){
                        al_draw_bitmap(Win,0,0,0);
                        al_flip_display();
                        al_play_sample(svictory,1,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
                    }
                    else{
                        al_draw_bitmap(Defeat,0,0,0);
                        al_flip_display();
                        al_play_sample(sdefeat,1,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
                    }
                    al_destroy_event_queue(fila);
                    al_destroy_display(display);
                    al_rest(2);
                    al_destroy_display(end);
                    menudisplay = al_create_display(w, h);
                    al_toggle_display_flag(menudisplay, ALLEGRO_NOFRAME, false);
                    al_set_window_title(menudisplay, "Dark Dwellers");
                    start=0;
                    cal_init_event_queues();
                    goto menustart;
                }
                if(pessoa[0].monstrowin==1){
                    end = al_create_display(width*3,height*3);
                    al_toggle_display_flag(end, ALLEGRO_NOFRAME, true);
                    if(pessoa[meuId].id==monstro){
                        al_draw_bitmap(Win,0,0,0);
                        al_flip_display();
                        al_play_sample(svictory,1,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
                    }
                    else{
                        al_draw_bitmap(Defeat,0,0,0);
                        al_flip_display();
                        al_play_sample(sdefeat,1,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
                    }
                    al_destroy_event_queue(fila);
                    al_destroy_display(display);
                    al_rest(2);
                    al_destroy_display(end);
                    menudisplay = al_create_display(w, h);
                    al_toggle_display_flag(menudisplay, ALLEGRO_NOFRAME, false);
                    al_set_window_title(menudisplay, "Dark Dwellers");
                    start=0;
                    cal_init_event_queues();
                    goto menustart;
                }
            }
            else if(ev.type==ALLEGRO_EVENT_KEY_DOWN){
                switch(ev.keyboard.keycode){
                    case ALLEGRO_KEY_UP:
                        pessoa[meuId].tecla='u';
                        pessoa[meuId].lado=1;
                        break;
                    case ALLEGRO_KEY_DOWN:
                        pessoa[meuId].tecla='d';
                        pessoa[meuId].lado=0;
                        break;
                    case ALLEGRO_KEY_LEFT:
                        pessoa[meuId].tecla='l';
                        pessoa[meuId].lado=2;
                        break;
                    case ALLEGRO_KEY_RIGHT:
                        pessoa[meuId].tecla='r';
                        pessoa[meuId].lado=3;
                        break;
                    case ALLEGRO_KEY_ESCAPE:
                        pessoa[meuId].vivo=0;
                        done = true;
                        break;
                    default:
                        pessoa[meuId].tecla='8';
                        break;
                }
                for(c1=0;c1<5;c1++){
                    life[c1]=pessoa[c1].vidas;
                }
                sendMsgToServer(&pessoa[meuId],sizeof(jogador));
            }
            if(redraw && al_is_event_queue_empty(fila)){
                redraw=false;
                int i;
                for(i=0;i<5;i++){
                    if(pessoa[meuId].plano==pessoa[i].plano&&pessoa[i].vivo==1){
                        leSkin(pessoa[i].id);
                        printRec(pessoa[i]);
                    }
                    if(pessoa[meuId].plano==pessoa[0].chaves[i].plano&&pessoa[0].chaves[i].vivo==1){
                        al_draw_bitmap(key,pessoa[0].chaves[i].x,pessoa[0].chaves[i].y,0);
                    }
                }
                if(pessoa[meuId].vidas!=life[meuId]&&meuId!=monstro){
                        life[meuId]=pessoa[meuId].vidas;
                        al_play_sample(sdead, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                }
                for(c1=0;c1<5;c1++){
                    if(c1==monstro){
                        c1++;
                    }
                    else if(pessoa[c1].vidas!=life[c1]&&meuId==monstro){
                        life[c1]=pessoa[c1].vidas;
                        al_play_sample(skill, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                    }
                }
                if(pessoa[meuId].id==monstro){
                    printStatusm(pessoa);
                }
                else{
                    printStatus(pessoa[meuId]);
                }
                if(pessoa[0].abriu == 1){
                    if(temp==0){
                        teste=tjogo;
                    }
                    temp=1;
                    mapa = mapaOpenGate;
                }
                else{
                    temp=0;
                    mapa = mapaClosedGate;
                }
                if(temp==1&&(teste-tjogo)<1){
                    al_draw_text(fonteportao,al_map_rgb(226,223,20),width/2,50,ALLEGRO_ALIGN_CENTRE,"PORTAO ABERTO");
                }
                al_flip_display();
                printTela(pessoa[meuId].plano);
            }
        }
    }
    cal_mass_destroy();
    exit(EXIT_SUCCESS);
    return 0;
}
