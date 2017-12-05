#include "server.h"
#include <stdio.h>
#include <string.h>
#include <time.h>


#define MSG_MAX_SIZE 350
#define BUFFER_SIZE (MSG_MAX_SIZE)
#define LOGIN_MAX_SIZE 13
#define MAX_CHAT_CLIENTS 6
#define LARGURA_TELA 640
#define ALTURA_TELA 480



typedef struct {
  int X,Y,permissao,id,tecla,pers,tamanho,xb,yb,fome,direcao,x_aux,y_aux,mens;
  char nome[30],time[10],texto[50];
}data;


data dados;
int clock_a=0,c2=0;
int x,y;
int x_player[6],y_player[6];
int vivos[6] = {1,1,1,1,1,1};
int correcaoTamanho_X[5] = {7,24,38,50,62};
int correcaoTamanho_Y[5] = {10,20,30,40,50};
int tamanho[6] = {0,0,0,0,0,0};
int fome[6] = {0,0,0,0,0,0};
int bx=1,by=0;
char marc[ALTURA_TELA] [LARGURA_TELA],map [ALTURA_TELA] [LARGURA_TELA],marcacao [ALTURA_TELA] [LARGURA_TELA];
int jogadores [ALTURA_TELA] [LARGURA_TELA];
data dados;
int posi_inic_x[6] = {10,110,210,310,410,510},posi_inic_y = 440;

// os personagens são colocados de arcordo com o seu id.
char pers[6] = {'L','U','I','S','F','E'};

void monta () {
    int i,j;

    for( i = 0 ; i < ALTURA_TELA  ; i++ ){

        for( j = 0 ; j < LARGURA_TELA ; j++ ){
            map[i] [j] =32;
            marcacao [i] [j] =32;
            jogadores [i] [j] = 32;
        }

    }
}
void GeraPosicao(){

    do{
            srand((unsigned)time(NULL));
            y=rand()%470;
            x=rand()%630;

    }while ( x < 10 || y <10);
}

// recebe a posição anterior para apagar e, a próxima "px" e "py" para marcar o personagem.  
void marcaPosicao(int xx, int yy, int px,int py, char personagem, int ID){
    marcacao[py] [px] = personagem;
    marcacao[yy] [xx] = 32;
    jogadores [py] [px] = ID;
    jogadores [yy] [xx] = 32;
}

