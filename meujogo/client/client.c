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
#define tilesize 20
#define monstro 0

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
}jogador;
/*
lado 0 == frente
lado 1 == costa
lado 2 == esquerda
lado 3 == direita
*/
enum KEYS{UP,DOWN,LEFT,RIGHT};
const int fps=60;
bool done=false;
bool redraw=true;
int tjogo=300; 

ALLEGRO_DISPLAY *menudisplay = NULL;
ALLEGRO_DISPLAY *display=NULL;
ALLEGRO_DISPLAY *end=NULL;
ALLEGRO_EVENT menu, menuhtp;
ALLEGRO_EVENT_QUEUE *fila=NULL;
ALLEGRO_BITMAP *gbutton = NULL, *bbutton = NULL, *wa = NULL, *background = NULL;
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
ALLEGRO_TIMER *timer=NULL;
ALLEGRO_FONT *fonte=NULL;
ALLEGRO_AUDIO_STREAM *bg = NULL;
ALLEGRO_SAMPLE *button = NULL;
ALLEGRO_FONT *sans = NULL, *titlesans = NULL;
ALLEGRO_TIMEOUT timeout;

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
        al_draw_bitmap(key,20,30,0);
        break;
    case 2:
        al_draw_bitmap(key,20,30,0);
        al_draw_bitmap(key,20,50,0);
        break;
    case 3:
        al_draw_bitmap(key,20,30,0);
        al_draw_bitmap(key,20,50,0);
        al_draw_bitmap(key,20,70,0);
        break;
    case 4:
        al_draw_bitmap(key,20,30,0);
        al_draw_bitmap(key,20,50,0);
        al_draw_bitmap(key,20,70,0);
        al_draw_bitmap(key,20,90,0);
        break;
    case 5:
        al_draw_bitmap(key,20,30,0);
        al_draw_bitmap(key,20,50,0);
        al_draw_bitmap(key,20,70,0);
        al_draw_bitmap(key,20,90,0);
        al_draw_bitmap(key,20,110,0);
    }
    int min;
    int seg;
    min=tjogo/60;
    seg=tjogo%60;
    al_draw_textf(fonte,al_map_rgb(226,223,20),width/2,10,ALLEGRO_ALIGN_CENTRE,"%i %i",min,seg);
}

