#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define width 400
#define height 300
#define tilesize 20
#define mClientes 2
#define monstro 1
int leCollision(int x,int y,int ex,int ey,int lado){//Funcao de coliso
    if(x+lado<ex||x>ex+lado||y+lado<ey||y>ey+lado){
        return 0;
    }
    return 1;
}
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
void initPes(int num, jogador *pessoa){
    switch(num){
        case 0:
            pessoa->vivo=1;
            pessoa->vidas=3;
            pessoa->lado=0;
            pessoa->plano=0;
            pessoa->planoi=0;
            pessoa->id=0;
            pessoa->inicialx=80;
            pessoa->inicialy=100;
            pessoa->x=80;
            pessoa->y=100;
            pessoa->key=0;
            pessoa->speed=10;
            pessoa->acabou=0;
            pessoa->abriu=0;
            break;
        case 1:
            pessoa->vivo=1;
            pessoa->vidas=3;
            pessoa->plano=2;
            pessoa->planoi=2;
            pessoa->lado=0;
            pessoa->id=1;
            pessoa->x=300;
            pessoa->y=100;
            pessoa->inicialx=300;
            pessoa->inicialy=100;
            pessoa->key=0;
            pessoa->speed=10;
            pessoa->acabou=0;
            pessoa->abriu=0;
            break;
        case 2:
            pessoa->vivo=1;
            pessoa->vidas=3;
            pessoa->plano=6;
            pessoa->planoi=6;
            pessoa->lado=1;
            pessoa->id=2;
            pessoa->x=80;
            pessoa->y=200;
            pessoa->inicialx=80;
            pessoa->inicialy=200;
            pessoa->speed=10;
            pessoa->key=0;
            pessoa->acabou=0;
            pessoa->abriu=0;
            break;
        case 3:
            pessoa->vivo=1;
            pessoa->vidas=3;
            pessoa->plano=8;
            pessoa->planoi=8;
            pessoa->lado=1;
            pessoa->x=300;
            pessoa->y=200;
            pessoa->id=3;
            pessoa->inicialx=300;
            pessoa->inicialy=200;
            pessoa->speed=10;
            pessoa->key=0;
            pessoa->acabou=0;
            pessoa->abriu=0;
            break;
        case 4://Monstro
            pessoa->vivo=1;
            pessoa->vidas=3;//Coloquei vida mas ele nunca vai perder vida
            pessoa->plano=4;
            pessoa->planoi=4;
            pessoa->x=200;
            pessoa->y=140;
            pessoa->id=4;
            pessoa->key=0;
            pessoa->inicialx=200;
            pessoa->inicialy=140;
            pessoa->speed=20;
            pessoa->acabou=0;
            pessoa->abriu=0;
            break;
    }
    return;
}
void getrand(int *x,int *y,int c){
    srand(time(NULL)+c);
    *y=rand()%15;
    *x=rand()%20;
}
chave initChaves(int i){
    chave key;
    getrand(&key.x,&key.y,i);
    key.vivo=1;
    switch(i){
        case 0:
            key.plano=1;
            break;
        case 1:
            key.plano=3;
            break;
        case 2:
            key.plano=4;
            break;
        case 3:
            key.plano=5;
            break;
        case 4:
            key.plano=7;
            break;
    }
    return key;
}
int tudovivo(jogador pessoa[5]){
    int i;
    for(i=0;i<mClientes;i++){
        if(pessoa[i].vivo==1&&pessoa[i].id!=monstro){
            return 0;
        }
    }
    return 1;
}
int main(){
    int monstrowin=0;
    int chavesT=0;
    int flag=0;
    int neox=0;
    int neoy=0;
    int c1,c2,c3;
    int Plano;
    int colisoes[10][15][20];
    jogador *pessoa=(jogador*)malloc(5*sizeof(jogador));
    jogador novo;
    FILE **arq=NULL;
    char plano[15][20];
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
    for(c1=0;c1<5;c1++){
        do{
            pessoa[0].chaves[c1]=initChaves(c1);
        }while(colisoes[pessoa[0].chaves[c1].plano][pessoa[0].chaves[c1].y][pessoa[0].chaves[c1].x]==1);
        pessoa[0].chaves[c1].x*=20;
        pessoa[0].chaves[c1].y*=20;
        printf("%i %i %i\n",pessoa[0].chaves[c1].x,pessoa[0].chaves[c1].y,pessoa[0].chaves[c1].vivo);
    }
    for(c1=0;c1<5;c1++){
        printf("Keys - %i %i %i %i\n",pessoa[0].chaves[c1].plano,pessoa[0].chaves[c1].x,pessoa[0].chaves[c1].y,pessoa[0].chaves[c1].vivo);
    }
    int nClients=0;
    serverInit(5);
    while(nClients<mClientes){
        int id=acceptConnection();
        if(id!=NO_CONNECTION){
            nClients++;
            sendMsgToClient(&id,sizeof(int),id);
            initPes(id, &pessoa[nClients-1]);
        }
    }
    for(c1=0;c1<5;c1++){
        printf("Keys - %i %i %i %i\n",pessoa[0].chaves[c1].plano,pessoa[0].chaves[c1].x,pessoa[0].chaves[c1].y,pessoa[0].chaves[c1].vivo);
    }
    broadcast(pessoa,mClientes*sizeof(jogador));
    while(1){
        rejectConnection();
        struct msg_ret_t t=recvMsg(&novo);
        if(t.status==MESSAGE_OK){
            pessoa[novo.id]=novo;
            if(pessoa[novo.id].id==monstro&&pessoa[novo.id].plano==1){
                Plano=9;
            }
            else{
                Plano=pessoa[novo.id].plano;
            }
            if(pessoa[novo.id].tecla=='u'){
                neoy=(pessoa[novo.id].y-10)/tilesize;
                neox=(pessoa[novo.id].x+10)/tilesize;
                if(colisoes[Plano][neoy][neox]==0){
                    if(pessoa[novo.id].y-pessoa[novo.id].speed>=0){
                        pessoa[novo.id].y-=pessoa[novo.id].speed;
                    }
                    else if(pessoa[novo.id].plano!=0 && pessoa[novo.id].plano!=1 && pessoa[novo.id].plano!=2){
                        pessoa[novo.id].plano-=3;
                        pessoa[novo.id].y=height-20;
                    }
                }
            }
            if(pessoa[novo.id].tecla=='d'){
                neoy=(pessoa[novo.id].y+20)/tilesize;
                neox=(pessoa[novo.id].x+10)/tilesize;
                if(colisoes[Plano][neoy][neox]==0){
                    if(pessoa[novo.id].y+pessoa[novo.id].speed<height-tilesize){
                        pessoa[novo.id].y+=pessoa[novo.id].speed;
                    }
                    else if(pessoa[novo.id].plano!=6&&pessoa[novo.id].plano!=7&&pessoa[novo.id].plano!=8){
                        pessoa[novo.id].plano+=3;
                        pessoa[novo.id].y=0;
                    }
                }
            }
            if(pessoa[novo.id].tecla=='l'){
                neoy=(pessoa[novo.id].y+10)/tilesize;
                neox=(pessoa[novo.id].x-10)/tilesize;
                if(colisoes[Plano][neoy][neox]==0){
                    if(pessoa[novo.id].x-pessoa[novo.id].speed>=0){
                        pessoa[novo.id].x-=pessoa[novo.id].speed;
                    }
                    else if(pessoa[novo.id].plano!=0&&pessoa[novo.id].plano!=3&&pessoa[novo.id].plano!=6){
                        pessoa[novo.id].plano-=1;
                        pessoa[novo.id].x=width-20;
                    }
                }
            }
            if(pessoa[novo.id].tecla=='r'){
                neoy=(pessoa[novo.id].y+10)/tilesize;
                neox=(pessoa[novo.id].x+20)/tilesize;
                if(colisoes[Plano][neoy][neox]==0){
                    if(pessoa[novo.id].x+pessoa[novo.id].speed<width-tilesize){
                        pessoa[novo.id].x+=pessoa[novo.id].speed;
                    }
                    else if(pessoa[novo.id].plano!=2&&pessoa[novo.id].plano!=5&&pessoa[novo.id].plano!=8){
                        pessoa[novo.id].plano+=1;
                        pessoa[novo.id].x=0;
                    }
                }
            }
            if(pessoa[novo.id].id!=monstro){
                if(leCollision(pessoa[novo.id].x,pessoa[novo.id].y,pessoa[monstro].x,pessoa[monstro].y,tilesize)==1&&pessoa[monstro].plano==pessoa[novo.id].plano&&pessoa[novo.id].vivo==1&&pessoa[monstro].vivo==1){
                    pessoa[novo.id].x=pessoa[novo.id].inicialx;
                    pessoa[novo.id].y=pessoa[novo.id].inicialy;
                    pessoa[novo.id].plano=pessoa[novo.id].planoi;
                    pessoa[novo.id].vidas--;
                    int aux=0;
                    int j;
                    for(j=0;j<5;j++){
                        if(pessoa[0].chaves[j].vivo==0&&aux<pessoa[novo.id].key){
                            aux++;
                            do{
                                pessoa[0].chaves[j]=initChaves(j);
                            }while(colisoes[pessoa[novo.id].plano][pessoa[0].chaves[j].y][pessoa[0].chaves[j].x]==1);
                            pessoa[0].chaves[j].x*=20;
                            pessoa[0].chaves[j].y*=20;
                            pessoa[0].chaves[j].vivo=1;
                        } 
                    }
                    chavesT-=pessoa[novo.id].key;
                    pessoa[novo.id].key=0;
                    if(pessoa[novo.id].vidas==0){
                            pessoa[novo.id].vivo=0;
                    }
                }
                for(c1=0;c1<5;c1++){
                    if(pessoa[0].chaves[c1].vivo==1&&leCollision(pessoa[novo.id].x,pessoa[novo.id].y,pessoa[0].chaves[c1].x,pessoa[0].chaves[c1].y,tilesize)==1&&pessoa[0].chaves[c1].plano==pessoa[novo.id].plano){
                        pessoa[0].chaves[c1].vivo=0;
                        pessoa[novo.id].key++;
                        chavesT++;
                        printf("%i\n",chavesT);
                    }
                }
            }
            else{
                int i;
                for(i=0;i<mClientes;i++){
                    if(leCollision(pessoa[novo.id].x,pessoa[novo.id].y,pessoa[i].x,pessoa[i].y,tilesize)==1&&pessoa[i].plano==pessoa[novo.id].plano&&i!=1&&pessoa[i].vivo==1&&i!=pessoa[novo.id].id){
                        pessoa[i].x=pessoa[i].inicialx;
                        pessoa[i].y=pessoa[i].inicialy;
                        pessoa[i].plano=pessoa[i].planoi;
                        pessoa[i].vidas--;
                        int j;
                        int aux=0;
                        for(j=0;j<5;j++){
                            if(pessoa[0].chaves[j].vivo==0&&aux<pessoa[i].key){
                                aux++;
                                do{
                                    pessoa[0].chaves[j]=initChaves(j);
                                }while(colisoes[pessoa[i].plano][pessoa[0].chaves[j].y][pessoa[0].chaves[j].x]==1);
                                pessoa[0].chaves[j].x*=20;
                                pessoa[0].chaves[j].y*=20;
                                pessoa[0].chaves[j].vivo=1;
                            } 
                        }
                        chavesT-=pessoa[novo.id].key;
                        pessoa[i].key=0;
                        if(pessoa[i].vidas==0){
                            pessoa[i].vivo=0;
                        }
                    }
                }
            }
            if(chavesT==1){
                pessoa[0].abriu=1;
            }
            else{
                pessoa[0].abriu=0;
            }
            if(pessoa[0].abriu==1&&pessoa[novo.id].plano==1&&pessoa[novo.id].x>=120&&pessoa[novo.id].x<=280&&pessoa[novo.id].vivo==1&&pessoa[novo.id].y<=20&&pessoa[novo.id].id!=monstro){
                pessoa[novo.id].acabou=1;
                pessoa[novo.id].vivo=0;
            }
            monstrowin=tudovivo(pessoa);
            if(monstrowin==1){
                pessoa[novo.id].acabou=1;
            }
            broadcast(pessoa,mClientes*sizeof(jogador));
        }
    }
}