int main() 
{   
    FILE *arq;
    time_t start, delta;
    int minutos,segundos,minutosC=0,segundosC=0;
    int t,i,j,k;
    int flagBoladona=0;
    char num[6];
    char client_names[MAX_CHAT_CLIENTS][LOGIN_MAX_SIZE];
    char str_buffer[BUFFER_SIZE];
    data dados_aux;
    serverInit(MAX_CHAT_CLIENTS);
    puts("Server is running!!");
    monta();
    while (1) {
        clock_a++;
        c2++;
        int id = acceptConnection();

        // se não estiver conectado
        if (id != NO_CONNECTION) {
            recvMsgFromClient(&dados_aux, id, WAIT_FOR_IT);
            strcpy(client_names[id], dados_aux.nome);
            strcpy(str_buffer, client_names[id]);
            strcat(str_buffer, " connected to server");
            strcpy(dados.nome,dados_aux.nome);
            printf("%s\n",str_buffer );
            dados.id=id;
            sendMsgToClient(&dados, sizeof(data),id);
            broadcast(&dados, sizeof(data));
        }
        if(flagBoladona){
        delta=time(NULL)-start;
        minutos=(delta/60)%60;
        segundos=delta%60;
        }   
        if(flagBoladona && segundos>segundosC ){
            segundosC=segundos;
            sprintf(dados.time,"%d:%d",minutos,segundos);
            printf("%s\n", dados.time );
        }
        if(segundosC=59){
            segundosC=-1;
        }

        if(minutos==5 && segundos > 0){
            dados.permissao=2;
            broadcast(&dados,sizeof(data));
        }

        struct msg_ret_t msg_ret = recvMsg(&dados_aux);

        //mensagem nova
        if (msg_ret.status == MESSAGE_OK) {


            if(dados_aux.id==0 && dados_aux.permissao == 10 ){
                start=time(NULL);
                flagBoladona=1;
                dados.mens=2;
                broadcast(&dados,sizeof(data));
                
            }    
            if(dados_aux.mens==1){
                strcpy(str_buffer,client_names[dados_aux.id]);
                strcat(str_buffer,": ");
                strcat(str_buffer,dados_aux.texto);
                strcpy(dados.texto,str_buffer);
                dados.mens=1;
                printf("aa %s\n", dados_aux.texto);
                printf("ss %s\n", dados.texto );
                broadcast(&dados,sizeof(data));
                dados_aux.mens=0;
                dados.mens=0;
                arq = fopen("AllegroAquar/Resources/Arquivos/chat.txt","a");
                fprintf(arq, "%s\n",str_buffer);
                fclose(arq);
                strcpy(str_buffer,"");
            }

            
            
            if(dados_aux.tecla == 85){ // up

                if(dados_aux.Y+10 < ALTURA_TELA-10 && map[dados_aux.Y+10] [dados_aux.X]!='l'){
                    dados.Y= dados_aux.Y+10;
                    dados.X = dados_aux.X;
                    dados.tecla= dados_aux.tecla;
                    dados.fome=fome[dados_aux.id];
                    dados.pers=dados_aux.pers;
                    dados.tamanho = tamanho [dados_aux.id];
                    marcaPosicao(dados_aux.X,dados_aux.Y,dados.X,dados.Y, pers[dados_aux.id],dados_aux.id );
                    dados.id = dados_aux.id;
                    dados.permissao=1;
                    x_player[dados_aux.id] = dados.X;
                    y_player[dados_aux.id] = dados.Y;
                    broadcast(&dados, sizeof(data));    
                }
                else {
                    dados.permissao = 0;
                    broadcast(&dados, sizeof(data));   
                }
            }
            else if(dados_aux.tecla == 84){ // down
                if(dados_aux.Y-10 >= correcaoTamanho_Y[dados_aux.tamanho] && map[dados_aux.Y-10] [dados_aux.X]!='l'){
                    dados.Y= dados_aux.Y-10;
                    //printf("x %d y %d\n", dados_aux.X, dados_aux.Y );
                    dados.X = dados_aux.X;
                    dados.id = dados_aux.id;
                    dados.fome=fome[dados_aux.id];
                    dados.tecla= dados_aux.tecla;
                    dados.pers=dados_aux.pers;
                    dados.tamanho = tamanho [dados_aux.id];
                    //printf("x %d y %d\n", dados.X, dados.Y );
                    marcaPosicao(dados_aux.X,dados_aux.Y,dados.X,dados.Y, pers[dados_aux.id],dados_aux.id);
                    dados.permissao=1;
                    x_player[dados_aux.id] = dados.X;
                    y_player[dados_aux.id] = dados.Y;
                    broadcast(&dados, sizeof(data));    
                }
                else {
                    dados.permissao = 0;
                    broadcast(&dados, sizeof(data));   
                }
            }
            else if(dados_aux.tecla == 83 ){ // left
                if(dados_aux.X+10 < LARGURA_TELA-10 && map[dados_aux.Y] [dados_aux.X+10]!='l'){
                    dados.X= dados_aux.X+10;
                    dados.Y = dados_aux.Y;
                    dados.id = dados_aux.id;

                    dados.fome=fome[dados_aux.id];
                    
                    dados.tecla= dados_aux.tecla;
                    dados.pers = dados_aux.pers;
                    dados.tamanho = tamanho [dados_aux.id];
                    marcaPosicao(dados_aux.X,dados_aux.Y,dados.X,dados.Y, pers[dados_aux.id],dados_aux.id);
                    dados.permissao=1;
                    x_player[dados_aux.id] = dados.X;
                    y_player[dados_aux.id] = dados.Y;
                    broadcast(&dados, sizeof(data));    
                }
                else {
                    dados.permissao = 0;
                    broadcast(&dados, sizeof(data));   
                }
            }
            else if(dados_aux.tecla == 82){ // right
                if(dados_aux.X-10 >=10 && map[dados_aux.Y] [dados_aux.X-10]!='l'){
                    dados.X= dados_aux.X-10;
                    dados.Y = dados_aux.Y;
                    dados.id = dados_aux.id;
                    dados.tecla= dados_aux.tecla;
                    dados.fome=fome[dados_aux.id];
                    dados.pers=dados_aux.pers;
                    dados.tamanho = tamanho [dados_aux.id];
                    marcaPosicao(dados_aux.X,dados_aux.Y,dados.X,dados.Y, pers[dados_aux.id],dados_aux.id);
                    dados.permissao=1;
                    x_player[dados_aux.id] = dados.X;
                    y_player[dados_aux.id] = dados.Y;
                    broadcast(&dados, sizeof(data));    
                }
                else {
                    dados.permissao = 0;
                    broadcast(&dados, sizeof(data));   
                }
            }

            else if (dados_aux.tecla== 23){

               if(dados_aux.direcao ==0 && dados_aux.X+10 < LARGURA_TELA-10 ){
                    
                    dados.X= dados_aux.X+10;
                    dados.Y = dados_aux.Y;
                    dados.id = dados_aux.id;
                    dados.tecla = dados_aux.tecla;
                    dados.pers = dados_aux.pers;
                    dados.direcao = dados_aux.direcao;
                    marcaPosicao(dados_aux.X,dados_aux.Y,dados.X,dados.Y, pers[dados_aux.id],dados_aux.id);
                    dados.permissao=1;
                    for(i=1;i<correcaoTamanho_Y[dados_aux.tamanho] && dados_aux.Y+i < ALTURA_TELA;i++){
                        for(j=1;j<=11;j++){
                            if(marcacao[dados_aux.Y+i] [(dados_aux.X+correcaoTamanho_X[dados_aux.tamanho])+j]=='r'){
                                marcacao[dados_aux.Y+i] [(dados_aux.X+correcaoTamanho_X[dados_aux.tamanho])+j]=32;
                                fome[dados_aux.id] = dados_aux.fome+1;
                                dados.x_aux = (dados_aux.X+correcaoTamanho_X[dados_aux.tamanho])+j;
                                dados.y_aux = dados_aux.Y+i;
                                dados.mens =3;
                                break;
                            }
                            else if(marcacao[dados_aux.Y-i] [(dados_aux.X+correcaoTamanho_X[dados_aux.tamanho])+j]=='r'){
                                marcacao[dados_aux.Y-i] [(dados_aux.X+correcaoTamanho_X[dados_aux.tamanho])+j]=32;
                                fome[dados_aux.id] = dados_aux.fome+1;
                                dados.x_aux = (dados_aux.X+correcaoTamanho_X[dados_aux.tamanho])+j;
                                dados.y_aux = dados_aux.Y-i;
                                dados.mens =3;
                                break;
                            }
                            else if (map[dados_aux.Y+i] [(dados_aux.X+correcaoTamanho_X[dados_aux.tamanho])+j]=='g' && dados_aux.tamanho > 1){
                                marcacao[dados_aux.Y+i] [(dados_aux.X+correcaoTamanho_X[dados_aux.tamanho])+j]=32;
                                fome[dados_aux.id] = dados_aux.fome+5;
                                dados.x_aux=(dados_aux.X+correcaoTamanho_X[dados_aux.tamanho])+j;
                                dados.y_aux=dados_aux.Y+i;
                                dados.mens=3;
                                break;
                                
                            }
                            else {
                                dados.mens=0;
                            }
                        }
                        if(fome[dados_aux.id] > dados_aux.fome)
                                break;
                    }   
                }
                else if(dados_aux.direcao ==1 && dados_aux.X-10 >= 10 ){
                    dados.X= dados_aux.X-10;
                    dados.Y = dados_aux.Y;
                    dados.id = dados_aux.id;
                    dados.tecla = dados_aux.tecla;
                    dados.pers = dados_aux.pers;
                    dados.direcao = dados_aux.direcao;
                    marcaPosicao(dados_aux.X,dados_aux.Y,dados.X,dados.Y, pers[dados_aux.id],dados_aux.id);
                    dados.permissao=1;
                    for(i=1;i<correcaoTamanho_Y[dados_aux.tamanho]+5 && dados_aux.Y+i < ALTURA_TELA ;i++){
                        for(j=1;j<=11;j++){

                            if(marcacao[dados_aux.Y+i] [(dados_aux.X-correcaoTamanho_X[dados_aux.tamanho])-j]=='r'){
                                marcacao[dados_aux.Y+i] [(dados_aux.X-correcaoTamanho_X[dados_aux.tamanho])-j]=32;
                                fome[dados_aux.id] = dados_aux.fome+1;
                                dados.x_aux = (dados_aux.X-correcaoTamanho_X[dados_aux.tamanho])-j;
                                dados.y_aux = dados_aux.Y+i;
                                dados.mens=3;
                                break;
                            }
                            else if(marcacao[dados_aux.Y-i] [(dados_aux.X-correcaoTamanho_X[dados_aux.tamanho])-j]=='r'){
                                marcacao[dados_aux.Y-i] [(dados_aux.X-correcaoTamanho_X[dados_aux.tamanho])-j]=32;
                                fome[dados_aux.id] = dados_aux.fome+1;
                                dados.x_aux = (dados_aux.X-correcaoTamanho_X[dados_aux.tamanho])-j;
                                dados.y_aux = dados_aux.Y-i;
                                dados.mens=3;
                                break;
                            }
                            else if (map[dados_aux.Y+i] [(dados_aux.X-correcaoTamanho_X[dados_aux.tamanho])-j]=='g' && dados_aux.tamanho > 1){
                                marcacao[dados_aux.Y+i] [(dados_aux.X-correcaoTamanho_X[dados_aux.tamanho])-j]=32;
                                fome[dados_aux.id] = dados_aux.fome+5;
                                dados.x_aux=(dados_aux.X-correcaoTamanho_X[dados_aux.tamanho])-j;
                                dados.y_aux=dados_aux.Y+i;
                                dados.mens=3;
                                break;
                                
                            }
                            else {
                                dados.mens=0;
                            }
                            
                            for(k=0;k<6;k++){
                                if (map[dados_aux.Y+i] [(dados_aux.X-correcaoTamanho_X[dados_aux.tamanho])-j]==pers[k] && dados_aux.fome >= fome[k]+30){
                                    
                                    fome[dados_aux.id] = dados_aux.fome+15;
                                    fome [k] = -1;
                                    dados.x_aux=(dados_aux.X-correcaoTamanho_X[dados_aux.tamanho])-j;
                                    dados.y_aux=dados_aux.Y+i;
                                    dados.permissao=1;
                                    break;
                                    
                                }
                                else {
                                    dados.mens=0;
                                }
                            }
                        }
                        if(fome[dados_aux.id] > dados_aux.fome)
                                break;
                    }   
                }
                dados.fome=fome[dados_aux.id];
                if(dados.fome > 30 && dados.fome <= 60){
                    tamanho[dados_aux.id]=1;
                }
                else if(dados.fome > 60 && dados.fome <= 90){
                    tamanho[dados_aux.id]=2;
                }
                else if(dados.fome > 90 && dados.fome <= 120){
                    tamanho[dados_aux.id]=3;
                }
                else if(dados.fome > 120 && dados.fome <= 150){
                    tamanho[dados_aux.id]=4;
                }
                x_player[dados_aux.id] = dados.X;
                y_player[dados_aux.id] = dados.Y;
                dados.tamanho=tamanho[dados_aux.id];
                //printf("yb %d xb %d \n", dados.y_aux,  dados.x_aux);
                broadcast(&dados, sizeof(data)); 
            }
        } 
        else if (msg_ret.status == DISCONNECT_MSG) {
            sprintf(str_buffer, "%s disconnected", client_names[msg_ret.client_id]);
            printf("%s disconnected, id = %d is free\n",client_names[msg_ret.client_id], msg_ret.client_id);
            if(msg_ret.client_id==0){
                segundos=0;
                segundosC=0;
                flagBoladona=0;
            }
            vivos[msg_ret.client_id]=1;
            strcpy(client_names[msg_ret.client_id],"");
            tamanho[msg_ret.client_id]=0;
            fome[msg_ret.client_id]=0;
            dados.id=msg_ret.client_id;
            dados.fome=-1;
            dados.X=x_player[dados_aux.id];
            dados.Y=y_player[dados_aux.id];
            vivos[msg_ret.client_id]=0;            
            broadcast(&dados,sizeof(data));
        }
        if(clock_a == 500){
            //printf("ok\n"); 
            clock_a=0;
            bx++;
            if(bx>=5)
                bx=1;
            dados.id=6;
            dados.xb = bx;
            GeraPosicao();
            dados.x_aux=x;
            dados.y_aux=y;
            if(marcacao[y] [x] ==32)
                marcacao[y] [x] = 'r';
            for(k=0;k<6;k++){
                if (fome[k]==-1){
                    dados.id=k;
                    dados.fome=-1;
                    vivos[k]=0;
                    for( i = 0 ; i < ALTURA_TELA  ; i++ ){
                        for( j = 0 ; j < LARGURA_TELA ; j++ ){
                            if(jogadores[i][j]==k){
                               dados.X=j;
                               dados.Y=i;
                            }   
                        }
                    }
                }
            }
            broadcast(&dados,sizeof(data));
        } 
        //peixe verde
         if(c2 == 2000){
            //printf("ok\n"); 
            c2=0;
            GeraPosicao();
            dados.x_aux=x;
            dados.y_aux=y;

            if(x%2==0){ // se a posição for par, gerará um "especial" podendo ser um peixe ou uma armadilha.

                //printf("Numero random: %d\n",x);
                if(x<=430){
                    //printf("OK, entrou no if \n");
                 if(marcacao[y][x] == 32){
                    dados.id=7;
                    marcacao[y][x] = 'g';
                    //printf("Peixe verde aparece\n");
                   }

                 }

                 else{
                    //printf("OK, entrou no else\n");

                    if(marcacao[y][x] == 32){
                    dados.id=7;
                    marcacao[y][x] = 't';
                    //printf("armadilha aparece\n");
                   }
                }

            broadcast(&dados,sizeof(data));
        }  


        }
    }
}


