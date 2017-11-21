#include "server.h"
#include <stdio.h>
#include <string.h>

#define MSG_MAX_SIZE 350
#define BUFFER_SIZE (MSG_MAX_SIZE)
#define LOGIN_MAX_SIZE 13
#define MAX_CHAT_CLIENTS 13
typedef struct {
      int X,Y,permissao,id;
      char tecla;
      char nome[30];
}data;
data move;

char map [20] [40];
data move;
    
void monta () {
        int i_des=0,j_des=0;

        for( i_des = 0 ; i_des < 20 ; i_des++ ){

            for( j_des = 0 ; j_des < 40 ; j_des++ ){
                map[i_des] [j_des] =32;
            }

        }

        for( i_des = 5 ; i_des < 10 ; i_des++ ){

            for( j_des = 10 ; j_des < 15 ; j_des++ ){
                map[i_des] [j_des] ='l';
            }

        }
        for( i_des = 13 ; i_des < 18 ; i_des++ ){

            for( j_des = 10 ; j_des < 15 ; j_des++ ){
                map[i_des] [j_des] ='l';
            }

        }

        for( i_des = 8 ; i_des < 15 ; i_des++ ){

            for( j_des = 25 ; j_des < 30 ; j_des++ ){
                map[i_des] [j_des] ='l';
            }

        }


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
            t=1;
            if(move_aux.tecla == 'w'){
                printf("%c  x %d  y  %d \n", move_aux.tecla, move_aux.X, move_aux.Y);
                if(move_aux.Y-1>=0 && map[move_aux.Y-1] [move_aux.X]!='l'){
                    printf("ok\n");
                    move.Y= move_aux.Y-1;
                    move.X = move_aux.X;
                    move.id = move_aux.id;
                    printf("id %d\n",move.id);
                    move.permissao=1;
                    printf("permi %d\n",move.permissao);
                    broadcast(&move, sizeof(data));    
                }
                else {
                    move.permissao = 0;
                    broadcast(&move, sizeof(data));   
                }
            }
            if(move_aux.tecla == 's'){
                if(move_aux.Y+1<20 && map[move_aux.Y+1] [move_aux.X]!='l'){
                    move.Y= move_aux.Y+1;
                    move.X = move_aux.X;
                    move.id = move_aux.id;
                    move.permissao=1;
                    broadcast(&move, sizeof(data));    
                }
                else {
                    move.permissao = 0;
                    broadcast(&move, sizeof(data));   
                }
            }
            if(move_aux.tecla == 'a' ){
                if(move_aux.X-1>=0 && map[move_aux.Y] [move_aux.X-1]!='l'){
                    move.X= move_aux.X-1;
                    move.Y = move_aux.Y;
                    move.id = move_aux.id;
                    move.permissao=1;
                    broadcast(&move, sizeof(data));    
                }
                else {
                    move.permissao = 0;
                    broadcast(&move, sizeof(data));   
                }
            }
            if(move_aux.tecla == 'd'){
                if(move_aux.X+1<40 && map[move_aux.Y] [move_aux.X+1]!='l'){
                    move.X= move_aux.X+1;
                    move.Y = move_aux.Y;
                    move.id = move_aux.id;
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