enum conn_ret_t tryConnect(){
    char server_ip[30];
    printf("Please enter the server IP: ");
    scanf(" %s",server_ip);
    getchar();
    return connectToServer(server_ip);
}
void leConnection(){
    enum conn_ret_t ans=tryConnect();
    while (ans!=SERVER_UP){
        if(ans==SERVER_DOWN){
            puts("O server caiu!");
        }
        else if(ans==SERVER_FULL){
            puts("O server esta cheio!");
        }
        else if(ans==SERVER_CLOSED){
            puts("O server esta fechado para novas conexoes");
        }
        printf("Gostaria de tentar novamente? [S/n]");
        int res;
        while(res=tolower(getchar()),res!='n'&&res!='s'&&res!='\n'){
            puts("Que?");
        }
        if(res=='n'){
            exit(EXIT_SUCCESS);
        }
        ans=tryConnect();
    }
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
          exit(1);
      }
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

    menudisplay = al_create_display(w,h);

    if (!menudisplay)
    {
        exit(1);
    }

    al_set_window_title(menudisplay, "Teste");
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

    fonte = al_load_font("Fonts/segment.otf",30,0);

    if (!fonte)
    {
        al_destroy_display(menudisplay);
        exit(1);
    }

    cal_import_bitmap("menu/graybutton.png", &gbutton);
    cal_import_bitmap("menu/bluebutton.png", &bbutton);
    cal_import_bitmap("menu/waaaa.png", &wa);
    cal_import_bitmap("menu/spookybg.jpg", &background);
    cal_import_bitmap("Mapa/mapa.jpg", &mapa);
    cal_import_bitmap("Mapa/mapa.jpg", &mapaClosedGate);
    cal_import_bitmap("Mapa/mapawin1.jpg", &mapaOpenGate);
    cal_import_bitmap("status/hearta.png", &hearta);
    cal_import_bitmap("status/heartb.png", &heartb);
    cal_import_bitmap("status/key.png", &key);
    cal_import_bitmap("Mapa/victory.png", &Win);
    cal_import_bitmap("Mapa/defeat.png", &Defeat);

    timer = al_create_timer(1.0/fps);
    if (!timer)
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

    al_register_event_source(fila, al_get_mouse_event_source());
    al_register_event_source(fila, al_get_keyboard_event_source());
    al_register_event_source(fila, al_get_display_event_source(menudisplay));
    al_register_event_source(fila, al_get_timer_event_source(timer));

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

    al_attach_audio_stream_to_mixer(bg, al_get_default_mixer());

    button = al_load_sample("Sounds/button.wav");
    if (!button)
    {
        al_destroy_display(menudisplay);
        exit(1);
    }

    al_init_timeout(&timeout, 1/fps);

    return 1;
}
int main(void)
{
    int start=0,menuval=0;
    int hover[4]={0};
    int neox=0;
    int neoy=0;
    int c1,c2,c3;
    int aux = 0;

    int colisoes[10][15][20];
    FILE **arq=NULL;
    char plano[20][20];
    //Matriz
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

    defaultbg();
    while (!done){
        while (!start && !done)
            {
                al_set_audio_stream_playmode(bg, ALLEGRO_PLAYMODE_LOOP);
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
                    if(!al_is_event_queue_empty(fila))
                    {
                        if (done != 1 || start != 1)
                        {
                            al_wait_for_event(fila, &menu);
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

                                defaultgamebg();
                                al_draw_text(sans, al_map_rgb(0, 0, 0), 200, 120, ALLEGRO_ALIGN_CENTER, "AWAITING IP...");
                                al_flip_display();

                                leConnection();
                                recvMsgFromServer(&meuId, WAIT_FOR_IT);
                                printf("%i\n", meuId);

                                defaultgamebg();
                                al_draw_text(sans, al_map_rgb(0, 0, 0), 200, 120, ALLEGRO_ALIGN_CENTER, "WAITING PLAYERS...");
                                al_flip_display();

                                retorno = recvMsgFromServer(pessoa, WAIT_FOR_IT);
                                if(retorno == SERVER_DISCONNECTED){
                                    return -1;
                                }

                                for(c1=0;c1<5;c1++){
                                    printf("Key - %i %i %i\n",pessoa[0].chaves[c1].plano,pessoa[0].chaves[c1].x,pessoa[0].chaves[c1].y);
                                }

                                for(c1=0;c1<3;c1++){
                                    printf("%i %i %i\n",pessoa[c1].id,pessoa[c1].x,pessoa[c1].y);
                                }

                                al_destroy_display(menudisplay);

                                al_set_new_display_flags(ALLEGRO_NOFRAME);
                                display = al_create_display(width,height);
                                if(!display){
                                    return -1;
                                }

                                printTela(pessoa[meuId].plano);
                                leSkin(pessoa[meuId].id);
                                printRec(pessoa[meuId]);
                                al_start_timer(timer);
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
                    al_draw_text(sans, al_map_rgb(0, 0, 0), 600, 120, ALLEGRO_ALIGN_CENTER, "YOU ARE GHOSTLY GHOST.");
                    al_draw_text(sans, al_map_rgb(0, 0, 0), 600, 180, ALLEGRO_ALIGN_CENTER, "RUN FROM BLACK GHOST MAN.");
                    al_draw_text(sans, al_map_rgb(0, 0, 0), 600, 240, ALLEGRO_ALIGN_CENTER, "DON'T DIE, THAT'S BAD FOR YOUR HEALTH.");
                    al_draw_text(sans, al_map_rgb(27, 22, 89), 600, 300, ALLEGRO_ALIGN_CENTER, "HAVE A COMPLIMENTARY WALUIGI. WAAA.");
                    al_draw_bitmap_region(wa, 0, 0, 240, 240, 480, 400, 0);
                    if (hover[4] == 0)
                        al_draw_bitmap_region(gbutton, 0, 0, 500, 60, 350, 690, 0);
                    else
                        al_draw_bitmap_region(bbutton, 0, 0, 500, 60, 350, 690, 0);
                    if(!al_is_event_queue_empty(fila))
                    {
                        if (done != 1 || start != 1)
                            al_wait_for_event(fila, &menu);
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
                    al_draw_text(sans, al_map_rgb(0, 0, 0), 600, 300, ALLEGRO_ALIGN_CENTER, "INSERT TEXT HERE");
                    if (hover[4] == 0)
                        al_draw_bitmap_region(gbutton, 0, 0, 500, 60, 350, 690, 0);
                    else
                        al_draw_bitmap_region(bbutton, 0, 0, 500, 60, 350, 690, 0);
                    if(!al_is_event_queue_empty(fila))
                    {
                        if (done != 1 || start != 1)
                            al_wait_for_event(fila, &menu);
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

        while(start && !done){
            ALLEGRO_EVENT ev;
            al_wait_for_event(fila,&ev);
            if(ev.type==ALLEGRO_EVENT_TIMER){
                aux++;
                if(aux==fps){
                    tjogo--;
                    aux=0;
                }
                redraw=true;
                retorno=recvMsgFromServer(pessoa,DONT_WAIT);
                if(retorno==SERVER_DISCONNECTED){
                    return -1;
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
                        al_destroy_display(display);
                        pessoa[meuId].vivo=0;
                        done=true;
                    default:
                        pessoa[meuId].tecla='8';
                        break;
                }
                sendMsgToServer(&pessoa[meuId],sizeof(jogador));
                pessoa[meuId].tecla==NO_KEY_PRESSED;
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
                //se n�o estiver no meu plano eu n�o printo pq se n�o vai ficar todo mundo num plano s�
                printStatus(pessoa[meuId]);
                al_flip_display();
                if(pessoa[0].abriu == 1){
                    mapa = mapaOpenGate;
                }
                else{
                    mapa = mapaClosedGate;
                }
                if(pessoa[meuId].vidas==0){
                    al_destroy_display(display);
                    end = al_create_display(width*3,height*3);
                    al_draw_bitmap(Defeat,0,0,0);
                    al_flip_display();
                    al_rest(3);
                    al_destroy_display(end);
                    exit(EXIT_SUCCESS);
                }
                if(pessoa[meuId].acabou == 1){
                    al_destroy_display(display);
                    end = al_create_display(width*3,height*3);
                    exit(EXIT_SUCCESS);
                }
                if(tjogo==0){
                    al_destroy_display(display);
                    end = al_create_display(width*3,height*3);
                    if(pessoa[meuId].id == monstro){
                        al_draw_bitmap(Win,0,0,0);
                    }
                    else{
                        al_draw_bitmap(Defeat,0,0,0);
                    }
                    al_flip_display();
                    al_rest(3);
                    al_destroy_display(end);
                    done=true;
                    exit(EXIT_SUCCESS);
                }
                printTela(pessoa[meuId].plano);
            }
        }
    }

    al_destroy_display(menudisplay);
    al_destroy_display(display);
    al_destroy_audio_stream(bg);
    al_destroy_sample(button);

    return 0;
}
