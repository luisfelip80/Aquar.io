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
      int X,Y,permissao,id,tecla,pers,tamanho,xb,yb,fome,direcao,x_aux,y_aux;
      char nome[30];
}data;
data dados;
int clock_a,c2;
int x,y;
int x_player[6],y_player[6];
int vivos[6] = {1,1,1,1,1,1};
int correcaoTamanho_X[5] = {7,24,38,50,62};
int correcaoTamanho_Y[5] = {17,28,36,46,53};
int tamanho[6] = {0,0,0,0,0,0};
int fome[6] = {0,0,0,0,0,0};
int bx=1,by=0;
char map [ALTURA_TELA] [LARGURA_TELA],marcao [ALTURA_TELA] [LARGURA_TELA];
data dados;
int posi_inic_x[6] = {10,110,210,310,410,510},posi_inic_y = 440;

// os personagens são colocados de arcordo com o seu id.
char pers[6] = {'L','U','I','S','F','E'};

void monta () {
    int i_des=0,j_des=0;

    for( i_des = 0 ; i_des < ALTURA_TELA  ; i_des++ ){

        for( j_des = 0 ; j_des < LARGURA_TELA ; j_des++ ){
            map[i_des] [j_des] =32;
            marcao [i_des] [j_des] =32;
        }

    }
    map[40] [530] ='l';
    map[420] [240] ='l';
    map[310] [120] ='l';
}
void GeraPosicao(){

    do{
            srand((unsigned)time(NULL));
            y=rand()%470;
            x=rand()%630;

    }while (( x < 10 || y <10) && x%10!=0 && y%10!=0);
}

// recebe a posição anterior para apagar e, a próxima "px" e "py" para marcar o personagem.  
void marcaPosicao(int xx, int yy, int px,int py, char personagem){
    marcao[py] [px] = personagem;
    marcao[yy] [xx] = 32;
}

