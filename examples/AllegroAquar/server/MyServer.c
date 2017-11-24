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
      int X,Y,permissao,id,tecla;
      char nome[30];
}data;
data move;

char map [ALTURA_TELA] [LARGURA_TELA],marcao [ALTURA_TELA] [LARGURA_TELA];
data move;
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
    data move_aux;
    serverInit(MAX_CHAT_CLIENTS);
    puts("Server is running!!");
    monta();
    while (1) {
        int id = acceptConnection();

        // se não estiver conectado
        if (id != NO_CONNECTION) {
            recvMsgFromClient(&move, id, WAIT_FOR_IT);
            strcpy(client_names[id], move.nome);
            strcpy(str_buffer, client_names[id]);
            strcat(str_buffer, " connected to server");
            printf("%s\n",str_buffer );
            move.id=id;
            sendMsgToClient(&move, sizeof(data),id);
            broadcast(&move, sizeof(data));
        }

        struct msg_ret_t msg_ret = recvMsg(&move_aux);
        //mensagem nova
        if (msg_ret.status == MESSAGE_OK) {
            if(move_aux.tecla == 85){ // up

                if(move_aux.Y+10 < ALTURA_TELA-10 && map[move_aux.Y+10] [move_aux.X]!='l'){
                    move.Y= move_aux.Y+10;
                    move.X = move_aux.X;
                    marcaPosicao(move_aux.X,move_aux.Y,move.X,move.Y, pers[move_aux.id]);
                    move.id = move_aux.id;
                    move.permissao=1;
                    broadcast(&move, sizeof(data));    
                }
                else {
                    move.permissao = 0;
                    broadcast(&move, sizeof(data));   
                }
            }
            if(move_aux.tecla == 84){ // down
                if(move_aux.Y-10 >= 10 && map[move_aux.Y-10] [move_aux.X]!='l'){
                    move.Y= move_aux.Y-10;
                    //printf("x %d y %d\n", move_aux.X, move_aux.Y );
                    move.X = move_aux.X;
                    move.id = move_aux.id;
                    //printf("x %d y %d\n", move.X, move.Y );
                    marcaPosicao(move_aux.X,move_aux.Y,move.X,move.Y, pers[move_aux.id]);
                    move.permissao=1;
                    broadcast(&move, sizeof(data));    
                }
                else {
                    move.permissao = 0;
                    broadcast(&move, sizeof(data));   
                }
            }
            if(move_aux.tecla == 83 ){ // left
                if(move_aux.X+10 < LARGURA_TELA-10 && map[move_aux.Y] [move_aux.X+10]!='l'){
                    move.X= move_aux.X+10;
                    move.Y = move_aux.Y;
                    move.id = move_aux.id;
                    marcaPosicao(move_aux.X,move_aux.Y,move.X,move.Y, pers[move_aux.id]);
                    move.permissao=1;
                    broadcast(&move, sizeof(data));    
                }
                else {
                    move.permissao = 0;
                    broadcast(&move, sizeof(data));   
                }
            }
            if(move_aux.tecla == 82){ // right
                if(move_aux.X-10 >=10 && map[move_aux.Y] [move_aux.X-10]!='l'){
                    move.X= move_aux.X-10;
                    move.Y = move_aux.Y;
                    move.id = move_aux.id;
                    marcaPosicao(move_aux.X,move_aux.Y,move.X,move.Y, pers[move_aux.id]);
                    move.permissao=1;
                    broadcast(&move, sizeof(data));    
                }
                else {
                    move.permissao = 0;
                    broadcast(&move, sizeof(data));   
                }
            }
            
        } 
        else if (msg_ret.status == DISCONNECT_MSG) {
            sprintf(str_buffer, "%s disconnected", client_names[msg_ret.client_id]);
            printf("%s disconnected, id = %d is free\n",client_names[msg_ret.client_id], msg_ret.client_id);
        }
    }
}
