#include "server.h"
#include <stdio.h>
#include <string.h>

#define MSG_MAX_SIZE 350
#define BUFFER_SIZE (MSG_MAX_SIZE)
#define LOGIN_MAX_SIZE 13
#define MAX_CHAT_CLIENTS 6
#define LARGURA_TELA 640
#define ALTURA_TELA 480

typedef struct {
      int X,Y,permissao,id,tecla,pers,tamanho,xb,yb;
      char nome[30],direcao;
}data;
data dados;
int clock,c2;
int bx=1,by=0;
char map [ALTURA_TELA] [LARGURA_TELA],marcao [ALTURA_TELA] [LARGURA_TELA];
data dados;
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
    /*
    for( i_des = 5 ; i_des < 10 ; i_des++ ){

        for( j_des = 10 ; j_des < 15 ; j_des++ ){
            map[i_des] [j_des] ='l';
            tela[i_des] [j_des] ='l';
        }

    }
    for( i_des = 13 ; i_des < 18 ; i_des++ ){

        for( j_des = 10 ; j_des < 15 ; j_des++ ){
            map[i_des] [j_des] ='l';
            tela[i_des] [j_des] ='l';
        }

    }

    for( i_des = 8 ; i_des < 15 ; i_des++ ){

        for( j_des = 25 ; j_des < 30 ; j_des++ ){
            map[i_des] [j_des] ='l';
            tela[i_des] [j_des] ='l';
        }

    }
*/

}

// recebe a posição anterior para apagar e, a próxima "px" e "py" para marcar o personagem.  
void marcaPosicao(int xx, int yy, int px,int py, char personagem){
    marcao[py] [px] = personagem;
    marcao[yy] [xx] = 32;
}

int main() {
    int t;
    char client_names[MAX_CHAT_CLIENTS][LOGIN_MAX_SIZE];
    char str_buffer[BUFFER_SIZE];
    data dados_aux;
    serverInit(MAX_CHAT_CLIENTS);
    puts("Server is running!!");
    monta();
    while (1) {
        clock++;
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
                    dados.pers=dados_aux.pers;
                    marcaPosicao(dados_aux.X,dados_aux.Y,dados.X,dados.Y, pers[dados_aux.id]);
                    dados.id = dados_aux.id;
                    dados.permissao=1;
                    broadcast(&dados, sizeof(data));    
                }
                else {
                    dados.permissao = 0;
                    broadcast(&dados, sizeof(data));   
                }
            }
            if(dados_aux.tecla == 84){ // down
                if(dados_aux.Y-10 >= 10 && map[dados_aux.Y-10] [dados_aux.X]!='l'){
                    dados.Y= dados_aux.Y-10;
                    //printf("x %d y %d\n", dados_aux.X, dados_aux.Y );
                    dados.X = dados_aux.X;
                    dados.id = dados_aux.id;
                    dados.tecla= dados_aux.tecla;
                    dados.pers=dados_aux.pers;
                    //printf("x %d y %d\n", dados.X, dados.Y );
                    marcaPosicao(dados_aux.X,dados_aux.Y,dados.X,dados.Y, pers[dados_aux.id]);
                    dados.permissao=1;
                    broadcast(&dados, sizeof(data));    
                }
                else {
                    dados.permissao = 0;
                    broadcast(&dados, sizeof(data));   
                }
            }
            if(dados_aux.tecla == 83 ){ // left
                if(dados_aux.X+10 < LARGURA_TELA-10 && map[dados_aux.Y] [dados_aux.X+10]!='l'){
                    dados.X= dados_aux.X+10;
                    dados.Y = dados_aux.Y;
                    dados.id = dados_aux.id;
                    dados.tecla= dados_aux.tecla;
                    dados.pers=dados_aux.pers;
                    marcaPosicao(dados_aux.X,dados_aux.Y,dados.X,dados.Y, pers[dados_aux.id]);
                    dados.permissao=1;
                    broadcast(&dados, sizeof(data));    
                }
                else {
                    dados.permissao = 0;
                    broadcast(&dados, sizeof(data));   
                }
            }
            if(dados_aux.tecla == 82){ // right
                if(dados_aux.X-10 >=10 && map[dados_aux.Y] [dados_aux.X-10]!='l'){
                    dados.X= dados_aux.X-10;
                    dados.Y = dados_aux.Y;
                    dados.id = dados_aux.id;
                    dados.tecla= dados_aux.tecla;
                    dados.pers=dados_aux.pers;
                    marcaPosicao(dados_aux.X,dados_aux.Y,dados.X,dados.Y, pers[dados_aux.id]);
                    dados.permissao=1;
                    broadcast(&dados, sizeof(data));    
                }
                else {
                    dados.permissao = 0;
                    broadcast(&dados, sizeof(data));   
                }
            }
        } 
        else if (msg_ret.status == DISCONNECT_MSG) {
            sprintf(str_buffer, "%s disconnected", client_names[msg_ret.client_id]);
            printf("%s disconnected, id = %d is free\n",client_names[msg_ret.client_id], msg_ret.client_id);
        }

        if(clock == 350){
            printf("ok\n"); 
            clock=0;
            bx++;
            if(bx>=5)
                bx=1;
            dados.id=7;
            dados.xb = bx;
            dados.tamanho=1;
            broadcast(&dados,sizeof(data));
        }   
    }
}