int main() {
    int t,i,j;
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
            recvMsgFromClient(&dados, id, WAIT_FOR_IT);
            strcpy(client_names[id], dados.nome);
            strcpy(str_buffer, client_names[id]);
            strcat(str_buffer, " connected to server");
            printf("%s\n",str_buffer );
            dados.id=id;
            sendMsgToClient(&dados, sizeof(data),id);
            broadcast(&dados, sizeof(data));
        }

        struct msg_ret_t msg_ret = recvMsg(&dados_aux);
        //mensagem nova
        if (msg_ret.status == MESSAGE_OK) {

            if(dados_aux.tecla == 85){ // up

                if(dados_aux.Y+10 < ALTURA_TELA-10 && map[dados_aux.Y+10] [dados_aux.X]!='l'){
                    dados.Y= dados_aux.Y+10;
                    dados.X = dados_aux.X;
                    dados.tecla= dados_aux.tecla;
                    dados.fome=fome[dados_aux.id];
                    dados.pers=dados_aux.pers;
                    dados.tamanho = tamanho [dados_aux.id];
                    marcaPosicao(dados_aux.X,dados_aux.Y,dados.X,dados.Y, pers[dados_aux.id]);
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
                    marcaPosicao(dados_aux.X,dados_aux.Y,dados.X,dados.Y, pers[dados_aux.id]);
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
                    marcaPosicao(dados_aux.X,dados_aux.Y,dados.X,dados.Y, pers[dados_aux.id]);
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
                    marcaPosicao(dados_aux.X,dados_aux.Y,dados.X,dados.Y, pers[dados_aux.id]);
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

               if(dados_aux.direcao ==0 && dados_aux.X+10 < LARGURA_TELA-10){
                    
                    dados.X= dados_aux.X+10;
                    dados.Y = dados_aux.Y;
                    dados.id = dados_aux.id;
                    dados.tecla = dados_aux.tecla;
                    dados.pers = dados_aux.pers;
                    dados.direcao = dados_aux.direcao;
                    marcaPosicao(dados_aux.X,dados_aux.Y,dados.X,dados.Y, pers[dados_aux.id]);
                    dados.permissao=1;
                    for(i=1;i<correcaoTamanho_Y[dados_aux.tamanho]+5;i++){
                        for(j=1;j<=11;j++){
                            if(map[dados_aux.Y+i] [(dados_aux.X+correcaoTamanho_X[dados_aux.tamanho])+j]=='r'){
                                marcao[dados_aux.Y+i] [(dados_aux.X+correcaoTamanho_X[dados_aux.tamanho])+j]=32;
                                fome[dados_aux.id] = dados_aux.fome+1;
                                dados.x_aux = dados_aux.X+j;
                                dados.y_aux = dados_aux.Y+i;
                                printf("y %d x %d \n", dados.y_aux,  dados.x_aux);
                                break;
                            }
                            else if (map[dados_aux.Y+i] [(dados_aux.X+correcaoTamanho_X[dados_aux.tamanho])+j]=='l' && dados_aux.tamanho > 1){
                                marcao[dados_aux.Y+i] [(dados_aux.X+correcaoTamanho_X[dados_aux.tamanho])+j]=32;
                                fome[dados_aux.id] = dados_aux.fome+5;
                                dados.x_aux=(dados_aux.X+correcaoTamanho_X[dados_aux.tamanho])+j;
                                dados.y_aux=dados_aux.Y+i;
                                printf("y %d x %d \n", dados.y_aux,  dados.x_aux);
                                break;
                                
                            }
                            if(fome[dados_aux.id] > dados_aux.fome)
                                break;
                        }
                    }   
                }
                else if(dados_aux.direcao ==1 && dados_aux.X-10 >= 10 ){
                    dados.X= dados_aux.X-10;
                    dados.Y = dados_aux.Y;
                    dados.id = dados_aux.id;
                    dados.tecla = dados_aux.tecla;
                    dados.pers = dados_aux.pers;
                    dados.direcao = dados_aux.direcao;
                    marcaPosicao(dados_aux.X,dados_aux.Y,dados.X,dados.Y, pers[dados_aux.id]);
                    dados.permissao=1;
                    for(i=1;i<correcaoTamanho_Y[dados_aux.tamanho]+5;i++){
                        for(j=1;j<=11;j++){

                            if(marcao[dados_aux.Y+i] [(dados_aux.X+correcaoTamanho_X[dados_aux.tamanho])-j]=='r'){
                                marcao[dados_aux.Y+i] [(dados_aux.X+correcaoTamanho_X[dados_aux.tamanho])-j]=32;
                                fome[dados_aux.id] = dados_aux.fome+1;
                                dados.x_aux = (dados_aux.X+correcaoTamanho_X[dados_aux.tamanho])-j;
                                dados.y_aux = dados_aux.Y+i;
                                printf("y %d x %d \n", dados.y_aux,  dados.x_aux);
                                break;
                                
                            }
                            else if (map[dados_aux.Y+i] [(dados_aux.X+correcaoTamanho_X[dados_aux.tamanho])-j]=='l' && dados_aux.tamanho > 1){
                                marcao[dados_aux.Y+i] [(dados_aux.X+correcaoTamanho_X[dados_aux.tamanho])-j]=32;
                                fome[dados_aux.id] = dados_aux.fome+5;
                                dados.x_aux=(dados_aux.X+correcaoTamanho_X[dados_aux.tamanho])-j;
                                dados.y_aux=dados_aux.Y+i;
                                printf("y %d x %d \n", dados.y_aux,  dados.x_aux);
                                break;
                                
                            }
                            if(fome[dados_aux.id] > dados_aux.fome)
                                break;
                        }
                    }   
                }
                dados.fome=fome[dados_aux.id];
                if(dados.fome > 30 && dados.fome <= 50){
                    tamanho[dados_aux.id]=1;
                }
                else if(dados.fome > 50 && dados.fome <= 100){
                    tamanho[dados_aux.id]=2;
                }
                else if(dados.fome > 100 && dados.fome <= 150){
                    tamanho[dados_aux.id]=3;
                }
                else if(dados.fome > 150 && dados.fome <= 200){
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
            dados.id=msg_ret.client_id;
            dados.fome=-1;
            dados.X=x_player[dados_aux.id];
            dados.Y=y_player[dados_aux.id];
            vivos[msg_ret.client_id]=0;            
            broadcast(&dados,sizeof(data));
        }
        if(clock_a == 350){
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

            if(marcao[y] [x] == 32){
                marcao[y] [x] = 'r';
            }
            printf("x %d  y %d\n", x, y);
            broadcast(&dados,sizeof(data));
        }   
    }
}